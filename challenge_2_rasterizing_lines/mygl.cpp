#include "definitions.h"
#include <cmath>
#include <iostream>
#include <stdio.h>
#include "mygl.h"

using namespace std;

Point line[2];
Point triangle[3];

struct Line {
  Point point_i;
  Point point_f;
  Color color;
};

//-----------------------------------------------------------------------------
void MyGlDraw(void) {
  //*************************************************************************
  // Chame aqui as funções do mygl.h
  //*************************************************************************

  // DrawPoint();
  // DrawLines();
  // DrawTriangleRectangle();
  
}

void DrawTriangleRectangle() {
  Color color;
  color.RGBA[0] = 0;
  color.RGBA[1] = 255;
  color.RGBA[2] = 0;
  color.RGBA[3] = 255;

  // DrawTriangle
  triangle[0].x = 256;
  triangle[0].y = 0;
  triangle[1].x = 0;
  triangle[1].y = 256;
  triangle[2].x = 256;
  triangle[2].y = 256;
  DrawTriangle(triangle[0], triangle[1], triangle[2], color);
}

void DrawPoint() {
  Point point;
  point.x = 256;
  point.y = 256;

  Color color;
  color.RGBA[0] = 0;
  color.RGBA[1] = 255;
  color.RGBA[2] = 0;
  color.RGBA[3] = 255;

  PutPixel(point, color);
}

void DrawLines() {
  Color color;
  color.RGBA[0] = 0;
  color.RGBA[1] = 255;
  color.RGBA[2] = 0;
  color.RGBA[3] = 255;

  line[0].x = 0;
  line[0].y = 0;
  line[1].x = IMAGE_WIDTH;
  line[1].y = IMAGE_WIDTH;
  DrawLine(line[0], line[1], color);

  line[0].x = 0;
  line[0].y = IMAGE_HEIGHT / 2;
  line[1].x = IMAGE_WIDTH;
  line[1].y = IMAGE_HEIGHT / 2;
  DrawLine(line[0], line[1], color);

  line[0].x = IMAGE_WIDTH / 2;
  line[0].y = 0;
  line[1].x = IMAGE_WIDTH / 2;
  line[1].y = IMAGE_HEIGHT;
  DrawLine(line[0], line[1], color);

  line[0].x = 120;
  line[0].y = 0;
  line[1].x = IMAGE_WIDTH - 120;
  line[1].y = IMAGE_HEIGHT;
  DrawLine(line[0], line[1], color);

  line[0].x = 0;
  line[0].y = 120;
  line[1].x = IMAGE_WIDTH;
  line[1].y = IMAGE_HEIGHT - 120;
  DrawLine(line[0], line[1], color);

  line[0].x = IMAGE_WIDTH;
  line[0].y = 0;
  line[1].x = 0;
  line[1].y = IMAGE_WIDTH;
  DrawLine(line[0], line[1], color);

  line[0].x = IMAGE_WIDTH - 120;
  line[0].y = 0;
  line[1].x = 120;
  line[1].y = IMAGE_WIDTH;
  DrawLine(line[0], line[1], color);

  line[0].x = IMAGE_WIDTH;
  line[0].y = 120;
  line[1].x = 0;
  line[1].y = IMAGE_WIDTH - 120;
  DrawLine(line[0], line[1], color);
}

// y = heigh
// x = width

// initial point (x, y)(512, 0)
// final point (x, y)(0, 512)

void PutPixel(Point p, Color c) {
  FBptr[4 * p.x + 4 * p.y * IMAGE_WIDTH + 0] = c.RGBA[0];
  FBptr[4 * p.x + 4 * p.y * IMAGE_WIDTH + 1] = c.RGBA[1];
  FBptr[4 * p.x + 4 * p.y * IMAGE_WIDTH + 2] = c.RGBA[2];
  FBptr[4 * p.x + 4 * p.y * IMAGE_WIDTH + 3] = c.RGBA[3];
}

int custom_abs(int value) {
  return (value < 0) ? -value : value;
}

void DrawLine(Point point_i, Point point_f, Color color) {

  Point print_p;

  int dy;
  int dx;
  int e;
  float m;
  // // Point aux;

  // dx = point_f.x - point_i.x;
  // dy = point_f.y - point_i.y;

  // print_p.x = point_i.x;
  // print_p.y = point_i.y;
  // e = 0;
  // // m = dx / dy;

  // while (print_p.x <= point_f.x) {
  //   PutPixel(print_p, color);
  //   print_p.x += 1;
  //   e = e + 2 * dy;
  //   if (e >= dx) {
  //     print_p.y += 1;
  //     e = e - 2 * dx;
  //   }
  // }

  dx = custom_abs(point_f.x - point_i.x);
  dy = custom_abs(point_f.y - point_i.y);
  int sx = (point_i.x < point_f.x) ? 1 : -1;
  int sy = (point_i.y < point_f.y) ? 1 : -1;
  int err = dx - dy;

  while (1) {
    if (point_i.x == point_f.x && point_i.y == point_f.y) {
      break;
    }
    PutPixel(point_i, color);

    int e2 = 2 * err;
    if (e2 > -dy) {
      err -= dy;
      point_i.x += sx;
    }
    if (e2 < dx) {
      err += dx;
      point_i.y += sy;
    }
  }
}

void DrawTriangle(Point point_1, Point point_2, Point point_3, Color color) {
  DrawLine(point_1, point_2, color);
  DrawLine(point_2, point_3, color);
  DrawLine(point_3, point_1, color);
}