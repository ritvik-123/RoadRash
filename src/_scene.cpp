#include "_scene.h"
#include <_lighting.h>
#include <_model.h>
#include <_inputs.h>
#include <_parallax.h>
#include <_textureLoader.h>
#include <_2Dplyer.h>

float eyeX = 0.0f, eyeY = 0.0f, eyeZ = 0.0f;
float roadTexOffset = 0.0f;
float grassTexOffset = 0.0f;

_2Dplyer *ply2D = new _2Dplyer();
_lighting *myLight = new _lighting();
_model *myModel = new _model();
_model *myModel1 = new _model();
_inputs *myKbMs = new _inputs();
_parallax *prlx1 = new _parallax();
_parallax *prlx3 = new _parallax();
_parallax *prlx4 = new _parallax();
_parallax *prlx5 = new _parallax();
_parallax *prlx6 = new _parallax();
_parallax *prlx7 = new _parallax();
_parallax *prlx8 = new _parallax();

_textureLoader* myTex = new _textureLoader();
_textureLoader* myTex1 = new _textureLoader();

bool autoScroll = false;
bool NMode = false;
float road_angle = 0;
bool isUpPressed = false;
bool isLeftPressed = false;
bool isRightPressed = false;

GLuint roadTexID;
GLuint grassTexID;
float texOffsetY = 0.0f; // For scrolling the road texture

_scene::_scene() {}
_scene::~_scene() {}

GLvoid _scene::resizeWindow(GLsizei width, GLsizei height)
{
    GLfloat Ratio = (GLfloat)width / (GLfloat)height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, Ratio, 0.1, 1000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    dim.x = GetSystemMetrics(SM_CXSCREEN);
    dim.y = GetSystemMetrics(SM_CYSCREEN);
}

GLint _scene::IniGL()
{
    glClearColor(0.0, 0.0, 1.0, 1.0);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    myLight->setupLight(GL_LIGHT0);

    // Load sky parallax
    prlx1->parallaxInit("images/Sky-4.jpg");
    prlx3->parallaxInit("images/prlx3.png");
    prlx4->parallaxInit("images/prlx5.png");

    ply2D->ply2DInit("images/Bike-1.png",4,3);

    // Load road texture
    myTex->loadImage("images/Road.jpg");
    myTex1->loadImage("images/prlx41.jpeg");
    if (myTex->texID == 0) {
        std::cerr << "Error: Road texture failed to load!" << std::endl;
        return false;
    }
    roadTexID = myTex->texID;
    grassTexID = myTex1->texID;

    // Set texture wrap to repeat
    glBindTexture(GL_TEXTURE_2D, roadTexID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindTexture(GL_TEXTURE_2D, grassTexID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


    dim.x = GetSystemMetrics(SM_CXSCREEN);
    dim.y = GetSystemMetrics(SM_CYSCREEN);

    return true;
}

GLvoid _scene::renderScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);  // <-- Add this!
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

    glLoadIdentity();

    //gluLookAt(0.0f, 0.0f, 5.0f,0.0f, 0.0f, 0.0f,0.0f, 1.0f, 0.0f);

    // --- Draw Sky Parallax ---
    glPushMatrix();
    glScalef(4.33f, 4.33f, 1.0f);
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glTranslatef(0.0f, 0.0f, 0.0f);
    prlx1->drawBkgrnd(dim.x, dim.y);
    glTranslatef(0.0f, 0.3f, 0.0f);
    prlx3->drawBkgrnd(dim.x, dim.y);
    prlx4->drawBkgrnd(dim.x, dim.y);

    glScalef(0.5f, 0.5f, 0.5f);

    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.1f);  // Only draw pixels with alpha > 0.1

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    ply2D->ply2DAction();
    glTranslatef(0.0f, -1.27f, -4.9f);
    ply2D->drawPly2D();

    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glPopMatrix();

    // --- Draw Road ---
    glPushMatrix();
    glTranslatef(0.0f, -2.55f, -5.0f);
    glRotatef(-80.0f, 1.0f, 0.0f, 0.0f);
    glScalef(1.5f, 5.0f, 1.0f);  // Large enough to cover screen

    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, roadTexID);
    glColor3f(1.0f, 1.0f, 1.0f);

    float repeatY = 8.0f;  // Texture repeats once over the height of the quad

    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f + roadTexOffset);         glVertex3f(-1.0f, -1.0f, 0.0f);
        glTexCoord2f(1.0f, 0.0f + roadTexOffset);         glVertex3f( 1.0f, -1.0f, 0.0f);
        glTexCoord2f(1.0f, repeatY + roadTexOffset);      glVertex3f( 0.9f,  1.0f, 0.0f);
        glTexCoord2f(0.0f, repeatY + roadTexOffset);      glVertex3f(-0.9f,  1.0f, 0.0f);
    glEnd();


    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glPopMatrix();

        // --- Draw Grass ---
    glPushMatrix();
    glTranslatef(0.0f, -2.55f, -5.1f);
    glRotatef(-80.0f, 1.0f, 0.0f, 0.0f);
    glScalef(15.0f, 5.0f, 1.0f);  // Large enough to cover screen

    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, grassTexID);
    glColor3f(1.0f, 1.0f, 1.0f);

    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f + grassTexOffset);         glVertex3f(-1.0f, -1.0f, 0.0f);
        glTexCoord2f(1.0f, 0.0f + grassTexOffset);         glVertex3f( 1.0f, -1.0f, 0.0f);
        glTexCoord2f(1.0f, repeatY + grassTexOffset);      glVertex3f( 1.0f,  1.0f, 0.0f);
        glTexCoord2f(0.0f, repeatY + grassTexOffset);      glVertex3f(-1.0f,  1.0f, 0.0f);
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glPopMatrix();
    if (isUpPressed) {
        prlx1->prlxScrollAuto("up", 0.002);
        prlx5->prlxScrollAuto("down", 0.003);
        myKbMs->keyPressed(prlx1, 0.005);
        roadTexOffset += 0.04f;
        if (roadTexOffset > 1.0f) roadTexOffset = 0.0f;
        grassTexOffset += 0.025f;
        if (grassTexOffset > 1.0f) grassTexOffset = 0.0f;
    }

    if (isRightPressed) {
        //myKbMs->keyPressed(prlx1, 0.001);
        myKbMs->keyPressed(prlx3, 0.002);
        myKbMs->keyPressed(prlx4, 0.005);
        if (eyeX < 5.0f) eyeX += 1.0f;
    }

    if (isLeftPressed) {
        //myKbMs->keyPressed(prlx1, 0.001);
        myKbMs->keyPressed(prlx3, 0.002);
        myKbMs->keyPressed(prlx4, 0.005);
        if (eyeX > -5.0f) eyeX -= 1.0f;
    }
}



int _scene::winMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_KEYDOWN:
        myKbMs->wParam = wParam;
        myKbMs->keyPressed(myModel);
        myKbMs->keyPressed(ply2D);
        myKbMs->keyPressed(myModel1);
        case VK_UP: isUpPressed = true; break;
        case VK_LEFT: isLeftPressed = true; break;
        case VK_RIGHT: isRightPressed = true; break;
        case VK_DOWN:
            myKbMs->keyPressed(prlx1, 0.005);
            if (NMode) {
                myKbMs->keyPressed(prlx4, 0.005);
                myKbMs->keyPressed(prlx6, 0.005);
            }
            break;

        case VK_SPACE:
            autoScroll = !autoScroll;
            std::cout << "Auto-scroll: " << (autoScroll ? "ON" : "OFF") << std::endl;
            break;

        case 'N':
        case 'n':
            NMode = !NMode;
            std::cout << "N-Mode: " << (NMode ? "ON" : "OFF") << std::endl;
            break;

        break;

    case WM_KEYUP:
        switch (wParam) {
            case VK_UP: isUpPressed = false; break;
            case VK_LEFT: isLeftPressed = false; break;
            case VK_RIGHT: isRightPressed = false; break;
        }
        ply2D->actiontrigger=0;
        break;

    case WM_LBUTTONDOWN:
    case WM_RBUTTONDOWN:
    case WM_MBUTTONDOWN:
        myKbMs->wParam = wParam;
        myKbMs->mouseEventDown(myModel, LOWORD(lParam), HIWORD(lParam));
        myKbMs->mouseEventDown(myModel1, LOWORD(lParam), HIWORD(lParam));
        break;

    case WM_LBUTTONUP:
    case WM_RBUTTONUP:
    case WM_MBUTTONUP:
        myKbMs->mouseEventUp();
        break;

    case WM_MOUSEMOVE:
        myKbMs->mouseMove(myModel, LOWORD(lParam), HIWORD(lParam));
        myKbMs->mouseMove(myModel1, LOWORD(lParam), HIWORD(lParam));
        break;

    case WM_MOUSEWHEEL:
        myKbMs->mouseWheel(myModel, (double)GET_WHEEL_DELTA_WPARAM(wParam));
        myKbMs->mouseWheel(myModel1, (double)GET_WHEEL_DELTA_WPARAM(wParam));
        break;
    }
    return 0;
}
