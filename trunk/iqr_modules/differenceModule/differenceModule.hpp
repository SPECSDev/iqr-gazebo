#ifndef DIFFERENCE_MODULE_HPP
#define DIFFERENCE_MODULE_HPP

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
#include "differenceModule.hpp"

namespace iqrcommon{
  class ClsDifferenceModule : public ClsModule{
  public:
    ClsDifferenceModule();
    void init();
    void update();
    void cleanup();
   
      
  private:
    ClsDifferenceModule(const ClsDifferenceModule&); 
    void checkSize(ClsStateVariable* var, int width, int height, string name = "");   
    void getMaxXY(StateArray& stateArray,  int width, int& x, int& y);
    int width,height;
    
    // Location
    StateVariablePtr *var_location;
    
    // Goal Location
    StateVariablePtr *var_goal;
    

    // Difference
    ClsStateVariable* var_diff;
    
  };
}
#endif
