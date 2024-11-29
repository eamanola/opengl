#ifndef LTEXTURED_POLYGON_PROGRAM_2D
#define LTEXTURED_POLYGON_PROGRAM_2D

#include "LShaderProgram.h"
#include <glm/glm.hpp>
#include "LColorRGBA.h"

class LTexturedPolygonProgram2D : public LShaderProgram
{
  public:
    LTexturedPolygonProgram2D();

    bool loadProgram();

    void setVertexPointer( GLsizei stride, const GLvoid* data );
    void setTexCoordPointer( GLsizei stride, const GLvoid* data );

    void enableVertexPointer();
    void enableTexCoordPointer();

    void disableVertexPointer();
    void disableTexCoordPointer();

    void setProjection( glm::mat4 matrix );
    void setModelView( glm::mat4 matrix );

    void leftMultProjection( glm::mat4 matrix );
    void leftMultModelView( glm::mat4 matrix );

    void updateProjection();
    void updateModelView();

    void setTextureColor( LColorRGBA color );
    void setTextureUnit( GLuint unit );

  private:
    GLint mVertexPos2DLocation;
    GLint mTexCoordLocation;
    GLint mTexColorLocation;
    GLint mTexUnitLocation;

    glm::mat4 mProjection;
    GLint mProjectionLocation;

    glm::mat4 mModelView;
    GLint mModelViewLocation;
};

#endif
