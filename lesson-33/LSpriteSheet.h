#ifndef LSPRITE_SHEET_H
#define LSPRITE_SHEET_H

#include "./LTexture.h"
#include "./LFRect.h"
#include <vector>

enum LSpriteOrigin
{
  LSPRITE_ORIGIN_CENTER,
  LSPRITE_ORIGIN_TOP_LEFT,
  LSPRITE_ORIGIN_BOTTOM_LEFT,
  LSPRITE_ORIGIN_TOP_RIGHT,
  LSPRITE_ORIGIN_BOTTOM_RIGHT
};

class LSpriteSheet : public LTexture
{

  public:
    LSpriteSheet(/* args */);
    ~LSpriteSheet();

    int addClipSprite( LFRect& newClip );
    LFRect getClip( int index );
    bool generateDataBuffer( LSpriteOrigin origin = LSPRITE_ORIGIN_CENTER );
    void freeSheet();
    void freeTexture();
    void renderSprite( int index );

  protected:
    std::vector<LFRect> mClips;

    GLuint mVertexDataBuffer;
    GLuint* mIndexBuffers;
};

#endif
