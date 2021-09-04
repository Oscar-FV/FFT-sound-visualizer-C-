#include "Cube.h"

  Cube :: Cube(){}

  Cube :: Cube(float width, float height){

    this -> width = width;
    this -> height = height;

    this -> prp.x = 0; this -> prp.y = 0; this -> prp.z = -8;

  }

  void Cube::createCube (){ //Store the cube's vertices into the structure

     cube.vertices[0].x = x; cube.vertices[0].y = y + height; cube.vertices[0].z = z + width;
     cube.vertices[1].x = x; cube.vertices[1].y = y + height; cube.vertices[1].z = z;
     cube.vertices[2].x = x + width; cube.vertices[2].y = y + height; cube.vertices[2].z = z;
     cube.vertices[3].x = x + width; cube.vertices[3].y = y + height; cube.vertices[3].z = z + width;
     cube.vertices[4].x = x; cube.vertices[4].y = y - height; cube.vertices[4].z = z + width;
     cube.vertices[5].x = x; cube.vertices[5].y = y - height; cube.vertices[5].z = z;
     cube.vertices[6].x = x + width; cube.vertices[6].y = y - height; cube.vertices[6].z = z;
     cube.vertices[7].x = x + width; cube.vertices[7].y = y - height; cube.vertices[7].z = z + width;


     cube.faces[0].vert[0] = 0; cube.faces[0].vert[1] = 1; cube.faces[0].vert[2] = 2; cube.faces[0].vert[3] = 3;
     cube.faces[1].vert[0] = 4; cube.faces[1].vert[1] = 7; cube.faces[1].vert[2] = 6; cube.faces[1].vert[3] = 5;
     cube.faces[2].vert[0] = 0; cube.faces[2].vert[1] = 4; cube.faces[2].vert[2] = 5; cube.faces[2].vert[3] = 1;
     cube.faces[3].vert[0] = 1; cube.faces[3].vert[1] = 5; cube.faces[3].vert[2] = 6; cube.faces[3].vert[3] = 2;
     cube.faces[4].vert[0] = 2; cube.faces[4].vert[1] = 6; cube.faces[4].vert[2] = 7; cube.faces[4].vert[3] = 3;
     cube.faces[5].vert[0] = 4; cube.faces[5].vert[1] = 0; cube.faces[5].vert[2] = 3; cube.faces[5].vert[3] = 7;

  }


   void Cube :: drawCube(){ //Displays the cube

     int f,v;

     calculateNormalVectors();
     calculateVisibleSurface();

     for(f = 0; f < N_FACES; f++){


     if(VisibleSurfaces[f] == 1){

         calculateIllumination(f);
         glColor3f(0.5*colorR, 0.5*colorG, 0.5*colorB);


       glBegin(GL_POLYGON);

         for(v = 0; v < VERT_PER_FACE; v++){

           glVertex3f(cube.vertices[cube.faces[f].vert[v]].x,
                      cube.vertices[cube.faces[f].vert[v]].y,
                      cube.vertices[cube.faces[f].vert[v]].z);

         }
         glEnd();

       }

     }

   }

  float Cube :: getX(){

    return this -> x;

  }

  float Cube :: getZ(){

    return this -> z;

  }

  float Cube :: getY(){

    return this -> y;

  }

  //Calculates the noramal vector of each face using the cross product
  void Cube :: calculateNormalVectors(){

    TVertex  Cross1, Cross2;

    for(int i = 0; i < N_FACES; i++){

      Cross1.x = cube.vertices[cube.faces[i].vert[1]].x - cube.vertices[cube.faces[i].vert[0]].x;
      Cross1.y = cube.vertices[cube.faces[i].vert[1]].y - cube.vertices[cube.faces[i].vert[0]].y;
      Cross1.z = cube.vertices[cube.faces[i].vert[1]].z - cube.vertices[cube.faces[i].vert[0]].z;

      Cross2.x = cube.vertices[cube.faces[i].vert[2]].x - cube.vertices[cube.faces[i].vert[0]].x;
      Cross2.y = cube.vertices[cube.faces[i].vert[2]].y - cube.vertices[cube.faces[i].vert[0]].y;
      Cross2.z = cube.vertices[cube.faces[i].vert[2]].z - cube.vertices[cube.faces[i].vert[0]].z;

      NormalVectors[i].x = (Cross1.y * Cross2.z) - (Cross1.z * Cross2.y);
      NormalVectors[i].y = (Cross1.x * Cross2.z) - (Cross1.z * Cross2.x);
      NormalVectors[i].z = (Cross1.x * Cross2.y) - (Cross1.y * Cross2.x);

    }

  }

  //Calculates the surfaces that are going to ve visible from the PRP
  void Cube :: calculateVisibleSurface(){

    double dotProduct;

    for(int i = 0; i < N_FACES; i++){

      dotProduct = (NormalVectors[i].x * prp.x) + (-NormalVectors[i].y * prp.y) + ((NormalVectors[i].z *  prp.z));

      if(dotProduct >= 0) VisibleSurfaces[i] = 1; //If the dot product is higher or equal to 0 it means that its a visible surface.
      else VisibleSurfaces[i] = -1;

    }

  }

  void Cube :: calculateIllumination(int index){

    double magnitudeNormals, magnitudeLights, magnitudePRP;
    magnitudeNormals = sqrt(pow(NormalVectors[index].x, 2) + pow(-NormalVectors[index].y, 2) + pow(NormalVectors[index].z, 2));
    magnitudeLights = sqrt(pow(10, 2) + pow(10, 2) + pow(0, 2));
    magnitudePRP = sqrt(pow(prp.x, 2) + pow(prp.y, 2) + pow(prp.z, 2));

    double newNormalX = NormalVectors[index].x / magnitudeNormals, newNormalY = -NormalVectors[index].y / magnitudeNormals, newNormalZ = NormalVectors[index].z / magnitudeNormals;
    double newLightX = -10 / magnitudeLights, newLightY = -10 / magnitudeLights, newLightZ = 0/ magnitudeLights;
    double newPRPX = prp.x / magnitudePRP, newPRPY = prp.y / magnitudePRP, newPRPZ = prp.z / magnitudePRP;

    //Ambient light calculation
    float ambientR = 0, ambientG = 0.1, ambientB = 0.1;
    float Ka = 0.3;
    ambientR *= Ka; ambientG *= Ka; ambientB *= Ka;

    //Diffuse ligth calculation
    float diffuseR = 0, diffuseG = 1, diffuseB = 1;
    float Kd = 0.8, Id, nl;
    diffuseR *= Kd; diffuseG *= Kd; diffuseB *= Kd;
    nl = ((newNormalX * newLightX) + (newNormalY * newLightY) + (newNormalZ * newLightZ));
    diffuseR *= nl; diffuseG *= nl; diffuseB *= nl;

    //Specular ligth calculation
    float specularR = 1.0, specularG = 1.0, specularB = 1.0;
    float Ks = 0.4, n = 3, rv;
    float rx = (2 * nl * newNormalX) - newLightX;
    float ry = (2 * nl * newNormalY) - newLightY;
    float rz = (2 * nl * newNormalZ) - newLightZ;

    rv = pow((rx * newPRPX) + (ry * newPRPY) + (rz * newPRPZ), n);
    if(rv < 0) rv = 0;

    specularR *= rv; specularG *= rv; specularB *= rv;

    //Sum of each light calculated
    colorR = ambientR + diffuseR + specularR;
    colorG = ambientG + diffuseG + specularG;
    colorB = ambientB + diffuseB + specularB;

  }

   void Cube :: Translate(float dx, float dy, float dz) { //Moves the figure in x, y or z

     for(int i = 0; i < N_VERTICES; i++){

         cube.vertices[i].x = cube.vertices[i].x + dx; cube.vertices[i].y = cube.vertices[i].y + dy; cube.vertices[i].z = cube.vertices[i].z + dz;

     }

   }

//Rotates in x, y or z axis.
    void Cube :: RotateX(float angle){

      float newY, newZ;


        for(int i = 0; i < N_VERTICES; i++){

            newY = (cos(angle) * cube.vertices[i].y) + (-sin(angle) * cube.vertices[i].z);
            newZ = (sin(angle) * cube.vertices[i].y) + (cos(angle) * cube.vertices[i].z);

            cube.vertices[i].y = newY;
            cube.vertices[i].z = newZ;

        }


    }

    void Cube :: RotateY(float angle){

      float newX, newZ;

      for(int i = 0; i < N_VERTICES; i++){

          newX = (cos(angle) * cube.vertices[i].x) + (sin(angle) * cube.vertices[i].z);
          newZ = (-sin(angle) * cube.vertices[i].x) + (cos(angle) * cube.vertices[i].z);

          cube.vertices[i].x = newX;
          cube.vertices[i].z = newZ;

      }

    }

    void Cube :: RotateZ(float angle){

      float newX, newY;

      for(int i = 0; i < N_VERTICES; i++){

          newX = (cos(angle) * cube.vertices[i].x) + (-sin(angle) * cube.vertices[i].y);
          newY = (sin(angle) * cube.vertices[i].x) + (cos(angle) * cube.vertices[i].y);

          cube.vertices[i].x = newX;
          cube.vertices[i].y = newY;

      }

    }
