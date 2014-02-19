#include "synapseHebbian.hpp"

// Interface for dynamic loading is built using a macro.
MAKE_SYNAPSE_DLL_INTERFACE(iqrcommon::ClsSynapseHebbian, 
			  "Hebbian Decay")

iqrcommon::ClsSynapseHebbian::ClsSynapseHebbian() : ClsSynapse() {    
    pPostSynapticActivity = addFeedbackInput("postact", "Post Synaptic Activity");

    pSynapticPotential = addOutputState("synpot", "Synaptic potential");

    pWeight = addStateVariable("weight", "Synaptic Weight");
    
    
    pPLR = addDoubleParameter("plrate",
			     "Potentiation Learning Rate",
			     .1,
			     0.0,	
			     1.0,
			     6,
			     "Potentiation Learning Rate");
    
    pDLR = addDoubleParameter("dlrate",
				  "Depression Learning Rate",
				  .1,
				  0.0,	
				  1.0,
				  6,
				  "Depression Learning Rate");
    
    pTau = addIntParameter("tau",
			     "Decay Time",
			     1,
			     0,	
			     100000,
			     "Decay Time");
}

void iqrcommon::ClsSynapseHebbian::update() {    
    StateArray &synIn = getInputState()->getStateArray();

    StateArray &postact = pPostSynapticActivity->getStateArray();
    StateArray &synpot = pSynapticPotential->getStateArray();

    StateArray &weight = pWeight->getStateArray();

    double potLRate = pPLR->getValue();
    double depLRate = pDLR->getValue();
    int tau = pTau->getValue();
    double decay=0;

    if (tau>0)
      decay= 1.0/(float)tau;
    
/* 
    weight[0] = weight[0] + postact[0]*synIn[0]*potLRate;
*/

/* with decay */
    weight[0] = weight[0] 
      + postact[0]*synIn[0]*potLRate
      -(postact[0]+synIn[0])*depLRate 
      - decay * weight[0];
    /* */
    
    weight[0][weight[0] < 0.0]=0.0;
    
    synpot[0] = weight[0] * synIn[0];

    
}




