#ifndef _INPUTS_H
#define _INPUTS_H

#include<_common.h>
#include<_model.h>
#include<_parallax.h>
#include<_2Dplyer.h>

class _inputs
{
    public:
        _inputs();
        virtual ~_inputs();

        void keyPressed(_model *);
        void keyUp(); // ToDo: address accordingly

        void keyPressed(_parallax *, float);

        void mouseEventDown(_model*, double,double);
        void mouseEventUp();// ToDo: address accordingly

        void mouseWheel(_model*,double);
        void mouseMove(_model*,double,double);

        void keyPressed(_2Dplyer *);

        double prev_mouseX;
        double prev_mousey;

        bool mouse_translate; // flag to indicate mouse move
        bool mouse_rotate; //flag to indicate mouse rotation

        WPARAM wParam;

    protected:

    private:
};

#endif // _INPUTS_H
