#include "LShaderProgram.h"

class LPlainPolygonProgram2D : public LShaderProgram
{
  public:
    LPlainPolygonProgram2D();

    bool loadProgram();

    void setColor( GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.f );

  private:
    GLint mPolygonColorLocation;
};
