/*
  MOOC Fabriquer un objet connecté : semaine 3

  Lit la valeur analogique du servo moteur sur la broche A0 et envoie
  cette valeur sur la plateforme TOM en utilisant une requête PATCH
  Toutes les 3 secondes, nous récupérons la valeur sur TOM afin de voir si le
  servo a bougé

  Le montage :
   Un servo moteur branché sur les broches 3V3, GND et D1
   La patte retour d'information du servo est branchée sur la broche A0

  créé le 30 Mars 2016
  par Laurent Mattlé et Baptiste Gaultier

  Ce code est en CC0 1.0 Universal

*/
#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

ESP8266WiFiMulti WiFiMulti;


uint8_t pinsOpen[2];
uint8_t pinsClose[2];

// Déclaration d'une variable payload que l'ont va utiliser pour lire les données
String payload;

// initialisation des constantes spécifiques à votre installation
const char* ssid = "Choux et Muxu";
const char* password = "NP26badG33114LB";

void setup() {
  // on souhaite communiquer avec l'ordinateur
  Serial.begin(9600);
  Serial.setDebugOutput(true);

  Serial.println("Nelson TP3 Start...");

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);

  pinsOpen[0] = D0;
  pinsOpen[1] = D2;

  pinsClose[0] = D1;
  pinsClose[1] = D3;
  
  // on se connecte au Wifi
  WiFiMulti.addAP(ssid, password);
}

char getInfoFromJson(String payload, String stringToSearch) {
  int start = payload.indexOf(stringToSearch);
  int end = payload.indexOf('\"', start + 1 );
  
  return payload.charAt(start + stringToSearch.length());
}

void loop() {
  // on regarde si on est bien connecté au point d'accès wifi
  if ((WiFiMulti.run() == WL_CONNECTED)) {
    // création d'un objet appelé http réutilisable et sorti du moule HTTPClient
    HTTPClient http;

    Serial.println("[HTTP] begin...");
    String uri = "/volets/action";
    http.begin("bureau", 9000, uri);

    Serial.println("[HTTP] GET...");
    // démarrer la connexion et envoyer les entêtes HTTP
    http.addHeader("Content-type", "application/json");
    int httpCode = http.GET();

    // httpCode sera négatif si on rencontre une erreur
    if (httpCode > 0) {
      // les entêtes HTTP ont été envoyés et
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);

      // si le serveur TOM répond par OK
      if (httpCode == HTTP_CODE_OK) {
        // alors on récupère la réponse sous forme de chaîne de caractères
        String payload = http.getString();
        Serial.println("[HTTP] GET... payload:");
        Serial.println(payload);

        char positionString = getInfoFromJson(payload, "action\":\"");
        long idVolet = (int)getInfoFromJson(payload, "id\":\"") - 48;
        
        Serial.print("positionString:");
        Serial.println(positionString);
        
        Serial.print("idVolet:");
        Serial.println(idVolet);

        if(positionString == 'U') {
          digitalWrite(pinsOpen[idVolet], HIGH);
          delay(100);
          digitalWrite(pinsOpen[idVolet], LOW);
        }
        
        if(positionString == 'D') {
          digitalWrite(pinsClose[idVolet], HIGH);
          delay(100);
          digitalWrite(pinsClose[idVolet], LOW);
        }
      }
    }
    else {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();

    delay(1000);
  }
}

