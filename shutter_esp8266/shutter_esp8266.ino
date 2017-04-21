#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "shutter.h"

const char* ssid = "Choux et Muxu";
const char* password = "NP26badG33114LB";

ESP8266WebServer server(80); // on instancie un serveur ecoutant sur le port 80

const int shutterUp = D5;
const int shutterDown = D6;
const int buttonUp = D7;
const int buttonDown = D8;
const int lightSensor = A0;

unsigned long automaticActionsTimer = 0;
unsigned long automaticActionsTimerStart = 0;

int lightValueToClose = 1025;

Shutter shutter;

void open() {  
  server.send(200, "text/plain", String(shutter.openCompletly()));
}

void close() {
  server.send(200, "text/plain", String(shutter.closeCompletly()));
}

void stop() {
  shutter.stop(false);
  server.send(200, "text/plain");
}

void status() {
  server.send(200, "text/plain", String(shutter.getState()));
}

void save() {
  lightValueToClose = analogRead(lightSensor);
  Serial.print("Valeur enregistrée : ");
  Serial.println(lightValueToClose);
  server.send(200, "text/plain", String(lightValueToClose));
}

bool connectionActiv() {
  return WiFi.status() == WL_CONNECTED;
}

void connection() {
  // on demande la connexion au WiFi
  WiFi.begin(ssid, password);
  Serial.println("");
  // on attend d'etre connecte au WiFi avant de continuer
  while (!connectionActiv()) {
    delay(500);
    Serial.print(".");
  }
  // on affiche l'adresse IP qui nous a ete attribuee
  Serial.println("");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void testConnexion() {
  Serial.print("Check connexion : ");
  if(!connectionActiv()) {
    Serial.println("KO");
    Serial.println("Connexion...");
    connection();
  } else {
    Serial.println("OK");
  }
}

void autoClose() {
  int sensorValue = analogRead(lightSensor);
  if((lightValueToClose + 10) / 10  < (sensorValue - 10) / 10) {
    // La lumière lue est plus sombre que la lumière enregistrée => On ferme
    shutter.closeCompletly();
  }
}

void setup(void){
  Serial.begin(9600);

  // Connexion au réseau WiFi
  connection();
  
  // Définitin des routes
  server.on("/open", open);
  server.on("/close", close);
  server.on("/stop", stop);
  server.on("/status", status);
  server.on("/save", save);
  
  // Lancement de l'écoute des requetes venant de l'exterieur
  server.begin();

  shutter.attach(shutterUp, shutterDown, buttonUp, buttonDown);

  pinMode(lightSensor, INPUT);
}

void loop(void){
  automaticActionsTimer = millis();
  
  if(automaticActionsTimer < automaticActionsTimerStart) {
    // There was had a reset of millis();
    automaticActionsTimerStart = millis();
  } else {
    if(automaticActionsTimer - automaticActionsTimerStart > AUTOMATICS_ACTIONS_TIMER) {
      // Le timer est atteint
      
      // on test la connexion et on reconnecte si besoin
      testConnexion();

      // on test s'il fait assez sombre pour fermer le volet automatiquement
      autoClose();
      automaticActionsTimerStart = millis();
    }
  }

  // a chaque iteration, on appelle handleClient pour que les requetes soient traitees
  server.handleClient();

  // a chaque iteration, on verifie si une action a lieu sur les boutons du volet
  shutter.action();

  delay(20);
}

