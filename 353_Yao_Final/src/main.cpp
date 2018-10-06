//g++ -std=c++14 -I../usr/local/include -L../usr/local/lib -lrtmidi -lportaudio ../modules/modular.cpp main.cpp -o final_project

//#include <cstring>
#include <cstdlib>
#include <cstring>
#include "RtMidi.h"
#include "../modules/modular.h"

void cinClear(){
    cin.clear();
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

//Call back will be called twice, once for note on, another for note off
void rtmidi_callback( double deltatime, std::vector< unsigned char > *message, void *userData ) {

  OSC_Basic* osc = (OSC_Basic*) userData;
  unsigned int nBytes = message->size();
  if((int)message->at(0) == 144 || (int)message->at(0) == 145){
    osc->start_stream((float)message->at(2), (int)message->at(1));
  }else if((int)message->at(0) == 128 || (int)message->at(0) == 129){
    osc->stop_stream();
  }
}

int main()
{
  ScopedPaHandler scope_pa_handler;
  OSC_Basic* osc = new OSC_Basic();
  osc->fill_table(Square);
  int sel;
  RtMidiIn *midiin = new RtMidiIn();
  std::string portName;
  // Check available ports.
  unsigned int nPorts = midiin->getPortCount();
  std::cout << "\nThere are " << nPorts << " MIDI input sources available.\n";

  cout<<"Pick a waveform to play, 1. Sine, 2. Saw, 3. Square, 4. Triangle, 5. Load Custom wavetable (currently not available)."<<endl;
  cin>>sel;
  switch(sel){
    case 1:
        osc->fill_table(Sine);
      break;
    case 2:
        osc->fill_table(Saw);
      break;
    case 3:
        osc->fill_table(Square);
      break;
    case 4:
        osc->fill_table(Triangle);
      break;
    case 5:
      cout<<"Load Custom wavetable is currently not available."<<endl;
      goto cleanup;
    default:
      cout<<"Non-valid choice."<<endl;
      goto cleanup;
  }

  cinClear();

  for ( unsigned int i=0; i<nPorts; i++ ) {
    try {
      portName = midiin->getPortName(i);
    }
    catch ( RtMidiError &error ) {
      error.printMessage();
      goto cleanup;
    }
    //std::cout << "  Input Port #" << i+1 << ": " << portName << '\n';
  }
  if ( nPorts == 0 ) {
    std::cout << "No ports available!\n";
    goto cleanup;
  }
  midiin->openPort( 0 );

  midiin->setCallback( &rtmidi_callback, osc);
  // Don't ignore sysex, timing, or active sensing messages.
  midiin->ignoreTypes( false, false, false );
  std::cout << "\nReading MIDI input ... press <enter> to quit.\n";

  while(1) {
    int get_cmd;
    cout<<"Command list: 1. add echo module 2. add filter module 3. adsr module, 4. show all current modules being added in the queue "<<endl;
    cout<<"5. remove one module in the queue from the back 6. quit the program"<<endl;
    cin>>get_cmd;

    switch (get_cmd) {
      case 1:
        osc->Add_Module(Delay);
        cout<<"echo module is added."<<endl;
        break;
      case 2:
        osc->Add_Module(LPF);
        cout<<"LPF module is added."<<endl;
        break;
      case 3:
        osc->Add_Module(ADSR);
        cout<<"ADSR module is added."<<endl;
        break;
      case 4:
        cout<<"All current modules in the queue: "<<endl;
        osc->Show_module();
        break;
      case 5:
        osc->rm_module();
        break;
      case 6:
        cout<<"Program quit."<<endl;
        goto cleanup;
        break;
      default:
        cout<<"Invalid command."<<endl;
        break;
    }
    cinClear();
  }

  // Clean up
 cleanup:
  delete midiin;
  delete osc;
  return 0;
}

