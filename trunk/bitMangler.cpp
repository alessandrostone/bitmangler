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
	currentSample = 0.0f;
	bufferCycle = 0;
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
	for (int channel = 0; channel < getNumInputChannels(); ++channel)
	{
		float *p = buffer.getSampleData (channel);
		int size = buffer.getNumSamples();

		for (int x=0; x<size; x++)
		{
			if (x == 2 && channel == 0 && bufferCycle == 0)
			{
				currentSample = *(p+x);
			}

			/* conversion */

			if (x == 2 && channel == 0 && bufferCycle == 0)
			{
				currentConvertedSample = *(p+x);

				if (editor)
					editor->triggerAsyncUpdate();
			}
		}
	}

	bufferCycle++;

	if (bufferCycle == 10)
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