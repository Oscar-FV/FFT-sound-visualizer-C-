#include <SFML/Audio.hpp>
#include <valarray>
#include <complex>
#include <iostream>
#include <math.h>

#pragma once

typedef std::complex<double> Complex;
typedef std::valarray<Complex> CArray;


class Song{

  private:

    sf::SoundBuffer buffer;
    sf::Sound sound;

    double maxMag,minMag;
    int sampleRate;
    int sampleCount;
    int bufferSize;
    int syncSong;

    std::vector<Complex> sample;
    std::vector<double> magnitude;

    CArray bin;

  public:

    Song();

    void setSong(std::string FileName, int bufferSize);

    void startSong(int *flag);

    void PerformFFT(CArray &x);//Performs FFT algorithm

    std::vector<double> updateBuffers();//Performs FFT and gets the magnitudes of the samples on the buffers.

    void getMagnitudes(double *min, double *max);//Returns the maximun and minimum magnitudes of the samples on the  buffers.

};
