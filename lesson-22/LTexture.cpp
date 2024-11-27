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
  mPixels32 = NULL;
  mPixels8 = NULL;
  mPixelFormat = GL_NONE;

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

  if( mPixels32 != NULL )
  {
    delete[] mPixels32;
    mPixels32 = NULL;
  }

  if( mPixels8 != NULL )
  {
    delete[] mPixels8;
    mPixels8 = NULL;
  }

  mImageWidth = 0;
  mImageHeight = 0;

  mTextureWidth = 0;
  mTextureHeight = 0;

  mPixelFormat = GL_NONE;
}

const GLuint LTexture::getTextureID()
{
  return mTextureID;
}

const GLuint LTexture::textureWidth()
{
  return mTextureWidth;
}

const GLuint LTexture::textureHeight()
{
  return mTextureHeight;
}

const GLuint LTexture::imageWidth()
{
  return mImageWidth;
}

const GLuint LTexture::imageHeight()
{
  return mImageHeight;
}

GLuint* LTexture::getPixelData32()
{
  return mPixels32;
}

const GLuint LTexture::getPixel32( GLuint x, GLuint y )
{
  return mPixels32[ y * mTextureWidth + x ];
}

void LTexture::setPixel32( GLuint x, GLuint y, GLuint pixel )
{
  mPixels32[ y * mTextureWidth + x ] = pixel;
}

GLubyte* LTexture::getPixelData8()
{
  return mPixels8;
}

const GLubyte LTexture::getPixel8( GLuint x, GLuint y )
{
  return mPixels8[ y * mTextureWidth + x ];
}

void LTexture::setPixel8( GLuint x, GLuint y, GLubyte pixel )
{
  mPixels8[ y * mTextureWidth + x ] = pixel;
}

bool LTexture::loadPixelsFromFile( std::string path, GLuint pixelFormat )
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
      initPixels( pixelFormat, size );

      mImageWidth = imgWidth;
      mImageHeight = imgHeight;
      mTextureWidth = texWidth;
      mTextureHeight = texHeight;

      std::memcpy(
        getPixelData(pixelFormat), ilGetData(), size * ( pixelFormat == GL_RGBA ? 4 : 1 )
      );
      pixelsLoaded = true;
    }

    ilDeleteImages( 1, &imgID );

    mPixelFormat = pixelFormat;
  }

  if( !pixelsLoaded )
  {
    printf( "Unable to load %s\n", path.c_str() );
  }

  return pixelsLoaded;
}

bool LTexture::loadPixelsFromFile8( std::string path )
{
  return loadPixelsFromFile( path, GL_ALPHA );
}

bool LTexture::loadPixelsFromFile32( std::string path )
{
  return loadPixelsFromFile( path, GL_RGBA );
}

void LTexture::createPixels( GLuint imgWidth, GLuint imgHeight, GLuint pixelFormat )
{
  if( imgWidth > 0 && imgHeight > 0 )
  {
    freeTexture();

    GLuint size = imgWidth * imgHeight;
    initPixels(pixelFormat, size);

    mImageWidth = imgWidth;
    mImageHeight = imgHeight;
    mTextureWidth = mImageWidth;
    mTextureHeight = mTextureHeight;

    mPixelFormat = pixelFormat;
  }
}

void LTexture::createPixels32( GLuint imgWidth, GLuint imgHeight )
{
  return createPixels( imgWidth, imgHeight, GL_RGBA );
}

void LTexture::createPixels8( GLuint imgWidth, GLuint imgHeight )
{
  return createPixels( imgWidth, imgHeight, GL_ALPHA );
}

void LTexture::copyPixels( GLuint* pixels, GLuint imgWidth, GLuint imgHeight, GLuint pixelFormat )
{
  if( imgWidth > 0 && imgHeight > 0 )
  {
    freeTexture();

    GLuint size = imgWidth * imgHeight;
    initPixels(pixelFormat, size);
    memcpy( getPixelData(pixelFormat), pixels, size * ( pixelFormat == GL_RGBA ? 4 : 1 ) );

    mImageWidth = imgWidth;
    mImageHeight = imgHeight;
    mTextureWidth = mImageWidth;
    mTextureHeight = mImageHeight;

    mPixelFormat = pixelFormat;
  }
}

void LTexture::copyPixels32( GLuint* pixels, GLuint imgWidth, GLuint imgHeight)
{
  return copyPixels( pixels, imgWidth, imgHeight, GL_RGBA );
}

void LTexture::copyPixels8( GLuint* pixels, GLuint imgWidth, GLuint imgHeight)
{
  return copyPixels( pixels, imgWidth, imgHeight, GL_ALPHA );
}

void LTexture::blitPixels32( GLuint x, GLuint y, LTexture& destination )
{
  if( mPixels32 != NULL && destination.getPixelData32() != NULL )
  {
    GLuint* dPixels = destination.getPixelData32();
    GLuint* sPixels = mPixels32;

    int height = (int)mImageHeight;
    for( int i = 0; i < height; ++i )
    {
      memcpy(
        &dPixels[ ( i + y ) * destination.textureWidth() + x ],
        &sPixels[ i * mTextureWidth ],
        mImageWidth * 4
      );
    }
  }
}

void LTexture::blitPixels8( GLuint x, GLuint y, LTexture& destination )
{
  if( mPixels8 != NULL && destination.getPixelData8() != NULL )
  {
    GLubyte* dPixels = destination.getPixelData8();
    GLubyte* sPixels = mPixels8;

    int height = (int)mImageHeight;
    for( int i = 0; i < height; ++i )
    {
      memcpy(
        &dPixels[ ( i + y ) * destination.textureWidth() + x ],
        &sPixels[ i * mTextureWidth ],
        mImageWidth
      );
    }
  }
}

void LTexture::padPixels32()
{
  if( mPixels32 != NULL )
  {
    GLuint oTextureWidth = mTextureWidth;
    // GLuint oTextureHeight = mTextureHeight;

    mTextureWidth = powerOfTwo( imageWidth() );
    mTextureHeight = powerOfTwo( mImageHeight );

    if( mTextureWidth != mImageWidth || mTextureHeight != mImageHeight )
    {
      GLuint size = mTextureWidth * mTextureHeight;
      GLuint* pixels = new GLuint[ size ];

      int height = (int)mImageHeight;
      for( int i = 0; i < height; ++i )
      {
        memcpy( &pixels[ i * mTextureWidth ], &mPixels32[ i * oTextureWidth ], mImageWidth * 4 );
      }

      delete[] mPixels32;
      mPixels32 = pixels;
    }
  }
}

void LTexture::padPixels8()
{
  if( mPixels8 != NULL )
  {
    GLuint oTextureWidth = mTextureWidth;
    // GLuint oTextureHeight = mTextureHeight;

    mTextureWidth = powerOfTwo( mImageWidth );
    mTextureHeight = powerOfTwo( mImageHeight );

    if( mTextureWidth != mImageWidth || mTextureHeight != mImageHeight )
    {
      GLuint size = mTextureWidth * mTextureHeight;
      GLubyte* pixels = new GLubyte[ size ];

      int height = (int)mImageHeight;
      for( int i = 0; i < height; ++i )
      {
        memcpy( &pixels[ i * mTextureWidth ], &mPixels8[ i * oTextureWidth ], mImageWidth );
      }

      delete[] mPixels8;
      mPixels8 = pixels;
    }
  }
}

bool LTexture::loadTextureFromPixels( GLuint pixelFormat )
{
  bool success = true;
  void* pixels = getPixelData(pixelFormat);

  if ( mTextureID == 0 && pixels != NULL )
  {
    glGenTextures( 1, &mTextureID );

    glBindTexture( GL_TEXTURE_2D, mTextureID );

    glTexImage2D(
      GL_TEXTURE_2D, 0, pixelFormat,
      mTextureWidth, mTextureHeight, 0, pixelFormat, GL_UNSIGNED_BYTE, pixels
    );

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, DEFAULT_TEXTURE_WRAP );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, DEFAULT_TEXTURE_WRAP );

    glBindTexture( GL_TEXTURE_2D, 0 );

    GLenum error = glGetError();
    if( error != GL_NO_ERROR )
    {
      printf(
        "Error loading texture from %p pixels! %s\n",
        pixels,
        gluErrorString( error )
      );
      success = false;
    }

    if( success )
    {
      initVBO();

      mPixelFormat = pixelFormat;
    }
  }
  else{
    printf( "Cannot load texture from current pixels! " );

    if( mTextureID != 0 )
    {
      printf( "A texture is already loaded!\n" );
    }
    else if( pixels == NULL )
    {
      printf( "No pixels to create texture from!\n" );
    }

    success = false;
  }

  return success;
}

bool LTexture::loadTextureFromPixels32()
{
  bool success = loadTextureFromPixels( GL_RGBA );
  if (success)
  {
    delete[] mPixels32;
    mPixels32 = NULL;
  }

  return success;
}

bool LTexture::loadTextureFromPixels8()
{
  bool success = loadTextureFromPixels( GL_ALPHA );
  if (success)
  {
    delete[] mPixels8;
    mPixels8 = NULL;
  }

  return success;
}

bool LTexture::loadTextureFromFileWithColorKey32(
  std::string path, GLubyte r, GLubyte g, GLubyte b, GLubyte a
)
{
  if( !loadPixelsFromFile32( path ) )
  {
    return false;
  }

  int size = (int)(mTextureWidth * mTextureHeight);

  for( int i = 0; i < size; ++i )
  {
    GLubyte* colors = (GLubyte*)&mPixels32[ i ];

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

bool LTexture::loadTextureFromFile32( std::string path )
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

  mPixelFormat = GL_RGBA;

  return true;
}

bool LTexture::lock()
{
  if( mPixels32 == NULL && mPixels8 == NULL && mTextureID != 0 )
  {
    GLuint size = mTextureWidth * mTextureHeight;
    initPixels(mPixelFormat, size);

    glBindTexture( GL_TEXTURE_2D, mTextureID );

    void* pixels = ( mPixelFormat == GL_RGBA ) ? (void*)mPixels32 : (void*)mPixels8;
    glGetTexImage( GL_TEXTURE_2D, 0, mPixelFormat, GL_UNSIGNED_BYTE, pixels );

    glBindTexture( GL_TEXTURE_2D, 0 );

    return true;
  }

  return false;
}

bool LTexture::unlock()
{
  if( mPixels32 != NULL && mTextureID != 0 )
  {
    glBindTexture( GL_TEXTURE_2D, mTextureID );

    void* pixels = ( mPixelFormat == GL_RGBA ) ? (void*)mPixels32 : (void*)mPixels8;
    glTexSubImage2D(
      GL_TEXTURE_2D, 0, 0, 0, mTextureWidth, mTextureHeight, mPixelFormat, GL_UNSIGNED_BYTE, pixels
    );

    if( mPixels32 != NULL )
    {
      delete[] mPixels32;
      mPixels32 = NULL;
    }
    if( mPixels8 != NULL )
    {
      delete[] mPixels8;
      mPixels8 = NULL;
    }

    glBindTexture( GL_TEXTURE_2D, 0 );

    return true;
  }

  return false;
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

void LTexture::initPixels(GLuint pixelFormat, GLuint size)
{
  if( pixelFormat == GL_RGBA ){
    mPixels32 = new GLuint[ size ];
  }
  else if( pixelFormat == GL_ALPHA )
  {
    mPixels8 = new GLubyte[ size ];
  }
}

void* LTexture::getPixelData(GLuint pixelFormat)
{
  return ( pixelFormat == GL_RGBA ? (void *)getPixelData32() : (void *)getPixelData8() );
}
