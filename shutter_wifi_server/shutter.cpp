#include <Arduino.h>
#include "shutter.h"


Shutter::Shutter() {
  stop(false);
}

void Shutter::attach(char relayUp, char relayDown, char buttonUp, char buttonDown) {
  this->relayUp = relayUp;
  this->relayDown = relayDown;
  this->buttonUp = buttonUp;
  this->buttonDown = buttonDown;
  
  pinMode(relayDown, OUTPUT);
  pinMode(relayUp, OUTPUT);
  pinMode(buttonDown, INPUT);
  pinMode(buttonUp, INPUT);
}

void Shutter::open() {
  if(!inAction() && state != OPENED) {
    digitalWrite(relayDown, LOW);
    delay(20);
    digitalWrite(relayUp, HIGH);
    state = GOING_UP;
  }
}

void Shutter::close() {
  if(!inAction() && state != CLOSED) {
    digitalWrite(relayUp, LOW);
    delay(20);
    digitalWrite(relayDown, HIGH);
    state = GOING_DOWN;
  }
}

int Shutter::getState() {
  return state;
}

bool Shutter::inAction() {
  return (state == GOING_UP || state == GOING_DOWN);
}

void Shutter::setTimeStop() {
  timeStop = millis() + TIME_SHORT_ACTION;
}

int Shutter::openCompletly() {
  if(!inAction() && state != OPENED) {
    open();
    setTimeStop();
    return TIME_SHORT_ACTION;
  }
  return -1;
}

int Shutter::closeCompletly() {
  if(!inAction() && state != CLOSED) {
    close();
    setTimeStop();
    return TIME_SHORT_ACTION;
  }
  return -1;
}

void Shutter::stop(bool timeout) {
  digitalWrite(relayUp, LOW);
  digitalWrite(relayDown, LOW);

  if(timeout) {
    if(state == GOING_UP) {
      // Shutter was going up
      state = OPENED;
    }
    if(state == GOING_DOWN) {
      // Shutter was going down
      state = CLOSED;
    }
  } else {
    state = STOPED;
  }
  
  timeUp = 0;
  timeDown = 0;
  timeStop = 0;
}

void Shutter::action() {
  buttonUpValue = digitalRead(buttonUp);

  if(buttonUpValue == HIGH && state != OPENED) {
    // Le bouton est enfoncé
    if(buttonUpMemory == LOW) {
      // Premier passage, le bouton n'était pas enfoncé avant
      timeUp = millis();
      buttonUpMemory = buttonUpValue;
      open();
    }
  } else if(buttonUpMemory == HIGH) {
    // Le bouton vient d'etre relaché
    if(timeStop != 0) {
      // Le volet est en mouvement
      stop(false);
    } else if(millis()-timeUp < TIME_SHORT_PUSH) {
      setTimeStop();
    } else {
      stop(false);
    }
    buttonUpMemory = buttonUpValue;
  } 

  buttonDownValue = digitalRead(buttonDown);
  
  if(buttonDownValue == HIGH && state != CLOSED) {
    // Le bouton est enfoncé
    if(buttonDownMemory == LOW) {
      // Premier passage, le bouton n'était pas enfoncé avant
      timeDown = millis();
      buttonDownMemory = buttonDownValue;
      close();
    }
  } else if(buttonDownMemory == HIGH) {
    // Le bouton vient d'etre relaché
    if(timeStop != 0) {
      // Le volet est en mouvement
      stop(false);
    } else if(millis()-timeDown < TIME_SHORT_PUSH) {
      setTimeStop();
    } else {
      stop(false);
    }
    buttonDownMemory = buttonDownValue;
  }

  if(timeStop != 0){
    // Le bouton est relaché depuis quelque tour, mais le volet est en mouvement
    if(millis() > timeStop) {
      stop(true);
    }
  }
}

