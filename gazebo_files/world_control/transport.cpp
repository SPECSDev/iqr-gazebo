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
  targetSubscriber = node->Subscribe("/gazebo/default/targetReached", 
				      &Transport::OnTarget, this);      
  gazebo::transport::run();
  createPub = node->Advertise<msgs::Factory>("/gazebo/default/factory");
  
  deletePub = node->Advertise<msgs::Request>("/gazebo/default/request");

  timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(create()));
  
  

  numTargets=0;
  Side  side;
  side.pos_x=2.0;  side.pos_y=-2.0;  side.prob=1.0;  side.radius=0.5;
  targetSides.append(side);
  side.pos_x=-2.0;  side.pos_y=2.0;  side.prob=1.0;  side.radius=2.0;
  targetSides.append(side);
  

}

Transport::~Transport(){
  node->Fini();
  gazebo::transport::fini();
}


void Transport::OnTarget(ConstVector2dPtr &msg)
{
  if(msg->x()<0.5)
    emit addPointA();
  if(msg->x()>0.5)
    emit addPointB();    
}

void Transport::start(){
  std::cout<<"Transport::Start"<<std::endl;
  timer->start(1000);
}

void Transport::reset(){
  std::cout<<"Transport::Reset"<<std::endl;
  timer->stop();

  //TODO DELETE ALL TARGETS
  std::ostringstream sStream;
 
  std::string modelName;
  
  while(numTargets>=0){
    sStream.str(std::string());
    sStream << "SDIC_Target_"<<numTargets;
    modelName=sStream.str();
    //std::cout<<"Transport:deleting: "<<modelName<<std::endl;
    
    deleteMsg = *msgs::CreateRequest("entity_delete", modelName);
    deletePub->Publish(deleteMsg);
    
    numTargets--;
  }
  numTargets=0;
}

void Transport::create(){
  //  std::cout<<"Transport::Create"<<std::endl;
  foreach(const Side& side, targetSides){
    if(((float) std::rand() / (RAND_MAX)) < side.prob){
      std::ostringstream sStream;
      sStream << "SDIC_Target_"<<numTargets;
      std::string modelName= sStream.str();
      
      numTargets++;
      modelElem->GetAttribute("name")->SetFromString(modelName);
      
      math::Pose pose = modelElem->Get<math::Pose>("pose");

      float radius = (float) std::rand() / (RAND_MAX)*side.radius;
      float angle= (float) std::rand() / (RAND_MAX)*2*M_PI;
      pose.pos.x = side.pos_x+radius*cos(angle);
      pose.pos.y = side.pos_y+radius*sin(angle);
      pose.pos.z = 10.0;
      
      createMsg.set_sdf(sdf->ToString());
      msgs::Set(createMsg.mutable_pose(), pose);
      createPub->Publish(createMsg, true);
    }
  }
}

