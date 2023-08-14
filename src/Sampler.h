#ifndef Sampler_h
#define Sampler_h

#include <Arduino.h>
#include <Audio.h>
#include <SD.h>
#include <SPI.h>

class Sampler {
public:
  Sampler(AudioPlaySdWav &playWav);;
  void setup();
  void openRandomFileRecursively(File dir);
  String playRandomFile();

private:
  AudioPlaySdWav &playWav;
  File root;
  String currentFilePath;
  bool isPlaying;

void openRandomFile();
};

#endif
