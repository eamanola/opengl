#include "LTexturedPolygonProgram2D.h"
#include <glm/gtc/type_ptr.hpp>

LTexturedPolygonProgram2D::LTexturedPolygonProgram2D()
{
  mVertexPos2DLocation = 0;
  mTexCoordLocation = 0;
  mTexColorLocation = 0;
  mTexUnitLocation = 0;

  mProjectionLocation = 0;
  mModelViewLocation = 0;
}

bool LTexturedPolygonProgram2D::loadProgram()
{
  mProgramID = glCreateProgram();

  GLuint vertexShader = loadShaderFromFile(
    "./shaders/LTexturedPolygonProgram2D.glvs",
    GL_VERTEX_SHADER
  );
  if( vertexShader == 0)
  {
    glDeleteProgram( mProgramID );
    mProgramID = 0;
    return false;
  }
  glAttachShader( mProgramID, vertexShader );

  GLuint fragmentShader = loadShaderFromFile(
    "./shaders/LTexturedPolygonProgram2D.glfs",
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

  GLint linkSuccess = GL_TRUE;
  glGetProgramiv( mProgramID, GL_LINK_STATUS, &linkSuccess );
  if( linkSuccess != GL_TRUE )
  {
    printf( "Error linking program %d\n", mProgramID );
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
  if( mVertexPos2DLocation == -1 ){
    printf( "%s not variable", LVERTEX_POS_2D );
  }

  const GLchar* LTEX_COORD = "LTexCoord";
  mTexCoordLocation = glGetAttribLocation( mProgramID, LTEX_COORD );
  if( mTexCoordLocation == -1 ){
    printf( "%s not variable", LTEX_COORD );
  }

  const GLchar* LTEX_COLOR = "LTextureColor";
  mTexColorLocation = glGetUniformLocation( mProgramID, LTEX_COLOR );
  if( mTexColorLocation == -1 ){
    printf( "%s not variable", LTEX_COLOR );
  }

  const GLchar* LTEX_UNIT = "LTextureUnit";
  mTexUnitLocation = glGetUniformLocation( mProgramID, LTEX_UNIT );
  if( mTexUnitLocation == -1 ){
    printf( "%s not variable", LTEX_UNIT );
  }

  const GLchar* LPROJECTION = "LProjectionMatrix";
  mProjectionLocation = glGetUniformLocation( mProgramID, LPROJECTION );
  if( mProjectionLocation == -1 ){
    printf( "%s not variable", LPROJECTION );
  }

  const GLchar* LMODEL_VIEW = "LModelViewMatrix";
  mModelViewLocation = glGetUniformLocation( mProgramID, LMODEL_VIEW );
  if( mModelViewLocation == -1 ){
    printf( "%s not variable", LMODEL_VIEW );
  }

  return true;
}

void LTexturedPolygonProgram2D::setVertexPointer( GLsizei stride, const GLvoid* data )
{
  glVertexAttribPointer( mVertexPos2DLocation, 2, GL_FLOAT, GL_FALSE, stride, data );
}

void LTexturedPolygonProgram2D::setTexCoordPointer( GLsizei stride, const GLvoid* data )
{
  glVertexAttribPointer( mTexCoordLocation, 2, GL_FLOAT, GL_FALSE, stride, data );
}

void LTexturedPolygonProgram2D::enableVertexPointer()
{
  glEnableVertexAttribArray( mVertexPos2DLocation );
}

void LTexturedPolygonProgram2D::enableTexCoordPointer()
{
  glEnableVertexAttribArray( mTexCoordLocation );
}

void LTexturedPolygonProgram2D::disableVertexPointer()
{
  glDisableVertexAttribArray( mVertexPos2DLocation );
}

void LTexturedPolygonProgram2D::disableTexCoordPointer()
{
  glDisableVertexAttribArray( mTexCoordLocation );
}

void LTexturedPolygonProgram2D::setProjection( glm::mat4 matrix )
{
  mProjection = matrix;
}

void LTexturedPolygonProgram2D::setModelView( glm::mat4 matrix )
{
  mModelView = matrix;
}

void LTexturedPolygonProgram2D::leftMultProjection( glm::mat4 matrix )
{
  mProjection = matrix * mProjection;
}

void LTexturedPolygonProgram2D::leftMultModelView( glm::mat4 matrix )
{
  mModelView = matrix * mModelView;
}

void LTexturedPolygonProgram2D::updateProjection()
{
  glUniformMatrix4fv( mProjectionLocation, 1, GL_FALSE, glm::value_ptr( mProjection ) );
}

void LTexturedPolygonProgram2D::updateModelView()
{
  glUniformMatrix4fv( mModelViewLocation, 1, GL_FALSE, glm::value_ptr( mModelView ) );
}

void LTexturedPolygonProgram2D::setTextureColor( LColorRGBA color )
{
  glUniform4fv( mTexColorLocation, 4, (const GLfloat*)&color );
}

void LTexturedPolygonProgram2D::setTextureUnit( GLuint unit )
{
  glUniform1i( mTexUnitLocation, unit );
}
