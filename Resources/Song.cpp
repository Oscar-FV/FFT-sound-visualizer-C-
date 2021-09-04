#include "Song.h"

Song::Song(){}

//sets the information and variables used for the song.
void Song::setSong(std::string FileName, int bufferSize){

  if(buffer.loadFromFile("Songs/" + FileName)){

    sampleRate = buffer.getSampleRate();


    sound.setBuffer(buffer);
    sound.setVolume(50);
    syncSong = 0;

    this->bufferSize = bufferSize;
    sample.resize(bufferSize);
    magnitude.resize(bufferSize/2);

  }else{

    std :: exit(0);

  }

}

//starts the song, and turns the flag to 1.
void Song::startSong(int *flag){
    if(*flag == 0){
      sound.play();
      *flag = 1;
    }
}

//FFT to get the magnitudes of the samples
//
void Song::PerformFFT(CArray &x){

	const int N = x.size();
	if(N <= 1) return;

	CArray even = x[std::slice(0,N/2,2)];
	CArray  odd = x[std::slice(1,N/2,2)];

	PerformFFT(even);
	PerformFFT(odd);

	for(int k = 0 ; k < N/2 ; k++)
	{
		Complex t = std::polar(1.0,-2 * M_PI * k / N) * odd[k];
		x[k] = even[k] + t;
		x[k+N/2] = even[k] - t;
	}
}

//Updates the buffers to get the current samples that are played
std::vector<double> Song::updateBuffers(){

  syncSong = sound.getPlayingOffset().asSeconds()*sampleRate;//gets the current samples the song is playing

  if(syncSong < buffer.getSampleCount()){
    for(int i = syncSong; i < bufferSize+syncSong ; i++){
      sample[i-syncSong] = Complex(buffer.getSamples()[i], 0);
    }
 }

   bin = CArray(sample.data(),bufferSize);

   PerformFFT(bin);

   magnitude.clear();

   double re, im;

   for(int i = 0; i < bufferSize/2; i++){

     re = real(bin[i]);
     im = imag(bin[i]);
     magnitude.push_back(sqrt(re*re+im*im));

   }

   return(magnitude);

}

//returns the highest and lowest magnitud of the samples.
void Song::getMagnitudes(double *min, double *max){//finds the biggest magnitude of the samples at the current buffers

  maxMag = magnitude.at(0);
  minMag = magnitude.at(0);
  int index = 0;

  for(int i = 1; i < bufferSize/2; i++){

    if(magnitude.at(i) > maxMag){maxMag = magnitude.at(i); index = i;};
    if(magnitude.at(i) < minMag) minMag = magnitude.at(i);

  }

  *max = maxMag;
  *min = minMag;

}
