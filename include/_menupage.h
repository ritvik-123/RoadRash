#ifndef _MENUPAGE_H
#define _MENUPAGE_H

#include "_common.h"
#include "_textureloader.h"

class _menuPage
{
    public:
        _menuPage();
        virtual ~_menuPage();

        // Enums for menu options and selection
        enum MenuOption {
            NEW_GAME,
            HELP,
            EXIT,
            OPTION_COUNT
        };

        void menuInit();                     // Initialize menu page resources
        void drawMenuPage();                 // Render the menu page

        // Input handling
        void handleKeyEvent(unsigned char key); // Handle keyboard input
        void handleMouseEvent(int x, int y);    // Handle mouse input

        // State management
        bool isTransitionRequested();           // Check if transition to next scene is requested
        MenuOption getSelectedOption();         // Get which option was selected
        void resetTransitionState();            // Reset transition state after scene change

    private:
        _textureLoader* backgroundTexture;      // Background image texture
        bool transitionRequested;               // Flag to indicate transition request
        MenuOption selectedOption;              // Currently selected option
        int highlightedOption;                  // Currently highlighted option

        // Menu item positions and sizes
        struct MenuItem {
            float x, y;                         // Position (top-left corner)
            float width, height;                // Size
            const char* text;                   // Text to display
        };

        MenuItem menuItems[OPTION_COUNT];

        // Helper methods
        void initMenuItems();                   // Set up menu item positions and text
        bool isPointInMenuItem(int x, int y, const MenuItem& item); // Hit test
        void drawMenuItem(const MenuItem& item, bool highlighted);  // Draw a menu item
};

#endif // _MENUPAGE_H
