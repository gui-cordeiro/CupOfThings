let actualHTMLFile = window.location.pathname.split("/")[window.location.pathname.split("/").length - 1]

if(actualHTMLFile == "index.html"){
    document.querySelector("input#switch").addEventListener("click", infoTempoCafe)
}

let actualCoffeeMachineState = 0 // 0) Desligado | 1) Ligado | 2) Café pronto
let switchBtnIsLocked = false // 0) Botão switch desbloqueado | 1) Botão switch bloqueado

let contagem;
let flagShutdown;

let minutesFullTime = 1, secondsFullTime = 0
let totalFullTime = secondsFullTime + (minutesFullTime * 60)

let minutesLeftTime = 0, secondsLeftTime = 0
let totalLeftTime = secondsLeftTime + (minutesLeftTime * 60)

const hourShutdown = 22, minShutdown = 8

let tmp_div = document.querySelector("div#tempo_popup")

let msgInicial = tmp_div.innerHTML

let msgOn = "<div id=\"conteudo\"><p>Preparando seu café! Tempo restante: <span class=\"tempoRest\">01:30</span></p><div id=\"cobrir\"><div class=\"circular\"><div class=\"inner\"></div><div class=\"numb\">01:30</div><div class=\"circle\"><div class=\"bar left\"><div class=\"progress\"></div></div><div class=\"bar right\"><div id=\"delay\" class=\"progress\"></div></div></div></div></div><p id=\"msg\">Enquanto isso, aproveite para escutar uma musiquinha!</p><div id=\"spotify\"><iframe src=\"https://open.spotify.com/embed/playlist/37i9dQZF1DWWQRwui0ExPn\" width=\"320\" height=\"320\" frameborder=\"0\" allowtransparency=\"true\" allow=\"encrypted-media\"></iframe></div></div>"

let msgPronto = "<p>Seu café está pronto! ☕</p><div id=\"imgPronto\"><img src=\"imagens/9-01.jpg\" alt=\"\"></div><div id=\"lembrete\">Lembre-se de tomar um café da manhã reforçado. Aproveite seu café e tenha um bom dia!</div><div id=\"autoOff\">Sua cafeteira desligará automaticamente às: <div id=\"autoOffHora\">"
msgPronto += hourShutdown < 10 ? "0" + hourShutdown + ":" : hourShutdown + ":"
msgPronto += minShutdown < 10 ? "0" + minShutdown + "</div></div>" : minShutdown + "</div></div>"

/*******************************************************************************/
if(actualCoffeeMachineState == 1){
    if (actualHTMLFile == "index.html") document.querySelector("input#switch").checked = true
    tmp_div.style.backgroundColor = "#a53860d2"
    tmp_div.style.color = "#e7d8c9"
    document.getElementById("conteudo").style.visibility = ""
    setFooterBarTimer()
} else if(actualCoffeeMachineState == 2){
    if (actualHTMLFile == "index.html") document.querySelector("input#switch").checked = true
    tmp_div.style.backgroundColor = "#55d06ad2"
    tmp_div.style.color = "#2c2c2c"
    tmp_div.innerHTML = `${msgPronto}`
    telaPronto()
} else {
    actualCoffeeMachineState = 0
    document.querySelector("input#switch").checked = false
}


/*******************************************************************************/

//Função que verificará o actualCoffeeMachineState do Switch, para que seja realizada alguma ação com a barra inferior
function infoTempoCafe(){
    if(switchBtnIsLocked == true) {
        document.querySelector("input#switch").checked = !document.querySelector("input#switch").checked
    } else {
        if(document.querySelector("input#switch").checked){
            actualCoffeeMachineState = 1
            changeFooterBarState("#a53860d2", "#e7d8c9", msgOn)
            setFooterBarTimer()
        }else{
            actualCoffeeMachineState = 0
            clearInterval(contagem)
            clearInterval(flagShutdown)
            changeFooterBarState("#35353594", "#e7d8c9", msgInicial)
        }
    }
}

/*******************************************************************************/

// Changing the footer bar content
function changeFooterBarState(footerBarBGColor, footerBarColor, footerBarContent){
    switchBtnIsLocked = true
    for (let footerBarHeight = 35; footerBarHeight > - 10; footerBarHeight --) {
        tmp_div.style.height = `${footerBarHeight}px`
    }
    let changeContent = setTimeout(() => {
        tmp_div.style.backgroundColor = footerBarBGColor
        tmp_div.style.color = footerBarColor
        tmp_div.innerHTML = `<p>${footerBarContent}</p>`
    }, 400)
    let subir = setTimeout(() => {
        for (let footerBarHeight = -10; footerBarHeight < 35; footerBarHeight ++) {
            tmp_div.style.height = `${footerBarHeight}px`
        }
        switchBtnIsLocked = false
    }, 600)
}


/*******************************************************************************/

//Função que irá expandir a barra inferior
tmp_div.addEventListener("mouseenter", mouseEnter)

function mouseEnter(){
    if(!document.querySelector("input#menu").checked){
        document.body.style.overflow = "hidden"
        if(actualCoffeeMachineState == 0){
            tmp_div.style.height = "450px"
            document.querySelector("div#info").style.opacity = "1"
            document.querySelector("div#social").style.opacity = "1"
        }else if(actualCoffeeMachineState == 1){
            tmp_div.style.height = "450px"
            document.querySelector("div#cobrir").style.opacity = "1"
            document.querySelector("p#msg").style.opacity = "1"

            let circle = document.getElementsByClassName("progress")
            circle[0].style.animationDuration = `${totalFullTime / 2}s`
            circle[1].style.animationDuration = `${totalFullTime / 2}s`
            document.getElementById("delay").style.animationDelay = `${totalFullTime / 2}s`
            
            // circle[0].style.animationDuration = `${totalLeftTime / 2}s`
            // circle[1].style.animationDuration = `${totalLeftTime / 2}s`
            // document.getElementById("delay").style.animationDelay = `${totalLeftTime / 2}s`
        }else if(actualCoffeeMachineState == 2){
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
        if(actualCoffeeMachineState == 0){
            document.querySelector("div#info").style.opacity = "0"
            document.querySelector("div#social").style.opacity = "0"
        }else if(actualCoffeeMachineState == 1){
            document.querySelector("div#cobrir").style.opacity = "0"
            document.querySelector("p#msg").style.opacity = "0"
        }else if(actualCoffeeMachineState == 2){
            document.querySelector("div#imgPronto").style.opacity = "0"
            document.querySelector("div#lembrete").style.opacity = "0"
            document.querySelector("div#autoOff").style.opacity = "0"
        }
    }
}

/*******************************************************************************/

function setFooterBarTimer(){
    let minTimer = minutesFullTime, segTimer = secondsFullTime
    let msg = ""
    contagem = setInterval(() => {
        if(minTimer == 0 && segTimer == 0) {
            actualCoffeeMachineState = 2
            changeFooterBarState("#55d06ad2", "#2c2c2c", msgPronto)
            telaPronto()
            clearInterval(contagem)
        } else {
            if(segTimer == 0) {
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
            actualCoffeeMachineState = 0
            changeFooterBarState("#35353594", "#e7d8c9", msgInicial)
            clearInterval(flagShutdown)
        }
    }, 5000)
    
}