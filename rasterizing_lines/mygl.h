#ifndef _MYGL_H_
#define _MYGL_H_

#include "definitions.h"

struct Point {
  uint x;
  uint y;
};

struct Color {
  int RGBA[4];
};

//-----------------------------------------------------------------------------
void MyGlDraw(void);

//*****************************************************************************
// Defina aqui as suas funções gráficas
//*****************************************************************************

void PutPixel(Point, Color);
void DrawLine(Point, Point, Color);
void DrawTriangle(Point, Point, Point, Color);

void DrawPoint();
void DrawLines();
void DrawTriangleRectangle();

#endif