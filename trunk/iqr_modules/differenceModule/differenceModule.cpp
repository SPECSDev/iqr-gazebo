#include <ctime>
#include <float.h>
#include <math.h>

#define _USE_MATH_DEFINES


#include "differenceModule.hpp"


MAKE_MODULE_DLL_INTERFACE(iqrcommon::ClsDifferenceModule, "Difference Module")

iqrcommon::ClsDifferenceModule::ClsDifferenceModule() : ClsModule() {
    
  var_location     = addInputFromGroup("_location","Current Location");
  var_goal    = addInputFromGroup("_goal", "Goal Location");
  
  
  var_diff = addOutputToGroup("_diff", "Difference");
  
  
}



/* INIT MODULE */
void iqrcommon::ClsDifferenceModule::init(){
  width=var_location->getTarget()->getNrCellsHorizontal();
  height=var_location->getTarget()->getNrCellsVertical();
  
  checkSize(var_goal->getTarget(), width, height,"Goal Location");
  checkSize(var_diff, 2*width-1, 2*height-1);
}


void  iqrcommon::ClsDifferenceModule::getMaxXY(StateArray& stateArray, int width, 
					      int& x, 
					      int& y){
  bool valid;
  
  
  valarray<double> array = stateArray[0];
  
  float maxVal=-DBL_MAX, minVal=DBL_MAX;
  int maxIndex=-1;
  for (uint i=0; i<array.size(); ++i) {
    if (array[i] > maxVal) {
      maxIndex=i;
      maxVal=array[i];
    }
    if (array[i] < minVal)
      minVal=array[i];
  }
  
  
  valid=(maxVal>0) && (minVal!=maxVal);
 
    
  if(valid){
    x=maxIndex/width;
    y=maxIndex%width;
  }else{
    x=-1;
    y=-1;
  }
}






void iqrcommon::ClsDifferenceModule::update(){
  StateArray& location  = var_location->getTarget()->getStateArray();
  StateArray& goal  = var_goal->getTarget()->getStateArray();
    
  
  StateArray& diff = var_diff->getStateArray();
  bool valid =true;

  int xLocation =-1;
  int yLocation =-1;
  getMaxXY(location,width,xLocation, yLocation);
  if (xLocation<0)
    valid=false;
  
  //cout<<"location: "<<xLocation<<" "<<yLocation<<endl;

  int xGoal =-1;
  int yGoal =-1;
  getMaxXY(goal,width,xGoal, yGoal);
  if (xGoal<0)
    valid=false;
  //cout<<"goal: "<<xGoal<<" "<<yGoal<<endl;
  
  int diffX = xGoal-xLocation+width-1;
  int diffY = yGoal-yLocation+height-1;
  
  
  diff[0]=0;
  
  if(valid)
    diff[0][diffX*(2*width-1)+diffY]=1;
  
  //  cout<<goalAngle<<" "<<oriAngle<<" "<<difference<<" "<<angleDistance<<" "<<dirIndex<<endl;
  
}


void iqrcommon::ClsDifferenceModule::cleanup(){
  // Make sure to shut everything down.
  std::cout << "iqrcommon::ClsDifferenceModule::cleanup() \n";
}

/* CHECK WIDTH CONSISTENCY */
void iqrcommon::ClsDifferenceModule::checkSize(ClsStateVariable* var, 
					      int width, int height, string name){
  
  if (name.empty())
    name =  var->getLabel();
  
  if (height<0){
     if(var->getNrCellsHorizontal()*var->getNrCellsVertical()!=width){
        ostringstream w_int;
        w_int<<width;
        throw ModuleError(label() + string(": ") + name  +
                          string(" group must be of total size ") + w_int.str());
     }
  }else{
    if(var->getNrCellsHorizontal()!=width||var->getNrCellsVertical()!=height){
        ostringstream w_int;
        w_int<<width;
	ostringstream h_int;
        h_int<<height;
        throw ModuleError(label() + string(": ") + name +
                          string(" group must be of size ") + w_int.str()+"x"+h_int.str());
    }
  }
}



