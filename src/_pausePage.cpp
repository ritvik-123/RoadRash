#include "_pausePage.h"
#include <iostream>
#include <windows.h>

_pausePage::_pausePage()
{
    // Initialize variables
    transitionRequested = false;
    backgroundTexture = new _textureLoader();
    selectedOption = 0;
    highlightedOption = 0;

    // Initialize menu items
    initMenuItems();
}

_pausePage::~_pausePage()
{
    // Clean up resources
    if (backgroundTexture != nullptr) {
        delete backgroundTexture;
    }
}

void _pausePage::pauseInit()
{
    // Load the pause page background texture
    backgroundTexture->loadImage("images/Pause Menu.png");
}

void _pausePage::initMenuItems()
{
    // Setup menu item positions and text
    // These values are in normalized coordinates (0.0 to 1.0)

    // Resume button
    menuItems[0].x = 0.3f;
    menuItems[0].y = 0.6f;
    menuItems[0].width = 0.4f;
    menuItems[0].height = 0.1f;
    menuItems[0].text = "Resume";

    // Options button
    menuItems[1].x = 0.3f;
    menuItems[1].y = 0.45f;
    menuItems[1].width = 0.4f;
    menuItems[1].height = 0.1f;
    menuItems[1].text = "Options";

    // Exit button
    menuItems[2].x = 0.3f;
    menuItems[2].y = 0.3f;
    menuItems[2].width = 0.4f;
    menuItems[2].height = 0.1f;
    menuItems[2].text = "Exit";
}

void _pausePage::drawPausePage()
{
    // Set up orthographic projection for 2D rendering
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, 1, 0, 1, -1, 1); // Normalized coordinates (0,0) to (1,1)

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Disable lighting for 2D elements
    glDisable(GL_LIGHTING);

    // Draw background image
    glEnable(GL_TEXTURE_2D);

    if(backgroundTexture->texID != 0) {
        backgroundTexture->textureBinder();

        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, 0.0f, 0.0f);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 0.0f, 0.0f);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 0.0f);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 1.0f, 0.0f);
        glEnd();
    }

    glDisable(GL_TEXTURE_2D);

    // Draw each menu item
    for (int i = 0; i < 3; ++i) {
        drawMenuItem(menuItems[i], i == highlightedOption);
    }

    // Restore original matrices
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    // Re-enable lighting
    glEnable(GL_LIGHTING);
}

void _pausePage::drawMenuItem(const MenuItem& item, bool highlighted)
{
    // Draw the menu item
    if (highlighted) {
        glColor4f(1.0f, 0.8f, 0.0f, 0.8f); // Highlighted color (yellow/gold)
    } else {
        glColor4f(0.9f, 0.9f, 0.9f, 0.7f); // Normal color (light gray)
    }
}

void _pausePage::handleKeyEvent(unsigned char key)
{
    switch (key) {
        case VK_F2: // Explicitly handle Esc to go back to menu
            // std::cout << "Escape pressed in pause menu" << std::endl;
            selectedOption = MENU; // Set to EXIT option
            transitionRequested = true;
            break;

        case VK_UP:
            // Move selection up
            highlightedOption = (highlightedOption > 0) ? highlightedOption - 1 : OPTION_COUNT - 1;
            break;

        case VK_DOWN:
            // Move selection down
            highlightedOption = (highlightedOption < OPTION_COUNT - 1) ? highlightedOption + 1 : 0;
            break;

        case VK_RETURN: // Enter key
            // Select the highlighted option
            selectedOption = RESUME;
            transitionRequested = true;
            // std::cout << "Option selected in pause menu: " << selectedOption << std::endl;
            break;
        case 'h' :
        case 'H' : // help key
            // Select the highlighted option
            selectedOption = HELP;
            transitionRequested = true;
            // std::cout << "Option selected in pause menu: " << selectedOption << std::endl;
            break;
    }
}

void _pausePage::handleMouseEvent(int x, int y)
{
    // Convert window coordinates to normalized coordinates
    float normX = static_cast<float>(x) / GetSystemMetrics(SM_CXSCREEN);
    float normY = static_cast<float>(y) / GetSystemMetrics(SM_CYSCREEN);
    normY = 1.0f - normY; // Invert Y coordinate

    // Check if mouse is over any menu item
    for (int i = 0; i < 3; ++i) {
        if (isPointInMenuItem(normX, normY, menuItems[i])) {
            // Highlight this option
            highlightedOption = i;

            // If clicked, select this option
            selectedOption = i;
            transitionRequested = true;
            break;
        }
    }
}

bool _pausePage::isPointInMenuItem(int x, int y, const MenuItem& item)
{
    // Check if point is within menu item bounds
    float normX = static_cast<float>(x) / GetSystemMetrics(SM_CXSCREEN);
    float normY = static_cast<float>(y) / GetSystemMetrics(SM_CYSCREEN);
    normY = 1.0f - normY; // Invert Y coordinate

    return (normX >= item.x && normX <= item.x + item.width &&
            normY >= item.y && normY <= item.y + item.height);
}

bool _pausePage::isTransitionRequested()
{
    return transitionRequested;
}

void _pausePage::resetTransitionState()
{
    transitionRequested = false;
}
