// *****************************************************************************
// *                                                                           *
// *                                                                           *
// *                    * Função do relógio no cabeçalho *                     *
// *                                                                           *
// *                                                                           *
// *****************************************************************************



function relogio() {
    var dataXMLHttp = new XMLHttpRequest();
    dataXMLHttp.onreadystatechange = function(){
        if(this.readyState == 4 && this.status == 200){
            var msg = this.response;
            document.getElementById('relogio').innerHTML = msg;
        }
    }
    dataXMLHttp.open("GET", "/data", true);
    dataXMLHttp.send();
}
window.setInterval("relogio()", 500);



// *****************************************************************************
// *                                                                           *
// *                                                                           *
// *                    * Funções da página da cafeteira *                     *
// *                                                                           *
// *                                                                           *
// *****************************************************************************
// *                                                                           *
// *                      1) Funções da página inicial                         *
// *                                                                           *
// *****************************************************************************




// Irá mudar o "Status" da Cafeteira de acordo com o estado atual do botão ("Ligado" caso esteja marcado, "Desligado" caso esteja desmarcado)
function botaoCafe(){
    if(document.getElementsByClassName('checkCafe').checked){
        document.getElementById('estadoCafe').innerHTML = '"Ligado"';
        document.getElementById('estadoCafe').style.color = '#900';
    }else{
        document.getElementById('estadoCafe').innerHTML = '"Desligado"';
        document.getElementById('estadoCafe').style.color = '#4f4f4f';
    }
}


//Assim que o botão for pressionado, irá realizar uma solicitação XML para informar ao NodeMCU se ele deve acionar ou não o módulo Relê da Cafeteira.
function mudarEstadoCafe(element) {
    var msg = new XMLHttpRequest();
        if(element.checked){
            msg.open("GET", "/estadoCafe?rele=1", true);
        }else{
            msg.open("GET", "/estadoCafe?rele=0", true);
        }
    msg.send();
}


// Irá mudar, em tempo real, o "Status" da Cafeteira de acordo com o estado atual do botão ("Ligado" caso esteja marcado, "Desligado" caso esteja desmarcado)
function atualizarCafe(){
    var botaoCafeXMLHttp = new XMLHttpRequest();
    botaoCafeXMLHttp.onreadystatechange = function(){
        if(this.readyState == 4 && this.status == 200){
            if(this.responseText == 1){
                document.getElementById('liga-desliga').checked = true;
                document.getElementById('estadoCafe').innerHTML = '"Ligado"';
                document.getElementById('estadoCafe').style.color = '#900';
            }else{
                document.getElementById('liga-desliga').checked = false;
                document.getElementById('estadoCafe').innerHTML = '"Desligado"';
                document.getElementById('estadoCafe').style.color = '#4f4f4f';
            }
        }
    }
    botaoCafeXMLHttp.open("GET", "/atualizarCafe", true);
    botaoCafeXMLHttp.send();
}
window.setInterval("atualizarCafe()", 750); 




// *****************************************************************************
// *                                                                           *
// *                   2) Funções da página de agendamento                     *
// *                                                                           *
// *****************************************************************************




// Irá manter atualizado em tempo real o estado atual do próximo agendamento ("Sem horário", "Hoje, às[...]" ou "Amanhã, às[...]")
function atualizarAgendamento(){
    var agendaXMLHttp = new XMLHttpRequest();
    agendaXMLHttp.onreadystatechange = function(){
        if(this.readyState == 4 && this.status == 200){
            var msg = this.response;
            document.getElementById('acionamento').innerHTML = msg;
            atualizarApagar();
        }
    }
    agendaXMLHttp.open("GET", "/agendamento", true);
    agendaXMLHttp.send();
}
window.setInterval("atualizarAgendamento()", 500);


// Irá manter atualizado o estado do botão de "Cancelar agendamento", de acordo com o estado atual do próximo agendamento
function atualizarApagar(){
    var botaoCancXMLHttp = new XMLHttpRequest();
    botaoCancXMLHttp.onreadystatechange = function(){
        if(this.readyState == 4 && this.status == 200){
            var msg = this.response;
            document.getElementById('botaoCancelar').innerHTML = msg;
        }
    }
    botaoCancXMLHttp.open("GET", "/cancelar", true);
    botaoCancXMLHttp.send();
}


// Irá "montar" e mostrar ao usuário o alerta para apagar o agendamento atual
function apagar(){
    var confirmacao = confirm("Tem certeza de que quer cancelar o agendamento atual?");
    if(confirmacao == true){
        var enviarHTTP = new XMLHttpRequest();
        enviarHTTP.open("GET", "/apagar", true);
        enviarHTTP.send();
        alert("O agendamento foi cancelado com sucesso!");
    }else{
        return;
    }
}


// Irá "montar" e mostrar ao usuário o alerta para criar um novo agendamento
function enviar(){
    var input_L = document.getElementsByClassName('Lembrar');
    var input_T = document.getElementsByClassName('hora');
    var horario = input_T[0].value;
    var hora_str = "";
    var minuto_str = "";
    var hora = 0;
    var minuto = 0;
    var requisicao = "/agendar?"; // Irá receber todo o texto para a futura requisição HTTP
    var msgconfig = "Deseja criar um novo agendamento com estas configurações?\n\nHorário: * ";
        
    //Verificando se o usuário selecionou algum horário
    if(horario == ""){
        alert("Você não selecionou nenhum horário!");
        return;
    }
        
    var horario_array = horario.split('');
          
    for(var i = 0; i < 2; i ++){
        hora_str += horario_array[i];
        minuto_str += horario_array[i + 3];
    }
        
    hora = parseInt(hora_str);
    minuto = parseInt(minuto_str);

    if(hora < 10){
        msgconfig += "0";
    }
    msgconfig += "" + hora + ":";

    if(minuto < 10){
        msgconfig += "0";
    }
    msgconfig += "" + minuto + " *\nLembrete: ";
        
    requisicao += "Hora=" + hora + "&Minuto=" + minuto;
        
    if(input_L[0].checked == true){
        requisicao += "&" + input_L[0].name + "=on";
        msgconfig += "* Ativado *";
    }else{
        requisicao += "&" + input_L[0].name + "=off";
        msgconfig += "* Desativado *";
    }
          
    var confirmacao = confirm(msgconfig);
    if(confirmacao){
        //alert(requisicao);
        var enviarHTTP = new XMLHttpRequest();
        enviarHTTP.open("GET", requisicao, true);
        enviarHTTP.send();
        alert("O seu horário foi agendado com sucesso!");
        return;
    }
}




// *****************************************************************************
// *                                                                           *
// *                  3) Funções da página de configurações                    *
// *                                                                           *
// *****************************************************************************




function mostraCampo(obj){
    var select = document.getElementById("tempo");
    var txt = document.getElementById("opcao");
    txt.style.display = (select.value == 0)?"block":"none";
}


function salvarOpcoes(){
    //var horas = 0;
    var min = document.getElementById("tempo");
    var minutos = min.options[min.selectedIndex].value;
    var opcaoHTTP = new XMLHttpRequest();
    var msg = "/opcao?";
    var pergunta = "Deseja salvar estas configurações?";
    if(minutos == 0){
        //horas = document.getElementById("horasOpcao").value;
        minutos = document.getElementById("minutosOpcao").value;
        if(minutos == 0 || minutos == ""){
            alert("O campo de tempo personalizado está vazio!");
            return;
        }
        //horas = parseInt(horas);
        minutos = parseInt(minutos);
        //minutos += (horas * 60);
    }
    msg += "Tempo=" + minutos;
    var opcao = confirm(pergunta);
    if(opcao){
        opcaoHTTP.open("GET", msg, true);
        opcaoHTTP.send();
        alert("Configurações salvas com sucesso!");
        return;
    } 
}

function anoAtual(){
    document.querySelector("div#foot").innerHTML = `<em><p>Guilherme Cordeiro&copy - 2020, ${new Date().getFullYear()}</p></em>`;
}

setTimeout(anoAtual(), 0);