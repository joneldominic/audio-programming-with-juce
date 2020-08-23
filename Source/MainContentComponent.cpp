/*
  ==============================================================================

	MainContentComponent.cpp
	Created: 23 Aug 2020 2:17:22pm
	Author:  dominic

  ==============================================================================
*/

#include <JuceHeader.h>
#include "MainContentComponent.h"

//==============================================================================
MainContentComponent::MainContentComponent()
	:	state(Stopped)
{
	addAndMakeVisible(&openButton);
	openButton.setButtonText("Open Audio File");
	openButton.onClick = [this] { openButtonClicked(); };

	addAndMakeVisible(&playButton);
	playButton.setButtonText("Play");
	playButton.onClick = [this] { playButtonClicked(); };
	playButton.setColour(juce::TextButton::buttonColourId, juce::Colours::green);
	playButton.setEnabled(false);

	addAndMakeVisible(&stopButton);
	stopButton.setButtonText("Stop");
	stopButton.onClick = [this] { stopButtonClicked(); };
	stopButton.setColour(juce::TextButton::buttonColourId, juce::Colours::red);
	stopButton.setEnabled(false);

	setSize(400, 200);

	formatManager.registerBasicFormats();      
	transportSource.addChangeListener(this);   

	setAudioChannels(0, 2);
}

MainContentComponent::~MainContentComponent()
{
	shutdownAudio();
}

void MainContentComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate) 
{
	transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainContentComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
	if (readerSource.get() == nullptr)
	{
		bufferToFill.clearActiveBufferRegion();
		return;
	}

	transportSource.getNextAudioBlock(bufferToFill);
}

void MainContentComponent::releaseResources()
{
	transportSource.releaseResources();
}

void MainContentComponent::resized()
{
	openButton.setBounds(10, 10, getWidth() - 20, 20);
	playButton.setBounds(10, 40, getWidth() - 20, 20);
	stopButton.setBounds(10, 70, getWidth() - 20, 20);
}

void MainContentComponent::changeListenerCallback(juce::ChangeBroadcaster* source)
{
	if (source == &transportSource)
	{
		if (transportSource.isPlaying())
			changeState(Playing);
		else
			changeState(Stopped);
	}
}

//==============================================================================
void MainContentComponent::changeState(TransportState newState)
{
	if (state != newState)
	{
		state = newState;

		switch (state)
		{
		case Stopped:                           
			stopButton.setEnabled(false);
			playButton.setEnabled(true);
			transportSource.setPosition(0.0);
			break;

		case Starting:                          
			playButton.setEnabled(false);
			transportSource.start();
			break;

		case Playing:                          
			stopButton.setEnabled(true);
			break;

		case Stopping:                          
			transportSource.stop();
			break;
		}
	}
}

void MainContentComponent::openButtonClicked()
{
	juce::FileChooser chooser("Select a Wave file to play...",
		{},
		"*.wav;*.mp3");                                        

	if (chooser.browseForFileToOpen())                                         
	{
		auto file = chooser.getResult();                                       
		auto* reader = formatManager.createReaderFor(file);                    

		if (reader != nullptr)
		{
			std::unique_ptr<juce::AudioFormatReaderSource> newSource(new juce::AudioFormatReaderSource(reader, true)); 
			transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);                                
			playButton.setEnabled(true);                                                                               
			readerSource.reset(newSource.release());                                                                   
		}
	}
}


void MainContentComponent::playButtonClicked()
{
	changeState(Starting);
}

void MainContentComponent::stopButtonClicked()
{
	changeState(Stopping);
}