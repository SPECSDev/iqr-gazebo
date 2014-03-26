/*
 * Copyright (C) 2012-2014 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/
#include <boost/bind.hpp>
#include "gazebo/physics/physics.hh"
#include "gazebo/gazebo.hh"
#include "gazebo/math/Vector3.hh"



/// \example examples/plugins/vel_plugin.cc
/// This example creates a ModelPlugin, and applies a force to a box to move
/// it alone the ground plane.
namespace gazebo
{
  const float RAND_ROT=0.01;
  const float RAND_POS=0.01; 
  const int MAX_RESET_POSES=10; 
  

  
  class ResetPlugin : public ModelPlugin
  {
  private: physics::ModelPtr model;
  private: physics::WorldPtr world;  
    
  private: event::ConnectionPtr updateConnection;    
  private: std::string modelName; 
    
  private: std::vector< math::Pose> resetPoses;
  private: math::Box resetArea;
    
  public: void Load(physics::ModelPtr _parent, sdf::ElementPtr _sdf)
    {
      // Store the pointer to the model
      this->model = _parent;
      this->world = this->model->GetWorld();
      
      modelName = model->GetScopedName();
     
      for(int i=0;i<MAX_RESET_POSES;i++){
	std::ostringstream osName;
	osName << "Reset_"<<i;

	physics::ModelPtr modelReset = this->world->GetModel(osName.str());
	if(modelReset!=NULL){
	  std::cout<<osName.str()<<std::endl;
	  resetPoses.push_back(modelReset->GetWorldPose());
	}
      }
      
      physics::ModelPtr modelArea = this->world->GetModel("GroundPlane");
      //resetArea=RESET_AREA_DEFAULT;
      if(modelArea!=NULL){
	resetArea= modelArea->GetBoundingBox();
	std::cout<<resetArea<<std::endl;
      }
      // Listen to the update event. This event is broadcast every
      // simulation iteration.
      if(resetPoses.size()>0){
	this->updateConnection = event::Events::ConnectWorldUpdateBegin(
			         boost::bind(&ResetPlugin::OnUpdate, this));
      }
    }
    
    // Called by the world update start event
    public: void OnUpdate()
    {
      //std::cout<<"ResetPlugin::OnUpdate"<<std::endl;
      
      
      // Apply a small linear resetocity to the model.
      math::Pose pose = this->model->GetWorldPose();
      
      if(pose.pos[0]>resetArea.max[0] ||
	 pose.pos[1]>resetArea.max[1] ||
	 pose.pos[0]<resetArea.min[0] ||
	 pose.pos[1]<resetArea.min[1]){
	int randIndex = rand()%resetPoses.size();
	math::Pose resetPose = resetPoses[randIndex];
	
	//add noise
	resetPose.Set(
		 resetPose.pos[0]+RAND_POS*2*((std::rand()/(float)RAND_MAX)-0.5),
		 resetPose.pos[1]+RAND_POS*2*((std::rand()/(float)RAND_MAX)-0.5),
		 pose.pos[2],
		 pose.rot.GetRoll(),
		 pose.rot.GetPitch(),
		 resetPose.rot.GetYaw()+RAND_ROT*2*((std::rand()/(float)RAND_MAX)-0.5)
		 );
	
	this->model->SetWorldPose(resetPose,true,false);
	this->model->SetLinearVel(math::Vector3(0,0,0));
	this->model->SetAngularVel(math::Vector3(0,0,0));
      }
    }
      
    
  };

  // Register this plugin with the simulator
  GZ_REGISTER_MODEL_PLUGIN(ResetPlugin)
}
