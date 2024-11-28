#include "LPlainPolygonProgram2D.h"
#include <glm/gtc/type_ptr.hpp>

LPlainPolygonProgram2D::LPlainPolygonProgram2D()
{
  mPolygonColorLocation = 0;
  mProjectionMatrixLocation = 0;
  mModelViewMatrixLocation = 0;
}

bool LPlainPolygonProgram2D::loadProgram()
{
  mProgramID = glCreateProgram();

  GLuint vertexShader = loadShaderFromFile(
    "./shaders/LPlainPolygonProgram2D.glvs",
    GL_VERTEX_SHADER
  );

  if( vertexShader == 0 )
  {
    glDeleteProgram( mProgramID );
    mProgramID = 0;
    return false;
  }
  glAttachShader( mProgramID, vertexShader );

  GLuint fragmentShader = loadShaderFromFile(
    "./shaders/LPlainPolygonProgram2D.glfs",
    GL_FRAGMENT_SHADER
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

  const GLchar* LPOLYGON_COLOR = "LPolygonColor";
  mPolygonColorLocation = glGetUniformLocation( mProgramID, LPOLYGON_COLOR );
  if( mPolygonColorLocation == -1 )
  {
    printf( "%s is not a valid glsl program variable!\n", LPOLYGON_COLOR );
  }

  const GLchar* LPROJECTION_MATRIX = "LProjectionMatrix";
  mProjectionMatrixLocation = glGetUniformLocation( mProgramID, LPROJECTION_MATRIX );
  if( mProjectionMatrixLocation == -1 )
  {
    printf( "%s is not a valid glsl program variable!\n", LPROJECTION_MATRIX );
  }

  const GLchar* LMODELVIEW_MATRIX = "LModelViewMatrix";
  mModelViewMatrixLocation = glGetUniformLocation( mProgramID, LMODELVIEW_MATRIX );
  if( mModelViewMatrixLocation == -1 )
  {
    printf( "%s is not a valid glsl program variable!\n", LMODELVIEW_MATRIX );
  }

  return true;
}

void LPlainPolygonProgram2D::setColor( GLfloat r, GLfloat g, GLfloat b, GLfloat a )
{
  glUniform4f( mPolygonColorLocation, r, g, b, a );
}

void LPlainPolygonProgram2D::setProjection( glm::mat4 matrix )
{
  mProjectionMatrix = matrix;
}

void LPlainPolygonProgram2D::setModelView( glm::mat4 matrix )
{
  mModelViewMatrix = matrix;
}

void LPlainPolygonProgram2D::leftMultProjection( glm::mat4 matrix )
{
  mProjectionMatrix = matrix * mProjectionMatrix;
}

void LPlainPolygonProgram2D::leftMultModelView( glm::mat4 matrix )
{
  mModelViewMatrix = matrix * mModelViewMatrix;
}

void LPlainPolygonProgram2D::updateProjction()
{
  glUniformMatrix4fv( mProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr( mProjectionMatrix ) );
}

void LPlainPolygonProgram2D::updateModelView()
{
  glUniformMatrix4fv( mModelViewMatrixLocation, 1, GL_FALSE, glm::value_ptr( mModelViewMatrix ) );
}
