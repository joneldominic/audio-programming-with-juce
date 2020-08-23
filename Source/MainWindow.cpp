/*
  ==============================================================================

	MainWindow.cpp
	Created: 23 Aug 2020 1:56:21pm
	Author:  dominic

  ==============================================================================
*/

#include "MainWindow.h"

MainWindow::MainWindow(juce::String name, juce::Component* component) : DocumentWindow(
	name,
	juce::Colours::lightgrey,
	DocumentWindow::minimiseButton | DocumentWindow::closeButton)
{
	setUsingNativeTitleBar(true);

	setContentOwned(component, true);

	setResizable(true, false);
	setResizeLimits(300, 250, 10000, 10000);
	centreWithSize(getWidth(), getHeight());

	setVisible(true);
}

void MainWindow::closeButtonPressed()
{
	juce::JUCEApplication::getInstance()->systemRequestedQuit();
}