#include "_scene.h"
#include <_lighting.h>
#include <_model.h>
#include <_inputs.h>
#include <_parallax.h>
#include <_textureLoader.h>
#include <_2Dplyer.h>
#include <_helppage.h>
#include <_landingpage.h>
#include <_Skybox.h>
#include <_menupage.h>
#include<_sounds.h>
#include <cstdio>
#include <stdio.h>
#include <vector>


// Global instances - these should ideally be member variables of _scene
_2Dplyer *ply2D = nullptr;
_lighting *myLight = nullptr;
_model *myModel = nullptr;
_model *myModel1 = nullptr;
_inputs *myKbMs = nullptr;



// Parallax layers and skybox
_parallax *prlx1 = nullptr;
_parallax *prlx3 = nullptr;
_parallax *prlx4 = nullptr;
_parallax *prlx5 = nullptr;
_parallax *prlx6 = nullptr;

_skyBox *sky = nullptr;


_textureLoader* myTex = nullptr;
_textureLoader* myTex1 = nullptr;
_textureLoader* myTex2 = nullptr;
_textureLoader* myTex3 = nullptr;
_textureLoader* myTex4 = nullptr;
_textureLoader* myTex5 = nullptr;
_textureLoader *bTex1 = new _textureLoader();
_textureLoader *bTex2 = new _textureLoader();
_textureLoader *bTex3 = new _textureLoader();
_textureLoader *bTex4 = new _textureLoader();
_textureLoader *bTex5 = new _textureLoader();


_sounds* snds = nullptr;

struct Obstacle {
        vec2 pos;     // Position (x, z)
        float radius;      // Collision radius
        bool active = true; // Is the obstacle currently active
    };

std::vector<Obstacle> obstacles;
int playerHealth = 100;
float obstacleSpawnInterval = 1.5f;
float lastSpawnZ = 0.0f;

// Constructor
_scene::_scene() {
    currentState = STATE_LANDING;

    landingPage = new _landingPage();
    menuPage    = new _menuPage();
    helpPage    = new _helpPage();
    pausePage   = new _pausePage();

    //initializeGameComponents();

}

// Virtual Destructor - MANDATORY
_scene::~_scene() {

}



void _scene::initializeGameComponents()
{
    // Allocate and initialize game-specific components
    ply2D = new _2Dplyer();
    myLight = new _lighting();
    myModel = new _model();
    myModel1 = new _model();
    myKbMs = new _inputs();

    // Parallax layers
    prlx1 = new _parallax();
    prlx3 = new _parallax();
    prlx4 = new _parallax();
    prlx5 = new _parallax();
    prlx6 = new _parallax();

    sky = new _skyBox();


    myTex = new _textureLoader();
    myTex1 = new _textureLoader();
    myTex2 = new _textureLoader();
    myTex3 = new _textureLoader();
    myTex4 = new _textureLoader();
    myTex5 = new _textureLoader();

    snds = new _sounds();

    // Reset game-specific flags
    roadTexOffset = 0.0f;
    grassTexOffset = 0.0f;
    eyeX = 0.0f;
    isUpPressed = false;
    isLeftPressed = false;
    isRightPressed = false;
    isSpacePressed = false;
    currentLevel = 1;
    scrollDistance = 0.0f;
    //levelThreshold = 100.0f; // adjust as needed
    // Clear color and depth
    glClearColor(0.0, 0.0, 1.0, 1.0);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // Enable lighting and color material
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);

    // Setup light
    if (!myLight) {
        myLight = new _lighting();
    }
    myLight->setupLight(GL_LIGHT0);


    // Load textures for parallax layers
    prlx1->parallaxInit("images/Sky-3.jpg");
    prlx3->parallaxInit("images/prlx3.png");
    prlx4->parallaxInit("images/prlx5.png");
    prlx5->parallaxInit("images/prlx3C.png");
    prlx6->parallaxInit("images/night.png");

    // Initialize skybox
    sky->skyBoxInit();

    // Initialize 2D player with sprite sheet
    ply2D->ply2DInit("images/Bike-21 (3).png", 6, 5);

    // Load textures
    myTex->loadImage("images/Road.jpg");
    myTex1->loadImage("images/prlx41.jpeg");
    myTex2->loadImage("images/skin-2.jpeg");
    myTex3->loadImage("images/Build1.jpg");
    myTex4->loadImage("images/Build5.jpg");
    myTex5->loadImage("images/Build5.jpeg");
    bTex1->loadImage("images/level 1.png");
    bTex2->loadImage("images/level 2.png");
    bTex3->loadImage("images/Final Level.png");
    //bTex4->loadImage("images/pause.jpg");
    //bTex5->loadImage("images/exit.jpg");

    // Store texture IDs
    roadTexID = myTex->texID;
    grassTexID = myTex1->texID;
    bushTexID = myTex2->texID;
    build1TexID = myTex3->texID;
    build2TexID = myTex4->texID;
    build3TexID = myTex5->texID;


    // Set texture parameters for road and grass textures
    glBindTexture(GL_TEXTURE_2D, roadTexID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindTexture(GL_TEXTURE_2D, grassTexID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    snds->initSound();

    // Play background music
    snds->playMusic("sounds/Main Theme.MP3");
}

void _scene::spawnObstacle() {
    Obstacle obs;
    obs.pos.x = (rand() % 2 == 0) ? -0.3f : 0.3f; // left or right side of the road
    obs.pos.y = scrollDistance + 5.0f;            // Spawn ahead of player
    obs.radius = 0.1f;
    obstacles.push_back(obs);
}

void _scene::drawHealthBar() {
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600); // screen dimensions

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    float barWidth = playerHealth * 2.0f;
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
        glVertex2f(10, 580);
        glVertex2f(10 + barWidth, 580);
        glVertex2f(10 + barWidth, 560);
        glVertex2f(10, 560);
    glEnd();

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_LIGHTING);
}

void _scene::cleanupGameComponents()
{
    delete ply2D; ply2D = nullptr;
    delete myLight; myLight = nullptr;
    delete myModel; myModel = nullptr;
    delete myModel1; myModel1 = nullptr;
    delete myKbMs; myKbMs = nullptr;

    delete prlx1; prlx1 = nullptr;
    delete prlx3; prlx3 = nullptr;
    delete prlx4; prlx4 = nullptr;
    delete prlx5; prlx5 = nullptr;
    delete prlx6; prlx6 = nullptr;

    delete sky; sky = nullptr;

    delete myTex; myTex = nullptr;
    delete myTex1; myTex1 = nullptr;
    delete myTex2; myTex2 = nullptr;
    delete myTex3; myTex3 = nullptr;
    delete myTex4; myTex4 = nullptr;
    delete myTex5; myTex5 = nullptr;

    delete snds; snds = nullptr;
}


void _scene::resetPlayerState() {
    if (ply2D) {
        // Reset player position to starting point
        ply2D->pos.x = 0.0f;  // Adjust X as needed
        ply2D->pos.y = 0.0f;  // Adjust Y as needed
    }

    // Optional: Reset input flags
    isUpPressed = false;
    isLeftPressed = false;
    isRightPressed = false;
    isSpacePressed = false;
}


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

// Render full screen texture
void renderFullScreenTexture(GLuint texID) {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texID);

    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(0, 0);
        glTexCoord2f(1.0f, 1.0f); glVertex2f(800, 0);
        glTexCoord2f(1.0f, 0.0f); glVertex2f(800, 600);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(0, 600);
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);


}

// Screen rendering functions
void renderLVL1() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    renderFullScreenTexture(bTex1->texID);
}
// Screen rendering functions
void renderLVL2() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    renderFullScreenTexture(bTex2->texID);
}
// Screen rendering functions
void renderLVL3() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    renderFullScreenTexture(bTex3->texID);
}

GLint _scene::IniGL()
{
    initializeGameComponents();


        // Initialize pages
    if (landingPage) landingPage->landingInit();
    if (menuPage) menuPage->menuInit();
    if (helpPage) helpPage->helpInit();


    // Get screen dimensions
    dim.x = GetSystemMetrics(SM_CXSCREEN);
    dim.y = GetSystemMetrics(SM_CYSCREEN);

    // Reset game state variables
    currentLevel = 1;
    scrollDistance = 0.0f;
    eyeX = 0.0f;
    isUpPressed = false;
    isLeftPressed = false;
    isRightPressed = false;
    isSpacePressed = false;

    return true;
}

GLvoid _scene::renderScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Render based on current game state
    switch (currentState) {
        case STATE_LANDING:
            renderLanding();
            break;
        case STATE_MENU:
            renderMenu();
            break;
        case STATE_HELP:
            renderHelp();
            break;
        case STATE_GAME:
            renderGame();
            break;
        case STATE_PAUSE:
            renderPause();
            break;
        case STATE_LEVEL_TRANSITION_1:
            renderLVL1();
            break;
        case STATE_LEVEL_TRANSITION_2:
            renderLVL2();
            break;
        case STATE_LEVEL_TRANSITION_3:
            renderLVL3();
            break;
    }
}


void _scene::renderLanding()
{
    landingPage->drawLandingPage();

    // Check for transition from landing to menu
    if (landingPage->isTransitionRequested()) {
        currentState = STATE_MENU;
        menuPage->menuInit(); // Ensure menu is initialized
        landingPage->resetTransitionState();
    }
}

void _scene::renderMenu()
{
    menuPage->drawMenuPage();

    // Handle menu transitions
    if (menuPage->isTransitionRequested()) {
        switch (menuPage->getSelectedOption()) {
            case _menuPage::NEW_GAME:
                cleanupGameComponents();
                initializeGameComponents();
                currentLevel = 1;
                currentState = STATE_GAME;
                // Additional game initialization if needed
                break;
            case _menuPage::HELP:
                currentState = STATE_HELP;
                helpPage->helpInit();
                break;
            case _menuPage::EXIT:
                // Trigger application exit
                PostQuitMessage(0);
                break;
        }
        menuPage->resetTransitionState();
    }
}

void _scene::renderHelp()
{
    helpPage->drawHelpPage();

    // Check for return to menu
    if (helpPage->isTransitionRequested()) {
        currentState = STATE_MENU;
        helpPage->resetTransitionState();
    }
}

void _scene::renderPause()
{
    pausePage->drawPausePage();

    isUpPressed = false;
    isLeftPressed = false;
    isRightPressed = false;
    isSpacePressed = false;

    // Handle pause menu transitions
    if (pausePage->isTransitionRequested()) {
        int selectedOption = pausePage->selectedOption;
        switch(selectedOption) {
            case _pausePage::RESUME: // Resume
                currentState = STATE_GAME;
                break;
            case _pausePage::OPTIONS: // Options (you can implement this later)
                break;
            case _pausePage::EXIT: // Exit to menu
                // Explicitly set state and reinitialize menu
                currentState = STATE_MENU;

                // Reinitialize menu page components
                delete menuPage; // Delete existing menu page
                menuPage = new _menuPage(); // Create a new menu page instance
                menuPage->menuInit(); // Reinitialize menu

                // Reset any game-specific components if needed
                //cleanupGameComponents();
                initializeGameComponents();
                break;
            case _pausePage::HELP: // If you have a HELP option in pause menu
                currentState = STATE_HELP;
                helpPage->helpInit();
                break;
        }
        pausePage->resetTransitionState();
    }
}


void _scene::renderGame() {
    // Prepare scene
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glLoadIdentity();

    // Common setup
    glPushMatrix();
    glScalef(4.33f, 4.33f, 1.0f);
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glTranslatef(0.0f, 0.3f, 0.0f);

    // --- LEVEL-SPECIFIC BACKGROUND ---
    if (currentLevel == 1) {
        prlx1->drawBkgrnd(dim.x, dim.y);
        prlx3->drawBkgrnd(dim.x, dim.y);
        prlx4->drawBkgrnd(dim.x, dim.y);
    }
    else if (currentLevel == 2) {
        prlx5->drawBkgrnd(dim.x, dim.y);
        prlx4->drawBkgrnd(dim.x, dim.y);
    }
    else if (currentLevel == 3) {
        glColor3f(0.2f, 0.25f, 0.4f);
        sky->drawSkyBox();
    }

    glScalef(0.5f, 0.5f, 0.5f);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.1f);

    // --- PLAYER ---
    glScalef(1.2f, 1.1f, 1.0f);
    ply2D->ply2DAction();

    if (currentLevel == 3) {
        glTranslatef(0.05f, -1.0f, 1.2f);
    } else {
        glTranslatef(0.05f, -1.33f, -1.7f);
    }

    glColor3f(1.0f, 1.0f, 1.0f);
    ply2D->drawPly2D();

    // Cleanup
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glPopMatrix();

    // --- ROAD ---
    glPushMatrix();
    glTranslatef(0.0f, -2.55f, -5.0f);
    glRotatef(-80.0f, 1.0f, 0.0f, 0.0f);
    glScalef(3.5f, 5.0f, 1.0f);
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, roadTexID);

    if (currentLevel == 3) glColor3f(0.4f, 0.25f, 0.4f);
    else glColor3f(1.0f, 1.0f, 1.0f);

    float repeatY = 8.0f;
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, roadTexOffset);         glVertex3f(-1.0f, -1.0f, 0.0f);
        glTexCoord2f(1.0f, roadTexOffset);         glVertex3f( 1.0f, -1.0f, 0.0f);
        glTexCoord2f(1.0f, repeatY + roadTexOffset); glVertex3f( 0.8f, 1.0f, 0.0f);
        glTexCoord2f(0.0f, repeatY + roadTexOffset); glVertex3f(-0.8f, 1.0f, 0.0f);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glPopMatrix();

    // --- GRASS ---
    glPushMatrix();
    glTranslatef(0.0f, -2.55f, -5.1f);
    glRotatef(-80.0f, 1.0f, 0.0f, 0.0f);
    glScalef(15.0f, 5.0f, 1.0f);
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, grassTexID);

    if (currentLevel == 3)
        glColor3f(0.2f, 0.25f, 0.4f);
    else
        glColor3f(1.0f, 1.0f, 1.0f);

    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, grassTexOffset); glVertex3f(-1.0f, -1.0f, 0.0f);
        glTexCoord2f(1.0f, grassTexOffset); glVertex3f( 1.0f, -1.0f, 0.0f);
        glTexCoord2f(1.0f, repeatY + grassTexOffset); glVertex3f( 1.0f, 1.0f, 0.0f);
        glTexCoord2f(0.0f, repeatY + grassTexOffset); glVertex3f(-1.0f, 1.0f, 0.0f);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glPopMatrix();

    // --- BUSHES ---
    glPushMatrix();
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, bushTexID);
    glColor3f(1.0f, 1.0f, 1.0f);
    float spacing = 1.0f, visibleRange = 6.0f, startZ = 0.0f;
    float endZ = startZ - visibleRange;
    float scrollZ = fmod(scrollDistance, spacing);
    GLUquadric* quad = gluNewQuadric();
    gluQuadricTexture(quad, GL_TRUE);

    for (float z = startZ + scrollZ; z > endZ; z -= spacing) {
        glPushMatrix();
        glTranslatef(1.9f, -1.0f, z);
        glScalef(0.5f, 0.5f, 0.5f);
        gluSphere(quad, 0.3f, 32, 32);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(-1.9f, -1.0f, z);
        glScalef(0.5f, 0.5f, 0.5f);
        gluSphere(quad, 0.3f, 32, 32);
        glPopMatrix();
    }
    gluDeleteQuadric(quad);
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glPopMatrix();

    // --- BUILDINGS (only level 1 & 2) ---
    if (currentLevel >= 2) {
        glPushMatrix();
        glDisable(GL_LIGHTING);
        glEnable(GL_TEXTURE_2D);
        glColor3f(1.0f, 1.0f, 1.0f);

        float spacing = 1.2f;
        float startZ = -2.6f, endZ = startZ - 6.0f;
        float scrollZ = fmod(scrollDistance, spacing);
        float xPositions[] = { -4.5f, 4.5f };

        for (float z = startZ + scrollZ; z > endZ; z -= spacing) {
            for (int i = 0; i < 2; ++i) {
                float x = xPositions[i];
                GLuint texID = (i == 0) ? build1TexID : build2TexID;

                glPushMatrix();
                glTranslatef(x, -1.0f, z);
                glScalef(0.8f, 2.8f, 0.8f);
                glBindTexture(GL_TEXTURE_2D, texID);

                glBegin(GL_QUADS);
                    // Front
                    glTexCoord2f(0, 0); glVertex3f(-0.5f, -0.5f,  0.5f);
                    glTexCoord2f(1, 0); glVertex3f( 0.5f, -0.5f,  0.5f);
                    glTexCoord2f(1, 1); glVertex3f( 0.5f,  0.5f,  0.5f);
                    glTexCoord2f(0, 1); glVertex3f(-0.5f,  0.5f,  0.5f);
                    // Back
                    glTexCoord2f(0, 0); glVertex3f( 0.5f, -0.5f, -0.5f);
                    glTexCoord2f(1, 0); glVertex3f(-0.5f, -0.5f, -0.5f);
                    glTexCoord2f(1, 1); glVertex3f(-0.5f,  0.5f, -0.5f);
                    glTexCoord2f(0, 1); glVertex3f( 0.5f,  0.5f, -0.5f);
                    // Left
                    glTexCoord2f(0, 0); glVertex3f(-0.5f, -0.5f, -0.5f);
                    glTexCoord2f(1, 0); glVertex3f(-0.5f, -0.5f,  0.5f);
                    glTexCoord2f(1, 1); glVertex3f(-0.5f,  0.5f,  0.5f);
                    glTexCoord2f(0, 1); glVertex3f(-0.5f,  0.5f, -0.5f);
                    // Right
                    glTexCoord2f(0, 0); glVertex3f( 0.5f, -0.5f,  0.5f);
                    glTexCoord2f(1, 0); glVertex3f( 0.5f, -0.5f, -0.5f);
                    glTexCoord2f(1, 1); glVertex3f( 0.5f,  0.5f, -0.5f);
                    glTexCoord2f(0, 1); glVertex3f( 0.5f,  0.5f,  0.5f);
                    // Top
                    glTexCoord2f(0, 0); glVertex3f(-0.5f,  0.5f,  0.5f);
                    glTexCoord2f(1, 0); glVertex3f( 0.5f,  0.5f,  0.5f);
                    glTexCoord2f(1, 1); glVertex3f( 0.5f,  0.5f, -0.5f);
                    glTexCoord2f(0, 1); glVertex3f(-0.5f,  0.5f, -0.5f);
                    // Bottom
                    glTexCoord2f(0, 0); glVertex3f(-0.5f, -0.5f, -0.5f);
                    glTexCoord2f(1, 0); glVertex3f( 0.5f, -0.5f, -0.5f);
                    glTexCoord2f(1, 1); glVertex3f( 0.5f, -0.5f,  0.5f);
                    glTexCoord2f(0, 1); glVertex3f(-0.5f, -0.5f,  0.5f);
                glEnd();

                glPopMatrix();
            }
        }

        glDisable(GL_TEXTURE_2D);
        glEnable(GL_LIGHTING);
        glPopMatrix();
    }

    // --- OBSTACLES ---
    glPushMatrix();
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, grassTexID); // Use your obstacle texture
    glColor3f(1.0f, 0.0f, 0.0f); // red for visibility

    for (auto& obs : obstacles) {
        if (!obs.active) continue;

        float z = obs.pos.y - scrollDistance;

        // Check for collision with player
        float dx = obs.pos.x - ply2D->pos.x;
        float dz = z;
        float distSq = dx * dx + dz * dz;
        float collisionDist = obs.radius + 0.1f;

        if (distSq < collisionDist * collisionDist) {
            if (ply2D->actiontrigger == ply2D->KICKLEFT || ply2D->actiontrigger == ply2D->KICKRIGHT) {
                obs.pos.y += 1.5f;  // push away (kick)
                obs.active = false; // vanish
            } else {
                playerHealth -= 10;
                obs.active = false;
                snds->PlaySoundA("sounds/hit.mp3");
            }
        }

        glPushMatrix();
        glTranslatef(obs.pos.x, -1.33f, z - 1.7f); // align to road like player
        glScalef(1.0f, 1.0f, 1.0f);
        GLUquadric* quad = gluNewQuadric();
        gluSphere(quad, obs.radius, 16, 16);
        gluDeleteQuadric(quad);
        glColor3f(1.0,1.0,1.0);
        glPopMatrix();
    }

    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glPopMatrix();
    if (scrollDistance - lastSpawnZ >= obstacleSpawnInterval) {
        spawnObstacle();
        lastSpawnZ = scrollDistance;
    }


    drawHealthBar();

    // --- FINAL: Player movement and scrolling ---
    handlePlayerMovement();
}


void _scene::handlePlayerMovement()
{
    // Reset to standing position if no movement keys are pressed
    if (!isUpPressed && !isLeftPressed && !isRightPressed) {
        ply2D->actiontrigger = ply2D->STAND;
        return;
    }

    // Ensure player and sound system are initialized
    if (!ply2D || !snds) {
        // std::cerr << "Critical error: Player or sound system not initialized" << std::endl;
        return;
    }

    // Movement multipliers and modifiers
    float moveSpeed = 0.02f;
    float scrollSpeed = 0.02f;
    float parallaxSpeed = 0.0025f;

    // Up + Right movement
    if (isUpPressed && isRightPressed && !isLeftPressed) {
        // Increase scroll and movement
        scrollDistance += moveSpeed;

        // Play movement sound
        snds->PlaySoundA("sounds/Bike-1.mp3");

        // Update texture offsets
        roadTexOffset += 0.04f;
        if (roadTexOffset > 1.0f) roadTexOffset = 0.0f;

        grassTexOffset += 0.025f;
        if (grassTexOffset > 1.0f) grassTexOffset = 0.0f;

        // Parallax scrolling
        prlx3->prlxScrollAuto("right", parallaxSpeed);
        prlx4->prlxScrollAuto("right", parallaxSpeed + 0.0015f);
        prlx5->prlxScrollAuto("right", parallaxSpeed);
        prlx6->prlxScrollAuto("right", parallaxSpeed);
        prlx1->prlxScrollAuto("up", 0.001f);

        // Camera/eye movement
        eyeX = std::min(eyeX + 1.0f, 5.0f);

        // Player position and action
        if (ply2D->pos.x < 0.60f) {
            ply2D->pos.x += 0.002f;
        }

        // Action based on space press
        if (isSpacePressed) {
            ply2D->actiontrigger = ply2D->KICKRIGHT;
            snds->PlaySoundA("sounds/punch.mp3");
        } else {
            ply2D->actiontrigger = ply2D->WALKRIGHT;
        }
    }
    // Up + Left movement
    else if (isUpPressed && isLeftPressed && !isRightPressed) {
        // Increase scroll and movement
        scrollDistance += moveSpeed;

        // Play movement sound
        snds->PlaySoundA("sounds/Bike-1.mp3");

        // Update texture offsets
        roadTexOffset += 0.04f;
        if (roadTexOffset > 1.0f) roadTexOffset = 0.0f;

        grassTexOffset += 0.025f;
        if (grassTexOffset > 1.0f) grassTexOffset = 0.0f;

        // Parallax scrolling
        prlx3->prlxScrollAuto("left", parallaxSpeed);
        prlx4->prlxScrollAuto("left", parallaxSpeed + 0.0015f);
        prlx5->prlxScrollAuto("left", parallaxSpeed);
        prlx6->prlxScrollAuto("left", parallaxSpeed);
        prlx1->prlxScrollAuto("up", 0.001f);

        // Camera/eye movement
        eyeX = std::max(eyeX - 1.0f, -5.0f);

        // Player position and action
        if (ply2D->pos.x > -0.6f) {
            ply2D->pos.x -= 0.002f;
        }

        // Action based on space press
        if (isSpacePressed) {
            ply2D->actiontrigger = ply2D->KICKLEFT;
            snds->PlaySoundA("sounds/punch.mp3");
        } else {
            ply2D->actiontrigger = ply2D->WALKLEFT;
        }
    }
    // Up-only movement
    else if (isUpPressed && !isRightPressed && !isLeftPressed) {
        // Increase scroll
        scrollDistance += moveSpeed;

        // Play movement sound
        snds->PlaySoundA("sounds/Bike-1.mp3");

        // Update texture offsets
        roadTexOffset += 0.04f;
        if (roadTexOffset > 1.0f) roadTexOffset = 0.0f;

        grassTexOffset += 0.025f;
        if (grassTexOffset > 1.0f) grassTexOffset = 0.0f;

        // Vertical scrolling
        prlx1->prlxScrollAuto("up", 0.001f);

        // Action based on space press
        if (isSpacePressed) {
            ply2D->actiontrigger = ply2D->KICKLEFT;
            snds->PlaySoundA("sounds/punch.mp3");
        } else {
            ply2D->actiontrigger = ply2D->WALKFRONT;
        }
    }

    // Level Progression Mechanism
    try {
        // Check if scroll distance has reached the threshold
        if (scrollDistance >= levelThreshold) {
            // Prevent multiple simultaneous transitions
            if (currentState != STATE_LEVEL_TRANSITION_1 || currentState != STATE_LEVEL_TRANSITION_2 || currentState != STATE_LEVEL_TRANSITION_3) {
                // Reset scroll distance
                scrollDistance = 0.0f;



                // Trigger level transition only if within valid levels
                if (currentLevel == 1) {
                    currentLevel += 1;
                    currentState = STATE_LEVEL_TRANSITION_1;
                }
                else if (currentLevel == 2)
                {
                    currentLevel += 1;
                    currentState = STATE_LEVEL_TRANSITION_2;
                }
                else if (currentLevel == 3)
                {
                    // Game completed logic
                    currentLevel = 1;
                    currentState = STATE_LEVEL_TRANSITION_3;
                }
                resetPlayerState();
            }
        }
    }
    catch (...) {
        // Fallback error handling
        // std::cerr << "Unexpected error during level progression" << std::endl;
        currentState = STATE_GAME;
    }
}

int _scene::winMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    // Route input events based on current state
    switch (currentState) {
        case STATE_LANDING:
            switch (uMsg) {
                case WM_KEYDOWN:
                    landingPage->handleKeyEvent(static_cast<unsigned char>(wParam));

                    // Allow immediate transition to menu with any key
                    if (wParam == VK_RETURN || wParam == VK_SPACE) {
                        currentState = STATE_MENU;
                        menuPage->menuInit();
                        return 0;
                    }
                    break;
                case WM_LBUTTONDOWN:
                    landingPage->handleMouseEvent(LOWORD(lParam), HIWORD(lParam));

                    // Immediate transition to menu on mouse click
                    currentState = STATE_MENU;
                    menuPage->menuInit();
                    return 0;
            }
            break;

        case STATE_MENU:
            switch (uMsg) {
                case WM_KEYDOWN:
                    myKbMs->wParam = wParam;
                    menuPage->handleKeyEvent(static_cast<unsigned char>(wParam));

                    // Check for transition after key press
                    if (menuPage->isTransitionRequested()) {
                        switch (menuPage->getSelectedOption()) {
                            case _menuPage::NEW_GAME:
                                currentState = STATE_GAME;
                                // Additional game initialization if needed
                                break;
                            case _menuPage::HELP:
                                helpPage->returnTarget = _helpPage::MENU_PAGE;
                                currentState = STATE_HELP;
                                helpPage->helpInit();
                                break;
                            case _menuPage::EXIT:
                                // Trigger application exit
                                PostQuitMessage(0);
                                break;
                        }
                        menuPage->resetTransitionState();
                    }
                    break;
                case WM_MOUSEMOVE:
                case WM_LBUTTONDOWN:
                    menuPage->handleMouseEvent(LOWORD(lParam), HIWORD(lParam));

                    // Check for transition after mouse interaction
                    if (menuPage->isTransitionRequested()) {
                        switch (menuPage->getSelectedOption()) {
                            case _menuPage::NEW_GAME:
                                currentState = STATE_GAME;
                                break;
                            case _menuPage::HELP:
                                currentState = STATE_HELP;
                                helpPage->helpInit();
                                break;
                            case _menuPage::EXIT:
                                PostQuitMessage(0);
                                break;
                        }
                        menuPage->resetTransitionState();
                    }
                    break;
            }
            break;

        case STATE_LEVEL_TRANSITION_1:
            switch(uMsg)
            {
                case WM_KEYDOWN:
                    myKbMs->wParam = wParam;

                    if(wParam == VK_RETURN)
                    {
                        currentState = STATE_GAME;
                        break;
                    }
                    break;
            }
            break;
        case STATE_LEVEL_TRANSITION_2:
            switch(uMsg)
            {
                case WM_KEYDOWN:
                    myKbMs->wParam = wParam;

                    if(wParam == VK_RETURN)
                    {
                        currentState = STATE_GAME;
                        break;
                    }
                    break;
            }
            break;
        case STATE_LEVEL_TRANSITION_3:
            switch(uMsg)
            {
                case WM_KEYDOWN:
                    myKbMs->wParam = wParam;
                    if(wParam == VK_RETURN)
                    {
                        currentState = STATE_MENU;
                        break;
                    }
                    break;
            }
            break;
        case STATE_HELP:
            switch (uMsg) {
                case WM_KEYDOWN:
                    helpPage->handleKeyEvent(static_cast<unsigned char>(wParam));

                    // Check if the Help page is requesting a return transition
                    if (helpPage->isTransitionRequested()) {
                        if (helpPage->returnTarget == _helpPage::MENU_PAGE) {
                            currentState = STATE_MENU;
                            menuPage->menuInit();
                        } else if (helpPage->returnTarget == _helpPage::PAUSE_PAGE) {
                            currentState = STATE_PAUSE;
                            pausePage->pauseInit();
                        }

                        helpPage->resetTransitionState();
                    }
                    break;
            }
            break;
        case STATE_PAUSE:
            switch (uMsg) {
                case WM_KEYDOWN:
                    myKbMs->wParam = wParam;
                    pausePage->handleKeyEvent(static_cast<unsigned char>(wParam));

                    // Handle pause menu transitions
                    if (pausePage->isTransitionRequested()) {
                        int selectedOption = pausePage->selectedOption;
                        switch(selectedOption) {
                            case _pausePage::RESUME: // Resume
                                currentState = STATE_GAME;
                                break;
                            case _pausePage::OPTIONS: // Options (you can implement this later)
                                break;
                            case _pausePage::MENU: // Exit to menu
                                currentState = STATE_MENU;

                                // Cleanup and reinitialize components
                                //cleanupGameComponents();
                                initializeGameComponents();

                                // Reinitialize menu page
                                menuPage->menuInit();
                                break;
                            case _pausePage::HELP: // If you have a HELP option in pause menu
                                helpPage->returnTarget = _helpPage::PAUSE_PAGE;
                                currentState = STATE_HELP;
                                helpPage->helpInit();
                                break;
                        }
                        pausePage->resetTransitionState();
                    }
                    break;
                case WM_MOUSEMOVE:
                case WM_LBUTTONDOWN:
                    pausePage->handleMouseEvent(LOWORD(lParam), HIWORD(lParam));
                    break;
            }
            break;

        case STATE_GAME:
            switch (uMsg) {
                case WM_KEYDOWN:
                    myKbMs->wParam = wParam;

                    // Pause game handling
                    if (wParam == VK_F2) {
                        currentState = STATE_PAUSE;
                        pausePage->pauseInit();
                        return 0;
                    }

                    // Existing game key handling
                    myKbMs->keyPressed(sky);
                    switch (wParam) {
                        case VK_UP:
                            isUpPressed = true;
                            break;
                        case VK_LEFT:
                            isLeftPressed = true;
                            break;
                        case VK_RIGHT:
                            isRightPressed = true;
                            break;
                        case VK_SPACE:
                            isSpacePressed = true;
                            break;
                    }
                    break;

                case WM_KEYUP:
                    myKbMs->wParam = wParam;
                    switch (wParam) {
                        case VK_UP:
                            isUpPressed = false;
                            ply2D->actiontrigger = ply2D->STAND;
                            break;
                        case VK_LEFT:
                            isLeftPressed = false;
                            ply2D->actiontrigger = ply2D->STAND;
                            break;
                        case VK_RIGHT:
                            isRightPressed = false;
                            ply2D->actiontrigger = ply2D->STAND;
                            break;
                        case VK_SPACE:
                            isSpacePressed = false;
                            ply2D->actiontrigger = ply2D->STAND;
                            break;
                    }
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
            break;
    }

    return 0;
}
