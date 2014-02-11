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
  private: msgs::Vector2d targetCmd;  
  
  private: bool targetSent;
    
  private: physics::ModelPtr model;
  private: event::ConnectionPtr updateConnection;    
  private: std::string modelName; 
    
    
  public: void Load(physics::ModelPtr _parent, sdf::ElementPtr _sdf)
    {
      // Store the pointer to the model
      this->model = _parent;
      targetSent=false;      
      node = transport::NodePtr(new transport::Node());
      node->Init();
      
      modelName = model->GetSDF()->GetAttribute("name")->GetAsString();
      
      transport::run();
      
      std::string topic;
      topic = "/gazebo/default/";
      topic +="TargetReached";  // publish to the joints of the bugs
      
      this->targetPub = this->node->Advertise<gazebo::msgs::Vector2d>
	("/gazebo/default/targetReached");
      
      deletePub = this->node->Advertise<msgs::Request>("/gazebo/default/request");
      deletePub->WaitForConnection();
      
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
	if (pos.Distance(TARGET_A)<RADIUS){
	  targetCmd.set_x(0);
	  targetCmd.set_y(0);
	}
	if (pos.Distance(TARGET_B)<RADIUS){
	  targetCmd.set_x(1);
	  targetCmd.set_y(0);
	}
	
	if(!targetSent){
	  targetPub->Publish(targetCmd);
	  targetSent=true;
	}
	
	//Delete Sphere when on target
	deleteCmd = *msgs::CreateRequest("entity_delete", modelName);
	deletePub->Publish(deleteCmd);
      }
    }
  };

 
  
  // Register this plugin with the simulator
  GZ_REGISTER_MODEL_PLUGIN(TargetPlugin)
}
