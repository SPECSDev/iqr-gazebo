
#include "neuronTDValue.hpp"

// Interface for dynamic loading is built using a macro.
MAKE_NEURON_DLL_INTERFACE(iqrcommon::ClsNeuronTDValue, 
			  "TDValue Learning")

iqrcommon::ClsNeuronTDValue::ClsNeuronTDValue()
    : ClsNeuron(),
  pValue(0)
{    
  pAlphaPlus = addDoubleParameter("alphaPlus", "Plus Learning Rate",
				  0.5, 0.0, 1.0, 5, 
				  "Learning Rate for positive  value differences",
				  "General");
  
  pAlphaMinus = addDoubleParameter("alphaMinus", "Minus Learning Rate",
				  0.1, 0.0, 1.0, 5, 
				  "Learning Rate for positive  value differences",
				  "General");
  pGamma = addDoubleParameter("gamma", "Gamma",
				    1.0, 0.0, 1.0, 4, 
				    "Gamma of Value",
				    "General");
  
  // Add state variables.
  pOldState = addStateVariable("old", "Old State"); 
  pCurrentState = addStateVariable("curr", "Current State");
  pTotalReward = addStateVariable("reward", "Total Reward");
  pValue  = addOutputState("value", "Value");
}

void
iqrcommon::ClsNeuronTDValue::update()
{
  StateArray &state = getExcitatoryInput();
  StateArray &reward = getModulatoryInput();   

  
  StateArray &value   = pValue->getStateArray();
  StateArray &currentState = pCurrentState->getStateArray();
  StateArray &oldState = pOldState->getStateArray();
  StateArray &totalReward = pTotalReward->getStateArray();

  
  double alphaPlus = pAlphaPlus->getValue();
  double alphaMinus = pAlphaMinus->getValue();
  double gamma   = pGamma->getValue();
  

  //value[0]*=gamma;
  totalReward[0]+=reward[0];  
  value[0][value[0]<0.01]=0;
  
  
  //check if new state
  if(abs(currentState[0]-state[0]).sum()>0.001){
    oldState[0]=currentState[0];
    currentState[0]=state[0];
    
    double valueDiff= totalReward[0][0]
      + gamma*(value[0]*currentState[0]).sum()
      - (value[0]*oldState[0]).sum();

    double alpha;
    alpha=valueDiff>0?alphaPlus:alphaMinus;
    
    value[0]+=alpha*valueDiff*oldState[0];

    totalReward[0]=0;
  }
}
