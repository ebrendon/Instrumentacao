const workTimeInput = document.querySelector("#worktime");
const breakTimeInput = document.querySelector("#breaktime");
const distanciaInput = document.querySelector("#distanciaIdeal");

document.querySelector("form").addEventListener("submit", (e)=>{
    e.preventDefault();
    localStorage.setItem("worktime", workTimeInput.value);
    localStorage.setItem("breaktime", breakTimeInput.value);
    localStorage.setItem("distanciaIdeal", distanciaInput.value);
    clearTimeout(initial);
    mindiv.textContent = "00";
    secdiv.textContent = "00";
    startbtn.style.transform = "scale(1)";
    pausebtn.style.transform = "scale(0)";
});

const el = document.querySelector(".content__timer");

const mindiv = document.querySelector(".minutes");
const secdiv = document.querySelector(".seconds");

const startbtn = document.querySelector("#start");
const pausebtn = document.querySelector("#pause");
pausebtn.style.transform = "scale(0)";

let initial, totalsec, paused, mins, seconds, start = 0;

const temp = document.querySelector("#temporizador");
const inter = document.querySelector("#intervalo");

localStorage.setItem("btn", "work");
temp.style.backgroundColor = "#15767d";

temp.addEventListener("click", ()=>{
    localStorage.setItem("btn", "work");
    temp.style.backgroundColor = "#15767d";
    inter.style.backgroundColor = "#5a9599";

    clearTimeout(initial);
    mindiv.textContent = "00";
    secdiv.textContent = "00";

});

inter.addEventListener("click", ()=>{
    localStorage.setItem("btn", "break");
    inter.style.backgroundColor = "#15767d";
    temp.style.backgroundColor = "#5a9599";

    clearTimeout(initial);
    mindiv.textContent = "00";
    secdiv.textContent = "00";
    
});

var btn = document.querySelector('#btn-config');


btn.addEventListener('click', function(){
    let ctn1 = document.querySelector('#conteudo__timer');
    let ctn2 = document.querySelector('#content__config');

    if(ctn1.style.display == 'flex' || ctn1.style.display == ''){
        ctn1.style.display = 'none';
        ctn2.style.display = 'flex';
        btn.style.backgroundColor = "#15767d";
    }
    else{
        ctn1.style.display = 'flex';
        ctn2.style.display = 'none';
        btn.style.backgroundColor = "#5a9599"; 
    }

});



startbtn.addEventListener("click", ()=> {
    
    let btn = localStorage.getItem("btn");
    if(btn === "work") {
        mins = +localStorage.getItem("worktime");
    }
    else {
        mins = +localStorage.getItem("breaktime");
    }

    seconds = mins*60;
    totalsec = mins*60;
    setTimeout(decrement(),60);

    paused = false;
    startbtn.style.transform = "scale(0)";
    pausebtn.style.transform = "scale(1)";

});

pausebtn.addEventListener("click", ()=>{
    if(paused) {
        paused = false;
        initial = setTimeout("decrement()", 60);
        pausebtn.textContent = "PAUSE";
        pausebtn.classList.remove("RESUME");
    }
    else {
        clearTimeout(initial);
        pausebtn.textContent = "RESUME";
        pausebtn.classList.add("RESUME");
        paused = true;
    }
});


function decrement() {
    mindiv.textContent = mins>9 ? Math.floor(seconds/60) : `0${Math.floor(seconds/60)}`;
    secdiv.textContent = seconds % 60 > 9 ? seconds % 60 : `0${seconds % 60}`;

    if(seconds >= 0){
        seconds--;
        initial = window.setTimeout("decrement()", 1000);
    }
    else {
        mins = 0;
        seconds = 0;
        alert("Work time over");
        mindiv.textContent = "00";
        secdiv.textContent = "00";
        
        let btn = localStorage.getItem("btn");

        if(btn === "work") {
            startbtn.style.transform = "scale(1)";
            pausebtn.style.transform = "scale(0)";
            startbtn.textContent = "start break";
            startbtn.classList.add("break");
            localStorage.setItem("btn", "break");
        }
        else{
            startbtn.style.transform = "scale(1)";
            pausebtn.style.transform = "scale(0)";
            startbtn.classList.remove("break");
            startbtn.textContent = "start work";
            localStorage.setItem("btn", "work");
        }
    }

}


