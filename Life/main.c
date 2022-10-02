#include "life.h"

Field_t Field1, Field2;
FieldPtr pActiveField = Field1, pPrevField = Field2;
Frame_t Frame;

enum Keys
{
  KEY_ESC = 27,
  KEY_SPACE = ' ',
  KEY_D = 'd',
  KEY_S = 's'
};

int Zoom = 8;
bool IsStart = false;

void PutPixel( size_t x, size_t y, Byte r, Byte g, Byte b)
{
  Frame[y][x][0] = b;
  Frame[y][x][1] = g;
  Frame[y][x][2] = r;
}

void OnDisplay( void )
{
  glClearColor(0.3, 0.5, 0.7, 1);
  glClear(0);

  glRasterPos2d(-1, 1);
  glPixelZoom(Zoom, -Zoom);

  FieldDraw(pActiveField);
  if (IsStart)
  {
    NewGeneration(pActiveField, pPrevField);
    Swap(&pActiveField, &pPrevField);
  }

  glDrawPixels(FRAME_W, FRAME_H, GL_BGR_EXT, GL_UNSIGNED_BYTE, Frame);

  glFinish();
  glutSwapBuffers();
  glutPostRedisplay();
}

bool IsInFrameStop( int x, int y )
{
  return !IsStart && y>= 0 && y < (int)FRAME_H && x >= 0 && x < (int)FRAME_W;
}

void OnKeyboard( unsigned char Key, int x, int y )
{
  assert(Zoom != 0);

  int xu = x / Zoom, yu = y / Zoom;
  switch (Key)
  {
  case KEY_ESC:
    glutDestroyWindow(glutGetWindow());
    break;
  case KEY_SPACE:
    if (IsInFrameStop(xu, yu))
      SetCell(Field1, xu, yu, true);
    break;
  case KEY_S:
    IsStart = !IsStart;
    break;
  case KEY_D:
    if (IsInFrameStop(xu, yu))
      SetCell(Field1, xu, yu, false);
    break;
  default:
    break;
  }
}

int main( int argc, char *argv[] )
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
  FieldInit(Field1);

  glutInitWindowPosition(0, 0);
  glutInitWindowSize(WND_W, WND_H);
  glutCreateWindow("Game of life");

  glutDisplayFunc(OnDisplay);
  glutKeyboardFunc(OnKeyboard);

  glutMainLoop();
}
