#ifndef LFONT_H
#define LFONT_H

#include "LSpriteSheet.h"
#include <freetype2/ft2build.h>
#include FT_FREETYPE_H

class LFont : private LSpriteSheet
{
  public:
    static bool initFreeType();
    LFont();
    ~LFont();

    bool loadBitmap( std::string path );
    bool loadFreeType( std::string path, GLuint pixelSize );

    void freeFont();
    void renderText( GLfloat x, GLfloat y, std::string text );

  private:
    static FT_Library mLibrary;

    GLfloat mSpace;
    GLfloat mLineHeight;
    GLfloat mNewLine;
};

#endif
