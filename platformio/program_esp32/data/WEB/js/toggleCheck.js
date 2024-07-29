class toggleCheck {
    constructor() {
        this._xhr = new XMLHttpRequest();
    }
    static toggleCheckbox(element) {
        var thisClass = new toggleCheck();
        let xhr = thisClass._xhr;
        
        if (element.checked) {
            xhr.open("GET", "/check?output=" + element.id + "&state=1", true);
        }
        else {
            xhr.open("GET", "/check?output=" + element.id + "&state=0", true);
        }
        xhr.send();
    }

    static updateStatus() {
        const thisClass = new toggleCheck();
        let xhr = thisClass._xhr;
        try {
            xhr.open("GET", "/getRelayStatus", true);
            xhr.onreadystatechange = async () => {
                if (xhr.readyState === 4 && xhr.status === 200) {
                    let response = await JSON.parse(xhr.responseText);
                    for (let key in response) {
                        let element = document.getElementById(response[key].id);
                        if (element) {
                            element.checked = response[key].status;
                        }
                    }
                }
            };
            xhr.send();
            
        } catch (error) {
            console.error(error);
        }

        // Call updateAllRelayStatus function every 3 seconds
        setTimeout(toggleCheck.updateStatus, 3000);
    }
}