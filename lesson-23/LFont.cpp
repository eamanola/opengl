#include "LFont.h"
#include "LVertexData2D.h"
#include FT_BITMAP_H

FT_Library LFont::mLibrary;

bool LFont::initFreeType()
{
  #ifndef __FREEGLUT_H__
    FT_Error error = FT_Init_FreeType( &mLibrary );
    if( error )
    {
      printf( "FreeType error: %X\n", error );
      return false;
    }
  #endif

  return true;
}

LFont::LFont()
{
  mSpace = 0.f;
  mLineHeight = 0.f;
  mNewLine = 0.f;
}

LFont::~LFont()
{
  freeFont();
}

bool LFont::loadBitmap( std::string path )
{
  bool success = true;

  const GLubyte BLACK_PIXEL = 0x00;

  freeFont();

  if( loadPixelsFromFile8( path ) )
  {
    GLfloat cellW = imageWidth() / 16.f;
    GLfloat cellH = imageHeight() / 16.f;

    GLuint top = cellH;
    GLuint bottom = 0;
    GLuint aBottom = 0;

    int pX = 0;
    int pY = 0;

    int bX = 0;
    int bY = 0;

    GLuint currentChar = 0;
    LFRect nextClip = { 0.f, 0.f, cellW, cellH };

    for( unsigned int rows = 0; rows < 16; rows++ )
    {
      for( unsigned int cols = 0; cols < 16; cols++ )
      {
        bX = cellW * cols;
        bY = cellH * rows;

        nextClip.x = cellW * cols;
        nextClip.y = cellH * rows;

        nextClip.w = cellW;
        nextClip.h = cellH;

        //Find left
        for( int pCol = 0; pCol < cellW; ++pCol )
        {
          for( int pRow = 0; pRow < cellH; ++pRow )
          {
            pX = bX + pCol;
            pY = bY + pRow;

            if( getPixel8( pX, pY ) != BLACK_PIXEL )
            {
              nextClip.x = pX;

              //Break the loops
              pCol = cellH;
              pRow = cellH;
            }
          }
        }

        //Right side
        for( int pCol_w = cellW - 1; pCol_w >= 0; --pCol_w )
        {
          for( int pRow_w = 0; pRow_w < cellH; ++pRow_w )
          {
            pX = bX + pCol_w;
            pY = bY + pRow_w;

            if( getPixel8( pX, pY ) != BLACK_PIXEL )
            {
              nextClip.w = ( pX - nextClip.x ) + 1;

              //Break the loops
              pCol_w = -1;
              pRow_w = cellH;
            }
          }
        }

        //Find Top
        for( int pRow = 0; pRow < cellH; ++pRow )
        {
          for( int pCol = 0; pCol < cellW; ++pCol )
          {
            pX = bX + pCol;
            pY = bY + pRow;

            if( getPixel8( pX, pY ) != BLACK_PIXEL )
            {
              if( pRow < (int)top )
              {
                top = pRow;
              }

              pCol = cellW;
              pRow = cellH;
            }
          }
        }

        //Find Bottom
        for( int pRow_b = cellH - 1; pRow_b >= 0; pRow_b-- )
        {
          for( int pCol_b = 0; pCol_b < cellW; pCol_b++ )
          {
            pX = bX + pCol_b;
            pY = bY + pRow_b;

            if( getPixel8( pX, pY ) != BLACK_PIXEL )
            {
              if( currentChar == 'A' )
              {
                aBottom = pRow_b;
              }

              if( pRow_b > (int)bottom )
              {
                bottom = pRow_b;
              }

              pCol_b = cellW;
              pRow_b = -1;
            }
          }
        }

        addClipSprite( nextClip );
        currentChar++;
      }
    }

    for( int t = 0; t < 256; ++t )
    {
      mClips[ t ].y += top;
      mClips[ t ].h -= top;
    }

    if( loadTextureFromPixels8() )
    {
      if( !generateDataBuffer( LSPRITE_ORIGIN_TOP_LEFT ) )
      {
        printf( "Unable to create vertex buffer from bitmap font!\n" );
        success = false;
      }
    }
    else
    {
      printf( "Unable to create texture from bitmap font pixels!\n" );
      success = false;
    }

    glBindTexture( GL_TEXTURE_2D, getTextureID() );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER );

    mSpace = cellW / 2;
    mNewLine = aBottom - top;
    mLineHeight = bottom - top;
  }
  else
  {
    printf( "Could not load bitmap font image: %s!\n", path.c_str() );
    success = false;
  }

  return success;
}

bool LFont::loadFreeType( std::string path, GLuint pixelSize )
{
  FT_Error error = 0;
  #ifdef __FREEGLUT_H__
    error = FT_Init_FreeType( &mLibrary );
    if( error )
    {
      printf( "FreeType error: %X", error );
      return false;
    }
  #endif

  GLuint cellW = 0;
  GLuint cellH = 0;
  int maxBearing = 0;
  int minHang = 0;

  LTexture bitmaps[ 256 ];
  FT_Glyph_Metrics metrics[ 256 ];

  FT_Face face = NULL;

  error = FT_New_Face( mLibrary, path.c_str(), 0, &face );

  if( error )
  {
    printf( "Unable to load font face. FreeType error: %X\n", error );
    return false;
  }

  error = FT_Set_Pixel_Sizes( face, 0, pixelSize );
  if( !error )
  {
    for( int i = 0; i < 256; ++i )
    {
      error = FT_Load_Char( face, i, FT_LOAD_RENDER );
      if( !error )
      {
        metrics[ i ] = face->glyph->metrics;

        bitmaps[ i ].copyPixels8(
          face->glyph->bitmap.buffer,
          face->glyph->bitmap.width,
          face->glyph->bitmap.rows
        );

        int bearing = metrics[ i ].horiBearingY / 64;
        if( bearing > maxBearing )
        {
          maxBearing = bearing;
        }

        int width = metrics[ i ].width / 64;
        if( width > (int)cellW )
        {
          cellW = width;
        }

        int hang = ( metrics[ i ].horiBearingY - metrics[ i ].height ) / 64;
        if( hang < minHang )
        {
          minHang = hang;
        }
      }
      else{
        printf( "Unable to load glyph. FreeType error: %X\n", error );
        error = 0;
      }
    }

    cellH = maxBearing - minHang;
    createPixels8( cellW * 16, cellH * 16 );

    GLuint currentChar = 0;
    LFRect nextClip = { 0.f, 0.f, (GLfloat)cellW, (GLfloat)cellH };

    int bX = 0;
    int bY = 0;

    for( unsigned int rows = 0; rows < 16; rows++ )
    {
      for( unsigned int cols = 0; cols < 16; cols++ )
      {

        bX = cellW * cols;
        bY = cellH * rows;

        nextClip.x = bX;
        nextClip.y = bY;
        nextClip.w = metrics[ currentChar ].width / 64;
        nextClip.h = cellH;

        bitmaps[ currentChar ].blitPixels8(
          bX, bY + maxBearing - metrics[ currentChar ].horiBearingY / 64, *this
        );

        addClipSprite( nextClip );
        currentChar++;
      }
    }

    padPixels8();

    if( loadTextureFromPixels8() )
    {
      if( !generateDataBuffer( LSPRITE_ORIGIN_TOP_LEFT ) )
      {
        printf( "Unable to create bertex buffer from bitmap font!\n" );
        error = 0xA2;
      }
    }
    else
    {
      printf( "Unable to create texture from generated bitmap font!\n" );
      error = 0xA2;
    }

    glBindTexture( GL_TEXTURE_2D, getTextureID() );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER );

    mSpace = cellW / 2;
    mNewLine = maxBearing;
    mLineHeight = cellH;
  }
  else
  {
    printf( "Unable to set font size. FreeType error: %X\n", error );
  }

  FT_Done_Face( face );

  #ifdef __FREEGLUT_H__
    FT_Done_FreeType( mLibrary );
  #endif

  return error == 0;
}

void LFont::freeFont()
{
  freeTexture();

  mSpace = 0.f;
  mLineHeight = 0.f;
  mNewLine = 0.f;
}

void LFont::renderText( GLfloat x, GLfloat y, std::string text )
{
  if( getTextureID() != 0 )
  {
    GLfloat dX = x;
    GLfloat dY = y;

    glTranslatef( x, y, 0.f );

    glBindTexture( GL_TEXTURE_2D, getTextureID() );

    glEnableClientState( GL_VERTEX_ARRAY );
    glEnableClientState( GL_TEXTURE_COORD_ARRAY );

    glBindBuffer( GL_ARRAY_BUFFER, mVertexDataBuffer );

    glTexCoordPointer(
      2, GL_FLOAT, sizeof(LVertexData2D), (GLvoid*) offsetof( LVertexData2D, texCoord )
    );

    glVertexPointer(
      2, GL_FLOAT, sizeof(LVertexData2D), (GLvoid*) offsetof( LVertexData2D, position )
    );

    int textLength = (int)text.length();
    for( int i = 0; i < textLength; ++i )
    {
      if( text[ i ] == ' ' )
      {
        glTranslatef( mSpace, 0.f, 0.f );
        dX += mSpace;
      }
      else if( text[ i ] == '\n' )
      {
        glTranslatef( x - dX, mNewLine, 0.f );
        dY += mNewLine;
        dX = x;
      }
      else
      {
        GLuint ascii = (unsigned char)text[ i ];

        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, mIndexBuffers[ ascii ] );
        glDrawElements( GL_QUADS, 4, GL_UNSIGNED_INT, NULL );

        glTranslatef( mClips[ ascii ].w, 0.f, 0.f );
        dX += mClips[ ascii ].w;
      }
    }

    glDisableClientState( GL_VERTEX_ARRAY );
    glDisableClientState( GL_TEXTURE_COORD_ARRAY );
  }
}
