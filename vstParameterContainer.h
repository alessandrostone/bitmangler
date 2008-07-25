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
		void addParameter (vstParameter *ptr);
		void addParameter (unsigned int pIndex, String pName, float minValue, float maxValue);
		vstParameter getParameter (unsigned int pIndex);

		float getValue (unsigned int pIndex);
		void setValue (unsigned int pIndex, float newValue, bool notifyHost=false);
		String getParameterName (unsigned int pIndex);
		String getParameterText (unsigned int pIndex);

	private:
		vstParameter *_getParameter (unsigned int pIndex);
		OwnedArray <vstParameter> param;
};

#endif // !defined(AFX_VSTPARAMETERCONTAINER_H__15CDE4A0_30CF_4903_A338_A09A7BED016C__INCLUDED_)
