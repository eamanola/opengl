#include "LMultiColorPolygonProgram2D.h"
#include <glm/gtc/type_ptr.hpp>

LMultiColorPolygonProgram2D::LMultiColorPolygonProgram2D()
{
  mVertexPos2DLocation = 0;
  mMultiColorLocation = 0;
  mProjectionLocation = 0;
  mModelViewLocation = 0;
}

bool LMultiColorPolygonProgram2D::loadProgram()
{
  mProgramID = glCreateProgram();

  GLuint vertexShader = loadShaderFromFile(
    "./shaders/LMultiColorPolygonProgram2D.glvs", GL_VERTEX_SHADER
  );
  if( vertexShader == 0 )
  {
    glDeleteProgram( mProgramID );
    mProgramID = 0;
    return false;
  }
  glAttachShader( mProgramID, vertexShader );

  GLuint fragmentShader = loadShaderFromFile(
    "./shaders/LMultiColorPolygonProgram2D.glfs", GL_FRAGMENT_SHADER
  );
  if( fragmentShader == 0 )
  {
    glDeleteShader( vertexShader );
    glDeleteProgram( mProgramID );
    mProgramID = 0;
    return false;
  }
  glAttachShader( mProgramID, fragmentShader );

  glLinkProgram( mProgramID );

  GLint programSuccess = GL_TRUE;
  glGetProgramiv( mProgramID, GL_LINK_STATUS, &programSuccess );
  if( programSuccess != GL_TRUE )
  {
    printf( "Error linking program %d!\n", mProgramID );
    printProgramLog( mProgramID );
    glDeleteShader( vertexShader );
    glDeleteShader( fragmentShader );
    glDeleteProgram( mProgramID );
    mProgramID = 0;
    return false;
  }

  glDeleteShader( vertexShader );
  glDeleteShader( fragmentShader );

  const GLchar* LVERTEX_POS_2D = "LVertexPos2D";
  mVertexPos2DLocation = glGetAttribLocation( mProgramID, LVERTEX_POS_2D );
  if( mVertexPos2DLocation == -1 )
  {
    printf( "%s is not a valid glsl program variable!\n", LVERTEX_POS_2D );
  }

  const GLchar* LMULTI_COLOR = "LMultiColor";
  mMultiColorLocation = glGetAttribLocation( mProgramID, LMULTI_COLOR );
  if( mMultiColorLocation == -1 )
  {
    printf( "%s is not a valid glsl program variable!\n", LMULTI_COLOR );
  }

  const GLchar* LPROJECTION = "LProjectionMatrix";
  mProjectionLocation = glGetUniformLocation( mProgramID, LPROJECTION );
  if( mProjectionLocation == -1 )
  {
    printf( "%s is not a valid glsl program variable!\n", LPROJECTION );
  }

  const GLchar* LMODELVIEW = "LModelViewMatrix";
  mModelViewLocation = glGetUniformLocation( mProgramID, LMODELVIEW );
  if( mModelViewLocation == -1 )
  {
    printf( "%s is not a valid glsl program variable!\n", LMODELVIEW );
  }

  return true;
}

void LMultiColorPolygonProgram2D::setVertexPointer( GLsizei stride, const GLvoid* data )
{
  glVertexAttribPointer( mVertexPos2DLocation, 2, GL_FLOAT, GL_FALSE, stride, data );
}

void LMultiColorPolygonProgram2D::setColorPointer( GLsizei stride, const GLvoid* data )
{
  glVertexAttribPointer( mMultiColorLocation, 4, GL_FLOAT, GL_FALSE, stride, data );
}

void LMultiColorPolygonProgram2D::enableVertexPointer()
{
  glEnableVertexAttribArray( mVertexPos2DLocation );
}

void LMultiColorPolygonProgram2D::disableVertexPointer()
{
  glDisableVertexAttribArray( mVertexPos2DLocation );
}

void LMultiColorPolygonProgram2D::enableColorPointer()
{
  glEnableVertexAttribArray( mMultiColorLocation );
}

void LMultiColorPolygonProgram2D::disableColorPointer()
{
  glDisableVertexAttribArray( mMultiColorLocation );
}

void LMultiColorPolygonProgram2D::setProjection( glm::mat4 matrix )
{
  mProjection = matrix;
}

void LMultiColorPolygonProgram2D::setModelView( glm::mat4 matrix )
{
  mModelView = matrix;
}

void LMultiColorPolygonProgram2D::leftMultProjection( glm::mat4 matrix )
{
  mProjection = matrix * mProjection;
}

void LMultiColorPolygonProgram2D::leftMultModelView( glm::mat4 matrix )
{
  mModelView = matrix * mModelView;
}

void LMultiColorPolygonProgram2D::updateProjection()
{
  glUniformMatrix4fv( mProjectionLocation, 1, GL_FALSE, glm::value_ptr( mProjection ) );
}

void LMultiColorPolygonProgram2D::updateModelView()
{
  glUniformMatrix4fv( mModelViewLocation, 1, GL_FALSE, glm::value_ptr( mModelView ) );
}
