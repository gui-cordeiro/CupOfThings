#include <Arduino.h>
#include <Hash.h>
#include "FS.h"
#include "LittleFS.h"
#include <NTPClient.h>
#include <ESPAsyncTCP.h>
#include "ESPAsyncWebServer.h"
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>

#include <interface.h>

WiFiUDP udp;
NTPClient ntp(udp, "a.st1.ntp.br", -3 * 3600, 60000);

const char *ssid = "ssid";
const char *senha = "password";

char diasDaSemana[7][14] = {"Domingo", "Segunda-feira", "Terça-feira", "Quarta-feira", "Quinta-feira", "Sexta-feira", "Sábado"};
char mesesCorretos[12][10] = {"Janeiro", "Fevereiro", "Março", "Abril", "Maio", "Junho", "Julho",
                              "Agosto", "Setembro", "Outubro", "Novembro", "Dezembro"};

int dia, mes, ano, hora, minuto, segundo;
/*int mes = 0;
int ano = 0;
int hora = 0;
int minuto = 0;
int segundo = 0;*/


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

const char* PARAM_INPUT = "rele";
const char* PARAM_ESTADO = "agendar";
const char* PARAM_APAGAR = "apagar";

AsyncWebServer server(80);

//Configurando a informação dinâmica do estado do botão
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

void iniciarWebServer(){
  // Carregando os documentos HTML
  server.on("/index.html", HTTP_GET, [](AsyncWebServerRequest *request){ // Página principal da Cafeteira
    request->send(LittleFS, "/index.html", String(), false, configSwitch);
  });

  server.on("/agendar.html", HTTP_GET, [](AsyncWebServerRequest *request){ // Página do Agendamento da Cafeteira
    request->send(LittleFS, "/agendar.html", String(), false, configAgendamento);
  });
  
  server.on("/config.html", HTTP_GET, [](AsyncWebServerRequest *request){ // Página das Configurações da Cafeteira
    request->send(LittleFS, "/config.html", String(), false);
  });
  
  // Carregando os documentos CSS
  server.on("/estilos/agendar.css", HTTP_GET, [](AsyncWebServerRequest *request){ 
    request->send(LittleFS, "/estilos/agendar.css", "text/css");
  });

  server.on("/estilos/footerBar.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/estilos/footerBar.css", "text/css");
  });

  server.on("/estilos/layout.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/estilos/layout.css", "text/css");
  });

  server.on("/estilos/menu.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/estilos/menu.css", "text/css");
  });

  server.on("/estilos/switch.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/estilos/switch.css", "text/css");
  });

  server.on("/estilos/timer.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/estilos/timer.css", "text/css");
  });

  // Carregando os documentos Javascript
  server.on("/scripts/footerBar.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/scripts/footerBar.js", "text/javascript");
  });

  server.on("/scripts/menu.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/scripts/menu.js", "text/javascript");
  });

  server.on("/scripts/timer.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/scripts/timer.js", "text/javascript");
  });

  server.on("/scripts/weather.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/scripts/weather.js", "text/javascript");
  });

  // Carregando os documentos de imagem
  server.on("/icons/clima/01d.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/icons/clima/01d.png", "image/png");
  });

  server.on("/icons/clima/01n.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/icons/clima/01n.png", "image/png");
  });

  server.on("/icons/clima/02d.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/icons/clima/02d.png", "image/png");
  });

  server.on("/icons/clima/02n.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/icons/clima/02n.png", "image/png");
  });

  server.on("/icons/clima/03d.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/icons/clima/03d.png", "image/png");
  });

  server.on("/icons/clima/03n.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/icons/clima/03n.png", "image/png");
  });

  server.on("/icons/clima/04d.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/icons/clima/04d.png", "image/png");
  });

  server.on("/icons/clima/04n.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/icons/clima/04n.png", "image/png");
  });

  server.on("/icons/clima/09d.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/icons/clima/09d.png", "image/png");
  });

  server.on("/icons/clima/09n.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/icons/clima/09n.png", "image/png");
  });

  server.on("/icons/clima/10d.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/icons/clima/10d.png", "image/png");
  });

  server.on("/icons/clima/10n.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/icons/clima/10n.png", "image/png");
  });

  server.on("/icons/clima/11d.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/icons/clima/11d.png", "image/png");
  });

  server.on("/icons/clima/11n.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/icons/clima/11n.png", "image/png");
  });

  server.on("/icons/clima/13d.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/icons/clima/13d.png", "image/png");
  });

  server.on("/icons/clima/13n.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/icons/clima/13n.png", "image/png");
  });

  server.on("/icons/clima/50d.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/icons/clima/50d.png", "image/png");
  });

  server.on("/icons/clima/50n.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/icons/clima/50n.png", "image/png");
  });

  server.on("/icons/clima/unknown.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/icons/clima/unknown.png", "image/png");
  });

  server.on("/icons/outros/delete.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/icons/outros/delete.png", "image/png");
  });

  server.on("/icons/social/Facebook.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/icons/social/Facebook.png", "image/png");
  });

  server.on("/icons/social/GitHub.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/icons/social/GitHub.png", "image/png");
  });

  server.on("/icons/social/Instagram.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/icons/social/Instagram.png", "image/png");
  });

  server.on("/icons/social/LinkedIn.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/icons/social/LinkedIn.png", "image/png");
  });

  server.on("/icons/social/Twitter.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/icons/social/Twitter.png", "image/png");
  });

  server.on("/icons/social/hover/Facebook.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/icons/social/hover/Facebook.png", "image/png");
  });

  server.on("/icons/social/hover/GitHub.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/icons/social/hover/GitHub.png", "image/png");
  });

  server.on("/icons/social/hover/Instagram.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/icons/social/hover/Instagram.png", "image/png");
  });

  server.on("/icons/social/hover/LinkedIn.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/icons/social/hover/LinkedIn.png", "image/png");
  });

  server.on("/icons/social/hover/Twitter.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/icons/social/hover/Twitter.png", "image/png");
  });

  server.on("/imagens/9-01.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/imagens/9-01.png", "image/png");
  });

  server.on("/imagens/gif2.gif", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/imagens/gif2.gif", "image/gif");
  });

  server.on("/imagens/index.gif", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/imagens/index.gif", "image/gif");
  });

  server.on("/imagens/menu.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/imagens/menu.png", "image/png");
  });

  server.on("/imagens/perfil/foto_perfil.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/imagens/perfil/foto_perfil.png", "image/png");
  });

  // Irá captar o estado do botão da página inicial (Ligado/Desligado)
  server.on("/estadoCafe", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    String inputParam;
    if (request->hasParam(PARAM_INPUT)) {
      inputMessage = request->getParam(PARAM_INPUT)->value();
      inputParam = PARAM_INPUT;

      if(inputMessage == "1"){
        Serial.print("\nLigando! ");
        digitalWrite(rele, HIGH);
      }
      else{
        Serial.print("\nDesligando! ");
        digitalWrite(rele, LOW);
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

    Serial.println("Horário definido no Web Server: *" + hh + ":" + mm + "*");
    
    request->send(LittleFS, "/agendar.html", String(), false, configAgendamento);
  });

  // Irá apagar o agendamento atual
  server.on("/apagar", HTTP_GET, [] (AsyncWebServerRequest *request) {    

    hh = "";
    mm = "";
    request->send(LittleFS, "/agendar.html", String(), false, configAgendamento);
  });

  server.on("/opcao", HTTP_GET, [] (AsyncWebServerRequest *request){
    String msg = request->getParam("Tempo")->value();
    duracao_Cafe = (msg.toInt() * 60) * 1000;
    Serial.print("Cafeteira ligada! Tempo restante: ");
    Serial.print(msg);
    Serial.println(" minutos.");
    
    request->send(LittleFS, "/config.html", String(), false);
  });

  server.on("/atualizarCafe", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", String(digitalRead(rele)).c_str());
  });

  server.on("/agendamento", HTTP_GET, [](AsyncWebServerRequest *request){
    String msg = montarAgenda();
    request->send(200, "text/plain", msg);
  });

  server.on("/cancelar", HTTP_GET, [](AsyncWebServerRequest *request){
    String msg = botaoCancelar();
    request->send(200, "text/plain", msg);
  });

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
  Serial.print("Wi-fi conectado! Acesse '");
  Serial.print(WiFi.localIP());
  Serial.println("/index.html' para entrar!");
  Serial.print("Canal Wi-Fi utilizado: ");
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
