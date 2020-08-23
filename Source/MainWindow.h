/*
  ==============================================================================

	MainWindow.h
	Created: 23 Aug 2020 1:56:21pm
	Author:  dominic

  ==============================================================================
*/

#pragma 

#include <JuceHeader.h>

class MainWindow : public juce::DocumentWindow
{
public:
	MainWindow(juce::String name, juce::Component* component);
	
	void closeButtonPressed() override;

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow);
};