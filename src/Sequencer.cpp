#include "Sequencer.h"

Sequencer::Sequencer(int bpm, int ppqn, EventCallback callback)
  : bpm(bpm), ppqn(ppqn), eventCallback(callback) {
    currentStep = 0;
    previousMillis = 0;
    noteDuration = 4;
}

void Sequencer::update() {
  unsigned long currentMillis = millis();
  unsigned long stepIntervalMillis = calculateStepInterval();

  if (currentMillis - previousMillis >= stepIntervalMillis) {
    previousMillis = currentMillis;

    int event = events[currentStep];
    if (event != -1 && eventCallback != nullptr) {
      eventCallback(event);
    }

    currentStep = (currentStep + 1) % events.size();
  }
}

void Sequencer::addEvent(int event) {
  events.push_back(event);
}

void Sequencer::removeEvent(int index) {
  if (index >= 0 && index < events.size()) {
    events.erase(events.begin() + index);
    if (currentStep >= events.size()) {
      currentStep = 0;
    }
  }
}

void Sequencer::setBPM(int newBPM) {
  bpm = newBPM;
}

void Sequencer::setNoteDuration(int newNoteDuration) {
  noteDuration = newNoteDuration;
}

unsigned long Sequencer::calculateStepInterval() {
  unsigned long msPerBeat = 60000 / bpm;
  unsigned long msPerNote = msPerBeat * 4 / noteDuration / ppqn;
  return msPerNote;
}
