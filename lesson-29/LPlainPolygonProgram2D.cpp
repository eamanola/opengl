#include "LPlainPolygonProgram2D.h"

bool compileShader(GLuint shader, const GLchar** source)
{
  glShaderSource( shader, 1, source, NULL );

  glCompileShader( shader );

  GLint shaderCompiled = GL_FALSE;
  glGetShaderiv( shader, GL_COMPILE_STATUS, &shaderCompiled );
  return shaderCompiled == GL_TRUE;
}

bool LPlainPolygonProgram2D::loadProgram()
{
  GLint programSuccess = GL_TRUE;

  mProgramID = glCreateProgram();

  GLuint vertexShader = glCreateShader( GL_VERTEX_SHADER );

  const GLchar* vertexShaderSource[] =
  {
    "void main() { gl_Position = gl_Vertex; }"
  };

  if( !compileShader(vertexShader, vertexShaderSource) )
  {
    printf( "Unable to compile vertex shader %d!\n", vertexShader );
    printShaderLog( vertexShader );
    return false;
  }
  glAttachShader( mProgramID, vertexShader );

  GLuint fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );

  const GLchar* fragmentShaderSource[] =
  {
    "void main() { gl_FragColor = vec4( 1.0, 0.0, 0.0, 1.0 ); }"
  };

  if( !compileShader(fragmentShader, fragmentShaderSource) )
  {
    printf( "Unable to compile fragment shader %d!\n", fragmentShader );
    printShaderLog( fragmentShader );
    return false;
  }
  glAttachShader( mProgramID, fragmentShader );

  glLinkProgram( mProgramID );

  glGetProgramiv( mProgramID, GL_LINK_STATUS, &programSuccess );
  if( programSuccess != GL_TRUE )
  {
    printf( "Error linking program %d!\n", mProgramID );
    return false;
  }

  return true;
}
