const openModalButton = document.querySelector("div#modal-new-schedule");
const closeModalButton = document.querySelector("div#create-cancel");
const modal = document.querySelector("div#modal");

const openEraseModalButton = document.querySelector("div#modal-delete-schedule");
const closeEraseModalButton = document.querySelector("div#erase-cancel");
const eraseModal = document.querySelector("div#modal-erase");

const fade = document.querySelector("div#fade");
const fade2 = document.querySelector("div#fade2");

// const toggleModal = () => {
//     [modal, eraseModal, fade, fade2].forEach((el) => el.classList.toggle("hide"));
// }

// [openModalButton,openEraseModalButton, closeModalButton, closeEraseModalButton, fade, fade2].forEach((el) => {
//     el.addEventListener("click", () => toggleModal());
// });

[openModalButton, closeModalButton].forEach((element) => {
    element.addEventListener("click", () => {
        modal.classList.toggle("hide")
        fade.classList.toggle("hide")
    });
});

openEraseModalButton.addEventListener("click", () => {
    eraseModal.classList.toggle("hide")
    fade.classList.toggle("hide")
});
closeEraseModalButton.addEventListener("click", () => {
    eraseModal.classList.toggle("hide")
    fade.classList.toggle("hide")
});


let stateNRadio = document.querySelector("input#form-radio-no")
let stateYRadio = document.querySelector("input#form-radio-yes")

stateNRadio.addEventListener("click", showInfoDay)
stateYRadio.addEventListener("click", hideInfoDay)

document.querySelector("input#slider").addEventListener("input", updateInfo)

document.querySelector("input#form-radio-yes").checked = "true"

function showInfoDay() {
    if (stateNRadio.checked) {
        document.getElementById("form-days").style.display = "block";
        document.querySelector("div#create-cancel").style.bottom = "477px"
        
    }
}

function hideInfoDay() {
    if (stateYRadio.checked) {
        document.getElementById("form-days").style.display = "none";
        document.querySelector("div#create-cancel").style.bottom = "312px"
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