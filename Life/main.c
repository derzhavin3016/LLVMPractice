/*  */

#include "life.h"

Byte Frame[FRAME_H][FRAME_W][3];
int Zoom = 8;
int IsStart = 0;

void PutPixel( int x, int y, Byte r, Byte g, Byte b)
{
  Frame[y][x][0] = b;
  Frame[y][x][1] = g;
  Frame[y][x][2] = r;
}

void Display( void )
{
  glClearColor(0.3, 0.5, 0.7, 1);
  glClear(0);

  glRasterPos2d(-1, 1);
  glPixelZoom(Zoom, -Zoom);

  FieldDraw(Field1);
  if (IsStart)
  {
    NewGeneration(Field1, Field2);
    Swap(Field1, Field2);
  }

  glDrawPixels(FRAME_W, FRAME_H, GL_BGR_EXT, GL_UNSIGNED_BYTE, Frame);

  glFinish();
  glutSwapBuffers();
  glutPostRedisplay();
}

void Keyboard( unsigned char Key, int X, int Y )
{
  switch (Key)
  {
  case 27:
    exit(0);
    break;
  case ' ':
    X /= Zoom;
    Y /= Zoom;
    if (!IsStart && X >= 0 && Y >= 0 && Y < FRAME_H && X < FRAME_W)
      SetCell(Field1, X, Y, 1);
    break;
  case 's':
    IsStart = 1 - IsStart;
    break;
  case 'd':
    X /= Zoom;
    Y /= Zoom;
    if (!IsStart && X >= 0 && Y >= 0 && Y < FRAME_H && X < FRAME_W)
      SetCell(Field1, X, Y, 0);
    break;
  }
}

int main( int argc, char *argv[] )
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
  FieldInit(Field1);

  glutInitWindowPosition(0, 0);
  glutInitWindowSize(800, 800);
  glutCreateWindow("Happy New Year!");

  glutDisplayFunc(Display);
  glutKeyboardFunc(Keyboard);

  glutMainLoop();
}
