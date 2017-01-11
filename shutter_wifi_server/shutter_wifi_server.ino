    #include <ESP8266WiFi.h>
    #include <WiFiClient.h>
    #include <ESP8266WebServer.h>
    #include <ESP8266mDNS.h>
    
    const char* ssid = "Choux et Muxu";
    const char* password = "NP26badG33114LB";
    
    ESP8266WebServer server(80); // on instancie un serveur ecoutant sur le port 80
    
    int myPins[] = {D0, D1, D2, D3, D4};

    int getPinParameter() {
        int pin = server.arg("volet").toInt();
        Serial.print("Pin parameter : ");
        Serial.println(myPins[pin]);
        return myPins[pin];
    }

    void open() {
        int pin = getPinParameter();
        digitalWrite(pin, HIGH);
        server.send(200, "text/plain", String(pin) + " est allumé");
    }
    
    void close() {
        int pin = getPinParameter();
        digitalWrite(pin, LOW);
        server.send(200, "text/plain", String(pin) + " est éteint");
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
      
      // on commence a ecouter les requetes venant de l'exterieur
      server.begin();
    
      pinMode(D0, OUTPUT);
      pinMode(D1, OUTPUT);
      pinMode(D2, OUTPUT);
      pinMode(D3, OUTPUT);
      pinMode(D4, OUTPUT);
    }
    
    void loop(void){
      // a chaque iteration, on appelle handleClient pour que les requetes soient traitees
      server.handleClient();
    }
    
