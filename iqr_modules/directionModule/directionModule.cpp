#include <ctime>
#include <float.h>
#include <math.h>

#define _USE_MATH_DEFINES


#include "directionModule.hpp"


MAKE_MODULE_DLL_INTERFACE(iqrcommon::ClsDirectionModule, "Goal Direction Module")

iqrcommon::ClsDirectionModule::ClsDirectionModule() : ClsModule() {
  par_sigma=addDoubleParameter("_sigma", "Sigma", 0.2, 0, 100, 2, 
  				     "Sigma of  Direction  Cells", "General");

  
  var_location     = addInputFromGroup("_location","Current Location");
  var_ori    = addInputFromGroup("_ori", "Current Orientation");
  var_goal    = addInputFromGroup("_goal", "Goal Location");
 
  var_dir = addOutputToGroup("_dir", "Turnig Direction");
  var_dist = addOutputToGroup("_dist", "Distance To Goal");
  
  
}



/* INIT MODULE */
void iqrcommon::ClsDirectionModule::init(){
 
  width=var_location->getTarget()->getNrCellsHorizontal();
  height=var_location->getTarget()->getNrCellsVertical();
  lengthOri=var_ori->getTarget()->getNrCellsHorizontal();
  lengthDir=var_dir->getNrCellsHorizontal();
  
  checkSize(var_goal->getTarget(), width, height);
  checkSize(var_dist,1,1);
}


void  iqrcommon::ClsDirectionModule::getMaxXY(StateArray& stateArray, int width, 
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






void iqrcommon::ClsDirectionModule::update(){
  StateArray& location  = var_location->getTarget()->getStateArray();
  StateArray& goal  = var_goal->getTarget()->getStateArray();
  StateArray& ori  = var_ori->getTarget()->getStateArray();
  
  StateArray& dir = var_dir->getStateArray();
  StateArray& dist = var_dist->getStateArray();
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
  
  // Distance
  dist[0][0] = sqrt((xGoal-xLocation)*(xGoal-xLocation)
		    +(yGoal-yLocation)*(yGoal-yLocation));
  
  
  // Direction
  
  float goalAngle = atan((float)(xLocation-xGoal)/(float)(yLocation-yGoal));
  if(yGoal-yLocation>0){
    goalAngle +=M_PI;
  }  
  
  if(yGoal-yLocation<=0 && xGoal-xLocation>0){
    goalAngle +=2*M_PI;
  }
  
  //cout<< goalAngle<<endl;
  
  int tmp=-1;
  int oriIndex=-1;
  
  getMaxXY(ori,lengthOri,tmp,oriIndex);
  if (oriIndex<0)
    valid=false;
  
  goalAngle /=2*M_PI;
  goalAngle =1-goalAngle;
  float oriAngle = (float)oriIndex/(float)(lengthOri-1);
  
 

  float angleDistance  = max(min( goalAngle -oriAngle, 1-(goalAngle -oriAngle)),min(oriAngle-goalAngle, 1-(oriAngle-goalAngle)));
  
  
  float direction;
  if (angleDistance<0.05)
    direction=0;
  else if (angleDistance<0.1)
    direction=.1;
  else if (angleDistance<0.15)
    direction=.33;  
  else 
    direction=.45;

  
  float difference=goalAngle-oriAngle;
  if (difference > 0 && abs(difference) <= .5) 
    direction=1*abs(direction);
  else if (difference > 0 && abs(difference) > .5) 
    direction=-1*abs(direction); 
  else if (difference < 0 && abs(difference) <= .5) 
    direction=-1*abs(direction);
  else if (difference < 0 && abs(difference) > .5) 
    direction=1*abs(direction);
  
  direction+=0.5;
  
  dir[0]=0;
  
  
  float dirIndex = (1-direction)*lengthDir;
  if(valid)
    dir[0][(int)dirIndex]=1;
  
  //  cout<<goalAngle<<" "<<oriAngle<<" "<<difference<<" "<<angleDistance<<" "<<dirIndex<<endl;
  
}


void iqrcommon::ClsDirectionModule::cleanup(){
  // Make sure to shut everything down.
  std::cout << "iqrcommon::ClsDirectionModule::cleanup() \n";
}


/* CHECK WIDTH CONSISTENCY */
void iqrcommon::ClsDirectionModule::checkSize(ClsStateVariable* var,  int width, int height){
  if(var->getNrCellsHorizontal()!=width||var->getNrCellsVertical()!=height){
        ostringstream w_int;
        w_int<<width;
	ostringstream h_int;
        h_int<<height;
        throw ModuleError(label() + string(": ") + var->getLabel() +
                          string(" group must be of size ") + w_int.str()+"x"+h_int.str());
    }
}



