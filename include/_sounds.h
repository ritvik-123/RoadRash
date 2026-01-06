#ifndef _SOUNDS_
#define _SOUNDS_

#include<_common.h>
#include<SNDS/irrKlang.h>

using namespace irrklang;

class _sounds
{
    public:
        _sounds();
        virtual ~_sounds();

        ISoundEngine* eng;

        void playMusic(char *);
        void PlaySoundA(char *);
        void pauseSound(char *);
        int initSound();

    protected:

    private:
};

#endif // _SOUNDS_
