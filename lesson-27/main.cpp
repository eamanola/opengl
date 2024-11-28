#include "LUtil.h"

void runMainLoop( int val );

int main( int argc, char* args[] )
{
  glutInit( &argc, args );

  glutInitContextVersion( 2, 1 );

  glutInitDisplayMode( GLUT_DOUBLE | GLUT_STENCIL );
  glutInitWindowSize( SCREEN_WIDTH, SCREEN_HEIGHT );
  glutCreateWindow( "OpenGL" );

  if ( !initGL() )
  {
    printf( "Unable to initialize graphics library!" );
    return 1;
  }

  if( !loadMedia() )
  {
    printf( "Unable to load media!\n" );
    return 2;
  }

  glutDisplayFunc( render );

  glutKeyboardFunc( handleKeys );

  glutMotionFunc( handleMouseMotion );

  glutTimerFunc( 1000 / SCREEN_FPS, runMainLoop, 0 );

  glutMainLoop();

  return 0;
}

void runMainLoop( int val )
{
  update();
  render();

  glutTimerFunc( 1000 / SCREEN_FPS, runMainLoop, val );
}
