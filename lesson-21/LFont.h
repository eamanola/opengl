#ifndef LFONT_H
#define LFONT_H

#include "LSpriteSheet.h"

class LFont : private LSpriteSheet
{
  public:
    LFont();
    ~LFont();

    bool loadBitmap( std::string path );
    void freeFont();
    void renderText( GLfloat x, GLfloat y, std::string text );

  private:
    GLfloat mSpace;
    GLfloat mLineHeight;
    GLfloat mNewLine;
};

#endif
