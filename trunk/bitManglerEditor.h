/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  21 Jul 2008 10:12:26 pm

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Jucer version: 1.11

  ------------------------------------------------------------------------------

  The Jucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-6 by Raw Material Software ltd.

  ==============================================================================
*/

#ifndef __JUCER_HEADER_BITMANGLEREDITOR_BITMANGLEREDITOR_341EF80C__
#define __JUCER_HEADER_BITMANGLEREDITOR_BITMANGLEREDITOR_341EF80C__

//[Headers]     -- You can add your own extra header files here --
#include "juce.h"
#include "bitMangler.h"


#define BITS_FLT 32
#define MANT_FLT 23
#define SIGN_FLT ((floatint)1<<(BITS_FLT-1))
typedef unsigned long int floatint;
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Jucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class bitManglerEditor  : public AudioProcessorEditor,
                          public ChangeListener,
                          public AsyncUpdater,
                          public ButtonListener
{
public:
    //==============================================================================
    bitManglerEditor (DemoJuceFilter* const ownerFilter);
    ~bitManglerEditor();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	String getBinaryString (floatint b, int d);
	void changeListenerCallback(void *ptr);
	void handleAsyncUpdate();
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void buttonClicked (Button* buttonThatWasClicked);

    // Binary resources:
    static const char* lcd_bin;
    static const int lcd_binSize;
    static const char* metal2_png;
    static const int metal2_pngSize;
    static const char* power_off_png;
    static const int power_off_pngSize;
    static const char* power_on_png;
    static const int power_on_pngSize;

    //==============================================================================
    juce_UseDebuggingNewOperator

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	DemoJuceFilter *owner;
	void updateBitDisplay();
	union mix
	{
		float f;
		unsigned long int i;
	} uSample, uConvertedSample;

	floatint e;
	floatint m;
	floatint ce;
	floatint cm;
	Font *lcdBigFont;
    //[/UserVariables]

    //==============================================================================
    Label* bitDisplayInput;
    Label* bitDisplayOutput;
    ImageButton* processButton;
    Image* internalCachedImage1;

    //==============================================================================
    // (prevent copy constructor and operator= being generated..)
    bitManglerEditor (const bitManglerEditor&);
    const bitManglerEditor& operator= (const bitManglerEditor&);
};


#endif   // __JUCER_HEADER_BITMANGLEREDITOR_BITMANGLEREDITOR_341EF80C__
