#include "LTexture.h"
#include <IL/il.h>
#include <IL/ilu.h>
#include <string>
#include <cstring>
#include "./LVertexData2D.h"

GLenum DEFAULT_TEXTURE_WRAP = GL_REPEAT;

LTexture::LTexture()
{
  mTextureID = 0;
  mPixels = NULL;

  mTextureWidth = 0;
  mTextureHeight = 0;

  mImageWidth = 0;
  mImageHeight = 0;

  mVBOID = 0;
  mIBOID = 0;
}

LTexture::~LTexture()
{
  freeTexture();

  freeVBO();
}

void LTexture::render( GLfloat x, GLfloat y, LFRect* clip )
{
  if(mTextureID != 0)
  {
    //Texture coordinates
    GLfloat texTop = 0.f;
    GLfloat texBottom = (GLfloat)mImageHeight / (GLfloat)mTextureHeight;
    GLfloat texLeft = 0.f;
    GLfloat texRight = (GLfloat)mImageWidth / (GLfloat)mTextureWidth;

    //Vertex coordinates
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

    glTranslatef( x, y, 0.f );

    LVertexData2D vData[ 4 ];

    vData[ 0 ].texCoord.s = texLeft;  vData[ 0 ].texCoord.t = texTop;
    vData[ 1 ].texCoord.s = texRight; vData[ 1 ].texCoord.t = texTop;
    vData[ 2 ].texCoord.s = texRight; vData[ 2 ].texCoord.t = texBottom;
    vData[ 3 ].texCoord.s = texLeft;  vData[ 3 ].texCoord.t = texBottom;

    vData[ 0 ].position.x = 0.f;       vData[ 0 ].position.y = 0.f;
    vData[ 1 ].position.x = quadWidth; vData[ 1 ].position.y = 0.f;
    vData[ 2 ].position.x = quadWidth; vData[ 2 ].position.y = quadHeight;
    vData[ 3 ].position.x = 0.f;       vData[ 3 ].position.y = quadHeight;;


    glBindTexture( GL_TEXTURE_2D, mTextureID );

    glEnableClientState( GL_VERTEX_ARRAY );
    glEnableClientState( GL_TEXTURE_COORD_ARRAY );

    glBindBuffer( GL_ARRAY_BUFFER, mVBOID );
    glBufferSubData( GL_ARRAY_BUFFER, 0, 4 * sizeof(LVertexData2D), vData );

    glTexCoordPointer(
      2, GL_FLOAT, sizeof(LVertexData2D), (GLvoid*)offsetof( LVertexData2D, texCoord )
    );

    glVertexPointer(
      2, GL_FLOAT, sizeof(LVertexData2D), (GLvoid*)offsetof( LVertexData2D, position )
    );

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, mIBOID );
    glDrawElements( GL_QUADS, 4, GL_UNSIGNED_INT, NULL );

    glDisableClientState( GL_VERTEX_ARRAY );
    glDisableClientState( GL_TEXTURE_COORD_ARRAY );
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
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, DEFAULT_TEXTURE_WRAP );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, DEFAULT_TEXTURE_WRAP );

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

bool LTexture::loadTextureFromFile( std::string path )
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

bool LTexture::loadTextureFromPixels32(
  GLuint* pixels, GLuint imgWidth, GLuint imgHeight, GLuint texWidth, GLuint texHeight
)
{
  freeTexture();

  mImageWidth = imgWidth;
  mImageHeight = imgHeight;
  mTextureWidth = texWidth;
  mTextureHeight = texHeight;

  glGenTextures( 1, &mTextureID );

  glBindTexture( GL_TEXTURE_2D, mTextureID );

  glTexImage2D(
    GL_TEXTURE_2D, 0, GL_RGBA, mTextureWidth, mTextureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels
  );

  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, DEFAULT_TEXTURE_WRAP );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, DEFAULT_TEXTURE_WRAP );

  glBindTexture( GL_TEXTURE_2D, 0 );

  GLenum error = glGetError();
  if( error != GL_NO_ERROR )
  {
    printf( "Error loading texture from %p pixels! %s\n", pixels, gluErrorString( error ) );
    return false;
  }

  initVBO();

  return true;
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

void LTexture::initVBO()
{
  if( mTextureID != 0 && mVBOID == 0)
  {
    LVertexData2D vData[ 4 ];
    GLuint iData[ 4 ];

    iData[ 0 ] = 0;
    iData[ 1 ] = 1;
    iData[ 2 ] = 2;
    iData[ 3 ] = 3;

    glGenBuffers( 1, &mVBOID );
    glBindBuffer( GL_ARRAY_BUFFER, mVBOID );
    glBufferData( GL_ARRAY_BUFFER, 4 * sizeof(LVertexData2D), vData, GL_DYNAMIC_DRAW );

    glBindBuffer( GL_ARRAY_BUFFER, 0 );

    glGenBuffers( 1, &mIBOID );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, mIBOID );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), iData, GL_DYNAMIC_DRAW );

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
  }
}

void LTexture::freeVBO()
{
  if( mVBOID != 0)
  {
    glDeleteBuffers( 1, &mVBOID );
    glDeleteBuffers( 1, &mIBOID );
  }
}
