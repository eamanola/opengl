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

    const GLuint getTextureID();

    const GLuint textureWidth();
    const GLuint textureHeight();

    const GLuint imageWidth();
    const GLuint imageHeight();

    GLuint* getPixelData32();
    const GLuint getPixel32( GLuint x, GLuint y );
    void setPixel32( GLuint x, GLuint y, GLuint pixel );

    GLubyte* getPixelData8();
    const GLubyte getPixel8( GLuint x, GLuint y );
    void setPixel8( GLuint x, GLuint y, GLubyte pixel );

    bool loadPixelsFromFile8( std::string path );
    bool loadPixelsFromFile32( std::string path );

    void createPixels32( GLuint imgWidth, GLuint imgHeight );
    void copyPixels32( GLuint* pixels, GLuint imgWidth, GLuint imgHeight );
    void padPixels32();
    void blitPixels32( GLuint x, GLuint y, LTexture& destination );

    void createPixels8( GLuint imgWidth, GLuint imgHeight );
    void copyPixels8( GLubyte* pixels, GLuint imgWidth, GLuint imgHeight );
    void padPixels8();
    void blitPixels8( GLuint x, GLuint y, LTexture& destination );

    bool loadTextureFromFile32( std::string path );
    bool loadTextureFromFileWithColorKey32(
      std::string path, GLubyte r, GLubyte g, GLubyte b, GLubyte a = 000
    );

    bool loadTextureFromPixels8();
    bool loadTextureFromPixels32();

    bool loadTextureFromPixels32(
      GLuint* pixels,
      GLuint imgWidth, GLuint imgHeight,
      GLuint texWidth, GLuint texHeight
    );

    bool lock();
    bool unlock();

  private:
    GLuint powerOfTwo( GLuint num );

    void initVBO();
    void freeVBO();

    void initPixels( GLuint pixelFormat, GLuint size );
    void* getPixelData( GLuint pixelFormat );

    bool loadPixelsFromFile( std::string path, GLuint pixelFormat );
    bool loadTextureFromPixels( GLuint pixelFormat );

    void createPixels( GLuint imgWidth, GLuint imgHeight, GLuint pixelFormat );
    void copyPixels( void* pixels, GLuint imgWidth, GLuint imgHeight, GLuint pixelFormat );

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
