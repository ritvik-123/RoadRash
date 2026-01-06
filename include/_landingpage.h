#ifndef _LANDINGPAGE_H
#define _LANDINGPAGE_H

#include "_common.h"
#include "_textureloader.h"

class _landingPage
{
    public:
        _landingPage();
        virtual ~_landingPage();

        void landingInit();                     // Initialize landing page resources
        void drawLandingPage();                 // Render the landing page
        
        // Input handling
        void handleKeyEvent(unsigned char key); // Handle keyboard input
        void handleMouseEvent(int x, int y);    // Handle mouse input
        
        // State management
        bool isTransitionRequested();           // Check if transition to next scene is requested
        void resetTransitionState();            // Reset transition state after scene change

    private:
        _textureLoader* backgroundTexture;      // Background image texture
        bool transitionRequested;               // Flag to indicate transition request
};

#endif // _LANDINGPAGE_H
