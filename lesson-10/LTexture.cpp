#include "LTexture.h"
#include <IL/il.h>
#include <IL/ilu.h>
#include <string>
#include <cstring>

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

bool LTexture::loadPixelsFromFile( std::string path )
{
  freeTexture();

  bool pixelsLoaded = false;

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

      GLuint size = texWidth * texHeight;
      mPixels = new GLuint[ size ];

      mImageWidth = imgWidth;
      mImageHeight = imgHeight;
      mTextureWidth = texWidth;
      mTextureHeight = texHeight;

      std::memcpy( mPixels, ilGetData(), size * 4 );
      pixelsLoaded = true;
    }

    ilDeleteImages( 1, &imgID );
  }

  if( !pixelsLoaded )
  {
    printf( "Unable to load %s\n", path.c_str() );
  }

  return pixelsLoaded;
}

bool LTexture::loadTextureFromPixels32()
{
  bool success = true;

  if ( mTextureID == 0 && mPixels != NULL )
  {
    glGenTextures( 1, &mTextureID );

    glBindTexture( GL_TEXTURE_2D, mTextureID );

    glTexImage2D(
      GL_TEXTURE_2D, 0, GL_RGBA,
      mTextureWidth, mTextureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, mPixels
    );

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

    glBindTexture( GL_TEXTURE_2D, 0 );

    GLenum error = glGetError();
    if( error != GL_NO_ERROR )
    {
      printf( "Error loading texture from %p pixels! %s\n", mPixels, gluErrorString( error ) );
      success = false;
    }

    if( success )
    {
      delete[] mPixels;
      mPixels = NULL;
    }
  }
  else{
    printf( "Cannot load texture from current pixels! " );

    if( mTextureID != 0 )
    {
      printf( "A texture is already loaded!\n" );
    }
    else if( mPixels == NULL )
    {
      printf( "No pixels to create texture from!\n" );
    }

    success = false;
  }

  return success;
}

bool LTexture::loadTextureFromFileWithColorKey(
  std::string path, GLubyte r, GLubyte g, GLubyte b, GLubyte a
)
{
  if( !loadPixelsFromFile( path ) )
  {
    return false;
  }

  int size = (int)(mTextureWidth * mTextureHeight);

  for( int i = 0; i < size; ++i )
  {
    GLubyte* colors = (GLubyte*)&mPixels[ i ];

    if( colors[ 0 ] == r && colors[ 1 ] == g && colors[ 2 ] == b && ( 0 == a || colors[ 3 ] == a ) )
    {
      colors[ 0 ] = 255;
      colors[ 1 ] = 255;
      colors[ 2 ] = 255;
      colors[ 3 ] = 000;
    }
  }

  return loadTextureFromPixels32();
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
