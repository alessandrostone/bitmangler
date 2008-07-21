#include "wrapper/formats/Standalone/juce_StandaloneFilterWindow.h"

class app : public JUCEApplication
{
    StandaloneFilterWindow *wnd;
	
	public:
	    app()
	    {
			wnd = 0;
		}
		~app()
		{
		}
	    void initialise (const String& commandLine)
	    {
			ApplicationProperties::getInstance()->setStorageParameters(T("bitMangler"),T(".options"),T("settings"),250,PropertiesFile::storeAsXML);
			wnd = new StandaloneFilterWindow(T("bitMangler"), Colours::white);

			wnd->centreWithSize (380, 155+28);
			wnd->setVisible (true);
		}
		void shutdown()
		{
		    if (wnd != 0)
				delete wnd;
		}
		const String getApplicationName()
		{
	        return T("surface");
	    }
		const String getApplicationVersion()
		{
	        return T("1.0");
	    }
		bool moreThanOneInstanceAllowed()
		{
	        return false;
		}
		void anotherInstanceStarted (const String& commandLine)
		{
		}
};

START_JUCE_APPLICATION (app)
