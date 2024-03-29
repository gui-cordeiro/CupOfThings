/*******************************************************************************/

document.querySelector("input#menu").addEventListener("click", scroll)

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