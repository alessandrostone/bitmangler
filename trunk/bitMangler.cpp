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
    return 1;
}

float DemoJuceFilter::getParameter (int index)
{
	return (0.0);
}

void DemoJuceFilter::setParameter (int index, float newValue)
{
}

const String DemoJuceFilter::getParameterName (int index)
{
    return String::empty;
}

const String DemoJuceFilter::getParameterText (int index)
{
    return String::empty;
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
    return true;
}

bool DemoJuceFilter::producesMidi() const
{
    return true;
}

void DemoJuceFilter::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    keyboardState.reset();
}

void DemoJuceFilter::releaseResources()
{
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
				sendChangeMessage (this);
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
    XmlElement xmlState (T("MYPLUGINSETTINGS"));
    xmlState.setAttribute (T("pluginVersion"), 1);
    copyXmlToBinary (xmlState, destData);
}

void DemoJuceFilter::setStateInformation (const void* data, int sizeInBytes)
{
    XmlElement* const xmlState = getXmlFromBinary (data, sizeInBytes);
    if (xmlState != 0)
    {
        if (xmlState->hasTagName (T("MYPLUGINSETTINGS")))
        {
            sendChangeMessage (this);
        }
        delete xmlState;
    }
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
			ret = xorbit (sample, x, xorWith[x]);
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
	clearXorTable();
	xorBits.set (pos-1, true);
	xorWith.set (pos-1, bit);
}

void DemoJuceFilter::setAndBit (int pos, bool bit)
{
	clearAndTable();
	andBits.set (pos-1, true);
	andWith.set (pos-1, bit);
}

void DemoJuceFilter::setClearBit (int pos)
{
	clearClearTable();
	clearBits.set (pos-1, true);
}

void DemoJuceFilter::setSetBit (int pos)
{
	clearSetTable();
	setBits.set (pos-1, true);
}

void DemoJuceFilter::clearXorTable()
{
	Logger::writeToLog (T("clearXorTable()"));
	for (int x=0; x<32; x++)
	{
		xorBits.set (x, false);
		xorWith.set (x, false);
	}
}

void DemoJuceFilter::clearAndTable()
{
	Logger::writeToLog (T("clearAndTable()"));
	for (int x=0; x<32; x++)
	{
		andBits.set (x, false);
		andWith.set (x, false);
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
		xorWith.set (x, false);
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