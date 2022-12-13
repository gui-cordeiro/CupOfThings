#include <Arduino.h>
#include <Hash.h>
#include "LittleFS.h"
#include <NTPClient.h>
#include <ESPAsyncTCP.h>
#include "ESPAsyncWebServer.h"
#include <ESP8266mDNS.h>

//Declarações
#include <global_variables.h>
#include <interface.h>

#define PARAM_INPUT "rele"
#define PARAM_ESTADO "agendar"
#define PARAM_APAGAR "apagar"

//Protocolo UDP
WiFiUDP udp;
NTPClient ntp(udp, "a.st1.ntp.br", -3 * 3600, 60000);

const char *ssid = "FREILUIZ_2.4";
const char *senha = "escorpiao70";

char diasDaSemana[7][14] = {"Domingo", "Segunda-feira", "Terça-feira", "Quarta-feira", "Quinta-feira", "Sexta-feira", "Sábado"};
char mesesCorretos[12][10] = {"Janeiro", "Fevereiro", "Março", "Abril", "Maio", "Junho", "Julho",
                              "Agosto", "Setembro", "Outubro", "Novembro", "Dezembro"};

int dia, mes, ano, hora, minuto, segundo;

String input = "off";
String hh = "";
String mm = "";

String hora_I = "";
String minuto_I = "";

int i = 0;

unsigned long delayWifi = 0;
unsigned long delayRele = 0;
unsigned long horaSerial = 0;
unsigned long tempoAlarme = 0;
unsigned long delayEnvio = 0;
unsigned long duracao_Cafe = 15; //Define o tempo em que a cafeteira permanecerá ligada

boolean cont = false;

// Definindo a porta D5 para o botão (GPIO14)
#define botao 14

// Definindo a porta D6 para o relê (GPIO12)
#define rele 12

boolean estadoBotao = true;
boolean estadoAntBotao = true;

void ajustarHora(){  
  unsigned long epochTime = ntp.getEpochTime();
  
  struct tm *ptm = gmtime ((time_t *)&epochTime); //Gera o dia, mês e ano
  dia = ptm->tm_mday; //Captura o dia;
  mes = ptm->tm_mon; //Captura o mês + 1;
  ano = ptm->tm_year+1900; //Soma o valor recebido com 1900 (gerando o ano atual)
  
  hora = ntp.getHours();
  minuto = ntp.getMinutes();
  segundo = ntp.getSeconds();
}

void configurarWifi(){
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, senha); 
  delayWifi = millis();
  Serial.print("Conectando ao WiFi...");
  do{
    if((millis() - delayWifi) >= 1000){
      Serial.print(".");
      delayWifi = millis();
    }
  }while (WiFi.waitForConnectResult() != WL_CONNECTED);
    
  // Escreve o IP Local do NodeMCU
  Serial.print("\n --> Wi-fi conectado! Acesse '");
  Serial.print(WiFi.localIP());
  Serial.println("/index.html' para entrar!");
  Serial.print(" --> Canal Wi-Fi utilizado: ");
  Serial.println(WiFi.channel());
  
  // Iniciando a configuração do protocolo NTP
  ntp.begin(); //Inicia o NTP.
  ntp.forceUpdate(); //Força o Update.
}

//Função Setup
void setup(){
  // Iniciando memória LittleFS para obter os arquivos web
  do{
    Serial.println("Erro ao inicializar o LittleFS...");
    delay(1000);
  }while(!LittleFS.begin());

  // Iniciando monitor serial
  Serial.begin(115200);

  // Definindo estado das portas referentes ao relê, botão e LED
  pinMode(rele, OUTPUT);
  pinMode(botao, INPUT_PULLUP);
  digitalWrite(rele, LOW);

  duracao_Cafe = (duracao_Cafe * 60) * 1000;
  
  configurarWifi();   // Configura e inicializa o WiFi
  iniciarWebServer(); // Configura e inicializa o Web Server  
  Serial.println();
}

//Função Loop
void loop(){  
  estadoBotao = digitalRead(botao);
  if(!estadoBotao && estadoAntBotao){
    if(digitalRead(rele) == HIGH){
      digitalWrite(rele, LOW);
      cont = false;
    }else{
      digitalWrite(rele, HIGH);
    }
  }
  estadoAntBotao = estadoBotao;
  
  if(((hora == 00) && (minuto == 00)) && ((segundo == 00)||(ano < 2020))){
    Serial.println("Atualizando relógio...");
    horaSerial = millis();
    while((millis() - horaSerial) <= 1000){}
    ajustarHora();
    Serial.println("Atualização concluída.");
  }

  if(((hora_I == hh) && (minuto_I == mm)) && (segundo == 00)){
    digitalWrite(rele, HIGH);
  }

  if(digitalRead(rele) == HIGH){
    if(!cont){
      delayRele = millis();
      cont = true;
    }
    if((millis() - delayRele) >= duracao_Cafe){
      digitalWrite(rele, LOW);
      cont = false;
      Serial.println("O café está pronto!");
      Serial.println();
    }
  }
  
  delay(10);
}
