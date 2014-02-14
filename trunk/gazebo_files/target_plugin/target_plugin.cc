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

#include "gazebo/common/Console.hh"

#include "gazebo/transport/transport.hh"
#include "gazebo/msgs/msgs.hh"
#include "gazebo/msgs/vector2d.pb.h"



/// \example examples/plugins/vel_plugin.cc
/// This example creates a ModelPlugin, and applies a force to a box to move
/// it alone the ground plane.
namespace gazebo
{
  const math::Vector3 TARGET_A(-10,10,0);
  const math::Vector3 TARGET_B(10,-10,0);
  const float RADIUS =3;

  class TargetPlugin : public ModelPlugin
  {
  private: transport::NodePtr node;
  private: transport::PublisherPtr targetPub;
  private: transport::PublisherPtr deletePub;
  
  private: msgs::Request deleteCmd;
  private: msgs::Vector3d targetCmd;  
  

    
  private: physics::ModelPtr model;
  private: event::ConnectionPtr updateConnection;    
  private: std::string modelName; 
  
  public: void Load(physics::ModelPtr _parent, sdf::ElementPtr _sdf)
    {
      // Store the pointer to the model
      this->model = _parent;
      
      
      
      modelName = model->GetScopedName();
      
      
      node = transport::NodePtr(new transport::Node());
      node->Init();
      this->targetPub = this->node->Advertise<gazebo::msgs::Vector3d>
	("/gazebo/default/Score");
      
      // Listen to the update event. This event is broadcast every
      // simulation iteration.
      this->updateConnection = 
	event::Events::ConnectWorldUpdateBegin(boost::bind(&TargetPlugin::OnUpdate, this));
    }
    
    // Called by the world update start event
  public: void OnUpdate()
    {
      math::Vector3 pos = this->model->GetWorldPose().pos;
      if (pos.Distance(TARGET_A)<RADIUS||pos.Distance(TARGET_B)<RADIUS){
	targetCmd.set_x(atoi( modelName.c_str()));
	targetCmd.set_y(this->model->GetId());	 
	if (pos.Distance(TARGET_A)<RADIUS){
	  targetCmd.set_z(0);
	}
	if (pos.Distance(TARGET_B)<RADIUS){
	  targetCmd.set_z(1);
	}
	
	targetPub->Publish(targetCmd);
	model->SetWorldPose(math::Pose(0,0,0,0,0,0));
      }
    }
  };
  
   // Register this plugin with the simulator
  GZ_REGISTER_MODEL_PLUGIN(TargetPlugin)
}
