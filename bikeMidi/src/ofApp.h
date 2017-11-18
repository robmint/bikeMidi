#pragma once

#include "ofMain.h"
#include "ofxMidi.h"
#include "ofxSimpleSerial.h"
#include <vector>
#include "ofxDatGui.h"
#include "ofxXmlSettings.h"
#include "ofxMidiPlayer.h"

#define BIKES 3
#define NOTEON 0
#define NOTEOFF 1

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
        void exit();


		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

        // gui
        bool guiInitSerial;
        string serialDevice;
    
        bool mFullscreen;
        void refreshWindow();
        void toggleFullscreen();
    
        void onButtonEvent(ofxDatGuiButtonEvent e);
        void onToggleEvent(ofxDatGuiToggleEvent e);
        void onSliderEvent(ofxDatGuiSliderEvent e);
        void onDropdownEvent(ofxDatGuiDropdownEvent e);
        void onMatrixEvent(ofxDatGuiMatrixEvent e);
        void onColorPickerEvent(ofxDatGuiColorPickerEvent e);
        void on2dPadEvent(ofxDatGui2dPadEvent e);
        void onTextInputEvent(ofxDatGuiTextInputEvent e);
        
        ofxDatGui* gui;
        ofxDatGuiToggle* serialOk;
        ofxDatGuiValuePlotter* plotter[BIKES];
        ofxDatGuiToggle* redButton[BIKES];
        ofxDatGuiToggle* greenButton[BIKES];
        ofxDatGuiSlider* durationSlider[BIKES];
        ofxDatGuiSlider* midiChannelSlider[BIKES];
        ofxDatGuiSlider* redNoteSlider[BIKES];
        ofxDatGuiSlider* greenNoteSlider[BIKES];
    
        vector<ofxDatGuiComponent*> components;
    
        // midi
        ofxMidiOut midiOut;
        int channel;
        void midiCallback(midi_event *pev);


        unsigned int currentPgm;
        int note, velocity;
        int pan, bend, touch, polytouch;
        
        // serial
        ofxSimpleSerial	serial;
        string		message;
        bool		inputChannel;
        bool		inputData;
        bool		inputValue;
        bool		requestRead;
        ofTrueTypeFont		font;
    
        int device, key, value;

        vector <ofSerialDeviceInfo> deviceList;
        void		onNewMessage(string & message);
    
        // settings
        ofxXmlSettings XML;
        void updateDeviceList();
        void saveSettings();
        void loadSettings();

        // midi player
        SdFat	SD;
        MD_MIDIFile SMF;

        // main functions
        long int last_click[BIKES], now_click[BIKES], duration[BIKES];
	
};

class ofxDatGuiThemeRob : public ofxDatGuiTheme{
    
public:
    
    ofxDatGuiThemeRob()
    {
        font.size = 10;
        font.file = "DIN.otf";
        layout.upperCaseLabels = true;
        init();
    }
};
