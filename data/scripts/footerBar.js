let actualHTMLFile = window.location.pathname.split("/")[window.location.pathname.split("/").length - 1]

if(actualHTMLFile == "index.html"){
    document.querySelector("input#switch").addEventListener("click", infoTempoCafe)
}

let estado = 2 // 0) Desligado | 1) Ligado | 2) Café pronto
let controle = 0 // 0) Botão switch desbloqueado | 1) Botão switch bloqueado

let contagem;
let flagShutdown;

let min = 0, seg = 8
let tempoTotal = seg + (min * 60)

const hourShutdown = 13, minShutdown = 3

let tmp_div = document.querySelector("div#tempo_popup")

let msgInicial = tmp_div.innerHTML

let msgOn = "<div id=\"conteudo\"><p>Preparando seu café! Tempo restante: <span class=\"tempoRest\">01:30</span></p><div id=\"cobrir\"><div class=\"circular\"><div class=\"inner\"></div><div class=\"numb\">01:30</div><div class=\"circle\"><div class=\"bar left\"><div class=\"progress\"></div></div><div class=\"bar right\"><div id=\"delay\" class=\"progress\"></div></div></div></div></div><p id=\"msg\">Enquanto isso, aproveite para escutar uma musiquinha!</p><div id=\"spotify\"><iframe src=\"https://open.spotify.com/embed/playlist/37i9dQZF1DWWQRwui0ExPn\" width=\"320\" height=\"320\" frameborder=\"0\" allowtransparency=\"true\" allow=\"encrypted-media\"></iframe></div></div>"

let msgPronto = "<p>Seu café está pronto! ☕</p><div id=\"imgPronto\"><img src=\"imagens/9-01.jpg\" alt=\"\"></div><div id=\"lembrete\">Lembre-se de tomar um café da manhã reforçado. Aproveite seu café e tenha um bom dia!</div><div id=\"autoOff\">Sua cafeteira desligará automaticamente às: <div id=\"autoOffHora\">" + hourShutdown + ":" + minShutdown + "</div></div>"

/*******************************************************************************/
if(estado == 0){
    document.querySelector("input#switch").checked = false
}

if(estado == 1){
    document.querySelector("input#switch").checked = true
    tmp_div.style.backgroundColor = "#a53860d2"
    tmp_div.style.color = "#e7d8c9"
    tmp_div.innerHTML = `<p>${msgOn}</p>`
    timer()
}

if(estado == 2){
    document.querySelector("input#switch").checked = true
    tmp_div.style.backgroundColor = "#55d06ad2"
    tmp_div.style.color = "#2c2c2c"
    tmp_div.innerHTML = `<p>${msgPronto}</p>`
    telaPronto()
}

/*******************************************************************************/

//Função que verificará o estado do Switch, para que seja realizada alguma ação com a barra inferior
function infoTempoCafe(){
    if(controle == 0){
        if(document.querySelector("input#switch").checked){
            estado = 1
            mudarEstado(2, 5, 2, "#a53860d2", "#e7d8c9", msgOn)
            timer()
        }else{
            estado = 0
            clearInterval(contagem)
            clearInterval(flagShutdown)
            mudarEstado(2, 5, 2, "#35353594", "#e7d8c9", msgInicial)
        }
    }else{
        document.querySelector("input#switch").checked = !document.querySelector("input#switch").checked
    }
}

/*******************************************************************************/

// Função que irá diminuir a altura da barra inferior ao clicar no botão Switch
function mudarEstado(tmp1, pausa, tmp2, corFundo, cor, msg){
    controle = 1 //Cafeteira ligada
    let alt = 35
    descer = setInterval(function(){
        if(tmp_div.style.height != `0px`){
            tmp_div.style.height = `${alt}px`
            alt --
        }else{
            trocarCont(pausa, tmp2, corFundo, cor, msg)
            clearInterval(descer)
        }
    }, tmp1)
}

// Função que irá alterar o conteúdo e a cor da barra inferior (Continuação da função 'mudarEstado()')
function trocarCont(pausa, tmp2, corFundo, cor, msg){
    cont = 100
    trocar = setInterval(function(){
        if(cont == 0){
            tmp_div.style.backgroundColor = corFundo
            tmp_div.style.color = cor
            tmp_div.innerHTML = `<p>${msg}</p>`
            subirBarra(tmp2)
            clearInterval(trocar)
        }else{
            cont --
        }
    }, pausa)
}

//Função que irá aumentar a altura da barra inferior (Continuação da função 'trocarCont()')
function subirBarra(tmp2){
    let alt = -10
    subir = setInterval(function(){
        if(tmp_div.style.height == `35px`){
            controle = 0
            clearInterval(subir)
        }else{
            tmp_div.style.height = `${alt}px`
            alt ++
        }
    }, tmp2)
}

/*******************************************************************************/

//Função que irá expandir a barra inferior
tmp_div.addEventListener("mouseenter", mouseEnter)

function mouseEnter(){
    if(!document.querySelector("input#menu").checked){
        document.body.style.overflow = "hidden"
        if(estado == 0){
            tmp_div.style.height = "450px"
            document.querySelector("div#info").style.opacity = "1"
            document.querySelector("div#social").style.opacity = "1"
        }else if(estado == 1){
            tmp_div.style.height = "450px"
            document.querySelector("div#cobrir").style.opacity = "1"
            document.querySelector("p#msg").style.opacity = "1"

            let circle = document.getElementsByClassName("progress")
            circle[0].style.animationDuration = `${tempoTotal / 2}s`
            circle[1].style.animationDuration = `${tempoTotal / 2}s`
            document.getElementById("delay").style.animationDelay = `${tempoTotal / 2}s`
        }else if(estado == 2){
            tmp_div.style.height = "450px"
            document.querySelector("div#imgPronto").style.opacity = "1"
            document.querySelector("div#lembrete").style.opacity = "1"
            document.querySelector("div#autoOff").style.opacity = "1"
        }
    }
}

//Função que irá diminuir a barra inferior
tmp_div.addEventListener("mouseleave", mouseLeave)

function mouseLeave(){
    if(!document.querySelector("input#menu").checked){
        //alert(tmp_div.getBoundingClientRect().height)
        document.body.style.overflow = "visible"  
        tmp_div.style.height = "35px"
        if(estado == 0){
            document.querySelector("div#info").style.opacity = "0"
            document.querySelector("div#social").style.opacity = "0"
        }else if(estado == 1){
            document.querySelector("div#cobrir").style.opacity = "0"
            document.querySelector("p#msg").style.opacity = "0"
        }else if(estado == 2){
            document.querySelector("div#imgPronto").style.opacity = "0"
            document.querySelector("div#lembrete").style.opacity = "0"
            document.querySelector("div#autoOff").style.opacity = "0"
        }
    }
}

/*******************************************************************************/

function timer(){
    let minTimer = min, segTimer = seg
    let msg = ""
    contagem = setInterval(() => {
        if(minTimer == 0 && segTimer == 0) {
            estado = 2
            mudarEstado(2 ,5 ,2, "#55d06ad2", "#2c2c2c", msgPronto)
            telaPronto()
            clearInterval(contagem)
        } else {
            if(segTimer == 00) {
                minTimer --
                segTimer = 60
            }
            segTimer --
            
            if(minTimer < 10) msg += "0"
            msg += `${minTimer}:`
            if(segTimer < 10) msg += "0"
            msg += `${segTimer}`

            document.querySelector(".tempoRest").textContent = msg
            document.querySelector(".numb").textContent = msg
            msg = ""
        }
    }, 1000)
}

function telaPronto(){
    let date 
    flagShutdown = setInterval(() => {
        date = new Date()
        if (date.getHours() == hourShutdown && date.getMinutes() == minShutdown) {
            if(actualHTMLFile == "index.html"){
                document.querySelector("input#switch").checked = false
            }
            estado = 0
            mudarEstado(2, 5, 2, "#35353594", "#e7d8c9", msgInicial)
            clearInterval(flagShutdown)
        }
    }, 5000)
    
}