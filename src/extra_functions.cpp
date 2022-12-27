#include <Arduino.h>
#include <Hash.h>

#include <NTPClient.h>
#include <ESPAsyncTCP.h>
#include "ESPAsyncWebServer.h"
#include <ESP8266mDNS.h>

// Declarações dos headers
#include <global_variables.h>
#include <interface.h>

const char *ssid = "ssid";
const char *senha = "password";

char diasDaSemana[7][14] = {"Domingo", "Segunda-feira", "Terça-feira", "Quarta-feira", "Quinta-feira", "Sexta-feira", "Sábado"};
char mesesCorretos[12][10] = {"Janeiro", "Fevereiro", "Março", "Abril", "Maio", "Junho", "Julho",
                              "Agosto", "Setembro", "Outubro", "Novembro", "Dezembro"};

int dia, mes, ano;

WiFiUDP udp;
NTPClient ntp(udp, "a.st1.ntp.br", -3 * 3600, 60000);

void ajustarHora(){
  ntp.update();
  unsigned long epochTime = ntp.getEpochTime();
  
  struct tm *ptm = gmtime ((time_t *)&epochTime); //Gera o dia, mês e ano
  dia = ptm->tm_mday; //Captura o dia;
  mes = ptm->tm_mon; //Captura o mês + 1;
  ano = ptm->tm_year+1900; //Soma o valor recebido com 1900 (gerando o ano atual)
}

void configurarWifi(){
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, senha); 
  initialSetupDelay = millis();
  Serial.print(" ---> Conectando ao WiFi . . .");
  do{
    if((millis() - initialSetupDelay) >= 100){
      Serial.print(" .");
      initialSetupDelay = millis();
    }
  }while (WiFi.waitForConnectResult() != WL_CONNECTED);
    
  // Printando o IP Local do micropowerPlugIsActiverolador
  Serial.print("\n\n ---> Wi-fi conectado com sucesso. Acesse o endereço raíz: \"");
  Serial.print(WiFi.localIP());
  Serial.println("/index.html\".");
  Serial.print(" --> Canal Wi-Fi utilizado: ");
  Serial.println(WiFi.channel());
  
  // Iniciando a configuração do protocolo NTP
  ntp.begin(); //Inicia o NTP.
  ntp.forceUpdate(); //Força o Update.
}

int getNTPValue(String option) {
  if (option.equals("hours")) {
    return ntp.getHours();
  } else if (option.equals("minutes")) {
    return ntp.getMinutes();
  } else if (option.equals("seconds")) {
    return ntp.getSeconds();
  }
  return 0;
}