/****************************************************************************
 ** $Header$
 **
 ** $Author: Mark Blanchard
 ** 
 ** $CreateDate: Sun Nov  2 23:08:00 2003
 ** $Date$
 **
 ** $Keywords:
 ** $Description: Apical dendrite synapse model.
 **
 *****************************************************************************/

#ifndef SYNAPSEHEBBIAN_HPP
#define SYNAPSEHEBBIAN_HPP

#include <Common/Item/synapse.hpp>

namespace iqrcommon {

    class ClsSynapseHebbian : public ClsSynapse
    {
    public:
	ClsSynapseHebbian();

	void update();

    private:
	// Hide copy constructor.
	ClsSynapseHebbian(const ClsSynapseHebbian&);



	ClsDoubleParameter *pPLR;
	ClsDoubleParameter *pDLR;
	ClsIntParameter *pTau;

	// Feedback inputs
	ClsStateVariable *pPostSynapticActivity;

	// Pointer to output state.
	ClsStateVariable *pSynapticPotential;
	ClsStateVariable* pWeight;
    };
}

#endif


