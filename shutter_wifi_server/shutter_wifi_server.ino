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

void setup(void){
  Serial.begin(9600);
  
  // on demande la connexion au WiFi
  WiFi.begin(ssid, password);
  Serial.println("");
  // on attend d'etre connecte au WiFi avant de continuer
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // on affiche l'adresse IP qui nous a ete attribuee
  Serial.println("");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  // on definit ce qui doit etre fait lorsque la route /bonjour est appelee
  // ici on va juste repondre avec un "hello !"
  server.on("/open", open);
  server.on("/close", close);
  server.on("/stop", stop);
  server.on("/status", status);
  
  // on commence a ecouter les requetes venant de l'exterieur
  server.begin();
  
  shutter.attach(shutterUp, shutterDown, buttonUp, buttonDown);
  
  pinMode(lightSensor, INPUT);
}

void loop(void){
  // a chaque iteration, on appelle handleClient pour que les requetes soient traitees
  server.handleClient();

  shutter.action();

  delay(20);
}

