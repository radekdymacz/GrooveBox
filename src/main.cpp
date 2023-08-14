#include <Arduino.h>
#include <Audio.h>
#include "Sequencer.h"
#include "Sampler.h"
#include "AudioEffectDynamicEnvelope.h"

const int bpm = 120;
const int ppqn = 1;

AudioPlaySdWav playWav;          // Audio play object
// GUItool: begin automatically generated code
AudioSynthSimpleDrum     drum1;          //xy=288,275
AudioSynthNoiseWhite   noise;          //xy=289,313
AudioEffectDynamicEnvelope amp;
AudioOutputUSB           usb1;           //xy=666,284
AudioEffectDelay delayL;
AudioEffectDelay delayR;

AudioConnection          patchCord1(noise, 0, amp, 0);
AudioConnection          patchCord2(amp, 0, usb1, 0);
AudioConnection          patchCord3(amp ,0, usb1, 1);
// AudioConnection          patchCord4(amp, 0, delayL, 0);
// AudioConnection          patchCord5(amp, 0, delayR, 1);
Sampler sampler(playWav);

// put function declarations here:
// Callback function to handle events
void handleEvent(int eventData) {
  // Define event handling logic here
  // For example, you could control LEDs, trigger sounds, etc.
  Serial.println(eventData);
  drum1.frequency(eventData);
  drum1.noteOn();
  noise.amplitude(0.5);
}
Sequencer sequencer(bpm, ppqn, handleEvent);

 // Set note duration to eighth notes

void setup() {
  // put your setup code here, to run once:
  AudioMemory(100);
  Serial.begin(9600);
  sequencer.addEvent(60);
  sequencer.addEvent(-1);  // Rest
  sequencer.addEvent(290);
  sequencer.addEvent(90);
  sequencer.addEvent(490);
  sequencer.setNoteDuration(4); 

  // Set up the sampler
  sampler.setup();

  // drum1 
  drum1.frequency(70);
  drum1.length(50);
  drum1.secondMix(0.0);
  drum1.pitchMod(0.8);

  // amp.randomize(5000, 9); // 3 seconds total length and 5 stages
  
  amp.addStage(1.0,1,0.5); 
  amp.addStage(0.0,1000,0.1); 
  amp.visualize();
  amp.start();


  // haas effect
  delayL.delay(0, 30);
  delayR.delay(0, 20);

}

void loop() {
  // put your main code here, to run repeatedly:
  sequencer.update();
  amp.randomize(1000, random(2, 10));
  // For instance, if you want to randomize the amp again after a delay:
  delay(1000);  
  amp.start();
}

// put function definitions here:
