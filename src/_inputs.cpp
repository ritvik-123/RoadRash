#include "_inputs.h"

_inputs::_inputs()
{
    //ctor

    mouse_translate=false;
    mouse_rotate = false;
    bool autoScroll = false;
}

_inputs::~_inputs()
{
    //dtor
}
void _inputs::keyPressed(_2Dplyer* ply)
{
    switch(wParam)
    {
        case VK_LEFT:
             ply->actiontrigger=ply->WALKLEFT;
             break;
        case VK_RIGHT:
             ply->actiontrigger=ply->WALKRIGHT;
             break;
        case VK_UP:
             ply->actiontrigger=ply->WALKFRONT;
             break;
        case VK_DOWN:
             ply->actiontrigger=ply->WALKBACK;
             break;
    }
}
void _inputs::keyPressed(_model* mdl)
{
    switch(wParam)
    {
        case VK_LEFT:
              mdl->rot.y +=1.0;
             break;
        case VK_RIGHT:
              mdl->rot.y -=1.0;
             break;
        case VK_UP:
              mdl->rot.x +=1.0;
             break;
        case VK_DOWN:
              mdl->rot.x -=1.0;
             break;
    }

}

void _inputs::keyPressed(_parallax* prlx, float speed)
{
     switch(wParam)
    {
        case VK_LEFT:
             prlx->xMin +=0.005;
             prlx->xMax +=0.005;
             prlx->prlxScrollAuto("left",speed);
             break;
        case VK_RIGHT:
             prlx->xMin -=0.005;
             prlx->xMax -=0.005;
             prlx->prlxScrollAuto("right",speed);
             break;
        case VK_UP:
             prlx->yMin +=0.005;
             prlx->yMax +=0.005;
             prlx->prlxScrollAuto("up",speed);
             break;
        case VK_DOWN:
             prlx->yMin -=0.005;
             prlx->yMax -=0.005;
             prlx->prlxScrollAuto("down",speed);
             break;
        case VK_SPACE:
             prlx->prlxScrollAuto("up",0.005);
             break;
    }

}


void _inputs::keyUp()
{
   switch(wParam)
   {
       default: break;
   }
}

void _inputs::mouseEventDown(_model* mdl, double x, double y)
{
       switch(wParam)
       {
       case MK_LBUTTON:
              mouse_rotate = true;
             break;
       case MK_RBUTTON:
               mouse_translate =true;
             break;
       case MK_MBUTTON:
             break;
       }
       prev_mouseX = x;
       prev_mousey = y;
}

void _inputs::mouseEventUp()
{
     mouse_rotate = false;
     mouse_translate = false;
}

void _inputs::mouseWheel(_model* mdl, double delta)
{
     mdl->pos.z +=delta/100.0;
}

void _inputs::mouseMove(_model* mdl, double x, double y)
{
    if(mouse_rotate)
    {
        mdl->rot.y += (x-prev_mouseX)/3.0;
        mdl->rot.x += (y-prev_mousey)/3.0;
    }
    if(mouse_translate)
    {
       mdl->pos.x += (x-prev_mouseX)/100.0;
       mdl->pos.y -= (y-prev_mousey)/100.0;
    }
    prev_mouseX =x;
    prev_mousey =y;
}
