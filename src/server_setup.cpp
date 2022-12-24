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

// Configurando a informação dinâmica do estado do botão

// String configSwitch(const String& var){
//   String estado = "<input type=\"checkbox\" class=\"liga-desliga__checkbox checkCafe\" id=\"liga-desliga\" onclick=\"botaoCafe();\" onchange=\"mudarEstadoCafe(this)\"";
//   if(var == "ESTADODORELE") estado += (digitalRead(rele)) ? " checked/>" : "/>";
//   return estado;
// }

// String botaoCancelar(){
//   return ((hh != "") && (mm != "")) ? "<button class=\"btn btn-gray\" onclick=\"apagar();\">Cancelar agendamento</button>" : "<span class=\"Selec\" style=\"position: relative; top: 10px;\">Cancelar agendamento</span>";;
// }

// String montarAgenda(){
//   String msg;
//   if((hh == "")||(mm == "")){
//     msg += "<h2>* Sem horário *</h2>";
//   }else{
//     msg += ((hora_I <= hh) && (minuto_I < mm)) ? "<h2>* Hoje, às " : "<h2>* Amanhã, às ";
//     msg += (hh.toInt() > 0 && hh.toInt() < 10) ? "0" + hh + ":" : hh + ":";     
//     msg += (mm.toInt() > 0 && mm.toInt() < 10) ? "0" + mm + "*<h2>" : mm + "*<h2>"; 
//   }
//   return msg;
// }

// String configAgendamento(const String& var){
//   if(var == "ESTADOAGENDAMENTO") return montarAgenda();
//   else if(var == "BOTAOCANCELAR") return botaoCancelar();
//   return "";
// }

void iniciarWebServer(){
  // Preparando os documentos HTML
  server.on("/index.html", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/index.html", String(), false/*, configSwitch*/);
  });
  server.on("/agendar.html", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/agendar.html", String(), false);
  });

  // Preparando os documentos CSS
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

  // Preparando os documentos Javascript
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

  // Preparando os arquivos de imagem
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
  server.on("/imagens/9-01.jpg", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/imagens/9-01.jpg", "image/jpg");
  });
  server.on("/imagens/menu.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/imagens/menu.png", "image/png");
  });
  server.on("/imagens/perfil/Gui.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/imagens/perfil/Gui.png", "image/png");
  });
  
  // Preparando os arquivos de vídeo
  server.on("/imagens/agendar.mp4", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/imagens/agendar.mp4", "video/mp4");
  });
  server.on("/imagens/index.mp4", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/imagens/index.mp4", "video/mp4");
  });


  // Preparando a requisição que sincronizará o botão na página inicial
  server.on("/estadoCafe", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    //String inputParam;
    if (request->hasParam(PARAM_INPUT)) {
      inputMessage = request->getParam(PARAM_INPUT)->value();
      //inputParam = PARAM_INPUT;
      if(inputMessage == "1"){
        Serial.print("\nLigando! ");
        digitalWrite(rele, HIGH);
      } else if (inputMessage == "0"){
        Serial.print("\nDesligando! ");
        digitalWrite(rele, LOW);
        powerPlugIsActive = false;
      }
    } else {
      Serial.println("Nenhuma mensagem foi enviada... Algum problema ocorreu!");
      //inputParam = "";
    }
  });

  
  // // Preparando a requisição de dados para agendamento
  // server.on("/agendar", HTTP_GET, [] (AsyncWebServerRequest *request) {    
  //   hh = request->getParam("Hora")->value();
  //   mm = request->getParam("Minuto")->value();
  //   input = request->getParam("Lembrete")->value();
  //   Serial.println("Horário definido no Web Server: *" + hh + ":" + mm + "*");
  //   request->send(LittleFS, "/agendar.html", String(), false, configAgendamento);
  // });

  // // Irá apagar o agendamento atual
  // server.on("/apagar", HTTP_GET, [] (AsyncWebServerRequest *request) {    
  //   hh = "";
  //   mm = "";
  //   request->send(LittleFS, "/agendar.html", String(), false, configAgendamento);
  // });

  // Preparando a requisição do tempo desejado de preparo da cafeteira
  server.on("/opcao", HTTP_GET, [] (AsyncWebServerRequest *request){
    String msg = request->getParam("Tempo")->value();
    coffeeDuration = (msg.toInt() * 60) * 1000;
    Serial.print("Cafeteira ligada! Tempo restante: ");
    Serial.print(msg);
    Serial.println(" minutos.");
    
    request->send(LittleFS, "/config.html", String(), false);
  });

  // Preparando a rotina de atualização do estado atual da cafeteira
  server.on("/atualizarCafe", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", String(digitalRead(rele)).c_str());
  });

  // server.on("/agendamento", HTTP_GET, [](AsyncWebServerRequest *request){
  //   String msg = montarAgenda();
  //   request->send(200, "text/plain", msg);
  // });

  // server.on("/cancelar", HTTP_GET, [](AsyncWebServerRequest *request){
  //   String msg = botaoCancelar();
  //   request->send(200, "text/plain", msg);
  // });

  // Inicializando o servidor, devidamente configurado
  server.begin();
}