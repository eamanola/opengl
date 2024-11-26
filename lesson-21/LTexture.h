#ifndef LTEXTURE_H
#define LTEXTURE_H

#include "../LOpenGL.h"
#include "LFRect.h"
#include <string>

class LTexture
{
  public:
    LTexture();
    virtual ~LTexture();

    virtual void freeTexture();

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

    GLubyte* getPixelData8();
    GLubyte getPixel8( GLuint x, GLuint y );
    void setPixel8( GLuint x, GLuint y, GLubyte pixel );

    bool loadTextureFromFile32( std::string );

    bool loadTextureFromFileWithColorKey32(
      std::string path, GLubyte r, GLubyte g, GLubyte b, GLubyte a = 000
    );

  protected:
    bool loadPixelsFromFile8( std::string );
    bool loadPixelsFromFile32( std::string );

    bool loadTextureFromPixels8();
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

    GLuint* mPixels32;
    GLubyte* mPixels8;

    GLuint mPixelFormat;

    GLuint mTextureWidth;
    GLuint mTextureHeight;

    GLuint mImageWidth;
    GLuint mImageHeight;

    GLuint mVBOID;
    GLuint mIBOID;
};

#endif
