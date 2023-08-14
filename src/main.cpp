#include <Arduino.h>
#include <Audio.h>
#include "Sequencer.h"
#include "Sampler.h"

const int bpm = 120;
const int ppqn = 1;

AudioPlaySdWav playWav;          // Audio play object
// GUItool: begin automatically generated code
AudioSynthSimpleDrum     drum1;          //xy=288,275
AudioOutputUSB           usb1;           //xy=666,284
AudioConnection          patchCord1(drum1, 0, usb1, 0);
AudioConnection          patchCord2(drum1, 0, usb1, 1);
Sampler sampler(playWav);

// put function declarations here:
// Callback function to handle events
void handleEvent(int eventData) {
  // Define event handling logic here
  // For example, you could control LEDs, trigger sounds, etc.
  Serial.println(eventData);
  drum1.frequency(eventData);
  drum1.noteOn();
}
Sequencer sequencer(bpm, ppqn, handleEvent);

 // Set note duration to eighth notes

void setup() {
  // put your setup code here, to run once:
  AudioMemory(10);
  Serial.begin(9600);
  sequencer.addEvent(60);
  sequencer.addEvent(-1);  // Rest
  sequencer.addEvent(290);
  sequencer.setNoteDuration(4); 

  // Set up the sampler
  sampler.setup();

  // drum1 
  drum1.frequency(70);
  drum1.length(50);
  drum1.secondMix(0.0);
  drum1.pitchMod(0.8);
}

void loop() {
  // put your main code here, to run repeatedly:
  sequencer.update();
}

// put function definitions here:
