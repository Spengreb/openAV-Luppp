
#ifndef LUPPP_JACK_H
#define LUPPP_JACK_H

// Library
#include <vector>
#include <cstring>
#include <jack/jack.h>
#include <jack/midiport.h>
#include <jack/transport.h>

#include "logic.hxx"
#include "config.hxx"
#include "looper.hxx"
#include "metronome.hxx"
#include "gridlogic.hxx"
#include "trackoutput.hxx"
#include "timemanager.hxx"
#include "controllerupdater.hxx"

#include "dsp/dsp_reverb.hxx"
#include "dsp/dsp_dbmeter.hxx"
#include "dsp/dsp_sidechain_gain.hxx"

using namespace std;

/** Jack
  This code contains the JACK client.
  It allows reading / writing of audio / midi.
**/
class Jack
{
  public:
    Jack();
    
    void activate();
    int getBuffersize();
    int getSamplerate();
    
    
    /// get functions for components owned by Jack 
    Looper*             getLooper(int t) {return loopers.at(t); }
    Metronome*          getMetronome(){return metronome;}
    Logic*              getLogic(){return logic;}
    GridLogic*          getGridLogic(){return gridLogic;}
    TrackOutput*        getTrackOutput(int t){return trackOutputs.at(t);}
    TimeManager*        getTimeManager(){return &timeManager;}
    ControllerUpdater*  getControllerUpdater(){return controllerUpdater;}
    
    /// register MIDI observers: they're called when a MIDI message arrives on
    /// a port they're watching
    void registerMidiObserver( MidiObserver* mo )
    {
      midiObservers.push_back( mo );
    }
    
    
    /// sets reverb bus parameters
    void setReverb( bool e, float d, float s );
    
    /// writes MIDI messages to APC port
    void writeApcOutput( unsigned char* data );
  
  private:
    jack_client_t* client;
    
    Buffers             buffers;
    TimeManager         timeManager;
    Metronome*          metronome;
    Logic*              logic;
    GridLogic*          gridLogic;
    ControllerUpdater*  controllerUpdater;
    
    vector<Looper*>         loopers;
    vector<TrackOutput*>    trackOutputs;
    vector<MidiObserver*>   midiObservers;
    
    // FX
    Reverb* reverb;
    SidechainGain* sidechainGain;
    DBMeter* reverbMeter;
    DBMeter* masterMeter;
    
    // JACK member variables
    bool clientActive;
    
    jack_port_t*  masterInput;
    jack_port_t*  masterOutputL;
    jack_port_t*  masterOutputR;
    
    jack_port_t*  apcMidiInput;
    jack_port_t*  apcMidiOutput;
    jack_port_t*  masterMidiInput;
    
    // JACK callbacks
    int process (jack_nframes_t);
    
    int timebase(jack_transport_state_t,
                 jack_nframes_t,
                 jack_position_t*,
                 int );
    
    // static JACK callbacks
    static int static_process  (jack_nframes_t, void *);
    
    static int static_timebase (jack_transport_state_t,
                                jack_nframes_t,
                                jack_position_t*,
                                int,
                                void* );
    
    // UI update variables
    int uiUpdateCounter;
    int uiUpdateConstant;
};

#endif // LUPPP_JACK_H

