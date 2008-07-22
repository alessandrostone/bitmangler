#include "bitManglerFloat.h"

float setbit (float f, int bit)
{
  union
  {
	floatint i;
	float f;
  }mf;

  mf.f = f;
  mf.i = mf.i | (1<<bit);
  return (mf.f);
}

float clearbit (float f, int bit)
{
  union
  {
	floatint i;
	float f;
  }mf;

  mf.f = f;
  mf.i = mf.i &~ (1<<bit);
  return (mf.f);
}

float xorbit (float f, int bit, int b)
{
	union
	{
		floatint i;
		float f;
	}mf;

	mf.f = f;
	mf.i = mf.i ^ (b<<bit);
	return (mf.f);
}

float andbit (float f, int bit, int b)
{
	union
	{
		floatint i;
		float f;
	}mf;

	mf.f = f;
	mf.i = mf.i & (b<<bit);
	return (mf.f);
}

float orbit (float f, int bit, int b)
{
	union
	{
		floatint i;
		float f;
	}mf;

	mf.f = f;
	mf.i = mf.i | (b<<bit);
	return (mf.f);
}
