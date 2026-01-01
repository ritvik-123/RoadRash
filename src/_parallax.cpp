#include "_parallax.h"

_parallax::_parallax()
{
    //ctor
    xMin =0.0;
    xMax =1.0;
    yMin =0.0;
    yMax =1.0;
    zMin =0.0;
    zMax =1.0;

}

_parallax::~_parallax()
{
    //dtor
}

void _parallax::drawBkgrnd(float width, float height)
{

    //glColor3f(1.0,1.0,1.0);
    bTex->textureBinder();

    glBegin(GL_POLYGON);

       glTexCoord2f(xMin,yMax);
       glVertex3f(-width/height, -1,-10);

       glTexCoord2f(xMax,yMax);
       glVertex3f(width/height, -1,-10);

              glTexCoord2f(xMax,yMin);
       glVertex3f(width/height, 1,-10);

              glTexCoord2f(xMin,yMin);
       glVertex3f(-width/height, 1,-10);

    glEnd();

}
void _parallax::drawInclinedSurface(float width, float height, float angle) {
    glPushMatrix();  // Save the current matrix state

    // Perform the rotation and translation
    glTranslatef(0.0f, 0.0f, -2.7f);  // Ensure the object is correctly placed in the scene
    glRotatef(angle, 1.0f, 0.0f, 0.0f);  // Rotate the road around the X-axis

    // Bind the texture (make sure the texture is valid and properly bound)
    bTex->textureBinder();

    // Draw the surface with the applied transformations
    glBegin(GL_POLYGON);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-width / 2, -height / 2, 0.0f);

        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(width / 2, -height / 2, 0.0f);

        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(width / 2, height / 2, 0.0f);

        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-width / 2, height / 2, 0.0f);
    glEnd();

    glPopMatrix();  // Restore the previous matrix state
}

void _parallax::drawRoad(float width, float height, float angle) {
    glPushMatrix();  // Save the current matrix state

    // Position and scale the road (uncomment if necessary)
    glTranslatef(0.0f, -2.7f, 0.0f);  // Position the road on the XZ plane
    glScalef(1.5f, 1.5f, 1.0f);  // Scale the road if necessary

    // Call the drawInclinedSurface method to render the inclined road
    drawInclinedSurface(width, height, angle);

    glPopMatrix();  // Restore the previous matrix state
}





void _parallax::parallaxInit(char* fileName)
{
    bTex->loadImage(fileName);
}

void _parallax::prlxScrollAuto(string dir, float speed)
{
    if (tmr->getTicks()>2)
    {

         if(dir=="left")
         {
             xMin +=speed;
             xMax +=speed;
         }
          if(dir=="right")
         {
             xMin -=speed;
             xMax -=speed;
         }

          if(dir=="up")
         {
             yMin +=speed;
             yMax +=speed;
         }

          if(dir=="down")
         {
             yMin -=speed;
             yMax -=speed;
         }
         if(dir=="for")
         {
             zMin +=speed;
             zMax +=speed;
         }

          if(dir=="back")
         {
             zMin -=speed;
             zMax -=speed;
         }
         tmr->reset();
    } // End Timer
}


