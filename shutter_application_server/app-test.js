/**
 * Aplication de gestion des volets.
 */

var url = require('url');
var http = require('http');
var fs = require('fs');
var port = 9100;

var voletFile = './volets-test.json';
var voletsJson = fs.readFileSync(voletFile).toString();
var voletsData = JSON.parse(voletsJson);

http.createServer(function(request, response) {
    // Website you wish to allow to connect
    response.setHeader('Access-Control-Allow-Origin', '*');

    // Request methods you wish to allow
    response.setHeader('Access-Control-Allow-Methods', 'GET, POST, OPTIONS, PUT, PATCH, DELETE');

    // Request headers you wish to allow
    response.setHeader('Access-Control-Allow-Headers', 'X-Requested-With,content-type');

    // Set to true if you need the website to include cookies in the requests
    // sent to the API (e.g. in case you use sessions)
    response.setHeader('Access-Control-Allow-Credentials', true);

    var pathname = request.url;

    var resultat = "";

    if (pathname != undefined && pathname.startsWith('/infos')) {
        resultat = voletsJson;
        sendResult(response, resultat);
    } else if (pathname != undefined && pathname.endsWith('/open')) {
        resultat = "2000";
        sendResult(response, resultat);
    } else if (pathname != undefined && pathname.endsWith('/close')) {
        resultat = "100";
        sendResult(response, resultat);
    } else if (pathname != undefined && pathname.endsWith('/status')) {
        var wait = Math.floor(Math.random()*1000);
        console.log(wait);
        setTimeout(function function_name() {
            var states = [0, 1, 2, 3, 4];
            var aleatoire = Math.floor(Math.random()*5);
            console.log(aleatoire);

            resultat = states[aleatoire];
            sendResult(response, resultat.toString());
        }, wait);
    } else {
		resultat = "Le chemin demand&eacute; est introuvable : " + pathname;

		response.writeHead(404, {
		    'Content-Type' : 'application/json',
		    'Charset' : 'utf-8'
		});
        sendResult(response, resultat);
    }
}).listen(port);

console.log("Serveur tourne sur http://localhost:" + port);

function sendResult(response, resultat) {
    console.log(resultat);
    response.write(resultat);
    response.end();
}