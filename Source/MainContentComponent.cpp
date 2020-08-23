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
	:	state(Stopped),
	thumbnailCache(5),                            
	thumbnail(512, formatManager, thumbnailCache) 
{
	addAndMakeVisible(&openButton);
	openButton.setButtonText("Load Audio file");
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

	setSize(600, 400);

	formatManager.registerBasicFormats();      
	transportSource.addChangeListener(this); 
	thumbnail.addChangeListener(this);        

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
		bufferToFill.clearActiveBufferRegion();
	else
		transportSource.getNextAudioBlock(bufferToFill);
}

void MainContentComponent::releaseResources()
{
	transportSource.releaseResources();
}

void MainContentComponent::paint(juce::Graphics& g)
{
	juce::Rectangle<int> thumbnailBounds(10, 100, getWidth() - 20, getHeight() - 120);

	if (thumbnail.getNumChannels() == 0)
		paintIfNoFileLoaded(g, thumbnailBounds);
	else
		paintIfFileLoaded(g, thumbnailBounds);
}

void MainContentComponent::resized()
{
	openButton.setBounds(10, 10, getWidth() - 20, 20);
	playButton.setBounds(10, 40, getWidth() - 20, 20);
	stopButton.setBounds(10, 70, getWidth() - 20, 20);
}

void MainContentComponent::changeListenerCallback(juce::ChangeBroadcaster* source) {
	if (source == &transportSource) transportSourceChanged();
	if (source == &thumbnail)       thumbnailChanged();
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

		default:
			jassertfalse;
			break;
		}
	}
}

void MainContentComponent::transportSourceChanged()
{
	changeState(transportSource.isPlaying() ? Playing : Stopped);
}

void MainContentComponent::thumbnailChanged()
{
	repaint();
}

void MainContentComponent::paintIfNoFileLoaded(juce::Graphics& g, const juce::Rectangle<int>& thumbnailBounds)
{
	g.setColour(juce::Colours::darkgrey);
	g.fillRect(thumbnailBounds);
	g.setColour(juce::Colours::white);
	g.drawFittedText("No File Loaded", thumbnailBounds, juce::Justification::centred, 1);
}

void MainContentComponent::paintIfFileLoaded(juce::Graphics& g, const juce::Rectangle<int>& thumbnailBounds)
{
	g.setColour(juce::Colours::white);
	g.fillRect(thumbnailBounds);

	g.setColour(juce::Colours::red);                               

	thumbnail.drawChannels(g,                                      
		thumbnailBounds,
		0.0,                                    
		thumbnail.getTotalLength(),            
		1.0f);                                  
}

void MainContentComponent::openButtonClicked()
{
	juce::FileChooser chooser("Select a Audio file to play...",
		{},
		"*.wav;*.mp3");                                        

	if (chooser.browseForFileToOpen())                                         
	{
		auto file = chooser.getResult();                                       
		auto* reader = formatManager.createReaderFor(file);                    

		if (reader != nullptr)
		{
			changeState(Stopping);

			std::unique_ptr<juce::AudioFormatReaderSource> newSource(new juce::AudioFormatReaderSource(reader, true)); 
			transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);                                
			playButton.setEnabled(true);  
			thumbnail.setSource(new juce::FileInputSource(file));
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