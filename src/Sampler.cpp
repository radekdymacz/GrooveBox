#include "Sampler.h"

Sampler::Sampler(AudioPlaySdWav &playWav) : playWav(playWav), isPlaying(false) {}


void Sampler::setup() {
  AudioMemory(8);
//   SPI.setMOSI(7);  // MOSI pin on Teensy
//   SPI.setMISO(8);  // MISO pin on Teensy
//   SPI.setSCK(14);  // SCK pin on Teensy

  if (!(SD.begin(BUILTIN_SDCARD))) {
    while (1) {
      // SD card initialization failed
      delay(1000);
    }
  }
  root = SD.open("/Samples");
}

void Sampler::openRandomFileRecursively(File dir) {
  while (!playWav.isPlaying()) {
    File entry = dir.openNextFile();
    if (!entry) {
      dir.rewindDirectory();
      break;
    }
    if (entry.isDirectory()) {
      if (String(entry.name()) != "." && String(entry.name()) != "..") {
        openRandomFileRecursively(entry);
      }
    } else {
      String fileName = entry.name();
      if (fileName.endsWith(".wav") || fileName.endsWith(".WAV")) {
        currentFilePath = fileName;
        entry.close();
        break;
      }
      entry.close();
    }
  }
}

void Sampler::openRandomFile() {
  currentFilePath = "";
  openRandomFileRecursively(root);
}



String Sampler::playRandomFile() {
  if (isPlaying) {
    playWav.stop();
    isPlaying = false;
  }
  openRandomFile();
  if (currentFilePath != "") {
    playWav.play(currentFilePath.c_str());
    isPlaying = true;
  }

  return currentFilePath.c_str();
}
