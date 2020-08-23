/*
  ==============================================================================

	This file contains the basic startup code for a JUCE application.

  ==============================================================================
*/

#include <JuceHeader.h>

//==============================================================================
class AudioProgrammingwithJUCEApplication  : public juce::JUCEApplication
{
public:
	//==============================================================================
	AudioProgrammingwithJUCEApplication() {}

	const juce::String getApplicationName() override       { return ProjectInfo::projectName; }
	const juce::String getApplicationVersion() override    { return ProjectInfo::versionString; }
	bool moreThanOneInstanceAllowed() override             { return true; }

	//==============================================================================
	class MainWindow : public juce::DocumentWindow
	{
	public:
		MainWindow(juce::String name) : DocumentWindow(
			name,
			juce::Colours::lightgrey,
			DocumentWindow::minimiseButton | DocumentWindow::closeButton)
		{
			setUsingNativeTitleBar(true);
			setResizable(false, false);
			centreWithSize(500, 500);
			setVisible(true);
		}

		void closeButtonPressed() override
		{
			juce::JUCEApplication::getInstance()->systemRequestedQuit();
		}

	private:
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow);
	};

	//==============================================================================
	void initialise (const juce::String& commandLine) override
	{
		mainWindow.reset(new MainWindow(getApplicationName()));
	}

	void shutdown() override
	{
		mainWindow = nullptr;
	}

	//==============================================================================
	void systemRequestedQuit() override
	{
		quit();
	}

	void anotherInstanceStarted (const juce::String& commandLine) override
	{
		// When another instance of the app is launched while this one is running,
		// this method is invoked, and the commandLine parameter tells you what
		// the other instance's command-line arguments were.
	}

private:
	std::unique_ptr<MainWindow> mainWindow;
};

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (AudioProgrammingwithJUCEApplication)
