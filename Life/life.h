/*  */

#include <stdlib.h>
#include <GL/glut.h>

#define LIFE_H 100
#define LIFE_W 100
#define FRAME_H LIFE_H
#define FRAME_W LIFE_W

typedef unsigned char Byte;

extern Byte
         Field1[LIFE_H][LIFE_W],
         Field2[LIFE_H][LIFE_W],
         Frame[FRAME_H][FRAME_W][3];

void PutPixel( int x, int y, Byte r, Byte g, Byte b);

void SetCell( Byte (*F)[LIFE_W], int x, int y, int value );

int GetCell( Byte (*F)[LIFE_W], int x, int y );

void FieldInit( Byte (*F)[LIFE_W] );

int GetNeighbours( Byte (*F)[LIFE_W], int x, int y );

void FieldDraw( Byte (*F)[LIFE_W] );

void NewGeneration( Byte (*F1)[LIFE_W], Byte (*F2)[LIFE_W] );

void Swap( Byte (*F1)[LIFE_W], Byte (*F2)[LIFE_W] );
