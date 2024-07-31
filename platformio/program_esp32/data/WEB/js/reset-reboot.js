class reset_reboot {
    constructor() {
        this._xhr = new XMLHttpRequest();
    }

    _get(uri) {
        this._xhr.open("GET", uri, true);
        this._xhr.send();
    }

    static rebootServer() {
        const thisClass = new reset_reboot();
        Swal.fire({
            title: "Reboot Unit",
            text: "Reboot the web server?",
            icon: "info",
            showCancelButton: true,
            showCancelButton: true,
            confirmButtonColor: "#3085d6",
            cancelButtonColor: "#d33",
            confirmButtonText: "Reboot"
        }).then((result) => {
            if (result.isConfirmed) {
                thisClass._get("/rst-webserver");
                Swal.fire({
                    title: "Reboot Success",
                    text: "Server has been restart",
                    icon: "success",
                    confirmButtonColor: "#00b30c",
                    confirmButtonText: "OK"
                });
            }
        });
    }

    static resetServer(uri) {
        Swal.fire({
            title: "Reset System",
            text: "Reset the system to default?",
            icon: "warning",
            showCancelButton: true,
            confirmButtonColor: "#3085d6",
            cancelButtonColor: "#d33",
            confirmButtonText: "Yes, reset!"
        }).then((result) => {
            if (result.isConfirmed) {
                window.location.href = uri;
            }
        });
    }
}