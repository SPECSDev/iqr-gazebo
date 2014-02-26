#include <ctime>
#include <float.h>


#define _USE_MATH_DEFINES


#include "moduleGazeboBug.hpp"
#include <ModuleIcon_bug.h>

#define MIN_3(a, b, c)        MIN(MIN(a, b), c)
#define MAX_3(a, b, c)        MAX(MAX(a, b), c)


MAKE_MODULE_DLL_INTERFACE(iqrcommon::ClsModuleGazeboBug, "Gazebo Bug Module")

iqrcommon::ClsModuleGazeboBug::ClsModuleGazeboBug() : ClsThreadModule() {
    // General parameters    
    device_name = addStringParameter("device_name", "Bug name",
                                     "iqrGazebo", true, false,
                                     "Name of the bug to subscribe", "General");
    
    bugNumber=moduleGazeboBugNumber;
    moduleGazeboBugNumber++;
    cout<<"bugNumber: "<<bugNumber<<endl;

    par_cps = addIntParameter("cps", "Cycles/Second", 25, 0, 1000, 
                              "Bug update cycles per second", "General");   
    
    par_comKey = addStringParameter("comkey", "Communication Key","default_com_key",
				    true,false,"Key used for comunication", "General");   

    
    // Camera parameters
    par_grayscale = addBoolParameter("grayscale", "Grayscale", false, 
				     "Capture grayscale images","Image");
    par_hsv       = addBoolParameter("hsv", "HSV", false, "Use HSV colorspace", "Image");
    
    // Motor parameters
    //par_softWTA = addBoolParameter("softWTA", "Soft WTA", false, 
    //"Use soft WTA colorspace", "Motor");
    par_transSpeed     = addDoubleParameter("transSpeed", "Translationa Speed", 1, 0, 1000, 2, 
					    "Translational Speed ", "Motor");
    par_rotSpeed    = addDoubleParameter("rotSpeed", "Rotational Speed", 0.1, 0, 1000, 2, 
					 "Rotationals Speed", "Motor");
    // Navigation Parameters
 
    par_placeXOffset = addDoubleParameter("placeXOffset", "Place X Offset", 0, 0, 1000, 2, 
					    "Offset Coordinate System X", "Navigation");
    par_placeYOffset = addDoubleParameter("placeYOffset", "Place Y Offset", 0, 0, 1000, 2, 
					    "Offset Coordinate System Y", "Navigation");
    
    par_placeXMax = addDoubleParameter("placeXMax", "Place X Max", 10, 0, 1000, 2, 
				       "Max Coordinate System X", "Navigation");
    par_placeYMax =  addDoubleParameter("placeYMax", "Place Y Max", 10, 0, 1000, 2, 
					    "Max Coordinate System Y", "Navigation");
    
    par_placeSigma= addDoubleParameter("placeSigma", "Place Cells Sigma", 0.2, 0, 100, 2, 
				       "Sigma of Place Cells", "Navigation");
    
    par_headSigma=addDoubleParameter("headSigma", "HD Cells Sigma", 0.2, 0, 100, 2, 
				     "Sigma of Head Direction  Cells", "Navigation");

    // Group to motors
    var_motor     = addInputFromGroup("_motor", "Motor (square)");
    // Group to gripper
    var_gripper    = addInputFromGroup("_gripper", "Gripper");
    
    // Sensors to group
    var_proximity = addOutputToGroup("_proximity", "Proximity sensors (32x1)"); 
    var_target = addOutputToGroup("_target", "Target sensors (32x1)");  
    var_proximityGripper = addOutputToGroup("_proximityGripper", "Proximity sensors Gripper(16x1)");
    
    // Sensors to group
    var_gps = addOutputToGroup("_gps", "GPS sensors (3x1)");
    
    // Camera to groups
    var_image_RH  = addOutputToGroup("_image_RH", "Red/Hue");
    var_image_GS  = addOutputToGroup("_image_GS", "Green/Saturation");
    var_image_BV  = addOutputToGroup("_image_BV", "Blue/Value");

    // Nvigation to groups
    var_placeCell = addOutputToGroup("placeCell", "Place Cells");
    var_headCell = addOutputToGroup("headCell", "Head Direction Cells");
    
    // Audio to groups    
    var_scanAudio = addOutputToGroup("_audio_in", "Read Audio");
    // Group to audio
    var_emitAudio = addInputFromGroup("_audio_out", "Emit Audio");

    sendZeroAudio=false;

 }



/* INIT MODULE */
void iqrcommon::ClsModuleGazeboBug::init(){
  checkSize(var_proximity,MAX_RANGES,1);
  checkSize(var_target,MAX_RANGES_TARGET,1);
  checkSize(var_proximityGripper,MAX_RANGES_GRIPPER,1);
  checkSize(var_gps, MAX_GPS,1);
  checkSize(var_scanAudio, MAX_RANGES_AUDIO,-1);
  checkSize(var_emitAudio->getTarget(), MAX_RANGES_AUDIO,-1);
  
  // Check Camera size
  int widthCam = var_image_RH->getNrCellsHorizontal();
  int heightCam = var_image_RH->getNrCellsVertical();
  
  if(widthCam > MAX_WIDTH_CAM || heightCam > MAX_HEIGHT_CAM){
    ostringstream w_int; w_int<<MAX_WIDTH_CAM;
    ostringstream h_int; h_int<<MAX_HEIGHT_CAM;
    throw ModuleError(string("Module ")+label()
   		      +string(": The WidthxHeight product must be inferior to")
		      +w_int.str() +string("x")+h_int.str());
  }
  checkSize(var_image_RH, widthCam, heightCam);
  checkSize(var_image_GS, widthCam, heightCam);
  checkSize(var_image_BV, widthCam, heightCam);
  
  // ostringstream bugNumber_int;
  // bugNumber_int<<bugNumber;
  // if(!bug.Open(device_name->getValue()+bugNumber_int.str()))
 

  //Navigation
  placeCellWidth=var_placeCell->getNrCellsHorizontal();
  placeCellHeight=var_placeCell->getNrCellsVertical();
  headCellWidth=var_headCell->getNrCellsHorizontal();

  //Motor
  motorWidth=var_motor->getTarget()->getNrCellsHorizontal();
  motorHeight=var_motor->getTarget()->getNrCellsVertical();
  
  bug.setComKey(par_comKey->getValue());
  bug.setImageFormat(widthCam,heightCam);

  if(!bug.Open(device_name->getValue()))
    throw ModuleError(string("Module ")+label()+string(":")+bug.error());
  
  
  
 

  gettimeofday(&time_init, NULL);
  elapsed_cicles = 0;
}

void iqrcommon::ClsModuleGazeboBug::relax(){
  struct timeval time_now;
  double elapsed_time, next_computation_time, delta;
  int cps = par_cps->getValue();
  struct timespec sleep_time;
  
  if(cps){            // User specified a cycles per second value
    gettimeofday(&time_now, NULL);
    
    elapsed_time = (time_now.tv_sec - time_init.tv_sec) +
      (time_now.tv_usec - time_init.tv_usec)/1e6;
    
    next_computation_time = (double)elapsed_cicles/cps;
    delta = next_computation_time - elapsed_time;
    
    if(delta > 0){
      sleep_time.tv_sec = (time_t)floor(delta);
      delta -= floor(delta);
      sleep_time.tv_nsec = (long)(delta*1e9);
      nanosleep(&sleep_time, NULL);
    }
    ++elapsed_cicles;
  }
}

void  iqrcommon::ClsModuleGazeboBug::gpsToNavigation(vector<float> gps,
						     StateArray& placeCell,
						     StateArray& headCell){
   


  float xoffset = par_placeXOffset->getValue();
  float yoffset = par_placeYOffset->getValue();
  
  float xmax =par_placeXMax->getValue();
  float ymax =par_placeYMax->getValue();
  
  float placeSigma =par_placeSigma->getValue();
  float headSigma =par_headSigma->getValue();
  
  float xpos = (gps[0] - xoffset)/xmax;
  float ypos = (gps[1] - yoffset)/ymax;
  float rot = (gps[2]+M_PI)/2/M_PI;
  
  float xcell = 0;
  float ycell = 0;
  
  int length = placeCellWidth*placeCellHeight;
  for(int ii=0; ii<length; ii++){
    xcell = (float)(ii%placeCellWidth-placeCellWidth/2)/(float)(placeCellWidth/2);
    ycell =  -(float)(ii/placeCellWidth-placeCellHeight/2)/(float)(placeCellHeight/2);
    //  cout<<"Cell "<<ii<<" xycell  "<< xcell<<" "<< ycell<<" pos "<< xpos <<" "<<ypos<< endl;
    //calculate place cell
    placeCell[0][ii]=exp(-(((xcell-xpos)/placeSigma)*((xcell-xpos)/placeSigma) +
			   ((ycell-ypos)/placeSigma)*((ycell-ypos)/placeSigma)));
  }
  
 
  placeCell[0]/=placeCell[0].max();
  
  float distance;
  for(int ii=0; ii<headCellWidth; ii++){
    xcell =(float)ii/(float)(headCellWidth);
    distance = MAX(MIN( xcell -rot, 1-(xcell -rot)),MIN(rot-xcell, 1-(rot-xcell)));
    //cout<<"Cell "<<ii<<" "<< xcell<<" rot "<<rot<< endl;
    headCell[0][ii]=exp(-((distance)/headSigma)*((distance)/headSigma));
  }
  // cout<<"rot"<<rot<<endl;
  headCell[0]/=headCell[0].max();

  
}
void  iqrcommon::ClsModuleGazeboBug::motorGrpToSpeed(StateArray& motor,  
						     float& transSpeed, 
						     float& rotSpeed){
  bool valid;
  
  float transSpeedMax=par_transSpeed->getValue();
  float rotSpeedMax=par_rotSpeed->getValue(); 
  
 //  float rotNoise=transNoise=1;     	
 //  rNoise =  ((rand()% 3)-1)*rotNoise;
 //  tNoise =  ((rand()% 3)-1)*transNoise;
  
  qmutexThread->lock();
  valarray<double> motorArray = motor[0];
  
  float maxVal=-DBL_MAX, minVal=DBL_MAX;
  int maxIndex=-1;
  for (uint i=0; i<motorArray.size(); ++i) {
    if (motorArray[i] > maxVal) {
      maxIndex=i;
      maxVal=motorArray[i];
    }
    if (motorArray[i] < minVal)
      minVal=motorArray[i];
  }
  qmutexThread->unlock();

  valid=(maxVal>0) && (minVal!=maxVal);
 
    
  if(valid){
    transSpeed=-(float)(maxIndex/motorWidth-motorWidth/2)/(float)(motorWidth/2)*transSpeedMax;//+tNoise;
    rotSpeed=-(float)(maxIndex%motorWidth-motorHeight/2)/(float)(motorHeight/2)*rotSpeedMax; // +rNoise;
  }else{
    transSpeed=0;
    rotSpeed=0;
  }
}




void iqrcommon::ClsModuleGazeboBug::vectorToGroup(vector<float> origin, 
						    StateArray& destination){
  
  qmutexThread->lock();
  for(int i = 0; i < (int)origin.size(); ++i)
      destination[0][i] = origin[i];
  qmutexThread->unlock();
}

vector<float> iqrcommon::ClsModuleGazeboBug::groupToVector( StateArray& origin){
  vector<float> destination((int)origin[0].size());
  
  for(int i = 0; i < (int)origin[0].size(); ++i)
      destination[i] = origin[0][i];
  
  return destination;
}


void iqrcommon::ClsModuleGazeboBug::imageToGroups(vector<Pixel> image, 
						    StateArray& red_hue,
						    StateArray& green_saturation,
						    StateArray& blue_value, 
						    bool grayscale, 
						    bool hsv){
  
  if(grayscale)
    image=RGBtoBW(image);
  else if(hsv)
    image=RGBtoHSV(image);
  
  

  qmutexThread->lock();
  for(int i = 0; i < (int)image.size(); ++i){
    red_hue[0][i] = image[i].rh;
    green_saturation[0][i] = image[i].gs;
    blue_value[0][i] = image[i].bv;
  }
  qmutexThread->unlock();

}

void iqrcommon::ClsModuleGazeboBug::update(){
    relax(); 
    StateArray& motor     = var_motor->getTarget()->getStateArray();
    StateArray& gripper    = var_gripper->getTarget()->getStateArray();
    
    StateArray& proximity = var_proximity->getStateArray(); 
    StateArray& target = var_target->getStateArray(); 
    StateArray& proximityGripper = var_proximityGripper->getStateArray(); 
    StateArray& scanAudio = var_scanAudio->getStateArray();
    StateArray& emitAudio  = var_emitAudio->getTarget()->getStateArray();
    StateArray& gps = var_gps->getStateArray();
    
    StateArray& image_RH  = var_image_RH->getStateArray(); 
    StateArray& image_GS  = var_image_GS->getStateArray(); 
    StateArray& image_BV  = var_image_BV->getStateArray();
    
    bool grayscale = par_grayscale->getValue();
    bool hsv = par_hsv->getValue();
 
    StateArray &placeCell = var_placeCell->getStateArray();
    StateArray &headCell = var_headCell->getStateArray();
    
    /* Update Speed */
    float transSpeed, rotSpeed;
    motorGrpToSpeed(motor, 
		    transSpeed, rotSpeed);
    bug.setSpeed(transSpeed, rotSpeed);
    
    /* Set gripper*/
    //    bug.setGripper(gripper[0][0]>0.5,gripper[0][1]>.5);
      
    bug.setGripperClose(gripper[0][0]>0.5);
    
    /* Read sensor data */
    vectorToGroup(bug.readLaser(), proximity);
    vectorToGroup(bug.readLaserTarget(), target);
    vectorToGroup(bug.readLaserGripper(), proximityGripper);
  
    /* Read Audio */
    vectorToGroup(bug.readAudio(), scanAudio);
    

    /* Emit Audio */
   
    if(emitAudio[0].max()>0.00001){   
      sendZeroAudio=false;
      bug.setAudio(groupToVector(emitAudio));
    }else if(!sendZeroAudio){
       sendZeroAudio=true;
       bug.setAudio(groupToVector(emitAudio));
    }
    /* Read gps data */
    vector<float> gpsReading = bug.readGPS();
    gpsReading[0]+=par_placeXMax->getValue();
    gpsReading[1]+=par_placeYMax->getValue();
    gpsReading[2]+=M_PI;
    vectorToGroup(gpsReading, gps);

    /* Set Navigation Cells */
    gpsToNavigation(bug.readGPS(), placeCell, headCell);
    

    /* Read image from cmaera */
    imageToGroups(bug.readImage(),image_RH, image_GS, image_BV,grayscale,hsv);

  
}


void iqrcommon::ClsModuleGazeboBug::cleanup(){
  // Make sure to shut everything down.
    std::cout << "iqrcommon::ClsModuleGazeboBug::cleanup() \n";
    bug.Close();  
    
}

/* ICON */
moduleIcon iqrcommon::ClsModuleGazeboBug::getIcon() {
    moduleIcon mi(ModuleIcon_bug_png_data,  ModuleIcon_bug_png_len, 3 ,5);
    return mi;
} 

/* CHECK WIDTH CONSISTENCY */
void iqrcommon::ClsModuleGazeboBug::checkSize(ClsStateVariable* var,  int width, int height){
  if (height<0){
     if(var->getNrCellsHorizontal()*var->getNrCellsVertical()!=width){
        ostringstream w_int;
        w_int<<width;
        throw ModuleError(label() + string(": ") + var->getLabel() +
                          string(" group must be of total size ") + w_int.str()+"x");
     }
  }else{
    if(var->getNrCellsHorizontal()!=width||var->getNrCellsVertical()!=height){
        ostringstream w_int;
        w_int<<width;
	ostringstream h_int;
        h_int<<height;
        throw ModuleError(label() + string(": ") + var->getLabel() +
                          string(" group must be of size ") + w_int.str()+"x"+h_int.str());
    }
  }
}


// Adapted from http://en.wikipedia.org/wiki/HSL_and_HSV#Conversion_from_RGB_to_HSL_or_HSV
void iqrcommon::ClsModuleGazeboBug::RGBtoHSVPixel(float r, float g, float b, 
                          float& h, float& s, float& v){
    float min, max, delta;

    min = MIN_3(r, g, b);
    max = MAX_3(r, g, b);
    delta = max-min;

    // value
    v = max;

    // saturation
    if(max != 0) s = delta / max;
    else s = 0;

    // hue
    if(!delta) h = 0;
    else if(r == max) h = (g-b) / delta;
    else if(g == max) h = (b-r) / delta + 120/60; 
    else              h = (r-g) / delta + 240/60; 

    h *= 60;               // degrees
    if(h < 0) h += 360;   // positive values
    h /= 360;              // Normalized
}

vector<Pixel> iqrcommon::ClsModuleGazeboBug::RGBtoHSV(vector<Pixel> rgb_image){
    vector<Pixel> hsv_image(rgb_image.size());

    for(int i = 0; i < (int)rgb_image.size(); ++i)
      RGBtoHSVPixel(rgb_image[i].rh, rgb_image[i].gs, rgb_image[i].bv,
                    hsv_image[i].rh, hsv_image[i].gs, hsv_image[i].bv);

    return hsv_image;
}

vector<Pixel> iqrcommon::ClsModuleGazeboBug::RGBtoBW(vector<Pixel> rgb_image){
    vector<Pixel> bw_image(rgb_image.size());
    for(int i = 0; i < (int)rgb_image.size(); ++i){
	bw_image[i].rh=bw_image[i].gs=bw_image[i].bv=
	  0.2126*rgb_image[i].rh+0.7512*rgb_image[i].gs+0.0722*rgb_image[i].bv;
    }
    return bw_image;
}
