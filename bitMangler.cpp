#include <juce.h>
#include "bitMangler.h"
#include "bitManglerEditor.h"

AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DemoJuceFilter();
}

DemoJuceFilter::DemoJuceFilter()
{
    zeromem (&lastPosInfo, sizeof (lastPosInfo));
    lastPosInfo.timeSigNumerator = 4;
    lastPosInfo.timeSigDenominator = 4;
    lastPosInfo.bpm = 120;
	editor = 0;
	processing = true;
	xorProcessing = andProcessing = clearProcessing = setProcessing = false;
	currentSample = 0.0f;
	bufferCycle = 0;
	clearTable();
}

DemoJuceFilter::~DemoJuceFilter()
{
}

const String DemoJuceFilter::getName() const
{
    return "Juce Demo Filter";
}

int DemoJuceFilter::getNumParameters()
{
    return (kParams);
}

float DemoJuceFilter::getParameter (int index)
{
	Logger::writeToLog (T("getParameter(): ") + String(index));

	switch (index)
	{
		case kXorToggle:
			return (float)xorProcessing;
		case kAndToggle:
			return (float)andProcessing;
		case kClearToggle:
			return (float)clearProcessing;
		case kSetToggle:
			return (float)setProcessing;
		case kXorMin:
			return (float)getXorFirst()/32;
		case kXorMax:
			return (float)getXorLast()/32;
		case kAndMin:
			return (float)getAndFirst()/32;
		case kAndMax:
			return (float)getAndLast()/32;
		case kClearMin:
			return (float)getClearFirst()/32;
		case kClearMax:
			return (float)getClearLast()/32;
		case kSetMin:
			return (float)getSetFirst()/32;
		case kSetMax:
			return (float)getSetLast()/32;

		default:
			return (0.0);
	}

	return (0.0);
}

void DemoJuceFilter::setParameter (int index, float newValue)
{
	Logger::writeToLog (T("setParameter(): ") + String(index) + T(" value: ") + String(newValue*32));
	int x=0;
	int p;
    switch (index)
	{
		case kXorToggle:
			if (newValue == 1.0f)
				xorProcessing = true;
			else
				xorProcessing = false;
			break;

		case kAndToggle:
			if (newValue == 1.0f)
				andProcessing = true;
			else
				andProcessing = false;
			break;

		case kClearToggle:
			if (newValue == 1.0f)
				clearProcessing = true;
			else
				clearProcessing = false;
			break;

		case kSetToggle:
			if (newValue == 1.0f)
				setProcessing = true;
			else
				setProcessing = false;
			break;

		case kXorMod:
			if (newValue == 1.0f)
				xorWith = true;
			else
				xorWith = false;
			break;

		case kAndMod:
			if (newValue == 1.0f)
				andWith = true;
			else
				andWith = false;
			break;

		case kXorMin:
			p = getXorLast();
			clearXorTable();
			for (x=(int)(newValue*32); x<=p; x++)
			{
				setXorBit (x, xorWith);
			}
			break;

		case kXorMax:
			p = getXorFirst();
			clearXorTable();
			for (x=p; x<=(int)(newValue*32); x++)
			{
				setXorBit (x, xorWith);
			}
			break;

		case kAndMin:
			p = getAndLast();
			clearAndTable();
			for (x=(int)(newValue*32); x<=p; x++)
			{
				setAndBit (x, andWith);
			}
			break;

		case kAndMax:
			p = getAndFirst();
			clearAndTable();
			for (x=p; x<=(int)(newValue*32); x++)
			{
				setAndBit (x, andWith);
			}
			break;

		case kSetMin:
			p = getSetLast();
			clearSetTable();
			for (x=(int)(newValue*32); x<=p; x++)
			{
				setSetBit (x);
			}
			break;

		case kSetMax:
			p = getSetFirst();
			clearSetTable();
			for (x=p; x<=(int)(newValue*32); x++)
			{
				setSetBit (x);
			}
			break;

		case kClearMin:
			p = getClearLast();
			clearClearTable();
			for (x=(int)(newValue*32); x<=p; x++)
			{
				setClearBit (x);
			}
			break;

		case kClearMax:
			p = getClearFirst();
			clearClearTable();
			for (x=p; x<=(int)(newValue*32); x++)
			{
				setClearBit (x);
			}
			break;
		default:
			break;
	}
	sendChangeMessage (this);
}

const String DemoJuceFilter::getParameterName (int index)
{
	Logger::writeToLog (T("getParameterName(): ") + String(index));

    switch (index)
	{
		case kXorToggle:
			return (T("XOR Process"));
		case kAndToggle:
			return (T("AND Process"));
		case kClearToggle:
			return (T("CLEAR Process"));			
		case kSetToggle:
			return (T("SET Process"));
		case kXorMod:
			return (T("XOR Modulator"));
		case kAndMod:
			return (T("AND Modulator"));
		case kXorMin:
			return (T("XOR Range Start"));
		case kXorMax:
			return (T("XOR Range End"));
		case kAndMin:
			return (T("AND Range Start"));
		case kAndMax:
			return (T("AND Range End"));
		case kClearMin:
			return (T("CLEAR Range Start"));
		case kClearMax:
			return (T("CLEAR Range End"));
		case kSetMin:
			return (T("SET Range Start"));
		case kSetMax:
			return (T("SET Range End"));
		case kProces:
			return (T("Process"));
		default:
			break;
	}

	return (T("process"));
}

const String DemoJuceFilter::getParameterText (int index)
{
    switch (index)
	{
		case kXorToggle:
			return (String(xorProcessing));
		case kAndToggle:
			return (String(andProcessing));
		case kClearToggle:
			return (String(clearProcessing));
		case kSetToggle:
			return (String(setProcessing));
		case kXorMod:
			return (String((int)xorWith));
		case kAndMod:
			return (String((int)andWith));
		case kXorMin:
			return (String(getXorFirst()));
		case kXorMax:
			return (String(getXorLast()));
		case kAndMin:
			return (String(getAndFirst()));
		case kAndMax:
			return (String(getAndLast()));
		case kClearMin:
			return (String(getClearFirst()));
		case kClearMax:
			return (String(getClearLast()));
		case kSetMin:
			return (String(getSetFirst()));
		case kSetMax:
			return (String(getSetLast()));
		case kProces:
			return (String(processing));
		default:
			break;
	}

	return (String::empty);
}

const String DemoJuceFilter::getInputChannelName (const int channelIndex) const
{
    return String (channelIndex + 1);
}

const String DemoJuceFilter::getOutputChannelName (const int channelIndex) const
{
    return String (channelIndex + 1);
}

bool DemoJuceFilter::isInputChannelStereoPair (int index) const
{
    return false;
}

bool DemoJuceFilter::isOutputChannelStereoPair (int index) const
{
    return false;
}

bool DemoJuceFilter::acceptsMidi() const
{
    return false;
}

bool DemoJuceFilter::producesMidi() const
{
    return false;
}

void DemoJuceFilter::prepareToPlay (double sampleRate, int samplesPerBlock)
{
}

void DemoJuceFilter::releaseResources()
{
	clearTable();
}

void DemoJuceFilter::processBlock (AudioSampleBuffer& buffer,
                                   MidiBuffer& midiMessages)
{
	if (!isProcessing())
		return;

	for (int channel = 0; channel < getNumInputChannels(); ++channel)
	{
		float *p = buffer.getSampleData (channel);
		int size = buffer.getNumSamples() / buffer.getNumChannels();

		for (int x=0; x<size; x++)
		{
			currentSample = *(p+x);

			/* conversion */

			*(p+x) = currentConvertedSample = process (*(p+x));

			if (x == 2 && channel == 0 && bufferCycle == 0 && editor)
			{
				sendChangeMessage (0);
			}
		}
	}

	bufferCycle++;

	if (bufferCycle == 20)
		bufferCycle = 0;
}

AudioProcessorEditor* DemoJuceFilter::createEditor()
{
    return (editor = new bitManglerEditor (this));
}

void DemoJuceFilter::getStateInformation (MemoryBlock& destData)
{
    XmlElement xmlState (T("BITMUNGLER"));
    xmlState.setAttribute (T("pluginVersion"), 1);

	xmlState.setAttribute (T("xorBits"), serializeArray (xorBits));
	xmlState.setAttribute (T("xorWith"), xorWith);
	xmlState.setAttribute (T("xorProcessing"), xorProcessing);

	xmlState.setAttribute (T("andBits"), serializeArray (andBits));
	xmlState.setAttribute (T("andWith"), andWith);
	xmlState.setAttribute (T("andProcessing"), andProcessing);

	xmlState.setAttribute (T("clearBits"), serializeArray (clearBits));
	xmlState.setAttribute (T("clearProcessing"), clearProcessing);

	xmlState.setAttribute (T("setBits"), serializeArray (setBits));
	xmlState.setAttribute (T("setProcessing"), setProcessing);

	Logger::writeToLog (xmlState.createDocument (String::empty));

    copyXmlToBinary (xmlState, destData);
}

void DemoJuceFilter::setStateInformation (const void* data, int sizeInBytes)
{
    XmlElement* const xmlState = getXmlFromBinary (data, sizeInBytes);
    if (xmlState != 0)
    {
        if (xmlState->hasTagName (T("BITMUNGLER")))
        {
			Logger::writeToLog (xmlState->createDocument (String::empty));

			xorProcessing = xmlState->getBoolAttribute (T("xorProcessing"));
			andProcessing = xmlState->getBoolAttribute (T("andProcessing"));
			clearProcessing = xmlState->getBoolAttribute (T("clearProcessing"));
			setProcessing = xmlState->getBoolAttribute (T("setProcessing"));

			unserializeArray (xmlState->getStringAttribute (T("xorBits")), xorBits);
			xorWith = xmlState->getBoolAttribute (T("xorWith"));

			unserializeArray (xmlState->getStringAttribute (T("andBits")), andBits);
			andWith = xmlState->getBoolAttribute (T("andWith"));

			unserializeArray (xmlState->getStringAttribute (T("setBits")), setBits);
			unserializeArray (xmlState->getStringAttribute (T("clearBits")), clearBits);
        }
        delete xmlState;
    }

	sendChangeMessage (this);
}

float DemoJuceFilter::getCurrentSample()
{
	return (currentSample);
}

float DemoJuceFilter::getCurrentConvertedSample()
{
	return (currentConvertedSample);
}

void DemoJuceFilter::stopProcessing()
{
	processing = false;
	Logger::writeToLog (T("stopProcessing()"));
}

void DemoJuceFilter::startProcessing()
{
	processing = true;
	Logger::writeToLog (T("startProcessing()"));
}

bool DemoJuceFilter::isProcessing()
{
	return (processing);
}

float DemoJuceFilter::process(float sample)
{
	if (!isProcessing())
	{
		return (sample);
	}

	float ret = sample;

	for (int x=0; x<32; x++)
	{
		if (xorBits[x] && xorProcessing)
		{
			ret = xorbit (sample, x, xorWith);
		}
		if (andBits[x] && andProcessing)
		{
			ret = andbit (sample, x, andWith);
		}
		if (setBits[x] && setProcessing)
		{
			ret = setbit (sample, x);
		}
		if (clearBits[x] && clearProcessing)
		{
			ret = clearbit (sample, x);
		}
	}

	return (ret);
}

void DemoJuceFilter::setXorBit (int pos, bool bit)
{
	if (pos == 0)
		pos=1;

	xorBits.set (pos-1, true);
	xorWith = bit;
}

void DemoJuceFilter::setAndBit (int pos, bool bit)
{
	if (pos == 0)
		pos=1;

	andBits.set (pos-1, true);
	andWith = bit;
}

void DemoJuceFilter::setClearBit (int pos)
{
	if (pos == 0)
		pos=1;

	clearBits.set (pos-1, true);
}

void DemoJuceFilter::setSetBit (int pos)
{
	if (pos == 0)
		pos=1;

	setBits.set (pos-1, true);
}

void DemoJuceFilter::clearXorTable()
{
	Logger::writeToLog (T("clearXorTable()"));
	for (int x=0; x<32; x++)
	{
		xorBits.set (x, false);
		xorWith = false;
	}
}

void DemoJuceFilter::clearAndTable()
{
	Logger::writeToLog (T("clearAndTable()"));
	for (int x=0; x<32; x++)
	{
		andBits.set (x, false);
		andWith = false;
	}
}

void DemoJuceFilter::clearSetTable()
{
	Logger::writeToLog (T("clearSetTable()"));
	for (int x=0; x<32; x++)
	{
		setBits.set (x, false);
	}
}

void DemoJuceFilter::clearClearTable()
{
	Logger::writeToLog (T("clearClearTable()"));
	for (int x=0; x<32; x++)
	{
		clearBits.set (x, false);
	}
}

void DemoJuceFilter::clearTable()
{
	for (int x=0; x<32; x++)
	{
		xorBits.set (x, false);
		xorWith = false;
		andBits.set (x, false);
		andWith = false;
		clearBits.set (x, false);
		setBits.set (x, false);
	}
}

void DemoJuceFilter::setProcess (int processDef, bool b)
{
	switch (processDef)
	{
		case bitManglerEditor::XOR:
			xorProcessing = b;
			break;
		
		case bitManglerEditor::AND:
			andProcessing = b;
			break;

		case bitManglerEditor::CLEAR:
			clearProcessing = b;
			break;

		case bitManglerEditor::SET:
			setProcessing = b;
			break;

		default:
			break;
	}
}

String DemoJuceFilter::serializeArray (Array <bool>a)
{
	String str;

	for (int x=0; x<a.size(); x++)
	{
		if (x==0)
			str << a[x];
		else
			str <<T(",")<< a[x];
	}

	return (str);
}

void DemoJuceFilter::unserializeArray (String data, Array <bool>&a)
{
	StringArray ar;
	ar.addTokens (data, T(","), String::empty);

	for (int x=0; x<ar.size(); x++)
	{
		const int k = ar[x].getIntValue();
		bool b;

		if (k == 1)
			b = true;
		else
			b = false;
		
		a.set (x, b);
	}

	for (x=0; x<32; x++)
	{
		const int k = (int)a[x];
		Logger::writeToLog (String::formatted (T("unserialize: %d"), k));
	}
}

int DemoJuceFilter::getXorFirst()
{
	int x;
	for (x=0; x<32; x++)
	{
		const int k = (int)xorBits[x];
		Logger::writeToLog (String::formatted (T("getXorFirst: %d"), k));
	}

	for (x=0; x<32; x++)
	{
		if (xorBits[x])
			return (x+1);
	}

	return (0);
}

int DemoJuceFilter::getXorLast()
{
	for (int x=31; x>=0; x--)
	{
		if (xorBits[x])
			return (x+1);
	}

	return (0);
}

int DemoJuceFilter::getAndFirst()
{
	for (int x=0; x<32; x++)
	{
		if (andBits[x])
			return (x+1);
	}

	return (0);
}

int DemoJuceFilter::getAndLast()
{
	for (int x=31; x>=0; x--)
	{
		if (andBits[x])
			return (x+1);
	}

	return (0);
}

int DemoJuceFilter::getClearFirst()
{
	for (int x=0; x<32; x++)
	{
		if (clearBits[x])
			return (x+1);
	}

	return (0);
}

int DemoJuceFilter::getClearLast()
{
	for (int x=31; x>=0; x--)
	{
		if (clearBits[x])
			return (x+1);
	}

	return (0);
}

int DemoJuceFilter::getSetFirst()
{
	for (int x=0; x<32; x++)
	{
		if (setBits[x])
			return (x+1);
	}

	return (0);
}

int DemoJuceFilter::getSetLast()
{
	for (int x=31; x>=0; x--)
	{
		if (setBits[x])
			return (x+1);
	}

	return (0);
}

bool DemoJuceFilter::getXorWith()
{
	return (xorWith);
}

bool DemoJuceFilter::getAndWith()
{
	return (andWith);
}

void DemoJuceFilter::setXorWith(bool b)
{
	xorWith = b;
}

void DemoJuceFilter::setAndWith(bool b)
{
	andWith = b;
}