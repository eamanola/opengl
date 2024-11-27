#include "LSpriteSheet.h"
#include "LVertexData2D.h"

LSpriteSheet::LSpriteSheet()
{
  mVertexDataBuffer = 0;
  mIndexBuffers = NULL;
}

LSpriteSheet::~LSpriteSheet()
{
  freeSheet();
}

int LSpriteSheet::addClipSprite( LFRect& newClip )
{
  mClips.push_back( newClip );
  return mClips.size() - 1;
}

LFRect LSpriteSheet::getClip( int index )
{
  return mClips[ index ];
}

bool LSpriteSheet::generateDataBuffer( LSpriteOrigin origin )
{
  if ( getTextureID() != 0 && mClips.size() > 0 )
  {
    int totalSprites = mClips.size();
    LVertexData2D* vertexData = new LVertexData2D[ totalSprites * 4 ];
    mIndexBuffers = new GLuint[ totalSprites ];

    glGenBuffers( 1, &mVertexDataBuffer );

    glGenBuffers( totalSprites, mIndexBuffers );

    GLfloat tW = textureWidth();
    GLfloat tH = textureHeight();
    GLuint spriteIndices[ 4 ] = { 0, 0, 0, 0 };

    GLfloat vTop = 0.f;
    GLfloat vBottom = 0.f;
    GLfloat vLeft = 0.f;
    GLfloat vRight = 0.f;

    for( int i = 0; i < totalSprites; ++i )
    {
      spriteIndices[ 0 ] = i * 4 + 0;
      spriteIndices[ 1 ] = i * 4 + 1;
      spriteIndices[ 2 ] = i * 4 + 2;
      spriteIndices[ 3 ] = i * 4 + 3;

      switch ( origin )
      {
        case LSPRITE_ORIGIN_TOP_LEFT:
          vTop = 0.f;
          vBottom = mClips[ i ].h;
          vLeft = 0.f;
          vRight = mClips[ i ].w;
          break;

        case LSPRITE_ORIGIN_TOP_RIGHT:
          vTop = 0.f;
          vBottom = mClips[ i ].h;
          vLeft = -mClips[ i ].w;
          vRight = 0.f;
          break;

        case LSPRITE_ORIGIN_BOTTOM_LEFT:
          vTop = -mClips[ i ].h;
          vBottom = 0.f;
          vLeft = 0.f;
          vRight = mClips[ i ].w;
          break;

        case LSPRITE_ORIGIN_BOTTOM_RIGHT:
          vTop = -mClips[ i ].h;
          vBottom = 0.f;
          vLeft = -mClips[ i ].w;
          vRight = 0.f;
          break;

        case LSPRITE_ORIGIN_CENTER:
          vTop = -mClips[ i ].h / 2.f;
          vBottom = mClips[ i ].h / 2.f;
          vLeft = -mClips[ i ].w / 2.f;
          vRight = mClips[ i ].w / 2.f;
          break;

        default:
          break;
      }

      // Top Left
      vertexData[ spriteIndices[ 0 ] ].position.x = vLeft;
      vertexData[ spriteIndices[ 0 ] ].position.y = vTop;

      vertexData[ spriteIndices[ 0 ] ].texCoord.s = (mClips[ i ].x) / tW;
      vertexData[ spriteIndices[ 0 ] ].texCoord.t = (mClips[ i ].y) / tH;

      // Top Right
      vertexData[ spriteIndices[ 1 ] ].position.x = vRight;
      vertexData[ spriteIndices[ 1 ] ].position.y = vTop;

      vertexData[ spriteIndices[ 1 ] ].texCoord.s = (mClips[ i ].x + mClips[ i ].w) / tW;
      vertexData[ spriteIndices[ 1 ] ].texCoord.t = (mClips[ i ].y) / tH;

      // Bottom Right
      vertexData[ spriteIndices[ 2 ] ].position.x = vRight;
      vertexData[ spriteIndices[ 2 ] ].position.y = vBottom;

      vertexData[ spriteIndices[ 2 ] ].texCoord.s = (mClips[ i ].x + mClips[ i ].w) / tW;
      vertexData[ spriteIndices[ 2 ] ].texCoord.t = (mClips[ i ].y + mClips[ i ].h) / tH;

      // Bottom Left
      vertexData[ spriteIndices[ 3 ] ].position.x = vLeft;
      vertexData[ spriteIndices[ 3 ] ].position.y = vBottom;

      vertexData[ spriteIndices[ 3 ] ].texCoord.s = (mClips[ i ].x) / tW;
      vertexData[ spriteIndices[ 3 ] ].texCoord.t = (mClips[ i ].y + mClips[ i ].h) / tH;

      glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, mIndexBuffers[ i ] );
      glBufferData( GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), spriteIndices, GL_STATIC_DRAW );
    }

    glBindBuffer( GL_ARRAY_BUFFER, mVertexDataBuffer );
    glBufferData(
      GL_ARRAY_BUFFER, totalSprites * 4 * sizeof(LVertexData2D), vertexData, GL_STATIC_DRAW
    );

    delete[] vertexData;
  }
  else
  {
    if( getTextureID() == 0 )
    {
      printf( "No texture to render with!\n" );
    }

    if ( mClips.size() <= 0 )
    {
      printf( "No clips to generate vertex data from!\n" );
    }

    return false;
  }

  return true;
}

void LSpriteSheet::freeSheet()
{
  if( mVertexDataBuffer != 0 )
  {
    glDeleteBuffers( 1, &mVertexDataBuffer );
    mVertexDataBuffer = 0;
  }

  if( mIndexBuffers != NULL )
  {
    glDeleteBuffers( mClips.size(), mIndexBuffers );
    delete[] mIndexBuffers;
    mIndexBuffers = NULL;
  }

  mClips.clear();
}

void LSpriteSheet::freeTexture()
{
  freeSheet();

  LTexture::freeTexture();
}

void LSpriteSheet::renderSprite( int index )
{
  if( mVertexDataBuffer != 0 )
  {
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

      glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, mIndexBuffers[ index ] );
      glDrawElements( GL_QUADS, 4, GL_UNSIGNED_INT, NULL );

    glDisableClientState( GL_VERTEX_ARRAY );
    glDisableClientState( GL_TEXTURE_COORD_ARRAY );
  }
}
