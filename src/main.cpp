#include <Arduino.h>
#include <Hash.h>
#include "LittleFS.h"
#include <NTPClient.h>
#include <ESPAsyncTCP.h>
#include "ESPAsyncWebServer.h"
#include <ESP8266mDNS.h>

// Declarações dos headers
#include <global_variables.h>
#include <interface.h>

#define PARAM_INPUT "rele"
#define PARAM_ESTADO "agendar"
#define PARAM_APAGAR "apagar"

// Declarando variáveis (na ordem da interface)
String hh = "";
String mm = "";
String input = "off";

unsigned long powerPlugInterval = 0;
unsigned long timeUpdateInterval = 0;
unsigned long initialSetupDelay = 0;
unsigned long coffeeDuration = 15; // Define o tempo em que a cafeteira permanecerá ligada

boolean powerPlugIsActive = false;

// Definindo a porta D6 (relê, GPIO12) e D7 (LED, GPIO13)
#define relay 12
#define led 13

void setup() {
  // Iniciando sistema de arquivos LittleFS
  Serial.print(" ---> Inicializando o LittleFS . . .");
  initialSetupDelay = millis();
  while(!LittleFS.begin()) {
    if ((millis() - initialSetupDelay) >= 100) {
      Serial.print(" .");
      initialSetupDelay = millis();
    }
  }
  Serial.println(" ---> Sistema de arquivos LittleFS inicializado.\n");

  // Iniciando monitor Serial
  Serial.print(" ---> Inicializando o monitor Serial . . .");
  initialSetupDelay = millis();
  Serial.begin(115200);
  while (!Serial){
    if ((millis() - initialSetupDelay) >= 100) {
      Serial.print(" .");
      initialSetupDelay = millis();
    }
  }
  Serial.println(" ---> Monitor Serial inicializado.\n\n");

  // Definindo modo das portas do relê e do LED
  pinMode(relay, OUTPUT);
  pinMode(led, OUTPUT);
  digitalWrite(relay, LOW);

  coffeeDuration = coffeeDuration * 60000; // 60(sec to min) * 1000 (ms to sec)

  configurarWifi();   // Configura e inicializa o WiFi
  iniciarWebServer(); // Configura e inicializa o Web Server  
}

void loop(){  
  if(((getNTPValue("hours") == 00) && (getNTPValue("minutes") == 00)) && ((getNTPValue("seconds") == 00)||(ano < 2020))){
    Serial.println("Atualizando relógio...");
    timeUpdateInterval = millis();
    while((millis() - timeUpdateInterval) <= 1000){}
    ajustarHora();
    Serial.println("Atualização concluída.");
  }

  if(((getNTPValue("hours") == hh.toInt()) && (getNTPValue("minutes") == mm.toInt())) && (getNTPValue("seconds") > 0 && getNTPValue("seconds") < 1)) digitalWrite(relay, HIGH);

  if(digitalRead(relay)){
    if(!powerPlugIsActive){
      powerPlugInterval = millis();
      powerPlugIsActive = true;
    }
    if((millis() - powerPlugInterval) >= coffeeDuration){ // Desligando cafeteira, café pronto
      digitalWrite(relay, LOW);
      powerPlugIsActive = false;
      Serial.println("O café está pronto!\n");
    }
  }
  delay(10);
}
