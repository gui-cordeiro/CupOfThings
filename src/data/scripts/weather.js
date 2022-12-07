const api = {
    key: "64ed82577ced7f69cb1687f0ce536131",
    base: "https://api.openweathermap.org/data/2.5/",
    lang: "pt_br",
    units: "metric"
}

const container_img = document.querySelector("div#cont-menuweatherimg");
const temperaturas = document.querySelector("div#cont-menuweathertemp");
const weather_t = document.querySelector("div#cont-menuweatherdesc");

window.onload = function searchResults() {
    fetch(`${api.base}weather?q=Curitiba&lang=${api.lang}&units=${api.units}&APPID=${api.key}`)
        .then(response => {
            if (!response.ok) {
                throw new Error(`http error: status ${response.status}`)
            }
            return response.json();
        })
        .catch(error => {
            alert(error.message)
        })
        .then(response => {
            displayResults(response)
        });
}

function displayResults(weather) {
    //Exibindo no console o relatório completo da requisição à API
    console.log(weather)

    //Ícone da Página
    var iconName = weather.weather[0].icon;
    container_img.innerHTML = `<img src="icons/clima/${iconName}.png" alt="Ícone do Clima">`;

    //Temperaturas atual, mínima e máxima
    var temperature = `${Math.round(weather.main.temp)}`
    temperaturas.innerHTML = `<div id="cont-menuweathertemp_atual">${temperature}ºC</div>${Math.round(weather.main.temp_min)}ºC/${Math.round(weather.main.temp_max)}ºC`;

    //Descrição do Clima
    weather_tempo = weather.weather[0].description;
    weather_t.innerText = capitalizeFirstLetter(weather_tempo)
}

function capitalizeFirstLetter(string) {
    return string.charAt(0).toUpperCase() + string.slice(1);
}