#ifndef _2DPLYER_H
#define _2DPLYER_H

#include<_common.h>
#include<_timer.h>
#include<_textureloader.h>


class _2Dplyer
{
    public:
        _2Dplyer();

        virtual ~_2Dplyer();

        _timer *myTime = new _timer();
        _textureLoader *myTex = new _textureLoader();

        vec3 pos; // player position
        vec3 rot; // player rotation
        vec2 pSize; // player scale

        vec2 frames;
        vec3 vertx[4]; // for drawing Quad

        enum{STAND,WALKLEFT,WALKRIGHT, WALKFRONT,WALKBACK};
        float xMin, xMax, yMin, yMax;
        int actiontrigger;

        void ply2DInit(char *, int, int);
        void drawPly2D();
        void ply2DAction();



    protected:

    private:
};

#endif // _2DPLYER_H
