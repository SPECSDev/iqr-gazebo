#ifndef DIRECITON_MODULE_HPP
#define DIRECITON_MODULE_HPP

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
#include "directionModule.hpp"

namespace iqrcommon{
  class ClsDirectionModule : public ClsModule{
  public:
    ClsDirectionModule();
    void init();
    void update();
    void cleanup();
   
      
  private:
    ClsDirectionModule(const ClsDirectionModule&);
    void checkSize(ClsStateVariable* var,  int width, int height); 
    void getMaxXY(StateArray& stateArray,  int width, int& x, int& y);
    int width;
    int height;
    int lengthOri;
    int lengthDir;
    
    ClsDoubleParameter *par_sigma;

    // Location
    StateVariablePtr *var_location;
    
    // Goal Location
    StateVariablePtr *var_goal;

    // Orientation
    StateVariablePtr *var_ori;
    
    // Goal direction
    ClsStateVariable* var_dir;
    ClsStateVariable* var_dist;
    
  };
}
#endif
