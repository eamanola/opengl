#include "../LOpenGL.h"
#include <string>

class LShaderProgram
{
  public:
    LShaderProgram();
    ~LShaderProgram();

    virtual bool loadProgram() = 0;

    virtual void freeProgram();

    bool bind();

    void unbind();

    GLuint getProgramID();

    GLuint loadShaderFromFile( std::string path, GLenum shaderType );

  protected:
    bool compileShader(GLuint shader, const GLchar** source);

    void printProgramLog( GLuint program );

    void printShaderLog( GLuint shader );

    GLuint mProgramID;
};
