// Inserindo bibliotecas
#include <Arduino.h>
#include <Hash.h>
#include "FS.h"
#include <Wire.h> //Biblioteca da comunicação I2C
#include "RTClib.h" //Biblioteca do RTC
#include <NTPClient.h> //Biblioteca do NTP.
#include "ESP8266WiFi.h" //Biblioteca do WiFi.
#include <ESPAsyncTCP.h>
#include "ESPAsyncWebServer.h" //Biblioteca do Servidor Web Assíncrono
#include <ESP8266WiFi.h> //lib do wifi para o ESP8266
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>

//Criando objeto do RTC
RTC_DS3231 rtc;

//Cria um objeto "UDP".
WiFiUDP udp;

// Criando e configurando o objeto do NTP
NTPClient ntp(udp, "a.st1.ntp.br", -3 * 3600, 60000);//Cria um objeto "NTP" com as configurações.

// ************ Configurações do Wi-Fi 1 ************
const char *ssid = "OneTech_casa 19";
const char *senha = "onetech@987x";

// Criando vetores para printar os dias da semana e os meses corretamente
char diasDaSemana[7][14] = {"Domingo", "Segunda-feira", "Terça-feira", "Quarta-feira", "Quinta-feira", "Sexta-feira", "Sábado"};
char mesesCorretos[12][10] = {"Janeiro", "Fevereiro", "Março", "Abril", "Maio", "Junho", "Julho",
                              "Agosto", "Setembro", "Outubro", "Novembro", "Dezembro"};

// Criando variáveis do relógio
int dia = 0;
int mes = 0;
int ano = 0;
int hora = 0;
int minuto = 0;
int segundo = 0;
int ajustarDOW = 0;
String dOW = ""; //"dOW" significa "dayOfWeek"
String mC = ""; //"mC" significa "mesCerto"

//Criando variáveis para receber valores do WebServer
String input = "off";
String hh = "";
String mm = "";

String hora_I = "";
String minuto_I = "";

int i = 0;

int alarme_cont = 0;

unsigned long delayWifi = 0;
unsigned long delayRele = 0;
unsigned long horaSerial = 0;
unsigned long tempoAlarme = 0;
unsigned long delayEnvio = 0;

int duracao_Cafe = 15; //Define o tempo em que a cafeteira permanecerá ligada

boolean cont = false;

// Definindo a porta D5 para o botão (GPIO14)
#define botao 14

// Definindo a porta D6 para o relê (GPIO12)
#define rele 12

// Definindo a porta D7 para o LED (GPIO13)
#define led 13

boolean estadoBotao = true;
boolean estadoAntBotao = true;

//Definindo nomes dos métodos GET
const char* PARAM_INPUT = "rele";

const char* PARAM_ESTADO = "agendar";
const char* PARAM_APAGAR = "apagar";

// Criando um objeto do tipo "AsyncWebServer" na porta 80
AsyncWebServer server(80);

//Configurando a informação dinânica do estado do botão
String configSwitch(const String& var){
  if(var == "ESTADODORELE"){
    String estado = "";
    if(!digitalRead(rele)){
      estado += "<input type=\"checkbox\" class=\"liga-desliga__checkbox checkCafe\" id=\"liga-desliga\" onclick=\"botaoCafe();\" onchange=\"mudarEstadoCafe(this)\"/>";
    }else{
      estado += "<input type=\"checkbox\" class=\"liga-desliga__checkbox checkCafe\" id=\"liga-desliga\" onclick=\"botaoCafe();\" onchange=\"mudarEstadoCafe(this)\" checked/>";
    }
    return estado;
  }
  return String();
}

String configAgendamento(const String& var){
  if(var == "ESTADOAGENDAMENTO"){
    String msg = montarAgenda();
    return msg;
  }
  if(var == "BOTAOCANCELAR"){
    String msg = botaoCancelar();
    return msg;
  }
  return String();
}

String botaoCancelar(){
  String msg = "";
  if((hh != "")&&(mm != "")){
    msg += "<button class=\"btn btn-gray\" onclick=\"apagar();\">Cancelar agendamento</button>";
  }else{
    msg += "<span class=\"Selec\" style=\"position: relative; top: 10px;\">Cancelar agendamento</span>";
  }
  return msg;
}

String montarAgenda(){
  String msg = "";
  if((hh == "")||(mm == "")){
    msg += "<h2>* Sem horário *</h2>";
  }else{
    if(((hora_I <= hh)&&(minuto_I < mm))){
      msg += "<h2>* Hoje, às ";
    }else{
      msg += "<h2>* Amanhã, às ";  
    }
    if(((hh == "0")||(hh == "1"))||(hh == "2")){
      msg += "0";
    }
    if(((hh == "3")||(hh == "4"))||(hh == "5")){
      msg += "0";
    }
    if(((hh == "6")||(hh == "7"))||(hh == "8")){
      msg += "0";
    }

    if(hh == "9"){
      msg += "0";
    }
       
    msg += hh + ":";
      
    if(((mm == "0")||(mm == "1"))||(mm == "2")){
      msg += "0";
    }
    if(((mm == "3")||(mm == "4"))||(mm == "5")){
      msg += "0";
    }
    if(((mm == "6")||(mm == "7"))||(mm == "8")){
      msg += "0";
    }

    if(mm == "9"){
      msg += "0";
    }
      
    msg += mm + " *</h2>";
  }
  return msg;
}

String cabecalho(){
  String msg = "";

  String diaStr = String(dia, DEC);
  String mesStr = String(mes + 1, DEC);
  String anoStr = String(ano, DEC);
  String horaStr = String(hora, DEC);
  String minutoStr = String(minuto, DEC);
  String segundoStr = String(segundo, DEC);
  
  if(hora >= 0 && hora < 12){
    msg = "<p>Bom dia!";
  }else if(hora >=12 && hora < 18){
    msg += "<p>Boa tarde!";
  }else{
    msg += "<p>Boa noite!";
  }

  msg += " Hoje é " + dOW + ", ";
  if(dia < 10){
    msg += "0";
  }
  msg += "" + diaStr + "/";
  if(mes < 10){
    msg += "0";
  }
  msg += "" + mesStr + "/" + anoStr + " - ";  
  if(hora < 10){
    msg += "0";
  }
  msg += "" + horaStr + ":";
  if(minuto < 10){
    msg += "0";
  }
  msg += "" + minutoStr + ":";
  if(segundo < 10){
    msg += "0";
  }
  msg += "" + segundoStr + "</p>";
  
  return msg;
}

void iniciarWebServer(){
  // Carregando as páginas HTML
  server.on("/index.html", HTTP_GET, [](AsyncWebServerRequest *request){ // Página principal da Cafeteira
    request->send(SPIFFS, "/index.html", String(), false, configSwitch);
  });

  server.on("/agendar.html", HTTP_GET, [](AsyncWebServerRequest *request){ // Página do Agendamento da Cafeteira
    request->send(SPIFFS, "/agendar.html", String(), false, configAgendamento);
  });
  
  server.on("/config.html", HTTP_GET, [](AsyncWebServerRequest *request){ // Página das Configurações da Cafeteira
    request->send(SPIFFS, "/config.html", String(), false);
  });
  
  // Carregando os documentos CSS
  server.on("/estilos/estilo.css", HTTP_GET, [](AsyncWebServerRequest *request){ 
    request->send(SPIFFS, "/estilos/estilo.css", "text/css");
  });

  server.on("/estilos/normalize.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/estilos/normalize.css", "text/css");
  });

  server.on("/estilos/cafe_MQ.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/estilos/cafe_MQ.css", "text/css");
  });

  server.on("/estilos/agendar_MQ.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/estilos/agendar_MQ.css", "text/css");
  });

  server.on("/estilos/config_MQ.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/estilos/config_MQ.css", "text/css");
  });

  // Carregando o documento Javascript
  server.on("/scripts/script.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/scripts/script.js", "text/javascript");
  });
  
  server.on("/imagens/cafe.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/imagens/cafe.png", "image/png");
  });

  server.on("/imagens/engrenagem.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/imagens/engrenagem.png", "image/png");
  });

  // *****************************************************************************************************
  // *                                                                                                   *
  // *                                     Requisições da Cafeteira                                      *
  // *                                                                                                   *
  // *****************************************************************************************************

  // Irá captar o estado do botão da página inicial (Ligado/Desligado)
  server.on("/estadoCafe", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    String inputParam;
    // GET input values on <ESP_IP>/update
    if (request->hasParam(PARAM_INPUT)) {
      inputMessage = request->getParam(PARAM_INPUT)->value();
      inputParam = PARAM_INPUT;

      if(inputMessage == "1"){
        Serial.print("\nLigando! ");
        digitalWrite(rele, HIGH);
        digitalWrite(led, HIGH);
      }
      else{
        Serial.print("\nDesligando! ");
        digitalWrite(rele, LOW);
        digitalWrite(led, LOW);
        cont = false;
      }
    }
    else {
      inputMessage = "Nenhuma mensagem foi enviada... Algum problema ocorreu!";
      inputParam = "";
    }
    Serial.println(inputMessage);
  });

  
  // Irá receber os dados do horário para agendamento
  server.on("/agendar", HTTP_GET, [] (AsyncWebServerRequest *request) {    

    hh = request->getParam("Hora")->value();
    mm = request->getParam("Minuto")->value();
    input = request->getParam("Lembrete")->value();

    Serial.print("Horário definido no Web Server: *");
    Serial.print(hh);
    Serial.print(":");
    Serial.print(mm);
    Serial.println("*");
    
    request->send(SPIFFS, "/agendar.html", String(), false, configAgendamento);
  });

  // Irá apagar o agendamento atual
  server.on("/apagar", HTTP_GET, [] (AsyncWebServerRequest *request) {    

    hh = "";
    mm = "";
    request->send(SPIFFS, "/agendar.html", String(), false, configAgendamento);
  });

  server.on("/opcao", HTTP_GET, [] (AsyncWebServerRequest *request){
    String msg = request->getParam("Tempo")->value();
    duracao_Cafe = (msg.toInt() * 60) * 1000;
    Serial.print("Configuração alterada! A cafeteira estará ligada por ");
    Serial.print(msg);
    Serial.println(" minutos a partir de agora!");
    
    request->send(SPIFFS, "/config.html", String(), false);
  });

  server.on("/atualizarCafe", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", String(digitalRead(rele)).c_str());
  });

  server.on("/agendamento", HTTP_GET, [](AsyncWebServerRequest *request){
    String msg = montarAgenda();
    request->send(200, "text/plain", msg);
  });

  server.on("/data", HTTP_GET, [](AsyncWebServerRequest *request){
    String msg = cabecalho();
    request->send(200, "text/plain", msg);
  });

  server.on("/cancelar", HTTP_GET, [](AsyncWebServerRequest *request){
    String msg = botaoCancelar();
    request->send(200, "text/plain", msg);
  });
  
  // Iniciando o Web Server
  server.begin();
}

void ajustarHora(){  
  unsigned long epochTime = ntp.getEpochTime();
  
  struct tm *ptm = gmtime ((time_t *)&epochTime); //Gera o dia, mês e ano
  dia = ptm->tm_mday; //Captura o dia;
  mes = ptm->tm_mon; //Captura o mês + 1;
  ano = ptm->tm_year+1900; //Soma o valor recebido com 1900 (gerando o ano atual)
  
  hora = ntp.getHours();
  minuto = ntp.getMinutes();
  segundo = ntp.getSeconds();

  rtc.adjust(DateTime(ano, mes, dia, hora, minuto, segundo));
}

void valorHora(){
  DateTime agora = rtc.now(); //Inicia o objeto DateTime
  
  dia = agora.day(); //Captura o dia;
  mes = agora.month(); //Captura o mês;
  mC = mesesCorretos[mes]; //Corrige o mês;
  ano = agora.year(); //Captura o ano
  
  hora = agora.hour(); //Captura a hora
  minuto = agora.minute(); //Captura o minuto
  segundo = agora.second(); //Captura o segundo

  ajustarDOW = agora.dayOfTheWeek() + 3;
  if(ajustarDOW > 6){
    ajustarDOW -= 7;
  }
  
  dOW = diasDaSemana[ajustarDOW];

  hora_I = String(hora, DEC);
  minuto_I = String(minuto, DEC);

  
  Serial.print(dOW);
  Serial.print(", ");
  if(dia < 10){
    Serial.print("0");
  }
  Serial.print(dia);
  Serial.print(" de ");
  Serial.print(mC);
  Serial.print(" de ");
  Serial.print(ano);

  Serial.print(" - ");
  
  if(hora < 10){
    Serial.print("0");
  }
  Serial.print(hora);
  Serial.print(":");
  if(minuto < 10){
    Serial.print("0");
  }
  Serial.print(minuto);
  Serial.print(":");
  if(segundo < 10){
    Serial.print("0");
  }
  Serial.println(segundo);
}

void pularLinha1(){
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println("-----------------------------------------------------");
}

void pularLinha2(){
  Serial.println("-----------------------------------------------------");
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println();
}

void configurarWifi(){
  // Conecta ao wifi com IP estático
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, senha); 
  Serial.println();
  Serial.println();
  Serial.println();
  delayWifi = millis();
  Serial.print("Conectando ao WiFi...");
  do{
    if((millis() - delayWifi) < 500){
      digitalWrite(led, HIGH);
    }
    if((millis() - delayWifi) >= 500){
      digitalWrite(led, LOW);
    }
    if((millis() - delayWifi) >= 1000){
      Serial.print(".");
      delayWifi = millis();
    }
  }while (WiFi.waitForConnectResult() != WL_CONNECTED);
  digitalWrite(led, LOW);
  Serial.println();

  for(int pisca = 0; pisca <= 3; pisca ++){
    digitalWrite(led, HIGH);
    delay(100);
    digitalWrite(led, LOW);
    delay(100);
  }
  
  // Escreve o IP Local do NodeMCU
  Serial.print("Wi-fi conectado! Acesse '");
  Serial.print(WiFi.localIP());
  Serial.println("/index.html' para entrar!");
  Serial.print("Canal Wi-Fi utilizado: ");
  Serial.println(WiFi.channel());
  Serial.println();
  Serial.println();
  
  // Iniciando a configuração do protocolo NTP
  ntp.begin(); //Inicia o NTP.
  ntp.forceUpdate(); //Força o Update.

  // iniciando a configuração do Módulo RTC 
  if(rtc.begin()){
    
    ajustarHora();
  }else{
    Serial.println("Erro ao iniciar o RTC3231! Reiniciando NodeMCU...");
    delay(5000);
    ESP.restart();
  }
}

//Função Setup
void setup(){
  //Inicializando os pinos D1 e D2 para comunicação I2C
  Wire.begin(D1, D2);

  duracao_Cafe = (duracao_Cafe * 60) * 1000;

  SPIFFS.begin();
  
  // Iniciando monitor serial
  Serial.begin(115200);

  // Definindo estado das portas referentes ao relê, botão e LED
  pinMode(rele, OUTPUT);
  pinMode(botao, INPUT_PULLUP);
  pinMode(led, OUTPUT);
  digitalWrite(rele, LOW);
  digitalWrite(led, LOW);

  configurarWifi();   // Configura e inicializa o WiFi
  iniciarWebServer(); // Configura e inicializa o Web Server  

  Serial.println();
}

//Função Loop
void loop(){  
  //Imprime no monitor Serial a hora atual
  if((millis() - horaSerial) >= 1000){
    valorHora();
    horaSerial = millis();
  }
  
  estadoBotao = digitalRead(botao);
  if(!estadoBotao && estadoAntBotao){
    if(digitalRead(rele) == HIGH){
      digitalWrite(rele, LOW);
      digitalWrite(led, LOW);
      cont = false;
    }else{
      digitalWrite(rele, HIGH);
    }
  }
  estadoAntBotao = estadoBotao;
  
  if(((hora == 00) && (minuto == 00)) && ((segundo == 00)||(ano < 2020))){
  
    pularLinha1();
    Serial.println("Hora da manutenção! Sincronizando relógio...");
    delay(1000);
    ajustarHora();
    Serial.println("Hora sincronizada com sucesso!");
    pularLinha2();
  }

  if(((hora_I == hh) && (minuto_I == mm)) && (segundo == 00)){
    digitalWrite(rele, HIGH);
  }

  if(digitalRead(rele) == HIGH){
    if(!cont){
      delayRele = millis();
      cont = true;
      digitalWrite(led, HIGH);
    }
    if((millis() - delayRele) >= duracao_Cafe){
      digitalWrite(rele, LOW);
      cont = false;
      Serial.println("O café está pronto!");
      Serial.println();
      digitalWrite(led, LOW);
    }
  }
  
  delay(50);
}
