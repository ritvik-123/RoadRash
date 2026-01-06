#include "_sounds.h"

_sounds::_sounds()
{
    //ctor
    eng = createIrrKlangDevice();
}

_sounds::~_sounds()
{
    //dtor
    if(eng) {
        eng->drop();
    }
}

void _sounds::playMusic(char* fileName)
{
    if(eng) {
        eng->play2D(fileName, true);
    }
}

void _sounds::PlaySoundA(char* fileName)
{
    if(eng && fileName) {
        if(!eng->isCurrentlyPlaying(fileName))
            eng->play2D(fileName, false, false);
    }
}

void _sounds::pauseSound(char* fileName)
{
    if(eng) {
        eng->play2D(fileName, true, false);
    }
}

int _sounds::initSound()
{
    if(!eng)
    {
        cout<<"ERROR: *** the sound engine could not Load"<<endl;
        return 0;
    }
    return 1;
}
