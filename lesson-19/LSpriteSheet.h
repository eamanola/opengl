#ifndef LSPRITE_SHEET_H
#define LSPRITE_SHEET_H

#include "./LTexture.h"
#include "./LFRect.h"
#include <vector>

class LSpriteSheet : public LTexture
{

  public:
    LSpriteSheet(/* args */);
    ~LSpriteSheet();

    int addClipSprite( LFRect& newClip );
    LFRect getClip( int index );
    bool generateDataBuffer();
    void freeSheet();
    void freeTexture();
    void renderSprite( int index );

  protected:
    std::vector<LFRect> mClips;

    GLuint mVertexDataBuffer;
    GLuint* mIndexBuffers;
};

#endif
