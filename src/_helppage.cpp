#include "_helppage.h"
#include <iostream>
#include <windows.h>

_helpPage::_helpPage()
{
    // Initialize variables
    transitionRequested = false;
    backgroundTexture = new _textureLoader();
}

_helpPage::~_helpPage()
{
    // Clean up resources
    if (backgroundTexture != nullptr) {
        delete backgroundTexture;
    }
}

void _helpPage::helpInit()
{
    // Load the help page background texture
    backgroundTexture->loadImage("images/Help page.png");

    // Debug output
    // std::cout << "Help page initialized" << std::endl;
}

void _helpPage::drawHelpPage()
{
    // Set up orthographic projection for 2D rendering
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, 1, 0, 1, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Disable lighting for 2D elements
    glDisable(GL_LIGHTING);

    // Draw background image (full screen quad)
    glEnable(GL_TEXTURE_2D);
    backgroundTexture->textureBinder();

    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 1.0); glVertex3f(0.0, 0.0, 0.0);
    glTexCoord2f(1.0, 1.0); glVertex3f(1.0, 0.0, 0.0);
    glTexCoord2f(1.0, 0.0); glVertex3f(1.0, 1.0, 0.0);
    glTexCoord2f(0.0, 0.0); glVertex3f(0.0, 1.0, 0.0);
glEnd();

    glDisable(GL_TEXTURE_2D);

    // Draw title and instructions
    // Note: For proper text rendering, you would need to implement a text drawing system
    drawInstructions();

    // Draw return instruction
    glColor3f(1.0f, 1.0f, 0.2f); // Yellow text
    // Text would be rendered here with "Press ESC to return to Menu"

    // Restore previous matrices
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    // Re-enable lighting
    glEnable(GL_LIGHTING);
}

void _helpPage::drawInstructions()
{

}

void _helpPage::handleKeyEvent(unsigned char key)
{
    // std::cout << "Key pressed in Help page: " << (int)key << std::endl;

    if (key == VK_F2) { // Escape key
        if (returnTarget == MENU_PAGE) {
                selectedOption = MENU;
            } else if (returnTarget == PAUSE_PAGE) {
                selectedOption = PAUSE;
            }
        transitionRequested = true;
        // std::cout << "Escape key pressed in Help - returning to menu" << std::endl;
    }
}

bool _helpPage::isTransitionRequested()
{
    return transitionRequested;
}

void _helpPage::resetTransitionState()
{
    transitionRequested = false;
}
