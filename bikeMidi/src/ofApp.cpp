#include "ofApp.h"
#include <functional>

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetVerticalSync(true);
    
    loadSettings();
    
    //midi
    
    
    SMF.begin(&SD);
    
// pass std::cplus function to c handler
// https://stackoverflow.com/questions/20290476/how-to-use-a-stdfunction-as-a-c-style-callback

// use handler with std:cplus function
// https://stackoverflow.com/questions/14189440/c-class-member-callback-simple-examples
    
// simpler
// http://www.cplusplus.com/forum/general/120728/
//    std::function<void(midi_event)> func = ofApp::midiCallback;

//    SMF.setMidiHandler(ofApp::midiCallback);
    
    // print the available output ports to the console
    midiOut.listPorts(); // via instance
    //ofxMidiOut::listPorts(); // via static too
    
    // connect
//    midiOut.openPort(); // by number
    midiOut.openPort("IAC Driver IAC Bus 1"); // by name
    //midiOut.openVirtualPort("ofxMidiOut"); // open a virtual port
    
    channel = 1;
    currentPgm = 0;
    note = 0;
    velocity = 0;
    pan = 0;
    bend = 0;
    touch = 0;
    polytouch = 0;
    
    // serial
    
    font.load("DIN.otf", 64);
    
    serial.listDevices();
    ofAddListener(serial.NEW_MESSAGE,this,&ofApp::onNewMessage);
    // void (ListenerClass::*listenerMethod)(ArgumentsType&)
    message = "";
    
    
    // gui
    
    vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList();
    vector<string> deviceNames;
    
    for (int i = 0; i < deviceList.size(); i++){
        deviceNames.push_back(deviceList[i].getDeviceName());
    }
    
    int x = 40;
    int y = 40;
    int p = 0;
    int bike = 0;
    ofSetWindowPosition(0, 0);
    ofSetWindowShape(1920, 1080);
    
    ofxDatGuiComponent* component;
    
    component = new ofxDatGuiLabel("Serial");
    component->setPosition(x, y);
    components.push_back(component);
    
    y += component->getHeight() + p;
    serialOk = new ofxDatGuiToggle("Connection", false);
    serialOk->setPosition(x, y);
    components.push_back(serialOk);
    
    y += component->getHeight() + p;
    ofxDatGuiDropdown* serialMenu;
    serialMenu = new ofxDatGuiDropdown("Serial port", deviceNames);
    serialMenu->setPosition(x, y);
    serialMenu->expand();
    serialMenu->onDropdownEvent(this, &ofApp::onDropdownEvent);
    components.push_back(serialMenu);
    
    y += component->getHeight() + p;
    component = new ofxDatGuiButton("Save Settings");
    component->setPosition(x, y);
    component->onButtonEvent(this, &ofApp::onButtonEvent);
    components.push_back(component);

    
    x += component->getWidth() + p+60;

    y = 40;
    // capture the plotter in a variable so we can feed it values later //
    plotter[bike] = new ofxDatGuiValuePlotter("Drums", 0, 3000);
    plotter[bike]->setSpeed(2.0f);
    plotter[bike]->setDrawMode(ofxDatGuiGraph::LINES);
    plotter[bike]->setSpeed(0.1f);
    
    component = plotter[bike];
    component->setPosition(x, y);
    components.push_back(component);

    y += component->getHeight() + p;
    durationSlider[bike] = new ofxDatGuiSlider("Duration", 0, 1000, 500);
    durationSlider[bike]->setPosition(x, y);
    durationSlider[bike]->setPrecision(0);
    components.push_back(durationSlider[bike]);

    
    y += component->getHeight() + p;
    midiChannelSlider[bike] = new ofxDatGuiSlider("Midi Channel", 1, 16, 1);
    midiChannelSlider[bike]->setPosition(x, y);
    midiChannelSlider[bike]->setPrecision(0);
    midiChannelSlider[bike]->onSliderEvent(this, &ofApp::onSliderEvent);
    components.push_back(midiChannelSlider[bike]);

    y += component->getHeight();
    redNoteSlider[bike] = new ofxDatGuiSlider("Red Note", 0, 127, 63);
    redNoteSlider[bike]->setPosition(x, y);
    redNoteSlider[bike]->onSliderEvent(this, &ofApp::onSliderEvent);
    redNoteSlider[bike]->setPrecision(0);
    components.push_back(redNoteSlider[bike]);

    y += component->getHeight();
    greenNoteSlider[bike] = new ofxDatGuiSlider("Green Note", 0, 127, 63);
    greenNoteSlider[bike]->setPosition(x, y);
    greenNoteSlider[bike]->onSliderEvent(this, &ofApp::onSliderEvent);
    greenNoteSlider[bike]->setPrecision(0);
    components.push_back(greenNoteSlider[bike]);

    
    y += component->getHeight() + p;
    component = new ofxDatGuiButton("Min Set");
    component->setPosition(x, y);
    component->onButtonEvent(this, &ofApp::onButtonEvent);
    components.push_back(component);
    
    y += component->getHeight() + p;
    component = new ofxDatGuiSlider("max", 0, 100, 50);
    component->setPosition(x, y);
    component->onSliderEvent(this, &ofApp::onSliderEvent);
    components.push_back(component);
    
    y += component->getHeight() + p;
    component = new ofxDatGuiButton("Max Set");
    component->setPosition(x, y);
    component->onButtonEvent(this, &ofApp::onButtonEvent);
    components.push_back(component);


    y += component->getHeight() + p;
    component = new ofxDatGuiSlider("weight", 0, 100, 50);
    component->setPosition(x, y);
    component->onSliderEvent(this, &ofApp::onSliderEvent);
    components.push_back(component);

    y += component->getHeight() + p;
    component = new ofxDatGuiSlider("threshold", 0, 100, 50);
    component->setPosition(x, y);
    component->onSliderEvent(this, &ofApp::onSliderEvent);
    components.push_back(component);

    bike++;
 
    
    x += component->getWidth() + p+60;
    
    y = 40;
    // capture the plotter in a variable so we can feed it values later //
    plotter[bike] = new ofxDatGuiValuePlotter("Bass", 0, 3000);
    plotter[bike]->setSpeed(2.0f);
    plotter[bike]->setDrawMode(ofxDatGuiGraph::LINES);
    plotter[bike]->setSpeed(0.1f);
    
    component = plotter[bike];
    component->setPosition(x, y);
    components.push_back(component);
    
    y += component->getHeight() + p;
    durationSlider[bike] = new ofxDatGuiSlider("Duration", 0, 1000, 500);
    durationSlider[bike]->setPosition(x, y);
    durationSlider[bike]->setPrecision(0);
    components.push_back(durationSlider[bike]);
    
    
    y += component->getHeight() + p;
    midiChannelSlider[bike] = new ofxDatGuiSlider("Midi Channel", 1, 16, 1);
    midiChannelSlider[bike]->setPosition(x, y);
    midiChannelSlider[bike]->setPrecision(0);
    midiChannelSlider[bike]->onSliderEvent(this, &ofApp::onSliderEvent);
    components.push_back(midiChannelSlider[bike]);

    y += component->getHeight() + p;
    component = new ofxDatGuiSlider("max", 0, 100, 50);
    component->setPosition(x, y);
    component->onSliderEvent(this, &ofApp::onSliderEvent);
    components.push_back(component);
    
    bike++;
    
    x += component->getWidth() + p+60;
    
    y = 40;
    // capture the plotter in a variable so we can feed it values later //
    plotter[bike] = new ofxDatGuiValuePlotter("Lead", 0, 3000);
    plotter[bike]->setSpeed(2.0f);
    plotter[bike]->setDrawMode(ofxDatGuiGraph::LINES);
    plotter[bike]->setSpeed(0.1f);
    
    component = plotter[bike];
    component->setPosition(x, y);
    components.push_back(component);
    
    y += component->getHeight() + p;
    durationSlider[bike] = new ofxDatGuiSlider("Duration", 0, 1000, 500);
    durationSlider[bike]->setPosition(x, y);
    durationSlider[bike]->setPrecision(0);
    components.push_back(durationSlider[bike]);
    
    
    y += component->getHeight() + p;
    midiChannelSlider[bike] = new ofxDatGuiSlider("Midi Channel", 1, 16, 1);
    midiChannelSlider[bike]->setPosition(x, y);
    midiChannelSlider[bike]->setPrecision(0);
    midiChannelSlider[bike]->onSliderEvent(this, &ofApp::onSliderEvent);
    components.push_back(midiChannelSlider[bike]);
    
    y += component->getHeight() + p;
    component = new ofxDatGuiButton("Min Set");
    component->setPosition(x, y);
    component->onButtonEvent(this, &ofApp::onButtonEvent);
    components.push_back(component);
    
    y += component->getHeight() + p;
    component = new ofxDatGuiSlider("max", 0, 100, 50);
    component->setPosition(x, y);
    component->onSliderEvent(this, &ofApp::onSliderEvent);
    components.push_back(component);
    
    y += component->getHeight() + p;
    component = new ofxDatGuiButton("Max Set");
    component->setPosition(x, y);
    component->onButtonEvent(this, &ofApp::onButtonEvent);
    components.push_back(component);
    
    
    y += component->getHeight() + p;
    component = new ofxDatGuiSlider("weight", 0, 100, 50);
    component->setPosition(x, y);
    component->onSliderEvent(this, &ofApp::onSliderEvent);
    components.push_back(component);
    
    y += component->getHeight() + p;
    component = new ofxDatGuiSlider("threshold", 0, 100, 50);
    component->setPosition(x, y);
    component->onSliderEvent(this, &ofApp::onSliderEvent);
    components.push_back(component);
    
    bike++;
    
    
    gui = new ofxDatGui();
    for(int i = 0; i<components.size(); i++){
        components[i]->setTheme(new ofxDatGuiThemeRob());
    }
 
 
}

void ofApp::midiCallback(midi_event *pev)
// Called by the MIDIFile library when a file event needs to be processed
// thru the midi communications interface.
// This callback is set up in the setup() function.
{
#if USE_MIDI
/*
    if ((pev->data[0] >= 0x80) && (pev->data[0] <= 0xe0))
    {
        Serial.write(pev->data[0] | pev->channel);
        Serial.write(&pev->data[1], pev->size-1);
    }
    else
        Serial.write(pev->data, pev->size);
 */
#endif
    DEBUG("\n");
    DEBUG(ofGetElapsedTimeMillis());
    DEBUG("\tM T");
    DEBUG(pev->track);
    DEBUG(":  Ch ");
    DEBUG(pev->channel+1);
    DEBUG(" Data ");
    for (uint8_t i=0; i<pev->size; i++)
    {
        DEBUGX(pev->data[i]);
        DEBUG(' ');
    }
}


void ofApp::exit(){
//    ringButton.removeListener(this, &ofApp::ringButtonPressed);
    midiOut.closePort();

}

void ofApp::updateDeviceList(){
    deviceList = serial.getDeviceList();

}





//--------------------------------------------------------------
void ofApp::update(){

    for(int i=0; i<components.size(); i++) components[i]->update();
    
    for(int i=0; i<BIKES; i++) {
        plotter[i]->setValue(duration[i]);
        durationSlider[i]->setValue(duration[i]);
    }
    
    

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(ofColor::black);
//    gui->draw();
    for(int i=0; i<components.size(); i++) components[i]->draw();

    ofSetColor(255);
    stringstream text;
    text << "connected to port " << midiOut.getPort()
    << " \"" << midiOut.getName() << "\"" << endl
    << "is virtual?: " << midiOut.isVirtual() << endl << endl
    << "sending to channel " << channel << endl << endl
    << "current program: " << currentPgm << endl << endl
    << "note: " << note << endl
    << "velocity: " << velocity << endl
    << "pan: " << pan << endl
    << "bend: " << bend << endl
    << "touch: " << touch << endl
    << "polytouch: " << polytouch;
    ofDrawBitmapString(text.str(), 20, 300);
    

}

void ofApp::onNewMessage(string & message)
{
    cout << "onNewMessage, message: " << message << "\n";
    vector<string> splitString = ofSplitString( message, ",");
    if(splitString.size() >= 3){
        device = ofToInt(splitString.at(0));
        key = ofToInt(splitString.at(1));
        value =	ofToInt(splitString.at(2));
    }
    
    serialOk->setChecked(!serialOk->getChecked());
    if(key == 0) { // key 0 == TACH
        if(value == NOTEON) {
            last_click[device] = now_click[device];
            now_click[device] = ofGetElapsedTimeMillis();
            duration[device] = now_click[device] - last_click[device];

            midiOut.sendNoteOn(midiChannelSlider[device]->getValue(), 48, 64);
            cout << "noteon: ch:" << midiChannelSlider[device]->getValue() << " key:" << key << " value:" << value << "\n";

        } else if(value == NOTEOFF) {
            midiOut.sendNoteOff(midiChannelSlider[device]->getValue(), 48, 64);
            cout << "noteoff: ch:" << midiChannelSlider[device]->getValue() << " key:" << key << " value:" << value << "\n";
        }
    
    }
    
    
    if(key == 2 ) {
        midiOut.sendNoteOn(midiChannelSlider[device]->getValue(), greenNoteSlider[device]->getValue(), 64);
    } else if(key == 1 ) {
        midiOut.sendNoteOn(midiChannelSlider[device]->getValue(), redNoteSlider[device]->getValue(), 64);
    }
    
    
    
    
}


void ofApp::onButtonEvent(ofxDatGuiButtonEvent e)
{
    if (e.target->is("Save Settings")) saveSettings();
    cout << "onButtonEvent: " << e.target->getLabel() << endl;
}

void ofApp::onToggleEvent(ofxDatGuiToggleEvent e)
{
    cout << "onToggleEvent: " << e.target->getLabel() << " " << e.checked << endl;
}

void ofApp::onSliderEvent(ofxDatGuiSliderEvent e)
{
    cout << "onSliderEvent: " << e.target->getLabel() << " "; e.target->printValue();
}

void ofApp::onTextInputEvent(ofxDatGuiTextInputEvent e)
{
    cout << "onTextInputEvent: " << e.target->getLabel() << " " << e.target->getText() << endl;
}

void ofApp::on2dPadEvent(ofxDatGui2dPadEvent e)
{
    cout << "on2dPadEvent: " << e.target->getLabel() << " " << e.x << ":" << e.y << endl;
}

void ofApp::onDropdownEvent(ofxDatGuiDropdownEvent e)
{
    if (e.target->is("Serial port")) {
        serial.setup("/dev/"+e.target->getLabel(), 115200);
        serial.startContinuousRead();
        serialDevice = e.target->getLabel();

        cout << "port: " << e.target->getLabel() << " Selected" << endl;

    }

    cout << "onDropdownEvent: " << e.target->getLabel() << " Selected" << endl;
}

void ofApp::onColorPickerEvent(ofxDatGuiColorPickerEvent e)
{
    cout << "onColorPickerEvent: " << e.target->getLabel() << " " << e.target->getColor() << endl;
    ofSetBackgroundColor(e.color);
}

void ofApp::onMatrixEvent(ofxDatGuiMatrixEvent e)
{
    cout << "onMatrixEvent " << e.child << " : " << e.enabled << endl;
    cout << "onMatrixEvent " << e.target->getLabel() << " : " << e.target->getSelected().size() << endl;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    // send a note on if the key is a letter or a number
    if(isalnum((unsigned char) key)) {
        
        // scale the ascii values to midi velocity range 0-127
        // see an ascii table: http://www.asciitable.com/
        note = ofMap(key, 48, 122, 0, 127);
        velocity = 64;
        midiOut.sendNoteOn(channel, note,  velocity);
        
        // print out both the midi note and the frequency
        ofLogNotice() << "note: " << note
        << " freq: " << ofxMidi::mtof(note) << " Hz";
    }
    
    if(key == 'l') {
        ofxMidiOut::listPorts();
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    switch(key) {
            
            // aftertouch
        case '[':
            touch = 64;
            midiOut.sendAftertouch(channel, touch);
            break;
        case ']':
            touch = 127;
            midiOut << Aftertouch(channel, touch); // stream interface
            break;
            
            // poly aftertouch
        case '<':
            polytouch = 64;
            midiOut.sendPolyAftertouch(channel, 64, polytouch);
            break;
        case '>':
            polytouch = 127;
            midiOut << PolyAftertouch(channel, 64, polytouch); // stream interface
            break;
            
            // print the port list
        case '?':
            midiOut.listPorts();
            break;
            
            // note off using raw bytes
        case ' ':
            // send with the byte stream interface, noteoff for note 60
            midiOut << StartMidi() << 0x80 << 0x3C << 0x40 << FinishMidi();
            break;
            
        default:
            
            // send a note off if the key is a letter or a number
            if(isalnum(key)) {
                note = ofMap(key, 48, 122, 0, 127);
                velocity = 0;
                midiOut << NoteOff(channel, note, velocity); // stream interface
            }
            break;
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

void ofApp::saveSettings(){
    XML.setValue("config:device0:midichannel", (int)midiChannelSlider[0]->getValue());
    XML.setValue("config:serial", serialDevice);
    XML.saveFile("settings.xml");

}

void ofApp::loadSettings(){
    if( XML.loadFile("settings.xml") ){
        cout << "mySettings.xml loaded!\n";
    }else{
        cout << "unable to load mySettings.xml check data/ folder\n";
    }
    cout << "midichannel: " << XML.getValue("config:device0:midichannel", 1) << "\n";
    //read the colors from XML
    //if the settings file doesn't exist we assigns default values (170, 190, 240)
//    midiChannelSlider[0]->setValue((double)XML.getValue("config:device0:midiChannel", 1));

}

