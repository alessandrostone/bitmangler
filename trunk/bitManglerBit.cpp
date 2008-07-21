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

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "bitManglerBit.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
bitManglerBit::bitManglerBit (int _b)
    : bitManipulator (0),
      bitLabel (0)
{
    addAndMakeVisible (bitManipulator = new ComboBox (T("Manipulator")));
    bitManipulator->setTooltip (T("Manipulator"));
    bitManipulator->setEditableText (false);
    bitManipulator->setJustificationType (Justification::centredLeft);
    bitManipulator->setTextWhenNothingSelected (String::empty);
    bitManipulator->setTextWhenNoChoicesAvailable (T("(no choices)"));
    bitManipulator->addItem (T("Always Set"), 1);
    bitManipulator->addItem (T("Always Clear"), 2);
    bitManipulator->addItem (T("XOR 1"), 3);
    bitManipulator->addItem (T("XOR 0"), 4);
    bitManipulator->addItem (T("OR 0"), 5);
    bitManipulator->addItem (T("OR 1"), 6);
    bitManipulator->addItem (T("AND 0"), 7);
    bitManipulator->addItem (T("AND 1"), 8);
    bitManipulator->addListener (this);

    addAndMakeVisible (bitLabel = new Label (T("Label"),
                                             T("-")));
    bitLabel->setTooltip (T("Label"));
    bitLabel->setFont (Font (15.0000f, Font::bold));
    bitLabel->setJustificationType (Justification::centred);
    bitLabel->setEditable (false, false, false);
    bitLabel->setColour (TextEditor::textColourId, Colours::black);
    bitLabel->setColour (TextEditor::backgroundColourId, Colour (0x0));


    //[UserPreSize]
	bitLabel->setText (String(_b), false);
    //[/UserPreSize]

    setSize (64, 32);

    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

bitManglerBit::~bitManglerBit()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    deleteAndZero (bitManipulator);
    deleteAndZero (bitLabel);

    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void bitManglerBit::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void bitManglerBit::resized()
{
    bitManipulator->setBounds (0, 0, 64, 16);
    bitLabel->setBounds (0, 16, 64, 16);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void bitManglerBit::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == bitManipulator)
    {
        //[UserComboBoxCode_bitManipulator] -- add your combo box handling code here..
        //[/UserComboBoxCode_bitManipulator]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Jucer information section --

    This is where the Jucer puts all of its metadata, so don't change anything in here!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="bitManglerBit" componentName=""
                 parentClasses="public Component" constructorParams="int _b" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330000013"
                 fixedSize="1" initialWidth="64" initialHeight="32">
  <BACKGROUND backgroundColour="ffffff"/>
  <COMBOBOX name="Manipulator" id="6f50ff0c45ac7e5b" memberName="bitManipulator"
            virtualName="" explicitFocusOrder="0" pos="0 0 64 16" tooltip="Manipulator"
            editable="0" layout="33" items="Always Set&#10;Always Clear&#10;XOR 1&#10;XOR 0&#10;OR 0&#10;OR 1&#10;AND 0&#10;AND 1&#10;"
            textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <LABEL name="Label" id="7b9de4343df4b48b" memberName="bitLabel" virtualName=""
         explicitFocusOrder="0" pos="0 16 64 16" tooltip="Label" edTextCol="ff000000"
         edBkgCol="0" labelText="-" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="1" italic="0" justification="36"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif
