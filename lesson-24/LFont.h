#ifndef LFONT_H
#define LFONT_H

#include "LSpriteSheet.h"
#include <freetype2/ft2build.h>
#include FT_FREETYPE_H

enum LFontTextAlignment
{
  LFONT_TEXT_ALIGN_LEFT = 1,
  LFONT_TEXT_ALIGN_CENTERED_H = 2,
  LFONT_TEXT_ALIGN_RIGHT = 4,
  LFONT_TEXT_ALIGN_TOP = 8,
  LFONT_TEXT_ALIGN_CENTERED_V = 16,
  LFONT_TEXT_ALIGN_BOTTOM = 32
};

class LFont : private LSpriteSheet
{
  public:
    static bool initFreeType();
    LFont();
    ~LFont();

    bool loadBitmap( std::string path );
    bool loadFreeType( std::string path, GLuint pixelSize );

    void freeFont();
    void renderText(
      GLfloat x, GLfloat y, std::string text,
      LFRect* area = NULL, int align = LFONT_TEXT_ALIGN_LEFT
    );

    GLfloat getLineHeight();

  private:
    static FT_Library mLibrary;

    GLfloat substringWidth( const char* substring );
    GLfloat stringHeight( const char* thisString );

    GLfloat mSpace;
    GLfloat mLineHeight;
    GLfloat mNewLine;
};

#endif
