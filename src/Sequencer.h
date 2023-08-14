#ifndef Sequencer_h
#define Sequencer_h

#include <Arduino.h>
#include <vector>

class Sequencer {
public:
  using EventCallback = void (*)(int);

  Sequencer(int bpm, int ppqn, EventCallback callback);
  void update();
  void addEvent(int event);
  void removeEvent(int index);

  void setBPM(int newBPM);  // New method to set the BPM
  void setNoteDuration(int noteDuration);  // New method to set note duration

private:
  int bpm;
  int ppqn;
  int noteDuration;  // Holds the note duration (1, 2, 4 for quarter, eighth, sixteenth notes)
  std::vector<int> events;
  EventCallback eventCallback;
  int currentStep;
  unsigned long previousMillis;

  unsigned long calculateStepInterval();  // New private method to calculate step interval
};

#endif
