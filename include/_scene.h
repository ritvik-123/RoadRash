#ifndef _SCENE_H
#define _SCENE_H

#include<_common.h>
#include<_landingpage.h>
#include<_menupage.h>
#include<_helppage.h>
#include<_pausePage.h>
#include<_textureloader.h>

// Update GameState enum to include LEVEL_TRANSITION
enum GameState {
    STATE_LANDING,
    STATE_MENU,
    STATE_GAME,
    STATE_HELP,
    STATE_PAUSE,
    STATE_LEVEL_TRANSITION_1,
    STATE_LEVEL_TRANSITION_2,
    STATE_LEVEL_TRANSITION_3  // New state for level transition
};

class _scene
{
    public:
        _scene();
        virtual ~_scene();

        GLvoid resizeWindow(GLsizei width, GLsizei height);  // resizing window
        GLint IniGL();                                      // initialization
        GLvoid renderScene();                               // Draw Scene

        int winMsg(HWND	hWnd,			// Handle For This Window
                   UINT	uMsg,			// Message For This Window
                   WPARAM wParam,		// Additional Message Information
                   LPARAM	lParam);

        vec2 dim;

    protected:
        // Game state management
        GameState currentState;

        // Page instances
        _landingPage* landingPage;
        _menuPage* menuPage;
        _helpPage* helpPage;
        _pausePage* pausePage;

        // Texture IDs
        GLuint roadTexID;
        GLuint grassTexID;
        GLuint bushTexID;
        GLuint build1TexID;
        GLuint build2TexID;
        GLuint build3TexID;

        // Level transition texture
        _textureLoader* levelTransitionTexture;

        // Game rendering and movement variables
        float roadTexOffset;
        float grassTexOffset;
        float eyeX;
        bool isUpPressed;
        bool isLeftPressed;
        bool isRightPressed;
        bool isSpacePressed;

        int currentLevel = 1;
        float scrollDistance = 0.0f;
        const float levelThreshold = 100.0f; // adjust as needed

        // Level transition variables
        DWORD transitionStartTime;
        bool isLevelTransitionPending;

        // Rendering methods for different states
        void renderLanding();
        void renderMenu();
        void renderHelp();
        void renderGame();
        void renderPause();
        void renderLevelTransition();  // New method for level transition

        // Game component management
        void initializeGameComponents();
        void cleanupGameComponents();
        void resetPlayerState();
        void spawnObstacle();
        void drawHealthBar();

        // Player movement
        void handlePlayerMovement();

        // Level transition methods - ADD THESE
        void triggerLevelTransition();  // Replace startLevelTransition
        void checkLevelTransition();    // Add this method

    private:
        // Helper methods can be added here if needed
};

#endif // _SCENE_H
