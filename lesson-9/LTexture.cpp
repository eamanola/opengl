#include "LTexture.h"
#include <IL/il.h>
#include <IL/ilu.h>
#include <string>

LTexture::LTexture()
{
  mTextureID = 0;
  mPixels = NULL;

  mTextureWidth = 0;
  mTextureHeight = 0;

  mImageWidth = 0;
  mImageHeight = 0;
}

LTexture::~LTexture()
{
  freeTexture();
}

bool LTexture::loadTextureFromPixels32(
  GLuint* pixels,
  GLuint imgWidth, GLuint imgHeight,
  GLuint texWidth, GLuint texHeight
)
{
  freeTexture();

  mImageWidth = imgWidth;
  mImageHeight = imgHeight;

  mTextureWidth = texWidth;
  mTextureHeight = texHeight;

  glGenTextures( 1, &mTextureID );

  glBindTexture( GL_TEXTURE_2D, mTextureID );

  glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, mTextureWidth, mTextureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels );

  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

  glBindTexture( GL_TEXTURE_2D, 0 );

  GLenum error = glGetError();
  if( error != GL_NO_ERROR )
  {
    printf( "Error loading texture from %p pixels! %s\n", pixels, gluErrorString( error ) );
    return false;
  }

  return true;
}

bool LTexture::loadTexttureFromFile( std::string path )
{
  bool textureLoaded = false;

  ILuint imgID = 0;
  ilGenImages( 1, &imgID );
  ilBindImage( imgID );

  ILboolean success = ilLoadImage( path.c_str() );

  if ( success == IL_TRUE )
  {
    success = ilConvertImage( IL_RGBA, IL_UNSIGNED_BYTE );

    if ( success == IL_TRUE )
    {
      GLuint imgWidth = (GLuint)ilGetInteger( IL_IMAGE_WIDTH );
      GLuint imgHeight = (GLuint)ilGetInteger( IL_IMAGE_HEIGHT );

      GLuint texWidth = powerOfTwo( imgWidth );
      GLuint texHeight = powerOfTwo( imgHeight );

      if( imgWidth != texWidth || imgHeight != texHeight )
      {
        iluImageParameter( ILU_PLACEMENT, ILU_UPPER_LEFT );

        iluEnlargeCanvas( (int)texWidth, (int)texHeight, 1 );
      }

      textureLoaded = loadTextureFromPixels32(
        (GLuint*)ilGetData(),
        imgWidth, imgHeight,
        texWidth, texHeight
      );
    }

    ilDeleteImages( 1, &imgID );
  }

  if( !textureLoaded )
  {
    printf( "Unable to load %s\n", path.c_str() );
  }

  return textureLoaded;
}

void LTexture::render( GLfloat x, GLfloat y, LFRect* clip )
{
  if(mTextureID != 0)
  {
    glLoadIdentity();

    GLfloat texTop = 0.f;
    GLfloat texBottom = (GLfloat)mImageHeight / (GLfloat)mTextureHeight;
    GLfloat texLeft = 0.f;
    GLfloat texRight = (GLfloat)mImageWidth / (GLfloat)mTextureWidth;

    GLfloat quadWidth = mImageWidth;
    GLfloat quadHeight = mImageHeight;

    if ( clip != NULL )
    {
      texLeft = clip->x / mTextureWidth;
      texRight = ( clip->x + clip-> w ) / mTextureWidth;
      texTop = clip->y / mTextureHeight;
      texBottom = ( clip->y + clip->h ) / mTextureWidth;

      quadWidth = clip->w;
      quadHeight = clip->h;
    }

    glTranslatef( x, y, 0.0f );

    glBindTexture( GL_TEXTURE_2D, mTextureID );

    glBegin( GL_QUADS );
      glTexCoord2f( texLeft,  texTop );     glVertex2f( 0.0f,      0.0f );
      glTexCoord2f( texRight, texTop );     glVertex2f( quadWidth, 0.0f );
      glTexCoord2f( texRight, texBottom );  glVertex2f( quadWidth, quadHeight );
      glTexCoord2f( texLeft,  texBottom );  glVertex2f( 0.0f,      quadHeight );
    glEnd();
  }
}

void LTexture::freeTexture()
{
  if( mTextureID != 0 )
  {
    glDeleteTextures( 1, &mTextureID );
    mTextureID = 0;
  }

  if( mPixels != NULL )
  {
    delete[] mPixels;
    mPixels = NULL;
  }

  mImageWidth = 0;
  mImageHeight = 0;

  mTextureWidth = 0;
  mTextureHeight = 0;
}

GLuint LTexture::getTextureID()
{
  return mTextureID;
}

GLuint LTexture::textureWidth()
{
  return mTextureWidth;
}

GLuint LTexture::textureHeight()
{
  return mTextureHeight;
}

GLuint LTexture::imageWidth()
{
  return mImageWidth;
}

GLuint LTexture::imageHeight()
{
  return mImageHeight;
}

bool LTexture::lock()
{
  if( mPixels == NULL && mTextureID != 0 )
  {
    GLuint size = mTextureWidth * mTextureHeight;
    mPixels = new GLuint[ size ];

    glBindTexture( GL_TEXTURE_2D, mTextureID );

    glGetTexImage( GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, mPixels );

    glBindTexture( GL_TEXTURE_2D, 0 );

    return true;
  }

  return false;
}

bool LTexture::unlock()
{
  if( mPixels != NULL && mTextureID != 0 )
  {
    glBindTexture( GL_TEXTURE_2D, mTextureID );

    glTexSubImage2D(
      GL_TEXTURE_2D, 0, 0, 0, mTextureWidth, mTextureHeight, GL_RGBA, GL_UNSIGNED_BYTE, mPixels
    );

    delete[] mPixels;
    mPixels = NULL;

    glBindTexture( GL_TEXTURE_2D, 0 );

    return true;
  }

  return false;
}

GLuint* LTexture::getPixelData32()
{
  return mPixels;
}

GLuint LTexture::getPixel32( GLuint x, GLuint y )
{
  return mPixels[ y * mTextureWidth + x ];
}

void LTexture::setPixel32( GLuint x, GLuint y, GLuint pixel )
{
  mPixels[ y * mTextureWidth + x ] = pixel;
}

GLuint LTexture::powerOfTwo( GLuint num )
{
  if( num != 0 )
  {
      num--;
      num |= (num >> 1);
      num |= (num >> 2);
      num |= (num >> 4);
      num |= (num >> 8);
      num |= (num >> 16);
      num++;
  }

  return num;
}
