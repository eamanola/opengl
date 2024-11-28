#include "LShaderProgram.h"

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
