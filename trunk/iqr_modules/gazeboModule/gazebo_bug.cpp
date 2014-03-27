#include <iostream>
//#include <sstream>
#include <time.h>
#include <gazebo/math/MathTypes.hh>
#include <gazebo_bug.hpp>


/* Gazebo bug interface */
iqrcommon::GazeboBugInterface::GazeboBugInterface(){
  qMutex = new QMutex(); 
  current.speed_left = current.speed_right = 0;	
  current.closing=true;
  imageWidth=MAX_WIDTH_CAM;
  imageHeight=MAX_HEIGHT_CAM;
  comKey=DEFAULT_COM_KEY;
  hasGripper=true;
  
 
}

iqrcommon::GazeboBugInterface::~GazeboBugInterface(){
  Unsubscribe();
  if(!gazebo::transport::is_stopped()){
    gazebo::transport::fini();
    std::cout<<"FINI Gazebo Transport"<<std::endl;
  }
}

/*Open(const string& device): Opens the connection to Gazebo server */
bool iqrcommon::GazeboBugInterface::Open(const string& device, bool hasGripper){
    this->deviceName = device;	
    this->hasGripper = hasGripper;
    std::cout << device << std::endl;
    
    
    if(gazebo::transport::is_stopped()){
      cout<<"INIT Gazebo Transport !"<<endl;
      if(!gazebo::transport::init()){
	error_message = "Cannot initialize gazebo server";
	return 0;
      }
      gazebo::transport::run();
    }else{
      cout<<"Gazebo Transport ALREADY running!"<<endl;
    }
   
    this->node = gazebo::transport::NodePtr(new gazebo::transport::Node());
    this->node->Init();
    iqrcommon::GazeboBugInterface::Subscribe();
   
    iqrcommon::GazeboBugInterface::Publish();
    
    return true;
}

/* Subscribe(): subscribe to the topics to listen */
void iqrcommon::GazeboBugInterface::Subscribe(){
  string topic;
  /*Laser Scan*/
  topic= "/gazebo/default/";
  topic += this->deviceName + "/bug/laser/scan";  // subscribe to the laserscan
  std::cout << "Subscribing (listening) to: " << topic <<  " ... "<<std::flush;
  this->laserScanSub = this->node->Subscribe(topic, 
					     &iqrcommon::GazeboBugInterface::onScan, this);
  
  std::cout << "done!"<< std::endl;
  
  /*Laser Scan Target*/
  topic= "/gazebo/default/";
  topic += this->deviceName + "/bug/laser_target/scan";  // subscribe to the laserscan
  std::cout << "Subscribing (listening) to: " << topic <<  " ... "<<std::flush;
  this->laserScanTargetSub = this->node->Subscribe(topic, 
						    &iqrcommon::GazeboBugInterface::onScanTarget, this);
  
  std::cout << "done!"<< std::endl;

  if(hasGripper){
    /*Laser Scan Gripper*/
    topic= "/gazebo/default/";
    topic += this->deviceName + "/bug/laser_gripper/scan";  // subscribe to the laserscan
    std::cout << "Subscribing (listening) to: " << topic <<  " ... "<<std::flush;
    this->laserScanGripperSub = this->node->Subscribe(topic, 
						      &iqrcommon::GazeboBugInterface::onScanGripper, this);
    
    std::cout << "done!"<< std::endl;
  }
  /* Scan AUDIO*/
  topic= "/gazebo/default/";
  topic += this->comKey + "/bug/audio/scan";  // subscribe to the laserscan
  std::cout << "Subscribing (listening) to: " << topic <<  " ... "<<std::flush;
  this->scanAudioSub = this->node->Subscribe(topic, 
						    &iqrcommon::GazeboBugInterface::onScanAudio, this);
  
  std::cout << "done!"<< std::endl;

  /*Camera*/
  topic= "/gazebo/default/";

  topic += this->deviceName + "/bug/camera/image";  // subscribe to the camera
  //topic += this->deviceName + "/SDIC_Camera/link/camera/image";  // subscribe to the camera
  
  std::cout << "Subscribing (listening) to: " << topic <<  " ... "<<std::flush;
  this->cameraSub = this->node->Subscribe(topic, 
					  &iqrcommon::GazeboBugInterface::onImage, this);
  
  
  
  std::cout << "done!"<< std::endl;
}



/* Unsubscribe(): unsubscribe from a topic */
void iqrcommon::GazeboBugInterface::Unsubscribe()
{
  if(this->laserScanSub)
    this->laserScanSub->Unsubscribe();
  this->laserScanSub.reset();
  if(this->laserScanTargetSub)
    this->laserScanTargetSub->Unsubscribe();
  this->laserScanTargetSub.reset();
  
  if(this->hasGripper){
    if(this->laserScanGripperSub)
      this->laserScanGripperSub->Unsubscribe();
    this->laserScanGripperSub.reset();
  }
  if(this->scanAudioSub)
    this->scanAudioSub->Unsubscribe();
  this->scanAudioSub.reset();
  if(this->cameraSub)
    this->cameraSub->Unsubscribe();
  this->cameraSub.reset();
  
  this->motorPub.reset();
  
  if(this->hasGripper)
    this->jointPub.reset();
  
  this->audioPub.reset();
}

/* Publish(): opens a publisher to a topic */
void iqrcommon::GazeboBugInterface::Publish(){
  string topic;
  /*Motor*/
  topic = "/gazebo/default/";
  topic += this->deviceName + "/vel_cmd";  // publish to the joints of the bugs
  std::cout << "Publishing to: " << topic << " ...."<<std::flush;
  this->motorPub = this->node->Advertise<gazebo::msgs::Vector2d>(topic);
  this->motorPub->WaitForConnection();
  std::cout << "done!"<< std::endl;

  if(this->hasGripper){
    /*Grasp*/
    topic = "/gazebo/default/";
    topic += this->deviceName + "/joint_cmd";  // publish to the joints of the bugs
    std::cout << "Publishing to: " << topic << " ...."<<std::flush;
    this->jointPub = this->node->Advertise<gazebo::msgs::JointCmd>(topic);
    this->jointPub->WaitForConnection();
    std::cout << "done!"<< std::endl;
  }

  /*Audio*/
  topic = "/gazebo/default/";
  topic += this->comKey + "/bug/audio/scan";  // subscribe to the laserscan
  std::cout << "Publishing to: " << topic << " ...."<<std::flush;
  this->audioPub = this->node->Advertise<gazebo::msgs::LaserScanStamped>(topic);
  this->audioPub->WaitForConnection();
  std::cout << "done!"<< std::endl;
}


/* CLose(): Close the communication with Gazebo and clean */
void iqrcommon::GazeboBugInterface::Close(){
  setSpeed(0, 0);
  sleep(.1);
  
  
  
  if (this->node){
    this->node->Fini();
    this->node.reset();
  }
  Unsubscribe();
  
}

/* setSpeed(float left, float right): set the velocity of the wheels */
void iqrcommon::GazeboBugInterface::setSpeed(float left, float right){
  //   Cliping -1 1 might be important for challange. 
  //
  //   if(left > 1) left = 1;   // clip to 1 / -1
  //   else if(left < -1) left = -1;
  //   if(right > 1) right = 1;
  //   else if(right < -1) right = -1;
  

   
    qMutex->lock();
    if(current.speed_left == left && current.speed_right == right){
      qMutex->unlock();      
      return;
    }
    current.speed_left = left; current.speed_right = right;
    qMutex->unlock();
    
    velCmd.set_x(left);
    velCmd.set_y(right);
    motorPub->Publish(velCmd);  
    
    //std::cout << deviceName<<": Left speed: " << left*VEL_FACTOR << " m/s - Right speed: " << right*VEL_FACTOR << " m/s" << std::endl;
    
}

void  iqrcommon::GazeboBugInterface::setGripperClose(bool close){
  float graspForce;
  qMutex->lock();
  if(close){
    if(current.closing){
      qMutex->unlock();
      return;
    }
    graspForce=GRASP_FORCE;
    current.closing=true;
  }else{
    if(!current.closing){
      qMutex->unlock();     
      return;
    }
    graspForce=-GRASP_FORCE;
    current.closing=false;
  } 
  qMutex->unlock();
  jointCommandLeftFinger.set_name(this->deviceName + "::SDIC_Gripper::left_finger"); 
  jointCommandRightFinger.set_name(this->deviceName + "::SDIC_Gripper::right_finger");
  jointCommandLeftFingerTip.set_name(this->deviceName + "::SDIC_Gripper::left_finger_tip"); 
  jointCommandRightFingerTip.set_name(this->deviceName + "::SDIC_Gripper::right_finger_tip");

  jointCommandLeftFinger.set_force(graspForce); 
  jointCommandRightFinger.set_force(graspForce); 
  jointCommandLeftFingerTip.set_force(graspForce);
  jointCommandRightFingerTip.set_force(graspForce); 

  
  jointPub->Publish(jointCommandLeftFinger);  
  jointPub->Publish(jointCommandRightFinger);
  jointPub->Publish(jointCommandLeftFingerTip);  
  jointPub->Publish(jointCommandRightFingerTip);

 string commandClose;
 commandClose = close?"close":"open";
 std::cout << "Sending Gripper comand "<< commandClose <<std::endl;
}

void  iqrcommon::GazeboBugInterface::setAudio(vector<float> audio){
  gazebo::msgs::Set(this->emitAudio.mutable_time(), gazebo::common::Time(time(NULL)));
  gazebo::msgs::LaserScan *scan = this->emitAudio.mutable_scan();
  
  qMutex->lock();
  // Store the latest laser scans into laserMsg
  gazebo::msgs::Set(scan->mutable_world_pose(),
		    gazebo::math::Pose(current.gps[0],current.gps[1],0,0,0,current.gps[2]));
  qMutex->unlock();
  scan->set_frame(deviceName);
  scan->set_angle_min(0);
  scan->set_angle_max(0);
  scan->set_angle_step(3);
  
  scan->set_range_min(0);
  scan->set_range_max(1);

  scan->clear_ranges();
  scan->clear_intensities();
  
  for (int i = 0; i < MAX_RANGES_AUDIO; ++i){   
    scan->add_ranges(audio.at(i));      
  }
  
  audioPub->Publish(emitAudio);
 
  // std::cout << "Emiting Audio"<<std::endl;
}



/*
void  iqrcommon::GazeboBugInterface::setGripperUp(bool up){
  velocityTargetLifter = jointCommandLifter.mutable_velocity();
  if(up){
    if(current.lifting)
      return;
    velocityTargetLifter->set_target(UP_SPEED);
    current.lifting=true;
  }else{
    if(!current.lifting)
      return;
    velocityTargetLifter->set_target(-UP_SPEED);
    current.lifting=false;
  } 
  
  jointCommandLifter.set_name(this->deviceName + "::lifter_joint");
  jointPub->Publish(jointCommandLifter);  
  string commandUp;
  commandUp = up?"up":"down";
  std::cout << "Sending Gripper comand "<< commandUp <<std::endl;
}
*/

/*setGrasp(bool close): closes or opens gripper */
/*
void iqrcommon::GazeboBugInterface::setGripper(bool close, bool up){
  setGripperClose(close);
  setGripperUp(up);
}
*/


/* onScan(ConstLaserScanStampedPtr &): listen to the topic */
void iqrcommon::GazeboBugInterface::onScan(ConstLaserScanStampedPtr &_msg){
  //std::cout<<deviceName<<": onScan"<<std::endl;
  
  int i; 
  float range_max;    
  scan = _msg->scan();  // scan() returns a LaserScan object that can be accessed to retrieve the data
  range_max = scan.range_max(); 
  qMutex->lock();
  for (i = 0; i < MAX_RANGES; ++i){   
    current.ranges[i] = (range_max-scan.ranges(i))/range_max;
  }

  //Emualte GPS
  current.gps[0]=scan.world_pose().position().x();
  current.gps[1]=scan.world_pose().position().y();
  current.gps[2]=gazebo::msgs::Convert(scan.world_pose().orientation()).GetYaw();
  
  qMutex->unlock();
  //std::cout << "Range " << i << ": " << current.ranges[i] << endl;   
}

/* onScanTarget(ConstLaserScanStampedPtr &): listen to the topic */
void iqrcommon::GazeboBugInterface::onScanTarget(ConstLaserScanStampedPtr &_msg){
  // std::cout<<deviceName<<": onScanTarget"<<std::endl;
  
  int i; 
  float range_target_max;    
  scanTarget = _msg->scan();  // scan() returns a LaserScan object that can be accessed to retrieve the data
  range_target_max = scanTarget.range_max(); 
  qMutex->lock();
  for (i = 0; i < MAX_RANGES_TARGET; ++i){   
    current.rangesTarget[i] = (range_target_max-scanTarget.ranges(i))/range_target_max;
  }
  //std::cout << "Range " << i << ": " << current.ranges[i] << endl;   
  qMutex->unlock();
}


/* onScanGripper(ConstLaserScanStampedPtr &): listen to the topic */
void iqrcommon::GazeboBugInterface::onScanGripper(ConstLaserScanStampedPtr &_msg){
  // std::cout<<deviceName<<": onScanGripper"<<std::endl;
  
  int i; 
  float range_gripper_max;    
  scanGripper = _msg->scan();  // scan() returns a LaserScan object that can be accessed to retrieve the data
  range_gripper_max = scanGripper.range_max(); 
  qMutex->lock();
  for (i = 0; i < MAX_RANGES_GRIPPER; ++i){   
    current.rangesGripper[i] = (range_gripper_max-scanGripper.ranges(i))/range_gripper_max;
  }
  //std::cout << "Range " << i << ": " << current.ranges[i] << endl;   
  qMutex->unlock();
}

/*setComKey(const string& comKey): sets com Key */
void iqrcommon::GazeboBugInterface::setComKey(const string& comKey){
  this->comKey=comKey;
}

/* onScanAudio(ConstLaserScanStampedPtr &): listen to the topic */
void iqrcommon::GazeboBugInterface::onScanAudio(ConstLaserScanStampedPtr &_msg){
  

  scanAudio = _msg->scan();  
  
  std::string fromDevice = *scanAudio.release_frame();
  
  gazebo::math::Vector3 audioPos;
  audioPos.Set(scanAudio.world_pose().position().x(),scanAudio.world_pose().position().y(),0);
  
  qMutex->lock();
  gazebo::math::Vector3 bugPos;
  bugPos.Set(current.gps[0],current.gps[1],0);
  qMutex->unlock();
  
  
  float distance = bugPos.Distance(audioPos);
  
  if(distance > MIN_AUDIO_DISTANCE && distance < MAX_AUDIO_DISTANCE){
    distance= (MAX_AUDIO_DISTANCE-distance)/MAX_AUDIO_DISTANCE;
    
    vector<float> readings(MAX_RANGES_AUDIO);
    for (int i = 0; i < MAX_RANGES_AUDIO; ++i){   
      readings[i]=scanAudio.ranges(i)*distance;
    }
    qMutex->lock();
    currMap.insert(fromDevice,readings);
    qMutex->unlock();
  }
}

void  iqrcommon::GazeboBugInterface::onImage(ConstImageStampedPtr &_msg)
{
  // std::cout<<"onImage"<<std::endl;
  qMutex->lock();
  
  current.img.SetFromData(
      (unsigned char *)(_msg->image().data().c_str()),
      _msg->image().width(),
      _msg->image().height(),
      (gazebo::common::Image::PixelFormat)(_msg->image().pixel_format()));
  
  qMutex->unlock();
 
}

void iqrcommon::GazeboBugInterface::setImageFormat(int imageWidth, int imageHeight){
  this->imageWidth=imageWidth;
  this->imageHeight=imageHeight;
}

vector<Pixel> iqrcommon::GazeboBugInterface::readImage(){
  //std::cout<<"Read Image ...";
  vector<Pixel> readings;
  gazebo::common::Color imgPix;
  Pixel grpPix;
  
  qMutex-> lock(); 
  
  if(imageWidth!= MAX_WIDTH_CAM && imageHeight!= MAX_HEIGHT_CAM){
    current.img.Rescale(imageWidth, imageHeight);
    
    //cout<<"Rescaling: new size: "<<current.img.GetWidth()<<" x "<<current.img.GetHeight()<<endl;
  }
  

  
  for (int j = imageHeight-1;j>=0;--j){   
    for(int i = 0; i < imageWidth; ++i){
      imgPix = current.img.GetPixel(i,j);	
      grpPix.rh=imgPix[0]; grpPix.gs=imgPix[1]; grpPix.bv=imgPix[2]; 
      readings.push_back(grpPix);
    }
  }
  qMutex-> unlock();
  
  // std::cout<<"done!";
  return readings; 
}

/* readLaser(): fill the vector to send to iqr sensors group */
vector<float> iqrcommon::GazeboBugInterface::readLaser(){ 
  vector<float> readings;  
  qMutex->lock();
   
  for(int i = MAX_RANGES-1; i >= 0; --i){
    readings.push_back(current.ranges[i]);
  }
  qMutex->unlock();
  return readings;
}


/* readLaserTarget(): fill the vector to send to iqr sensors group */
vector<float> iqrcommon::GazeboBugInterface::readLaserTarget(){ 
  vector<float> readings;  
  qMutex->lock();   
  for(int i = MAX_RANGES_TARGET-1; i >= 0; --i){
    readings.push_back(current.rangesTarget[i]);
  }
  qMutex->unlock();
  return readings;
}


/* readLaserGripper(): fill the vector to send to iqr sensors group */
vector<float> iqrcommon::GazeboBugInterface::readLaserGripper(){ 
  vector<float> readings;  
  qMutex->lock();   
  for(int i = MAX_RANGES_GRIPPER-1; i >= 0; --i){ 
    readings.push_back(current.rangesGripper[i]);
  }
  qMutex->unlock();
  return readings;
}

/* readLaserAudio(): fill the vector to send to iqr sensors group */
vector<float> iqrcommon::GazeboBugInterface::readAudio(){ 
  vector<float> readings(MAX_RANGES_AUDIO);  
  qMutex->lock();   
  QMapIterator<std::string, vector<float> > iter(currMap);
  while(iter.hasNext()){
    iter.next();
    for(int i = 0; i < MAX_RANGES_AUDIO; ++i){
      readings[i]+=iter.value()[i];
    }
    //std::cout<<"Device : "<<deviceName<<" fromDevice:"<<iter.key()<<std::endl;
  }
  qMutex->unlock();
  return readings;
}



/* readLaser(): fill the vector to send to iqr sensors group */
vector<float> iqrcommon::GazeboBugInterface::readGPS(){ 
  vector<float> readings;  
  qMutex->lock();   
  for(int i = 0; i < MAX_GPS; ++i){
    readings.push_back(current.gps[i]);
  }
  qMutex->unlock();
  return readings;
}




/* error(): return a string with an error message */
string iqrcommon::GazeboBugInterface::error(){
    return error_message;
}
    
