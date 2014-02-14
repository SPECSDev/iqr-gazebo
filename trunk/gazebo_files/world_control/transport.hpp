#ifndef TRANSPORT_H
#define TRANSPORT_H

#include <QObject>			       
#include <QTimer>
#include <QVector>
#include <QList>
#include <boost/bind.hpp>
#include <boost/program_options.hpp>

#include "gazebo/transport/transport.hh"
#include "gazebo/msgs/msgs.hh"
#include "gazebo/msgs/vector3d.pb.h"
#include "gazebo/gui/GuiIface.hh"

namespace gazebo
{
  typedef struct side_t Side;
  struct side_t{
    float pos_x;
    float pos_y;
    float prob;
    float radius;
  };
  typedef struct resource_t Resource;
  struct resource_t{
    int id;
    int state;
  };
  
  const int MAX_RESOURCES=10;
  const math::Pose DEFAULT_POSE = math::Pose(0,0,0,0,0,0);
  const int NAME_KEY = 19299;

  class Transport : public QObject
  {
    Q_OBJECT
 
  public:
    Transport();
    ~Transport();
 
  public slots:
    void reset();
    void start();
    void stop();
    void close();
  
    
  private:
    transport::NodePtr node;
    transport::SubscriberPtr resourceSubscriber;
    
    transport::PublisherPtr createPub;
    transport::PublisherPtr deletePub;
    transport::PublisherPtr modelPub;
    transport::PublisherPtr controlPub;
    
    
    msgs::Request deleteMsg;
    msgs::Factory createMsg;
        
    sdf::ElementPtr modelElem;
    boost::shared_ptr<sdf::SDF> sdf;
    
    void setPoseResource(int modelNumber, math::Pose pose);
    void resetResource(int modelNumber);   
    void createResource(math::Pose pose);
    int findAvailableResource();
    
    std::string indexToName(int index);
    int nameToIndex(int name);
    
    
    void OnScore(ConstVector3dPtr &msg);
    QTimer *timer;
    
 		  
    QList<Side> resourceSides; // x,y,prob,spread
    QList<Resource> resources;
			     
			     
  private slots:
    void spawnResource();
  signals:
    void addPointA();
    void addPointB();
    void quit();
  };
}


#endif
