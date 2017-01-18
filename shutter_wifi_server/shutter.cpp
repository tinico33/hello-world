#include <Arduino.h>
#include "shutter.h"


Shutter::Shutter() {
  inAction = false;
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
  if(!inAction) {
    digitalWrite(relayDown, LOW);
    delay(20);
    digitalWrite(relayUp, HIGH);
    inAction = true;
  }
}

void Shutter::close() {
  if(!inAction) {
    digitalWrite(relayUp, LOW);
    delay(20);
    digitalWrite(relayDown, HIGH);
    inAction = true;
  }
}

void Shutter::setTimeStop() {
  timeStop = millis() + TIME_SHORT_ACTION;
}

void Shutter::openCompletly() {
  if(!inAction) {
    open();
    setTimeStop();
  }
}

void Shutter::closeCompletly() {
  if(!inAction) {
    close();
    setTimeStop();
  }
}

void Shutter::stop() {
  digitalWrite(relayUp, LOW);
  digitalWrite(relayDown, LOW);
  timeUp = 0;
  timeDown = 0;
  timeStop = 0;
  inAction = false;
}

void Shutter::action() {
  buttonUpValue = digitalRead(buttonUp);

  if(buttonUpValue == HIGH) {
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
      // Le volet est en train de descendre 
      stop();
    } else if(millis()-timeUp < 200) {
      setTimeStop();
    } else {
      stop();
    }
    buttonUpMemory = buttonUpValue;
  } 

  buttonDownValue = digitalRead(buttonDown);
  
  if(buttonDownValue == HIGH) {
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
      // Le volet est en train de monter 
      stop();
    } else if(millis()-timeDown < 200) {
      setTimeStop();
    } else {
      stop();
    }
    buttonDownMemory = buttonDownValue;
  }

  if(timeStop != 0){
    // Le bouton est relaché depuis quelque tour, mais le volet est en mouvement
    if(millis() > timeStop) {
      timeStop = 0;
      stop();
    }
  }
}
