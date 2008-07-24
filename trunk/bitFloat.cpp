#include "bitFloat.h"

bitFloat::bitFloat (float v)
{
	currentValue.f = v;
}

void bitFloat::setValue (float v)
{
	currentValue.f = v;
}

void bitFloat::setbit (unsigned int bit)
{
	currentValue.i = currentValue.i | (1<<bit);
}

void bitFloat::clearbit (unsigned int bit)
{
	currentValue.i = currentValue.i &~ (1<<bit);
}

void bitFloat::xorbit (unsigned int bit, int b)
{
	currentValue.i = currentValue.i ^ (b<<bit);
}

void bitFloat::andbit (unsigned int bit, int b)
{
	currentValue.i = currentValue.i & (b<<bit);
}

void bitFloat::orbit (unsigned int bit, int b)
{
	currentValue.i = currentValue.i | (b<<bit);
}

String bitFloat::getBinaryString (floatint b, int d)
{
	String r;
	while (--d >= 0)
	{
		r << String::formatted (T("%d"), ((b >> d) & 1));
	}

	return (r);
}

String bitFloat::getFloatAsString ()
{
	floatint e,m;
	String str;

	if (currentValue.i & SIGN_FLT)
	{
		str << T("-:");
	}
	else
	{
		 str << T("+:");
	}

	currentValue.i &= ~SIGN_FLT;
	e = currentValue.i >> MANT_FLT;
	str << getBinaryString (e, BITS_FLT-1-MANT_FLT);
	str << T(":");

	m	= (currentValue.i & (((floatint)1 << MANT_FLT) - 1));

	str << getBinaryString(m, MANT_FLT);

	return (str);
}

floatint bitFloat::getLongValue ()
{
	return (currentValue.i);
}