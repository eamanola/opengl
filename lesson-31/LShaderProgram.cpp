#include "LShaderProgram.h"
#include <fstream>

LShaderProgram::LShaderProgram()
{
  mProgramID = 0;
}

LShaderProgram::~LShaderProgram()
{
  freeProgram();
}

void LShaderProgram::freeProgram()
{
  glDeleteProgram( getProgramID() );
}

bool LShaderProgram::bind()
{
  glUseProgram( getProgramID() );

  GLenum error = glGetError();
  if( error != GL_NO_ERROR )
  {
    printf( "Error binding shader! %s\n", gluErrorString( error ) );
    printProgramLog( getProgramID() );
    return false;
  }

  return true;
}

void LShaderProgram::unbind()
{
  glUseProgram( 0 );
}

GLuint LShaderProgram::getProgramID()
{
  return mProgramID;
}

GLuint LShaderProgram::loadShaderFromFile( std::string path, GLenum shaderType )
{
  GLuint shaderID = 0;
  std::string shaderString;
  std::ifstream sourceFile( path.c_str() );

  if( sourceFile )
  {
    shaderString.assign(
      std::istreambuf_iterator<char>( sourceFile ),
      std::istreambuf_iterator<char>()
    );

    shaderID = glCreateShader( shaderType );

    const GLchar* shaderSource = shaderString.c_str();
    if( !compileShader( shaderID, &shaderSource ) )
    {
      printf( "Unable to compile shader %d!\n\nSource:\n%s\n", shaderID, shaderSource );
      printShaderLog( shaderID );
      glDeleteShader( shaderID );
      shaderID = 0;
    }
  }
  else
  {
    printf( "Unable to open file %s\n", path.c_str() );
  }

  return shaderID;
}

bool LShaderProgram::compileShader(GLuint shader, const GLchar** source)
{
  glShaderSource( shader, 1, source, NULL );

  glCompileShader( shader );

  GLint shaderCompiled = GL_FALSE;
  glGetShaderiv( shader, GL_COMPILE_STATUS, &shaderCompiled );

  return shaderCompiled == GL_TRUE;
}

void LShaderProgram::printProgramLog( GLuint program )
{
  if( glIsProgram( program ) )
  {
    int infoLogLength = 0;
    int maxLength = infoLogLength;

    glGetProgramiv( program, GL_INFO_LOG_LENGTH, &maxLength );

    char* infoLog = new char[ maxLength ];

    glGetProgramInfoLog( program, maxLength, &infoLogLength, infoLog );
    if( infoLogLength > 0 )
    {
      printf( "%s", infoLog );
    }

    delete[] infoLog;
  }
  else {
    printf( "Name %d is not a program\n", program );
  }
}

void LShaderProgram::printShaderLog( GLuint shader )
{
  if( glIsShader( shader ) )
  {
    int infoLogLength = 0;
    int maxLength = infoLogLength;

    glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &maxLength );

    char* infoLog = new char[ maxLength ];

    glGetShaderInfoLog( shader, maxLength, &infoLogLength, infoLog );
    if( infoLogLength > 0 )
    {
      printf( "%s", infoLog );
    }

    delete[] infoLog;
  }
  else {
    printf( "Name %d is not a shader\n", shader );
  }
}
