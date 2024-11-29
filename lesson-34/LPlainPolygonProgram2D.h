#include "LShaderProgram.h"
#include <glm/glm.hpp>

class LPlainPolygonProgram2D : public LShaderProgram
{
  public:
    LPlainPolygonProgram2D();

    bool loadProgram();

    void setColor( GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.f );

    void setProjection( glm::mat4 matrix );

    void setModelView( glm::mat4 matrix);

    void leftMultProjection( glm::mat4 matrix );

    void leftMultModelView( glm::mat4 matrix );

    void updateProjction();

    void updateModelView();

  private:
    GLint mPolygonColorLocation;

    glm::mat4 mProjectionMatrix;
    GLint mProjectionMatrixLocation;

    glm::mat4 mModelViewMatrix;
    GLint mModelViewMatrixLocation;
};
