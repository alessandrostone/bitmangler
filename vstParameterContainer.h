// vstParameterContainer.h: interface for the vstParameterContainer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VSTPARAMETERCONTAINER_H__15CDE4A0_30CF_4903_A338_A09A7BED016C__INCLUDED_)
#define AFX_VSTPARAMETERCONTAINER_H__15CDE4A0_30CF_4903_A338_A09A7BED016C__INCLUDED_

#include <juce.h>
#include "vstParameter.h"

class vstParameterContainer  
{
	public:
		vstParameterContainer();
		~vstParameterContainer();

	private:
		OwnedArray <vstParameter> param;
};

#endif // !defined(AFX_VSTPARAMETERCONTAINER_H__15CDE4A0_30CF_4903_A338_A09A7BED016C__INCLUDED_)
