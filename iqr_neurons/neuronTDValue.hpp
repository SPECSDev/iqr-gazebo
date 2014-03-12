/****************************************************************************
 ** $Header$
 **
 ** $Author$ 
 ** 
 ** Created: Wed Jan 15 14:29:30 2003
 ** Time-stamp: <Sun Nov 09 2003 17:22:49 Mark Blanchard>
 **
 ** $Date$
 **
 ** $Description$
 **
 ** $Log$
 **
 *****************************************************************************/

#ifndef NEURONVALUE_HPP
#define NEURONVALUE_HPP

#include <Common/Item/neuron.hpp>

namespace iqrcommon {

    class ClsNeuronTDValue : public ClsNeuron
    {
    public:	
	ClsNeuronTDValue();

	void update();

    private:
      // Hide copy constructor.
      ClsNeuronTDValue(const ClsNeuronTDValue&);
      
      // Pointers to parameter objects.  
      ClsDoubleParameter *pAlphaPlus, *pAlphaMinus, *pGamma;
          
      // Pointers to state variables.	
      ClsStateVariable *pValue;	

      ClsStateVariable *pCurrentState;
      ClsStateVariable *pOldState;
      ClsStateVariable *pTotalReward;
	
    };
}

#endif
