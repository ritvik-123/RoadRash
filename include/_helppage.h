#ifndef _HELPPAGE_H
#define _HELPPAGE_H

#include "_common.h"
#include "_textureloader.h"

class _helpPage
{
    public:
        _helpPage();
        virtual ~_helpPage();

        enum MenuOption {
            MENU,
            PAUSE
        };

        void helpInit();                      // Initialize help page resources
        void drawHelpPage();                  // Render the help page

        enum PreviousPage { MENU_PAGE, PAUSE_PAGE };
        PreviousPage returnTarget;

        // Input handling
        void handleKeyEvent(unsigned char key); // Handle keyboard input

        // State management
        bool isTransitionRequested();           // Check if transition to menu page is requested
        void resetTransitionState();            // Reset transition state after scene change

    private:
        _textureLoader* backgroundTexture;      // Background image texture
        bool transitionRequested;               // Flag to indicate transition request

        // Helper methods for drawing text instructions
        void drawInstructions();
        MenuOption selectedOption;
};

#endif // _HELPPAGE_H
