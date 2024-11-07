#ifndef LTEXTURE_H
#define LTEXTURE_H

#include "../LOpenGL.h"
#include "LFRect.h"
#include <string>

class LTexture
{
  public:
    LTexture();
    ~LTexture();

    bool loadTextureFromPixels32(
      GLuint* pixels,
      GLuint imgWidth, GLuint imgHeight,
      GLuint texWidth, GLuint texHeight
    );
    bool loadTexttureFromFile( std::string );
    void freeTexture();

    void render( GLfloat x, GLfloat y, LFRect* clip = NULL );

    GLuint getTextureID();

    GLuint textureWidth();
    GLuint textureHeight();

    GLuint imageWidth();
    GLuint imageHeight();

  private:
    GLuint powerOfTwo( GLuint num );

    GLuint mTextureID;

    GLuint mTextureWidth;
    GLuint mTextureHeight;

    GLuint mImageWidth;
    GLuint mImageHeight;
};

#endif
