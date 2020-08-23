/*
  ==============================================================================

	MainWindow.cpp
	Created: 23 Aug 2020 1:56:21pm
	Author:  dominic

  ==============================================================================
*/

#include "MainWindow.h"

MainWindow::MainWindow(juce::String name) : DocumentWindow(
	name,
	juce::Colours::lightgrey,
	DocumentWindow::minimiseButton | DocumentWindow::closeButton)
{
	setUsingNativeTitleBar(true);
	setResizable(true, false);
	setResizeLimits(300, 250, 10000, 10000);
	centreWithSize(500, 500);
	setVisible(true);
}

void MainWindow::closeButtonPressed()
{
	juce::JUCEApplication::getInstance()->systemRequestedQuit();
}