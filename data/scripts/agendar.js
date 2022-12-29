const openModalButton = document.querySelector("div#modal-new-schedule");
const closeModalButton = document.querySelector("div#agendar-cancel");
const modal = document.querySelector("div#modal");
const fade = document.querySelector("div#fade");

const toggleModal = () => {
    [modal, fade].forEach((el) => el.classList.toggle("hide"));
}

[openModalButton, closeModalButton, fade].forEach((el) => {
    el.addEventListener("click", () => toggleModal());
});

let stateNRadio = document.querySelector("input#form-radio-no")
let stateYRadio = document.querySelector("input#form-radio-yes")

stateNRadio.addEventListener("click", showInfoDay)
stateYRadio.addEventListener("click", hideInfoDay)

document.querySelector("input#slider").addEventListener("input", updateInfo)

document.querySelector("input#form-radio-no").checked = "true"

function showInfoDay() {
    if (stateNRadio.checked) {
        document.getElementById("form-days").style.display = "none";
        document.querySelector("div#agendar-cancel").style.bottom = "312px"
        
    }
}

function hideInfoDay() {
    if (stateYRadio.checked) {
        document.getElementById("form-days").style.display = "block";
        document.querySelector("div#agendar-cancel").style.bottom = "477px"
    }
}

function updateInfo() {
    if(document.querySelector("input#slider").value == "0") {
        document.querySelector("div#form-description").innerHTML = "A) Fins de Semana"
    } else if(document.querySelector("input#slider").value == "1") {
        document.querySelector("div#form-description").innerHTML = "B) Nos Dias Úteis"
    } else if(document.querySelector("input#slider").value == "2") {
        document.querySelector("div#form-description").innerHTML = "C) Todos os Dias"
    }
}