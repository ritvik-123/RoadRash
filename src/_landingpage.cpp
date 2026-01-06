// _landingpage.cpp
#include "_landingpage.h"
#include <iostream>
#include <windows.h>

_landingPage::_landingPage()
{
    // Initialize variables
    transitionRequested = false;
    backgroundTexture = new _textureLoader();
}

_landingPage::~_landingPage()
{
    // Clean up resources
    if (backgroundTexture != nullptr) {
        delete backgroundTexture;
    }
}

void _landingPage::landingInit()
{
    // Try to load the landing page background texture
    // std::cout << "Attempting to load landing page image..." << std::endl;

    // Get current directory for debugging
    char currentDir[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, currentDir);
    // std::cout << "Current directory: " << currentDir << std::endl;

    // Make sure we have a valid texture loader
    if (!backgroundTexture) {
        backgroundTexture = new _textureLoader();
        // std::cout << "Created new texture loader" << std::endl;
    }

    // Explicitly delete any existing texture
    if (backgroundTexture->texID != 0) {
        glDeleteTextures(1, &backgroundTexture->texID);
        backgroundTexture->texID = 0;
        // std::cout << "Deleted previous texture" << std::endl;
    }

    // Load the image
    backgroundTexture->loadImage("images/Landingpage.png");

    // Verify the texture was loaded
    if (backgroundTexture->texID != 0) {
        // std::cout << "Successfully loaded image from: images/Landingpage.png" << std::endl;
        // std::cout << "Landing page texture loaded successfully with ID: " << backgroundTexture->texID << std::endl;

        // Test bind the texture to verify it's valid
        glBindTexture(GL_TEXTURE_2D, backgroundTexture->texID);
        GLenum err = glGetError();
        if (err != GL_NO_ERROR) {
            // std::cout << "ERROR: Failed to bind texture during initialization: " << err << std::endl;
        } else {
            // std::cout << "Texture binding test successful" << std::endl;
        }
    } else {
        // std::cout << "Failed to load landing page image!" << std::endl;
    }
}

void _landingPage::drawLandingPage()
{
    // Reset any errors
    while(glGetError() != GL_NO_ERROR);

    // Save the current matrices
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    // Use a simple orthographic projection
    glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Disable everything that might interfere with 2D rendering
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_CULL_FACE);

    // Clear the color to black in case texture fails
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Set the color to white for texture rendering
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    // Check if we have a valid texture
    if (backgroundTexture && backgroundTexture->texID != 0) {
        // std::cout << "Rendering landing page with texture ID: " << backgroundTexture->texID << std::endl;

        // Enable texturing
        glEnable(GL_TEXTURE_2D);

        // Bind the texture directly
        glBindTexture(GL_TEXTURE_2D, backgroundTexture->texID);

        // Set basic texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Draw a simple quad covering the entire viewport
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 1.0f); glVertex2f(0.0f, 0.0f); // Bottom-left
            glTexCoord2f(1.0f, 1.0f); glVertex2f(1.0f, 0.0f); // Bottom-right
            glTexCoord2f(1.0f, 0.0f); glVertex2f(1.0f, 1.0f); // Top-right
            glTexCoord2f(0.0f, 0.0f); glVertex2f(0.0f, 1.0f); // Top-left
        glEnd();

        // Check for errors
        GLenum err = glGetError();
        if (err != GL_NO_ERROR) {
            // std::cout << "OpenGL error during landing page rendering: " << err << std::endl;
        } else {
            // std::cout << "Landing page quad rendered successfully" << std::endl;
        }

        // Disable texturing when done
        glDisable(GL_TEXTURE_2D);
    } else {
        // Fallback - draw a colored quad
        glDisable(GL_TEXTURE_2D);
        glColor3f(1.0f, 0.5f, 0.0f); // Orange

        glBegin(GL_QUADS);
            glVertex2f(0.0f, 0.0f);
            glVertex2f(1.0f, 0.0f);
            glVertex2f(1.0f, 1.0f);
            glVertex2f(0.0f, 1.0f);
        glEnd();

        // std::cout << "WARNING: Using fallback color (texture not available)" << std::endl;
    }


    // Restore the matrices
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    // Re-enable depth testing
    glEnable(GL_DEPTH_TEST);
}

void _landingPage::handleKeyEvent(unsigned char key)
{
    // Debug output
    // std::cout << "Key pressed in Landing page: " << (int)key << std::endl;

    // Check if it's Enter key (VK_RETURN = 13 or '\r' or '\n')
    if (key == VK_RETURN || key == '\r' || key == '\n') {
        // std::cout << "Enter key detected in landingPage handler" << std::endl;
        transitionRequested = true;
        // std::cout << "Transition requested from landing page to menu" << std::endl;
    }
}

void _landingPage::handleMouseEvent(int x, int y)
{
    // Any mouse click will also trigger a transition to the menu
    transitionRequested = true;
    // std::cout << "Mouse clicked on landing page - transitioning to menu" << std::endl;
}

bool _landingPage::isTransitionRequested()
{
    return transitionRequested;
}

void _landingPage::resetTransitionState()
{
    transitionRequested = false;
}
