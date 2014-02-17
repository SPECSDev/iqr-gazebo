#ifndef _GAZEBO_BUG_HPP
#define _GAZEBO_BUG_HPP

#include <gazebo/transport/transport.hh>
#include <gazebo/msgs/msgs.hh>
//#include <gazebo/msgs/image.pb.h>
#include <gazebo/msgs/laserscan_stamped.pb.h>
#include <gazebo/msgs/joint_cmd.pb.h>
#include <gazebo/msgs/vector2d.pb.h>
#include <gazebo/common/Image.hh>
#include <string>
#include <vector>
#include <QMutex>
#include <QMap>


using namespace std;


const int MAX_RANGES = 32;
const int MAX_RANGES_GRIPPER = 16;
const int MAX_RANGES_AUDIO = 16;
const int MAX_GPS =3;

const float MAX_AUDIO_DISTANCE=14.1421;
const float MIN_AUDIO_DISTANCE=0.1;




const double VEL_FACTOR = 10.0;
const double GRASP_FORCE = 3;
//const double UP_SPEED = 1;

const int MAX_WIDTH_CAM = 32;
const int MAX_HEIGHT_CAM = 28;

const string DEFAULT_COM_KEY = "default_com_key";


typedef struct current_t Current;
struct current_t{
  float speed_left;
  float speed_right;
  bool closing;
  // bool lifting;
  int id;
  float ranges[MAX_RANGES];
  float rangesGripper[MAX_RANGES_GRIPPER];
  float rangesAudio[MAX_RANGES_AUDIO];
  float gps[MAX_GPS];
  gazebo::common::Image img;
};
	
typedef struct pixel_t Pixel;
struct pixel_t{
  float rh, gs, bv;
};


typedef unsigned char uchar;


namespace iqrcommon{

  class GazeboBugInterface{
  private:
    static int NumberOfBugs;
    string error_message;
    Current current; // Current values
    
    QMap<std::string, vector<float> > currMap;
    
    
    gazebo::msgs::Vector2d velCmd;  
    
    gazebo::msgs::JointCmd jointCommandRightFinger;    
    gazebo::msgs::JointCmd jointCommandLeftFinger;    
    gazebo::msgs::JointCmd jointCommandRightFingerTip;    
    gazebo::msgs::JointCmd jointCommandLeftFingerTip;    
    // gazebo::msgs::JointCmd jointCommandLifter;    
    // gazebo::msgs::PID* velocityTargetLeftFinger;
    // gazebo::msgs::PID* velocityTargetRightFinger;
    // gazebo::msgs::PID* velocityTargetLeftFingerTip;
    // gazebo::msgs::PID* velocityTargetRightFingerTip;

    
    //gazebo::msgs::PID* velocityTargetLifter;

    gazebo::msgs::LaserScan scan;
    gazebo::msgs::LaserScan scanGripper;
    gazebo::msgs::LaserScan scanAudio;
    //gazebo::msgs::LaserScan* emitAudio;
    gazebo::msgs::LaserScanStamped emitAudio;
    
    

    gazebo::transport::NodePtr node;
    gazebo::transport::SubscriberPtr laserScanSub;
    gazebo::transport::SubscriberPtr laserScanGripperSub;
    gazebo::transport::SubscriberPtr scanAudioSub;

    gazebo::transport::SubscriberPtr cameraSub;
    gazebo::transport::PublisherPtr motorPub;
    gazebo::transport::PublisherPtr jointPub;
    gazebo::transport::PublisherPtr audioPub;

    
    
    void Subscribe();
    void Publish();
    void Unsubscribe();
    
    int imageWidth;
    int imageHeight;

    string  comKey;
    
    void onScan(ConstLaserScanStampedPtr &_msg);
    void onScanGripper(ConstLaserScanStampedPtr &_msg);		
    void onScanAudio(ConstLaserScanStampedPtr &_msg);		
    void onImage(ConstImageStampedPtr &_msg);
    
    QMutex *qMutex;
    
    string deviceName;        
    int scanId;
    
  
    void setGripperUp(bool up);
    
  public:
    GazeboBugInterface();
    bool Open(const string& device); // subscribe to topics
    void Close();
   
    
    
    void setSpeed(float left, float right);
    void setImageFormat(int imageWidth, int iamgeHeight);
    void setComKey(const string& comKey);
    //void setGripper(bool close,bool up);
    void setGripperClose(bool close); 
    void setAudio(vector<float>);    
    string error();
    
    
    
    
    vector<Pixel> readImage();    
    vector<float> readLaser();
    vector<float> readLaserGripper();
    vector<float> readAudio();
    vector<float> readGPS();
    
  };
}

#endif // _GAZEBO_BUG_HPP
        
