/*
  ==============================================================================

	MainContentComponent.h
	Created: 23 Aug 2020 2:17:22pm
	Author:  dominic

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class MainContentComponent  : public juce::Component
{
public:
	MainContentComponent();
	~MainContentComponent() override;

	void paint (juce::Graphics&) override;
	void resized() override;

private:
	enum TransportState
	{
		Stopped,
		Starting,
		Playing,
		Stopping
	};
	
	void changeState(TransportState newState);
	void openButtonClicked();
	void playButtonClicked();
	void stopButtonClicked();

	//==========================================================================
	juce::TextButton openButton;
	juce::TextButton playButton;
	juce::TextButton stopButton;

	juce::AudioFormatManager formatManager;
	std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
	juce::AudioTransportSource transportSource;
	TransportState state;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};
