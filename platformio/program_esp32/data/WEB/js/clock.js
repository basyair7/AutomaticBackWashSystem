class Clock {
    constructor() {
        this.rtclock = new Date();
    }
    static run() {
        const thisClass = new Clock();

        // date
        let day = thisClass.rtclock.getDay();
        let month = thisClass.rtclock.getMonth() + 1;
        let year = thisClass.rtclock.getFullYear();

        // time
        let hours = thisClass.rtclock.getHours();
        let minute = thisClass.rtclock.getMinutes();
        let seconds = thisClass.rtclock.getSeconds();

        // Add AM or PM System
        let AM_PM = (hours < 12) ? "AM" : "PM";

        // convert the hours component to 12-hour format
        hours = (hours > 12) ? hours - 12 : hours;
        
        // pad the hours, minute, and seconds with leading zero
        hours = ("0" + hours).slice(-2);
        minute = ("0" + minute).slice(-2);
        seconds = ("0" + seconds).slice(-2);

        document.getElementById("time").innerHTML = `${hours}:${minute}:${seconds} ${AM_PM}`;
        document.getElementById("date").innerHTML = `${month}/${day}/${year}`;
        setTimeout(Clock.run, 500);
    }
}