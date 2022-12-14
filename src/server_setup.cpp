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

// Definindo a porta D6 para o relê (GPIO12)
#define rele 12

AsyncWebServer server(80);

void requestImageToServer(String path, int countFiles, String imageFileExtension) {
  for (int numFile = 1; numFile <= countFiles; numFile ++) {
    static String fullPath;
    static String imageFileType;
    
    fullPath = path + "/" + (String)numFile + "." + imageFileExtension;
    imageFileType = "image/" + imageFileExtension;

    server.on(fullPath.c_str(), HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(LittleFS, fullPath.c_str(), imageFileType.c_str());
    });
  }
}

void requestFileToServer(String path) {
  String lastPath = path;
  Dir dir = LittleFS.openDir(path.c_str());
  String fileName = dir.fileName();
  while(dir.next()) {
    if (dir.isDirectory()) {
      path += (path.equals("/")) ? dir.fileName() : "/" + dir.fileName();
      requestFileToServer(path);
      path = lastPath;
    } else if (dir.isFile()) {
      static String fullPath, fileExtension;
      fullPath = path + dir.fileName();
      fileExtension = dir.fileName().substring(dir.fileName().indexOf(".") + 1);
      if(fileExtension.equals("html") || fileExtension.equals("css") || fileExtension.equals("js")) {
        fileExtension = "text/" + fileExtension;
      } else if(fileExtension.equals("png") || fileExtension.equals("jpg") || fileExtension.equals("gif") || fileExtension.equals("jpeg")) {
        fileExtension = "image/" + fileExtension;
      } else if(fileExtension.equals("mp3") || fileExtension.equals("ogg") || fileExtension.equals("wav") || fileExtension.equals("wma")) {
        fileExtension = "audio/" + fileExtension;
      } else if(fileExtension.equals("mp4") || fileExtension.equals("avi") || fileExtension.equals("m4v") || fileExtension.equals("mpg") || fileExtension.equals("mpeg")) {
        fileExtension = "video/" + fileExtension;
      server.on(fullPath.c_str(), HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(LittleFS, fullPath.c_str(), fileExtension.c_str());
      });
      }
    }
  }
}

//Configurando a informação dinâmica do estado do botão
String configSwitch(const String& var){
  String estado = "";
  if(var == "ESTADODORELE"){
    if(!digitalRead(rele)){
      estado += "<input type=\"checkbox\" class=\"liga-desliga__checkbox checkCafe\" id=\"liga-desliga\" onclick=\"botaoCafe();\" onchange=\"mudarEstadoCafe(this)\"/>";
    }else{
      estado += "<input type=\"checkbox\" class=\"liga-desliga__checkbox checkCafe\" id=\"liga-desliga\" onclick=\"botaoCafe();\" onchange=\"mudarEstadoCafe(this)\" checked/>";
    }
  }
  return estado;
}

/*String botaoCancelar(){
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
}*/

/*String configAgendamento(const String& var){
  if(var == "ESTADOAGENDAMENTO"){
    String msg = montarAgenda();
    return msg;
  }
  if(var == "BOTAOCANCELAR"){
    String msg = botaoCancelar();
    return msg;
  }
  return String();
}*/

void iniciarWebServer(){
  // Carregando os documentos HTML
  /*server.on("/index.html", HTTP_GET, [](AsyncWebServerRequest *request){ // Página principal da Cafeteira
    request->send(LittleFS, "/index.html", String(), false, configSwitch);
  });

  server.on("/agendar.html", HTTP_GET, [](AsyncWebServerRequest *request){ // Página do Agendamento da Cafeteira
    request->send(LittleFS, "/agendar.html", String(), false);
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
  });*/

  // Carregando os documentos de imagem
  requestFileToServer("/");

  /*server.on("/icons/outros/delete.png", HTTP_GET, [](AsyncWebServerRequest *request){
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
  });*/

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

  
  /*// Irá receber os dados do horário para agendamento
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
  });*/

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

  /*server.on("/agendamento", HTTP_GET, [](AsyncWebServerRequest *request){
    String msg = montarAgenda();
    request->send(200, "text/plain", msg);
  });*/

  /*server.on("/cancelar", HTTP_GET, [](AsyncWebServerRequest *request){
    String msg = botaoCancelar();
    request->send(200, "text/plain", msg);
  });*/

  server.begin();
}