#include <GL/glut.h>
#include <math.h>
#include <iostream>

#pragma once

// Constants
#define VERT_PER_FACE 4
#define N_VERTICES    8
#define N_FACES       6

// Static data structures
// Structure used to store the information of a vertex
typedef struct {
   float x, y, z;
} TVertex;

// Structure used to store the information of a face
typedef struct {
   int vert[VERT_PER_FACE];
} TFace;

// Structure used to store the information of a cube
typedef struct {
   TVertex vertices[N_VERTICES];
   TFace faces[N_FACES];
} TFig;


class Cube{

private:

  float width;
  float height;
  float x = -25;
  float z = -25;
  float y = 0;
  TFig cube;
  TVertex NormalVectors[N_FACES];
  TVertex prp;
  int VisibleSurfaces[N_FACES];
  float colorR;
  float colorG;
  float colorB;

public:

  Cube();

  Cube(float width, float height);

  void createCube ();

  void drawCube();

  float getX();

  float getZ();

  float getY();

  void calculateNormalVectors();

  void calculateVisibleSurface();

  void calculateIllumination(int index);

  void Translate(float dx, float dy, float dz);

  void RotateX(float angle);

  void RotateY(float angle);

  void RotateZ(float angle);

};
