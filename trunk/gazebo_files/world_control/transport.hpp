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
#include "gazebo/msgs/vector2d.pb.h"

namespace gazebo
{
  typedef struct side_t Side;
  struct side_t{
    float pos_x;
    float pos_y;
    float prob;
    float radius;
};

  class Transport : public QObject
  {
    Q_OBJECT
    
  public:
    Transport();
    ~Transport();
 
  public slots:
    void reset();
    void start();
    
  private:
    transport::NodePtr node;
    transport::SubscriberPtr targetSubscriber;
    
    transport::PublisherPtr createPub;
    transport::PublisherPtr deletePub;
    
    msgs::Factory createMsg;
    msgs::Request deleteMsg;
    
    sdf::ElementPtr modelElem;
    boost::shared_ptr<sdf::SDF> sdf;
    
    void OnTarget(ConstVector2dPtr &msg);
    QTimer *timer;
    
    int numTargets;
		  
    QList<Side> targetSides; // x,y,prob,spread
		  
  private slots:
    void create();
  signals:
    void addPointA();
    void addPointB();
  };
}


#endif
