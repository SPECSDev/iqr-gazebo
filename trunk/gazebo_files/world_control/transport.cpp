#include "transport.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>

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
  
  

  Side  side;
  side.pos_x=6.0;  side.pos_y=-6.0;  side.prob=1.0;  side.radius=0.5;
  resourceSides.append(side);
  side.pos_x=-2.0;  side.pos_y=2.0;  side.prob=1.0;  side.radius=2.0;
  resourceSides.append(side);
  
  createResources();
}

Transport::~Transport(){
  node->Fini();
  gazebo::transport::fini();
}


void Transport::OnScore(ConstVector3dPtr &msg)
{
  
  if((int)msg->z()==0)
    emit addPointA();
  if((int)msg->z()==1)
    emit addPointB();   

  if((int)msg->x()<MAX_RESOURCES){
    resources[(int)msg->x()].id=(int)msg->y();
    resources[(int)msg->x()].state=-1;
  }else{
    std::cout<<"OnScore::WrongName"<<std::endl;
  }
  std::cout<<"ID "<<msg->y()<<std::endl;
}

void Transport::start(){
  std::cout<<"Transport::Start"<<std::endl;
  timer->start(1000);
}

void Transport::reset(){
  std::cout<<"Transport::Reset"<<std::endl;
  
  timer->stop();
  for(int i=0;i<MAX_RESOURCES;i++){
    if (resources[i].state>0)
      resetResource(i);
  }
}

void Transport::setPoseResource(int  modelNumber, gazebo::math::Pose pose){
  std::ostringstream sStream;  
  sStream <<modelNumber;
  std::string modelName= sStream.str();  
  std::cout<<"Transport::setPosResource()::modelName "<<modelName<< " id "<<resources[modelNumber].id<<std::endl;  
  msgs::Model msg;
  msg.set_id(resources[modelNumber].id);
  msg.set_name(modelName);
  msgs::Set(msg.mutable_pose(), pose);
  this->modelPub->Publish(msg);
}

void Transport::resetResource(int modelNumber){
  std::cout<<"Transport::resetResources"<<std::endl;
  if(!modelNumber>MAX_RESOURCES)
    resources[modelNumber].state=-1;
}

void Transport::createResources(){
  std::cout<<"Transport::createResources"<<std::endl;
 
  
  for(int i=0;i<MAX_RESOURCES;i++){
    std::ostringstream sStream;  
    sStream <<i;
    std::string modelName = sStream.str();
    modelElem->GetAttribute("name")->SetFromString(modelName);
    math::Pose pose = modelElem->Get<math::Pose>("pose");
    pose=DEFAULT_POSE;
    createMsg.set_sdf(sdf->ToString());
    msgs::Set(createMsg.mutable_pose(), pose);
    createPub->WaitForConnection();
    createPub->Publish(createMsg, true);
    Resource resource;
    resource.state=-1;
    resource.id=-1;
    resources.push_back(resource);
    usleep(100000);
    std::cout<<"Transport::createResource::modelName"<<modelName<<std::endl;
  }
}

void Transport::spawnResource(){
  std::cout<<"Transport::spawnResource()"<<std::endl;  
  
  //TODO make random access to sides
  foreach(const Side& side, resourceSides){
    int i=findAvailableResource();
    if(i>=0){
      //std::cout<<"Transport::spawnResource()::index= "<<i<<std::endl;  
      if(((float) std::rand() / (RAND_MAX)) < side.prob){
  	math::Pose pose;
  	float radius = (float) std::rand() / (RAND_MAX)*side.radius;
  	float angle= (float) std::rand() / (RAND_MAX)*2*M_PI;
  	pose.pos.x = side.pos_x+radius*cos(angle);
  	pose.pos.y = side.pos_y+radius*sin(angle);
  	pose.pos.z = 10.0;
	resources[i].state=1;
  	setPoseResource(i,pose);
      }
    }
  }
}

int Transport::findAvailableResource(){
  int i=0;
  while(i<MAX_RESOURCES){
    //std::cout<<"Transport:findAvailableResource::state "<<resources[i].state<<std::endl;
    if(resources[i].state==-1){
      //std::cout<<"Transport:findAvailableResource::index "<<i<<std::endl;
      return i;
    }
    i++;
  }
    return -1;
}
