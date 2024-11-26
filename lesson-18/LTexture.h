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
    void setPixel32( GLuint x, GLuint y, GLuint pixel );

    bool loadTextureFromFileWithColorKey(
      std::string path, GLubyte r, GLubyte g, GLubyte b, GLubyte a = 000
    );

    bool loadTextureFromFile( std::string );

  protected:
    bool loadPixelsFromFile( std::string );
    bool loadTextureFromPixels32();

    bool loadTextureFromPixels32(
      GLuint* pixels,
      GLuint imgWidth, GLuint imgHeight,
      GLuint texWidth, GLuint texHeight
    );

  private:
    GLuint powerOfTwo( GLuint num );

    void initVBO();
    void freeVBO();

    GLuint mTextureID;

    GLuint* mPixels;

    GLuint mTextureWidth;
    GLuint mTextureHeight;

    GLuint mImageWidth;
    GLuint mImageHeight;

    GLuint mVBOID;
    GLuint mIBOID;
};

#endif
