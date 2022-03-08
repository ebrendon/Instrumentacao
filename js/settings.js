const workTimeInput = document.querySelector("#worktime");
const breakTimeInput = document.querySelector("#breaktime");

const pausebtn = document.querySelector(".pause");

document.querySelector("form").addEventListener("submit", (e)=>{
    e.preventDefault();
    localStorage.setItem("worktime", workTimeInput.value);
    localStorage.setItem("breaktime", breakTimeInput.value);
});

document.querySelector(".reset").addEventListener("click", () => {
    startbtn.style.transform = "scale(1)";
    clearTimeout(initial);
    mindiv.textContent = "00";
    secdiv.textContent = "00";
    pausebtn.disabled = true;
    startbtn.disabled = false;
});

pausebtn.addEventListener("click", () =>{
    if(paused === undefined){
        return;
    }

    if(paused) {
        paused = false;
        initial = setTimeout("decrement()", 60);
        pausebtn.textContent = "pause";
        pausebtn.classList.remove("remume");
    }
    else {
        clearTimeout(initial);
        pausebtn.textContent = "resume";
        pausebtn.classList.add("resume");
        paused = true;
    }
});
