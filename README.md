# ☕ ProjetoCafe-IoT 📱
 Automatize sua cafeteira e tome um bom café sem preocupações!

## 📕 Sobre o projeto
**ProjetoCafe-IoT** é um projeto que visa facilitar o dia a dia das pessoas, automatizando e simplificando o preparo do tão amado **cafézinho preto**.

## ⁉️ Como o projeto funciona?
O **ProjetoCafe-IoT** consiste numa tomada inteligente controlada por um microcontrolador **ESP-8266**. Esta, por sua vez, controla um relé que é o responsável pela **continuidade** ou **interrupção** da corrente elétrica da tomada.

Para controlar tal relé, o **ESP-8266** também é configurado para trabalhar como um ***Web Server***, hospedando um site projetado especialmente para **dispositivos móveis** e que fará a comunicação entre as **ações do usuário** e o **microcontrolador**.

Neste site, é possível acionar remotamente a cafeteira de **maneira imediata** ou **mediante agendamento prévio**.

## 🖼️ Galeria do projeto
Veja abaixo algumas das fotos do projeto:

### 1) Página principal do *Web Server*
(foto)

### 2) Página de agendamento do *Web Server*
(foto)

### 3) Menu de navegação do *Web Server*
(foto)


## 🤝 Como contribuir?
````bash
# 1) Clone o repositório do projeto:
$ git clone https://github.com/gui-cordeiro/ProjetoCafe-IoT.git
````

````bash
# 2) Entre na pasta do repositório:
$ cd ProjetoCafe-IoT
````

````bash
# 3) Altere a branch "main" para "platformio":
$ git checkout "platformio"
````

````bash
# a) Para contrubir com o código-fonte do ESP-8266:
$ cd src
````

````bash
# b) Para contrubir com o código-fonte do Web Server:
$ cd data
````

<hr>