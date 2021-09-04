#include <GL/glut.h>
#include <iostream>
#include <math.h>
#include <list>
#include <iterator>
#include <stdlib.h>
#include "Resources/Song.h"
#include "Resources/Cube.h"

//Song variables
Song PlayingSong;
int songFlag = 0;
double maxMag, minMag;
std::vector<double> magnitude;
//screen variables
int windowWidth = 50;
int windowHeight = 50;
float maxDistance = 0;

int refreshmills = 1000 / 60; //Refresh time of the screen(60 fps)

float toRadians = M_PI / 180; //Helps to transform the degrees to radians

//Cube variables
std :: list <Cube> CubeSet;
std :: list <Cube> :: iterator itCube;
float offset = 0;
float angle = 0;
float a = 0;
float width  = 2.0, height = 0;
int z = 0, x = 0, y = -20;
float dist;

//Maps a value between  maximum a minimum and the maximum and minimun values to map
float mapValues(float value, float max, float min, float mapMax, float mapMin){
  return (mapMax  + (mapMin - mapMax) * ((value - max) / (min - max)));
}

//Calculates the distance between two points
float distance(float x1, float y1, float x2, float y2){
  return sqrt( pow(x2-x1, 2) + pow(y2-y1, 2) );
}

//Display function
void display (void){

   //Clear all pixels
   glClear (GL_COLOR_BUFFER_BIT);

   //The cube is drawn following the painters algorithm (fardest to nearest)
   itCube = CubeSet.end();
   for(int i = CubeSet.size() + 1; i > 0; i--){

     itCube -> drawCube();
     itCube--;

   }
   glutSwapBuffers();

}

void Timer(int){

  CubeSet.clear();//Clears the list of cube each time the calculation is done.

  double maxHeight, minHeight, halfDistance;

  if(songFlag == 1){ //Starts the movement calculation only if the song is playing

      int j = 0, i = 0;
      int currentSet = 0, direction, HeightOption;

      magnitude = PlayingSong.updateBuffers(); //Calculates the magnitudes of the current samples that are being reproduced

      PlayingSong.getMagnitudes(&minMag, &maxMag);


      maxHeight = maxDistance/2; //Highest and lowest point that the cubes can reach
      minHeight = maxHeight/2;

      halfDistance =  2 + (maxDistance/3  - 2);

      for(int z = 0; z < windowWidth; z += width){ //starts filling the cubes list

        //-----------------//Helps to spread the magnitude array along all the cube
    		if(i % 2 == 0){

    			direction = 1;

    		}else{

    			direction = 0;

    		}

        j = i*24;
        currentSet = (i*24) + 24;
        //-------------//
        for (int x = 0;  x < windowWidth; x += width){

          dist = distance(x, z, windowWidth/2, windowHeight/2); //Calculates the distance form the current x,z  to the half of the screen
          offset = mapValues(dist, maxDistance, 0, 3, -3);
    			a =  offset + angle;

          height = mapValues(sin(a), 1 , -1, maxDistance/4 , 2);


            if(height < halfDistance - 4){ //Helps to keep the harmonic motion produced by the sine waves

              HeightOption = 1;

            }else{

              HeightOption = 0;

            }

            if(HeightOption == 1){

              switch(direction){

                case 1:

                   if(magnitude.at(currentSet) > 0 ) height = mapValues(magnitude.at(currentSet), maxMag , minMag, maxHeight, minHeight );
                   currentSet --;

                   break;

                 case 0:

                   if(magnitude.at(j) > 0) height = mapValues(magnitude.at(j), maxMag, minMag, maxHeight, minHeight);
                   j ++;

                   break;
               }

                if(height < minHeight + 0.35){ //Helps to "clean" the whole cube taking out the magnitudes that are irrelevant.

                  height = mapValues(sin(a), 1 , -1, maxDistance/4 , 2);

                }

              }

          Cube myCube (width, height + 2);

          //Makes the rotations and transalation needed to the get the cube in place//
          myCube.createCube();
          myCube.Translate(x, 0, -z);
          float cubeX = myCube.getX(), cubeZ = myCube.getZ();
          myCube.Translate(-(cubeX + width)/2, 0, (cubeZ + width)/2 );
          myCube.RotateY(-45 * toRadians);
          myCube.RotateX(45 * toRadians);
          myCube.Translate((cubeX + width)/2, 0, -(cubeZ + width)/2 );
          myCube.Translate(-40, -13, -(width * 25));
          CubeSet.push_back(myCube);

       }

    	 i++;

     }
       angle -= 0.05;//Decrement of the angle, it can be chanched to a increment and the motion will be a little different
  }

  glutPostRedisplay();

  PlayingSong.startSong(&songFlag);//Starts the song. The flag is changed to 1 after the method call.

  glutTimerFunc(refreshmills, Timer, 0);

}

void init (void)
{
   // Select clearing (background) color
   glClearColor (0.1, 0.1, 0.1, 0.1);

   // Initialize viewing values
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glFrustum(-8, 8, -8, 8, 8, 90);//Sets the perspective

   //Sets the song that is going to be played
	 std :: string FileName;
	 std :: cout << "Write the name of the file + .wav \n" << "(this program only works with .wav files) \n";
	 std :: cout << "File Name:";
	 std :: cin  >> FileName;

    int bufferSize = 8192;
   PlayingSong.setSong(FileName , bufferSize);

   //Size of the arrays of samples
	 magnitude.resize(bufferSize/2);

   maxDistance = distance(0, 0, windowWidth/2, windowHeight/2);
}

int main(int argc, char** argv){

  glutInit(&argc, argv);
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize (1024, 768);
  glutInitWindowPosition (2500, 100);
  glutCreateWindow (":: C U B E__W A V E::");


  init ();

  glutTimerFunc(0, Timer, 0);
  glutDisplayFunc(display);
  glutMainLoop();
  return 0;

}
