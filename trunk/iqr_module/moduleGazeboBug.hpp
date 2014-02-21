#ifndef MODULE_GAZEBO_BUG_HPP
#define MODULE_GAZEBO_BUG_HPP

#include <iostream>
using std::cerr;
using std::endl;

#include <fstream>
using std::ofstream;


#include <cstdlib> // for exit function
#include <vector>   
#include <Common/Item/threadModule.hpp>
#include <string>
#include <sys/time.h>
#include "moduleGazeboBug.hpp"
#include <gazebo_bug.hpp>
#include <vector>

namespace iqrcommon{
  static int moduleGazeboBugNumber =-1;

  class ClsModuleGazeboBug : public ClsThreadModule{
  public:
    ClsModuleGazeboBug();
    void init();
    void update();
    void cleanup();
    
    moduleIcon getIcon();
    
  private:
    int bugNumber;
    ClsModuleGazeboBug(const ClsModuleGazeboBug&);
    void relax();
    
    void motorGrpToSpeed(StateArray& motor,  
			 float& transSpeed, 
			 float& leftSpeed);

    void vectorToGroup(vector<float> origin, StateArray& destination);
    vector<float> groupToVector( StateArray& origin);
    
    void imageToGroups(vector<Pixel> image, StateArray& red_hue,
		       StateArray& green_saturation,
		       StateArray& blue_value,
		       bool grayscale,
		       bool hsv);
    void  gpsToNavigation(vector<float> gps,
			  StateArray& placeCell,
			  StateArray& headCell);
      
    void RGBtoHSVPixel(float r, float g, float b, 
		       float& h, float& s, float& v);
    vector<Pixel> RGBtoHSV(vector<Pixel> rgb_image);
    vector<Pixel> RGBtoBW(vector<Pixel> rgb_image);
    
    void checkSize(ClsStateVariable* var, int width, int height);
    
    
    iqrcommon::GazeboBugInterface bug;
    
    struct timeval time_init;
    int elapsed_cicles;
    int scanId;
    bool sendZeroAudio;
    
    // * Parameters *

    // CommKey
    ClsStringParameter *par_comKey;


    // Camera 
    ClsBoolParameter *par_grayscale;
    ClsBoolParameter *par_hsv;
        
    // Motor
    //ClsBoolParameter *par_softWTA;
    ClsDoubleParameter *par_transSpeed;
    ClsDoubleParameter *par_rotSpeed;
    int motorWidth, motorHeight;
    
    //Navigation
    int placeCellWidth,placeCellHeight,headCellWidth;
    
    ClsDoubleParameter *par_placeXOffset;
    ClsDoubleParameter *par_placeYOffset;
    
    ClsDoubleParameter *par_placeXMax;
    ClsDoubleParameter *par_placeYMax;
    
    ClsDoubleParameter *par_placeSigma;
    
    ClsDoubleParameter *par_headSigma;
        
    

    // System Parameters
    ClsStringParameter *device_name;
    ClsIntParameter *par_cps;
    

    // * State Variables *
    // Camera
    ClsStateVariable *var_image_RH;
    ClsStateVariable *var_image_GS;
    ClsStateVariable *var_image_BV;
    
    // Sensors
    ClsStateVariable *var_proximity;
    ClsStateVariable *var_target;   
    ClsStateVariable *var_proximityGripper;
    
    
    // Motor
    StateVariablePtr *var_motor;

    // Grasp
    StateVariablePtr *var_gripper;
    
    // Navigation
    ClsStateVariable* var_placeCell;
    ClsStateVariable* var_headCell;
    
    // GPS
    ClsStateVariable *var_gps;

    // AUDIO
    ClsStateVariable *var_scanAudio;
    StateVariablePtr *var_emitAudio;
  };
}
#endif
