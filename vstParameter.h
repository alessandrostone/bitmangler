// vstParameter.h: interface for the vstParameter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VSTPARAMETER_H__495CF2F6_C20C_4CC3_94AE_9AAFA3170F10__INCLUDED_)
#define AFX_VSTPARAMETER_H__495CF2F6_C20C_4CC3_94AE_9AAFA3170F10__INCLUDED_

#include <juce.h>

class vstParameter  
{
	public:
		vstParameter(unsigned int i, String name);
		~vstParameter();
		void setValue (float newValue);
		void setRange (float _minValue, float _maxValue);
		void setMaxValue (float _maxValue);
		void setMinValue (float _minValue);
		void setName (String name);
		void setIndex (int index);
		int getValueAsInt (int max);
		float getValue ();
		float getLastValue ();
		String getParameterName ();

	private:
		unsigned int parameterIndex;
		String parameterName;
		float currentValue;
		float lastValue;
		float minValue;
		float maxValue;
};

#endif // !defined(AFX_VSTPARAMETER_H__495CF2F6_C20C_4CC3_94AE_9AAFA3170F10__INCLUDED_)
