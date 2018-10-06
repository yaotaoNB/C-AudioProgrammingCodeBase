#include "waveGen.h"

void cinClear(){
    cin.clear();
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int main(void);
int main(void)
{
    ScopedPaHandler paInit;
    OSC_Basic *osc;
    int wavePick,pitch;
    string continune;

    if( paInit.result() != paNoError ) goto error;

 wavepick:
    cout<<"Choose a waveform, 1. sine 2. saw 3. square 4. triangle (type 1 or 2)"<<endl;
    cin>>wavePick;
    switch (wavePick)
    {
        case 1:
            cout<<"you picked sine"<<endl;
            osc = new SineGen();
            break;
        case 2:
            cout<<"you picked saw"<<endl;
            osc = new SawGen();
            break;
        case 3:
            cout<<"you picked square"<<endl;
            osc = new SquareGen();
            break;
        case 4:
            cout<<"you picked triangle"<<endl;
            osc = new TriangleGen();
            break;
        default:
            cout<<"Invalid value"<<endl;
            cinClear();
            goto wavepick;
    }
    cout<<"What's the frequency do you want? range from 50 Hz to 18000Hz"<<endl;
    cin>>pitch;

    if(pitch>=50 && pitch<=18000){//valid
        osc->frequency = pitch;
    }else{//invalid
        cout<<"Invalid frequency value."<<endl;
        return 1;
    }
    
    cinClear();

    if (osc->open(Pa_GetDefaultOutputDevice()))
                {
                    if (osc->start())
                    {
                        printf("Play for %d seconds.\n", NUM_SECONDS );
                        Pa_Sleep( NUM_SECONDS * 1000 );
                        osc->stop();
                    }
                    osc->close();
                }
    continuing:
        cout<<"Continune to play? type Yes or No."<<endl;
            cin>>continune;
            if(continune == "Yes" || continune == "yes"){
                cinClear();
                goto wavepick;
            }else if(continune == "No" || continune == "no"){
                cout<<"Bye!\n"<<endl;
                printf("Finished.\n");
                delete osc;
                return paNoError;
            }else{
                cout<<"Please type either Yes or No."<<endl;
                goto continuing;
            }

    error:
    fprintf( stderr, "An error occured while using the portaudio stream\n" );
    fprintf( stderr, "Error number: %d\n", paInit.result() );
    fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( paInit.result() ) );
    return 1;
}