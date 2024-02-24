/*******************************************************************************/

document.querySelector("input#menu").addEventListener("click", scroll)

document.querySelector("div#btn_reset").addEventListener("click", resetWebServer)

/*******************************************************************************/

window.addEventListener("load", function fadeOut(){
    processa(0.1, 100, 0)
})

function processa(time, initial, end){
    var tela = document.getElementById("telaloader")
    opt = initial
    intervalo = setInterval(function(){
        if(opt > end){
            opt -= 1
            tela.style.opacity = `${opt}%`
        }else{
            tela.style.display = "none"
            clearInterval(intervalo)
        }
    }, time)
}

function scroll(){
    if(document.querySelector("input#menu").checked){
        document.body.style.overflow = "hidden"
    }else{
        document.body.style.overflow = "visible"     
    }
}

function resetWebServer() {
    alert("Caso esteja presenciando instabilidades neste site, você pode reiniciar o sistema da tomada inteligente da cafeteira para tentar resolver estes problemas.\n\nSe os problemas persistirem, entre em contato com o desenvolvedor.")
    let opt = confirm("Você está prestes a reiniciar o sistema da tomada inteligente da cafeteira.\n\nDeseja MESMO prosseguir?");
    if (opt) alert("O sistema será reiniciado em instantes. Aguarde alguns minutos para que o site esteja disponível novamente.")
}