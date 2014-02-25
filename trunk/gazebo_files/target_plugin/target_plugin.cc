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

#include "gazebo/physics/ContactManager.hh"
#include "gazebo/physics/Contact.hh"
#include "gazebo/physics/Collision.hh"
#include "gazebo/physics/PhysicsTypes.hh"

namespace gazebo
{
  const math::Vector3 TARGET_A(-10,10,0);
  const math::Vector3 TARGET_B(10,-10,0);
  const float RADIUS =3;

  const float UPDATE_RATE =100;
  
   
  class TargetPlugin : public ModelPlugin
  {
  private: transport::NodePtr node;
  private: transport::PublisherPtr targetPub;
    
  private: transport::SubscriberPtr contactSub;

  private: msgs::Vector3d targetCmd;  
    
  private: physics::ModelPtr model;
  private: physics::WorldPtr world;
  private: std::string modelName; 
  
  private: int counter;
  private: int grasping;
  private: event::ConnectionPtr updateConnection;    
  private: std::string collisionTopic;

  public: void Load(physics::ModelPtr _parent, sdf::ElementPtr _sdf)
    {
      counter=0;
      // Store the pointer to the model
      this->model = _parent;
      this->world = this->model->GetWorld();
      modelName = model->GetScopedName();
      
      uint i =0;
      physics::CollisionPtr collision = model->GetLink("body")->GetCollision(i);
      physics::ContactManager *mgr =
	this->world->GetPhysicsEngine()->GetContactManager();
      collisionTopic = mgr->CreateFilter(modelName, collision->GetScopedName());
      
      // Listen to the update event. This event is broadcast every
      // simulation iteration.
      this->updateConnection = 
	event::Events::ConnectWorldUpdateBegin(boost::bind(&TargetPlugin::OnUpdate, this));
    }
    
  public: void Init(){
    counter=0;
    node = transport::NodePtr(new transport::Node());
    node->Init();
    this->targetPub = this->node->Advertise<gazebo::msgs::Vector3d>
      ("/gazebo/default/Score");
    
    targetCmd.set_x(atoi( modelName.c_str()));
    targetCmd.set_y(this->model->GetId());
    targetCmd.set_z(-1);
    targetPub->Publish(targetCmd);

    this->contactSub = this->node->Subscribe(collisionTopic,
					     &TargetPlugin::OnContacts, this);
   
  }

  public: void Reset(){
    counter=0;
    targetPub.reset();
    node->Fini();
    node = transport::NodePtr(new transport::Node());
    node->Init();
    this->targetPub = this->node->Advertise<gazebo::msgs::Vector3d>
      ("/gazebo/default/Score");

    this->contactSub = this->node->Subscribe(collisionTopic,
					     &TargetPlugin::OnContacts, this);
  }
    
    // Called by the world update start event
  public: void OnUpdate()
    {
      if(counter<UPDATE_RATE){
	counter++;
	return;
      }
      counter =0;
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
	//model->SetWorldPose(model->GetInitialRelativePose());
	model->Reset();	

      }
      std::cout<<"TargetPlugin::OnUpdate "<<counter<<std::endl;
	if (grasping>0)
	  std::cout<<"TargetPlugin::OnUpdate Grasping"<<grasping<<std::endl;
	grasping =0;
    }

  public: void OnContacts(ConstContactsPtr &_msg)
    {
      // std::cout<<"TargetPlugin::OnContacts"<<std::endl;
      for (int i = 0; i < _msg->contact_size(); ++i)
	{
	  physics::CollisionPtr collision1 = boost::dynamic_pointer_cast<physics::Collision>(
	     this->world->GetEntity(_msg->contact(i).collision1()));
	  physics::CollisionPtr collision2 = boost::dynamic_pointer_cast<physics::Collision>(
	     this->world->GetEntity(_msg->contact(i).collision2()));
	  
	  if ((collision1 && !collision1->IsStatic()) &&
	      (collision2 && !collision2->IsStatic()))
	    {
	      if (collision1->GetModel()->GetLink("bug")||collision2->GetModel()->GetLink("bug"))
		{
		  //  std::cout<<"TargetPlugin::OnContacts"<<std::endl;
		  grasping++;
		  //  std::cout<<"TargetPlugin::OnContacts"<< grasping<<std::endl;
		}
	    }
	}
    }
  };
  


  // Register this plugin with the simulator
  GZ_REGISTER_MODEL_PLUGIN(TargetPlugin)
}
