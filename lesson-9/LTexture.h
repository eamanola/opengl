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

    bool lock();
    bool unlock();

    GLuint* getPixelData32();
    GLuint getPixel32( GLuint x, GLuint y );
    void setPixel32( GLuint x, GLuint y, GLuint pixel ) ;

  private:
    GLuint powerOfTwo( GLuint num );

    GLuint mTextureID;

    GLuint* mPixels;

    GLuint mTextureWidth;
    GLuint mTextureHeight;

    GLuint mImageWidth;
    GLuint mImageHeight;
};

#endif
