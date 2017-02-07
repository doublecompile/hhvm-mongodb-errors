if (Promise === 'undefined') {
    alert("You need a browser that supports the Promise API");
}

function listener(id, url) {
    var output = document.getElementById(id);
    output.style.opacity = 0.61;
    return new Promise(function (resolve, reject) {
        var xhr = new XMLHttpRequest();
        xhr.addEventListener("load", function () {
            output.style.opacity = 1;
            if (xhr.status >= 400) {
                output.innerHTML = '<p style="color:red;font-weight:bold;">ERROR!</p>';
            } else {
                output.innerHTML = this.responseText;
            }
            resolve();
        });
        xhr.open("GET", url);
        xhr.send();
    });
}

function main() {
    return Promise.all([
        listener('attempt-1', '/ajax.php?a=1'),
        listener('attempt-2', '/ajax.php?a=2'),
        listener('attempt-3', '/ajax.php?a=3'),
        listener('attempt-4', '/ajax.php?a=4')
    ]).then(function () {
        setTimeout(main, 1000);
    });
}
main();
