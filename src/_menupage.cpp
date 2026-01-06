#include "_menupage.h"
#include <iostream>
#include <windows.h>

_menuPage::_menuPage()
{
    // Initialize variables
    transitionRequested = false;
    backgroundTexture = new _textureLoader();
    selectedOption = NEW_GAME;
    highlightedOption = 0;

    // Initialize menu items
    initMenuItems();
}

_menuPage::~_menuPage()
{
    // Clean up resources
    if (backgroundTexture != nullptr) {
        delete backgroundTexture;
    }
}

void _menuPage::menuInit()
{
    // Reset transition state
    transitionRequested = false;

    // Reset selected and highlighted options
    selectedOption = NEW_GAME;
    highlightedOption = 0;

    // Reinitialize menu items
    initMenuItems();

    // Load the menu page background texture
    // Delete any existing texture first
    if (backgroundTexture->texID != 0) {
        glDeleteTextures(1, &backgroundTexture->texID);
        backgroundTexture->texID = 0;
    }

    // Load the menu page background texture
    backgroundTexture->loadImage("images/Menu page.png");

    // Debug output
    if(backgroundTexture->texID == 0) {
        // std::cout << "WARNING: Failed to load menu page image!" << std::endl;

        // Try loading from a different path as fallback
        // std::cout << "Trying alternative path..." << std::endl;
        backgroundTexture->loadImage("../images/Menu page.png");

        if(backgroundTexture->texID == 0) {
            // std::cout << "Still failed to load menu page image!" << std::endl;

            // Print current directory for troubleshooting
            char currentDir[MAX_PATH];
            GetCurrentDirectory(MAX_PATH, currentDir);
            // std::cout << "Current directory: " << currentDir << std::endl;
        }
    } else {
        // std::cout << "Menu page texture loaded successfully with ID: " << backgroundTexture->texID << std::endl;
    }
}

void _menuPage::initMenuItems()
{
    // Setup menu item positions and text
    // These values are in normalized coordinates (0.0 to 1.0)

    // New Game button
    menuItems[NEW_GAME].x = 0.3f;
    menuItems[NEW_GAME].y = 0.6f;
    menuItems[NEW_GAME].width = 0.4f;
    menuItems[NEW_GAME].height = 0.1f;
    menuItems[NEW_GAME].text = "New Game";

    // Help button
    menuItems[HELP].x = 0.3f;
    menuItems[HELP].y = 0.45f;
    menuItems[HELP].width = 0.4f;
    menuItems[HELP].height = 0.1f;
    menuItems[HELP].text = "Help";

    // Exit button
    menuItems[EXIT].x = 0.3f;
    menuItems[EXIT].y = 0.3f;
    menuItems[EXIT].width = 0.4f;
    menuItems[EXIT].height = 0.1f;
    menuItems[EXIT].text = "Exit";
}

void _menuPage::drawMenuPage()
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
    } else

    glDisable(GL_TEXTURE_2D);


    // Restore original matrices
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    // Re-enable lighting
    glEnable(GL_LIGHTING);
}

void _menuPage::drawMenuItem(const MenuItem& item, bool highlighted)
{
    // Draw the menu item
    if (highlighted) {
        glColor4f(1.0f, 0.8f, 0.0f, 0.8f); // Highlighted color (yellow/gold)
    } else {
        glColor4f(0.9f, 0.9f, 0.9f, 0.7f); // Normal color (light gray)
    }

    // Draw a quad for the menu item (optional visual representation)
    glBegin(GL_QUADS);
        glVertex2f(item.x, item.y);
        glVertex2f(item.x + item.width, item.y);
        glVertex2f(item.x + item.width, item.y + item.height);
        glVertex2f(item.x, item.y + item.height);
    glEnd();
}

void _menuPage::handleKeyEvent(unsigned char key)
{
    // std::cout << "Key pressed in Menu page: " << (int)key << " ('" << key << "')" << std::endl;

    switch (key) {
        case VK_UP:
            // Move selection up
            highlightedOption = (highlightedOption > 0) ? highlightedOption - 1 : OPTION_COUNT - 1;
            // std::cout << "Menu selection changed to: " << highlightedOption << std::endl;
            break;

        case VK_DOWN:
            // Move selection down
            highlightedOption = (highlightedOption < OPTION_COUNT - 1) ? highlightedOption + 1 : 0;
            // std::cout << "Menu selection changed to: " << highlightedOption << std::endl;
            break;

        case VK_RETURN: // Enter key
            // Select the highlighted option
            selectedOption = static_cast<MenuOption>(highlightedOption);
            transitionRequested = true;
            // std::cout << "Menu option selected: " << selectedOption << std::endl;
            break;

        // Add direct key support for N, H, E (both uppercase and lowercase)
        case 'N':
        case 'n': // 'N' key for New Game
            selectedOption = NEW_GAME;
            highlightedOption = NEW_GAME;
            transitionRequested = true;
            // std::cout << "New Game selected via keyboard shortcut (N)" << std::endl;
            break;

        case 'H':
        case 'h': // 'H' key for Help
            selectedOption = HELP;
            highlightedOption = HELP;
            transitionRequested = true;
            // std::cout << "Help selected via keyboard shortcut (H)" << std::endl;
            break;

        case 'E':
        case 'e': // 'E' key for Exit
            selectedOption = EXIT;
            highlightedOption = EXIT;
            transitionRequested = true;
            // std::cout << "Exit selected via keyboard shortcut (E)" << std::endl;
            break;
    }
}

void _menuPage::handleMouseEvent(int x, int y)
{
    // Convert window coordinates to normalized coordinates
    float normX = static_cast<float>(x) / GetSystemMetrics(SM_CXSCREEN);
    float normY = static_cast<float>(y) / GetSystemMetrics(SM_CYSCREEN);
    normY = 1.0f - normY; // Invert Y coordinate

    // Check if mouse is over any menu item
    for (int i = 0; i < OPTION_COUNT; ++i) {
        if (isPointInMenuItem(normX, normY, menuItems[i])) {
            // Highlight this option
            highlightedOption = i;

            // If clicked, select this option
            selectedOption = static_cast<MenuOption>(i);
            transitionRequested = true;
            // std::cout << "Menu option clicked: " << i << std::endl;
            break;
        }
    }
}

bool _menuPage::isPointInMenuItem(int x, int y, const MenuItem& item)
{
    // Check if point is within menu item bounds
    float normX = static_cast<float>(x) / GetSystemMetrics(SM_CXSCREEN);
    float normY = static_cast<float>(y) / GetSystemMetrics(SM_CYSCREEN);
    normY = 1.0f - normY; // Invert Y coordinate

    return (normX >= item.x && normX <= item.x + item.width &&
            normY >= item.y && normY <= item.y + item.height);
}

bool _menuPage::isTransitionRequested()
{
    return transitionRequested;
}

_menuPage::MenuOption _menuPage::getSelectedOption()
{
    return selectedOption;
}

void _menuPage::resetTransitionState()
{
    transitionRequested = false;
}
