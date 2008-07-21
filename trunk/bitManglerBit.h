/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  21 Jul 2008 12:55:26 am

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Jucer version: 1.11

  ------------------------------------------------------------------------------

  The Jucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-6 by Raw Material Software ltd.

  ==============================================================================
*/

#ifndef __JUCER_HEADER_BITMANGLERBIT_BITMANGLERBIT_EB307185__
#define __JUCER_HEADER_BITMANGLERBIT_BITMANGLERBIT_EB307185__

//[Headers]     -- You can add your own extra header files here --
#include "juce.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Jucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class bitManglerBit  : public Component,
                       public ComboBoxListener
{
public:
    //==============================================================================
    bitManglerBit (int _b);
    ~bitManglerBit();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged);


    //==============================================================================
    juce_UseDebuggingNewOperator

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    ComboBox* bitManipulator;
    Label* bitLabel;

    //==============================================================================
    // (prevent copy constructor and operator= being generated..)
    bitManglerBit (const bitManglerBit&);
    const bitManglerBit& operator= (const bitManglerBit&);
};


#endif   // __JUCER_HEADER_BITMANGLERBIT_BITMANGLERBIT_EB307185__
