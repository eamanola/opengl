#include "../LOpenGL.h"

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

  protected:
    void printProgramLog( GLuint program );

    void printShaderLog( GLuint shader );

    GLuint mProgramID;
};
