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

  const float RAND_ROT=0.005;
  const float RAND_TRANS=0.005; 
  
  
  class VelPlugin : public ModelPlugin
  {
  private: transport::NodePtr node;
  private: transport::SubscriberPtr commandSubscriber;
    // Pointer to the model
  private: physics::ModelPtr model;
    // Pointer to the update event connection
  private: event::ConnectionPtr updateConnection;    
  private: std::string modelName; 
  private: double trans,rot;
    
  public: void Load(physics::ModelPtr _parent, sdf::ElementPtr _sdf)
    {
      // Store the pointer to the model
      this->model = _parent;
      modelName = model->GetScopedName();
      
      node = transport::NodePtr(new transport::Node());
      node->Init();
      
      
      std::string topic;
      topic= "/gazebo/default/";
      topic += modelName + "/vel_cmd"; 
     
      std::cerr << "Subscribing to: " << topic << " ..." << std::flush;
      commandSubscriber = node->Subscribe(topic, &gazebo::VelPlugin::OnCommand, this);      
      std::cout << "done!"<< std::endl;
      
      // Listen to the update event. This event is broadcast every
      // simulation iteration.
      
      this->updateConnection = event::Events::ConnectWorldUpdateBegin(
								      boost::bind(&VelPlugin::OnUpdate, this));
    }
    
    // Called by the world update start event
    public: void OnUpdate()
    {

      float rTrans =RAND_TRANS*2*((std::rand()/(float)RAND_MAX)-0.5);
      float rRot = RAND_ROT*2*((std::rand()/(float)RAND_MAX)-0.5);
     
      // Apply a small linear velocity to the model.
      math::Vector3 v(trans+rTrans, 0, 0);
      math::Pose pose = this->model->GetWorldPose();
     
      
      pose.Set(pose.pos[0],pose.pos[1],0,0,0,pose.rot.GetYaw());
      this->model->SetWorldPose(pose,true,false);
      
      v = pose.rot * v;
    
      
      // Apply a small linear velocity to the model. 
      this->model->SetLinearVel(v);
      this->model->SetAngularVel(math::Vector3(0, 0, rot+rRot));
    }
    
  public: void OnCommand(ConstVector2dPtr &msg)
    {
      
      trans=msg->x();
      rot=msg->y();
      
      // std::cout<<modelName<<": Command Received: trans="<<trans<<" rot="<<rot<<std::endl;
    }
    
    
  };

  // Register this plugin with the simulator
  GZ_REGISTER_MODEL_PLUGIN(VelPlugin)
}
