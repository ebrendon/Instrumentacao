const el = document.querySelector(".content__timer");

const mindiv = document.querySelector(".minutes");
const secdiv = document.querySelector(".seconds");

const startbtn = document.querySelector(".start");
localStorage.setItem("btn", "work");

let initial, totalsec, paused, mins, seconds;

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
    startbtn.disabled = true;
    paused = false;
    pausebtn.disabled = false;

});

function decrement() {
    mindiv.textContent = mins>9 ? Math.floor(seconds/60) : `0${Math.floor(seconds/60)}`;
    secdiv.textContent = seconds % 60 > 9 ? seconds % 60 : `0${seconds % 60}`;

    if(seconds > 0){
        seconds--;
        initial = window.setTimeout("decrement()", 1000);
    }
    else {
        mins = 0;
        seconds = 0;
        alert("Work time over");
        let btn = localStorage.getItem("btn");

        if(btn === "work") {
            startbtn.textContent = "start break";
            startbtn.classList.add("break");
            localStorage.setItem("btn", "break");
        }
        else{
            startbtn.classList.remove("break");
            startbtn.textContent = "start work";
            localStorage.setItem("btn", "work");
        }
        startbtn.disabled = false;
    }

}