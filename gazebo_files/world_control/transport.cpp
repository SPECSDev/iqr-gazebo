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

  modelPub = this->node->Advertise<msgs::Model>("~/model/modify");
  modelPub->WaitForConnection();

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
  side.pos_x=-2.0;  side.pos_y=2.0;  side.prob=1.0;  side.radius=2.0;
  resourceSides.append(side);
}

Transport::~Transport(){
  node->Fini();
  gazebo::transport::fini();
}


void Transport::OnScore(ConstVector3dPtr &msg)
{
  int name = (int)msg->x();
  int id = (int)msg->y();
  int target=(int)msg->z();
  
  if(target==0)
    emit addPointA();
  if(target==1)
    emit addPointB();   
  
  int index=nameToIndex(name);
  if(index>=0 && index<resources.size()){
    resources[index].id=id;
    resources[index].state=-1;
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
      setPoseResource(i,DEFAULT_POSE);
    }
  }
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
  resources[index].state=-1;
}

void Transport::createResource(math::Pose pose){
  std::string modelName= indexToName(resources.size());
  modelElem->GetAttribute("name")->SetFromString(modelName);
  createMsg.set_sdf(sdf->ToString());
  msgs::Set(createMsg.mutable_pose(), DEFAULT_POSE);
  createPub->Publish(createMsg, true);
  Resource resource;
  resource.state=1;
  resource.id=-1;
  resources.push_back(resource);
  int index=resources.size()-1;
  while(resources[index].id<0)
    usleep(100000);
  setPoseResource(index,pose);
}


void Transport::spawnResource(){
  
  
  //TODO make random access to sides
  std::random_shuffle(resourceSides.begin(), resourceSides.end());
  foreach(const Side& side, resourceSides){
    if(((float) std::rand() / (RAND_MAX)) < side.prob){    
      math::Pose pose;
      float radius = (float) std::rand() / (RAND_MAX)*side.radius;
      float angle= (float) std::rand() / (RAND_MAX)*2*M_PI;
      pose.pos.x = side.pos_x+radius*cos(angle);
      pose.pos.y = side.pos_y+radius*sin(angle);
      pose.pos.z = 10.0;
      
      if(resources.size()==0){
	createResource(pose);
      }else{
	int i=findAvailableResource();
	if(i>=0){
	  resources[i].state=1;
	  setPoseResource(i,pose);
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
    if(resources[i].state==-1){
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