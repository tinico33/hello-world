#include <Arduino.h>
#include "Volet.h"


Volet::Volet() {
  inAction = false;
}

void Volet::attach(char ru, char rd, char bu, char bd) {
  relayUp = ru;
  relayDown = rd;
  buttonUp = bu;
  buttonDown = bd;
  
  pinMode(relayDown, OUTPUT);
  pinMode(relayUp, OUTPUT);
  pinMode(buttonDown, INPUT);
  pinMode(buttonUp, INPUT);
}

void Volet::open() {
  if(!inAction) {
    digitalWrite(relayDown, LOW);
    delay(20);
    digitalWrite(relayUp, HIGH);
    inAction = true;
  }
}

void Volet::close() {
  if(!inAction) {
    digitalWrite(relayUp, LOW);
    delay(20);
    digitalWrite(relayDown, HIGH);
    inAction = true;
  }
}

void Volet::stop() {
  digitalWrite(relayUp, LOW);
  digitalWrite(relayDown, LOW);
  timeUp = 0;
  timeDown = 0;
  timeStop = 0;
  inAction = false;
}

void Volet::action() {
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
      timeStop = millis() + TIME_SHORT_ACTION;
    } else {
      stop();
    }
    buttonUpMemory = buttonUpValue;
  } else if(timeStop != 0){
    // Le bouton est relaché depuis quelque tour, mais le volet est en mouvement
    if(millis() > timeStop) {
      timeStop = 0;
      stop();
    }
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
      timeStop = millis() + TIME_SHORT_ACTION;
    } else {
      stop();
    }
    buttonDownMemory = buttonDownValue;
  } else if(timeStop != 0) {
    // Le bouton est relaché depuis quelque tour, mais le volet est en mouvement
    if(millis() > timeStop) {
      timeStop = 0;
      stop();
    }
  }
}
