#include "transport.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <algorithm>

#define _USE_MATH_DEFINES

using namespace gazebo;


Transport::Transport(){


  std::string filename;
  char* pPath;
  pPath = getenv ("GAZEBO_MODEL_PATH");
  if (pPath!=NULL){
    std::printf ("The current path is: %s",pPath);
    filename = std::string(pPath)+std::string("/SDIC_Target/model.sdf");
  }
  std::ifstream ifs(filename.c_str());
  if (!ifs)
    {
      std::cerr << "Error: Unable to open file[" << filename << "]\n";
      return;
    }
  
  sdf = boost::shared_ptr<sdf::SDF> (new sdf::SDF());
  if (!sdf::init(sdf))
    {
      std::cerr << "ERROR: SDF parsing the xml failed" << std::endl;
      return;
    }
  
  if (!sdf::readFile(filename, sdf))
    {
     std::cerr << "Error: SDF parsing the xml failed\n";
     return;
    }
  
  modelElem = sdf->root->GetElement("model");


  gazebo::transport::init();
  node = transport::NodePtr(new transport::Node());
  node->Init();
  resourceSubscriber = node->Subscribe("/gazebo/default/Score", 
				      &Transport::OnScore, this);      
  gazebo::transport::run();
  createPub = node->Advertise<msgs::Factory>("/gazebo/default/factory");
  createPub->WaitForConnection();
  
  deletePub = node->Advertise<msgs::Request>("/gazebo/default/request");
  deletePub->WaitForConnection();

  
  modelPub = this->node->Advertise<msgs::Model>("~/model/modify");
  modelPub->WaitForConnection();

  controlPub = this->node->Advertise<msgs::WorldControl>("~/world_control");
  controlPub->WaitForConnection();

  visualPub = this->node->Advertise<msgs::Visual>("~/visual");
  visualPub->WaitForConnection();
 
  timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(spawnResource()));
  
  //Creaet dummy resource
  std::string modelName="dummy";
  modelElem->GetAttribute("name")->SetFromString(modelName);
  createMsg.set_sdf(sdf->ToString());
  msgs::Set(createMsg.mutable_pose(), DEFAULT_POSE);
  createPub->Publish(createMsg, true);



  Side  side;
  side.pos_x=6.0;  side.pos_y=-6.0;  side.prob=1.0;  side.radius=0.5;
  resourceSides.append(side);
  side.pos_x=-3.0;  side.pos_y=3.0;  side.prob=1.0;  side.radius=1.0;
  resourceSides.append(side);
}

Transport::~Transport(){
  gazebo::transport::fini();
  
}


void Transport::OnScore(ConstVector3dPtr &msg)
{
  int name = (int)msg->x();
  int id = (int)msg->y();
  int target=(int)msg->z();
  
  int index=nameToIndex(name);
  //add points TODO plus minus
  if(target==0){
    if(resources[index].state==GOOD)
      emit plusPointA();
    else
      emit minusPointA();
  }

  if(target==1){
    if(resources[index].state==GOOD)
      emit plusPointB();   
    else
      emit minusPointB();
  }
  //Make resource available
 
  if(index>=0 && index<resources.size()){
    resources[index].id=id;
    resetResource(index);
  }else{
    std::cout<<"Transport::OnScore::WrongName"<<std::endl;
  }
}

void Transport::start(){
  std::cout<<"Transport::Start"<<std::endl;
  timer->start(1000);
}

void Transport::reset(){
  std::cout<<"Transport::Reset"<<std::endl;
  
  timer->stop();
  for(int i=0;i<resources.size();i++){
    if (resources[i].state>0){
      resetResource(i);
      setPoseResource(i,addNoisePose(DEFAULT_POSE,0.2));
    }
  }
  msgs::WorldControl worldMsg;
  msgs::WorldReset *resetMsg = worldMsg.mutable_reset();
  resetMsg->set_all(true);
  controlPub->Publish(worldMsg);
}

void Transport::stop(){
  std::cout<<"Transport::Stop"<<std::endl; 
  
  timer->stop();
}

void Transport::close(){
  std::cout<<"Transport::Close"<<std::endl;
  deleteMsg = *msgs::CreateRequest("entity_delete", "dummy");
  deletePub->Publish(deleteMsg);  
  
  for(int i=0;i<resources.size();i++){
    deleteMsg = *msgs::CreateRequest("entity_delete", indexToName(i));
    deletePub->Publish(deleteMsg);
  }  


  //  resourceSubscriber->Unsubscribe();
  resourceSubscriber.reset();
  createPub.reset();
  modelPub.reset();
  deletePub.reset();
  controlPub.reset();
  node->Fini();
  gazebo::transport::fini();
}

void Transport::setPoseResource(int  index, gazebo::math::Pose pose){
  std::string modelName= indexToName(index);  
  msgs::Model msg;
  msg.set_id(resources[index].id);
  msg.set_name(modelName);
  msgs::Set(msg.mutable_pose(), pose);
  this->modelPub->Publish(msg);
}

void Transport::resetResource(int index){
  resources[index].age=0;
  resources[index].state=AVAILABLE;
}

void Transport::recycleResource(int index, math::Pose pose){
  setColorResource(index, "Gazebo/Red");
  resources[index].state=GOOD;
  setPoseResource(index,pose);
}

void Transport::setColorResource(int index, std::string color){
  msgs::Visual visualMsg;
  visualMsg.set_id(resources[index].id);
  visualMsg.set_name(indexToName(index));
  visualMsg.set_parent_name("~");
  visualMsg.mutable_material()
    ->mutable_script()->add_uri("file://media/materials/scripts/gazebo.material");
  visualMsg.mutable_material()->mutable_script()->set_name(color);
  visualPub->Publish(visualMsg);
}

void Transport::createResource(math::Pose pose){
  std::string modelName= indexToName(resources.size());
  modelElem->GetAttribute("name")->SetFromString(modelName);
  createMsg.set_sdf(sdf->ToString());
  msgs::Set(createMsg.mutable_pose(), addNoisePose(DEFAULT_POSE,0.2));
  createPub->Publish(createMsg, true);
  Resource resource;
  resource.state=GOOD;
  resource.age=0;
  resource.id=-1;
  resources.push_back(resource);
  int index=resources.size()-1;
  while(resources[index].id<0)
    usleep(100000);
  setPoseResource(index,pose);
}


void Transport::spawnResource(){
  //age resources
  for(int i=0;i<resources.size();i++){
    if(resources[i].state>0)
      resources[i].age++;
    if(resources[i].age==TURN_AGE){
      resources[i].state=BAD;
      setColorResource(i,"Gazebo/Blue");
    }
    if(resources[i].age==MAX_AGE){
      resetResource(i);
      setPoseResource(i,addNoisePose(DEFAULT_POSE,0.2));
    }
  }
  
  //spawn resources
  std::random_shuffle(resourceSides.begin(), resourceSides.end());
  foreach(const Side& side, resourceSides){
    if(((float) std::rand() / (RAND_MAX)) < side.prob){    
      math::Pose pose;
      pose.pos.x = side.pos_x;
      pose.pos.y = side.pos_y;
      pose.pos.z = 10.0;
      pose = addNoisePose(pose, side.radius);
      
      if(resources.size()==0){
	createResource(pose);
      }else{
	int i=findAvailableResource();
	if(i>=0){
	  recycleResource(i,pose);
	}else{
	  if(resources.size()<MAX_RESOURCES){
	    createResource(pose);
	  }
	}
      }    
    }
  }
}

int Transport::findAvailableResource(){
  int i=0;
  while(i<resources.size()){
    if(resources[i].state==AVAILABLE){
      return i;
    }
    i++;
  }
  return -1;
}

std::string Transport::indexToName(int index){
  std::ostringstream sStream;  
  sStream << (index+NAME_KEY);
  return  sStream.str();
}


int Transport::nameToIndex(int name){
  return name-NAME_KEY;
}


math::Pose Transport::addNoisePose(math::Pose pose, float radius){
  float randRadius = (float) std::rand() / (RAND_MAX)*radius;
  float randAngle= (float) std::rand() / (RAND_MAX)*2*M_PI;
  pose.pos.x = pose.pos.x+randRadius*cos(randAngle);
  pose.pos.y = pose.pos.y+randRadius*sin(randAngle);
  return pose;
}
