#include "LShaderProgram.h"
#include <glm/glm.hpp>

class LMultiColorPolygonProgram2D : public LShaderProgram
{
  public:
    LMultiColorPolygonProgram2D();

    bool loadProgram();

    void setVertexPointer( GLsizei stride, const GLvoid* data );

    void setColorPointer( GLsizei stride, const GLvoid* data );

    void enableVertexPointer();

    void disableVertexPointer();

    void enableColorPointer();

    void disableColorPointer();

    void setProjection( glm::mat4 matrix );

    void setModelView( glm::mat4 matrix );

    void leftMultProjection( glm::mat4 matrix );

    void leftMultModelView( glm::mat4 matrix );

    void updateProjection();

    void updateModelView();

  private:
    GLint mVertexPos2DLocation;
    GLint mMultiColorLocation;

    glm::mat4 mProjection;
    GLint mProjectionLocation;

    glm::mat4 mModelView;
    GLint mModelViewLocation;
};
