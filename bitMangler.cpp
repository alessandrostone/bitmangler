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
	lastFormula = T("SETBIT(1-26);");
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
	clearTable();

	Logger::writeToLog (T("stopProcessing()"));
}

void DemoJuceFilter::startProcessing(bool p)
{
	if (p)
	{
		parseFormula (lastFormula);
	}

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

	float ret = 0.0f;

	for (int x=0; x<32; x++)
	{
		if (xorBits[x])
		{
			ret = xorbit (sample, x, xorWith[x]);
		}
		if (setBits[x])
		{
			ret = setbit (sample, x);
		}
		if (clearBits[x])
		{
			ret = clearbit (sample, x);
		}
	}

	return (ret);
}

void DemoJuceFilter::setXorBit (int pos, bool bit)
{
	xorBits.set (pos-1, true);
	xorWith.set (pos-1, bit);
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

void DemoJuceFilter::setClearBit (int pos)
{
	clearBits.set (pos-1, true);
}

void DemoJuceFilter::setSetBit (int pos)
{
	setBits.set (pos-1, true);
}

bool DemoJuceFilter::parseFormula (String s)
{
	stopProcessing();

	if (s.contains (T(";")))
	{
		/* we have a selecetion of bits/bit ranges */
		StringArray functionList;
		functionList.addTokens (s, T(";"), String::empty);

		for (int fid=0; fid<functionList.size(); fid++)
		{
			if (!functionList[fid].isEmpty())
			{
				if (!parseFunction (functionList[fid]))
				{
					Logger::writeToLog (T("parse incomplete"));
					return (false);
				}
			}
		}

		Logger::writeToLog (T("parse complete"));
		
		startProcessing (false);
		lastFormula = s;
		return (true);
	}

	return (false);
}

bool DemoJuceFilter::parseFunction (String f)
{
	if (f.isEmpty())
	{
		return (false);
	}

	if (!f.contains(T("(")) && !f.contains(T(")")) && !f.contains(T(";")))
	{
		Logger::writeToLog (T("can't parse this function"));
		return (false);
	}

	StringArray p;

	const String fname = f.upToFirstOccurrenceOf (T("("), false, true).toUpperCase();
	const String params = f.fromFirstOccurrenceOf (T("("), false, true).replaceCharacter(')', '\0');

	if (fname == T("XORBIT"))
	{
		p.addTokens (params, T(","), String::empty);

		String bitPosition = p[0];
		const int bitValue = p[1].getIntValue();

		Logger::writeToLog (T("xorbit found"));

		if (bitPosition.contains(T("-")))
		{
			p.clear();
			p.addTokens (bitPosition, T("-"), String::empty);

			const int start	= p[0].getIntValue();
			const int end	= p[1].getIntValue();

			if (start > end)
			{
				Logger::writeToLog (T("inavlid bit range"));
				return (false);
			}
			else
			{
				Logger::writeToLog (String::formatted (T("bit range %d-%d"), start, end));

				for (int pos=start; pos<=end; pos++)
				{
					setXorBit (pos, (bool)bitValue);
				}

				return (true);
			}
		}
		else
		{
			const int pos = p[0].getIntValue();
			if (pos > 0 && pos < 33)
			{
				setXorBit (pos, (bool)bitValue);
			}

			return (true);
		}
	}

	else if (fname == T("ORBIT"))
	{
		Logger::writeToLog (T("orbit found"));

		return (true);
	}
	else if (fname == T("SETBIT"))
	{
		p.addTokens (params, T(","), String::empty);

		String bitPosition = p[0];

		Logger::writeToLog (T("setbit found"));

		if (bitPosition.contains(T("-")))
		{
			p.clear();
			p.addTokens (bitPosition, T("-"), String::empty);

			const int start	= p[0].getIntValue();
			const int end	= p[1].getIntValue();

			if (start > end)
			{
				Logger::writeToLog (T("inavlid bit range"));
				return (false);
			}
			else
			{
				Logger::writeToLog (String::formatted (T("bit range %d-%d"), start, end));

				for (int pos=start; pos<=end; pos++)
				{
					Logger::writeToLog (T("set bits"));
					setSetBit (pos);
				}
				Logger::writeToLog (T("return true"));
				return (true);
			}
		}
		else
		{
			const int pos = p[0].getIntValue();
			if (pos > 0 && pos < 33)
			{
				setSetBit (pos);
			}

			return (true);
		}
	}

	else if (fname == T("CLEARBIT"))
	{
		p.addTokens (params, T(","), String::empty);

		String bitPosition = p[0];

		Logger::writeToLog (T("clearbit found"));

		if (bitPosition.contains(T("-")))
		{
			p.clear();
			p.addTokens (bitPosition, T("-"), String::empty);

			const int start	= p[0].getIntValue();
			const int end	= p[1].getIntValue();

			if (start > end)
			{
				Logger::writeToLog (T("inavlid bit range"));
				return (false);
			}
			else
			{
				Logger::writeToLog (String::formatted (T("bit range %d-%d"), start, end));

				for (int pos=start; pos<=end; pos++)
				{
					setClearBit (pos);
				}

				return (true);
			}
		}
		else
		{
			const int pos = p[0].getIntValue();
			if (pos > 0 && pos < 33)
			{
				setClearBit (pos);
			}

			return (true);
		}
	}
	else
	{
		Logger::writeToLog (T("return false"));
		return (false);
	}

	Logger::writeToLog (T("return false"));
	return (false);
}

String DemoJuceFilter::getLastFormula()
{
	return (lastFormula);
}