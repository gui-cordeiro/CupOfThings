# ☕ ProjetoCafe-IoT 📱
 Automatize sua cafeteira e tome um bom café sem preocupações!

## 📕 Sobre o projeto
**ProjetoCafe-IoT** é um projeto que visa facilitar o dia a dia das pessoas, automatizando e simplificando o preparo do tão amado **cafézinho preto**.

## ⁉️ Como o projeto funciona?
O **ProjetoCafe-IoT** consiste numa tomada inteligente controlada por um microcontrolador **ESP-8266**. Esta, por sua vez, controla um relé que é o responsável pela **continuidade** ou **interrupção** da corrente elétrica da tomada.

Para controlar tal relé, o **ESP-8266** também é configurado para trabalhar como um ***Web Server***, hospedando um site projetado especialmente para **dispositivos móveis** e que fará a comunicação entre as **ações do usuário** e o **microcontrolador**.

Neste site, é possível acionar remotamente a cafeteira de **maneira imediata** ou **mediante agendamento prévio**.

## 🛠️ Linguagens e ferramentas utilizadas no desenvolvimento do projeto
<a href="https://developer.mozilla.org/pt-BR/docs/Web/HTML/Reference"><img alt="Ícone da HTML" width="60em" src="https://github.com/devicons/devicon/blob/master/icons/html5/html5-original.svg"/></a>
<a href="#"><img alt="Ícone transparente" width="10em" height="20em" src="https://user-images.githubusercontent.com/83349463/151667763-eed1f9e2-ff23-462e-be41-a6046ad8434a.png"/></a>
<a href="https://developer.mozilla.org/pt-BR/docs/Web/CSS/Reference"><img alt="Ícone das CSS" width="60em" src="https://github.com/devicons/devicon/blob/master/icons/css3/css3-original.svg"/></a>
<a href="#"><img alt="Ícone transparente" width="10em" height="20em" src="https://user-images.githubusercontent.com/83349463/151667763-eed1f9e2-ff23-462e-be41-a6046ad8434a.png"/></a>
<a href="https://developer.mozilla.org/pt-BR/docs/Web/JavaScript/Reference"><img alt="Ícone do JavaScript" width="60px" src="https://github.com/devicons/devicon/blob/master/icons/javascript/javascript-original.svg"/></a>
<a href="#"><img alt="Ícone transparente" width="10em" height="20em" src="https://user-images.githubusercontent.com/83349463/151667763-eed1f9e2-ff23-462e-be41-a6046ad8434a.png"/></a>
<a href="https://www.arduino.cc/reference/pt/"><img alt="Ícone do Arduino" width="60em" src="https://github.com/devicons/devicon/blob/master/icons/arduino/arduino-original.svg"/></a>
<a href="#"><img alt="Ícone transparente" width="10em" height="20em" src="https://user-images.githubusercontent.com/83349463/151667763-eed1f9e2-ff23-462e-be41-a6046ad8434a.png"/></a>
<a href="https://platformio.org/"><img alt="Ícone do PlatformIO" width="60em" src="https://github.com/gui-cordeiro/ProjetoCafe-IoT/assets/83349463/0bc1ed91-5989-49aa-8849-cb53f44925a5"/></a>

## 🖼️ Galeria do projeto
Veja abaixo algumas das fotos do projeto. As imagens do *Web Server* foram capturadas no Google Chrome, com a configuração de prévia de visualização baseada no **Google Pixel 5**:

### 1) Página principal do *Web Server*
![Design_sem_nome-removebg](https://github.com/gui-cordeiro/ProjetoCafe-IoT/assets/83349463/4d592135-f26d-4cb9-9182-abfc798289a0)

### 2) Página de agendamento do *Web Server*
![Design_sem_nome__1_-removebg](https://github.com/gui-cordeiro/ProjetoCafe-IoT/assets/83349463/481e3c67-2fdc-48b6-bb95-686ac8ba4c44)

### 3) Menu de navegação do *Web Server*
![07](https://github.com/gui-cordeiro/ProjetoCafe-IoT/assets/83349463/e24aa45a-8e32-4cc3-a9a1-43d4e056ab35)


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
