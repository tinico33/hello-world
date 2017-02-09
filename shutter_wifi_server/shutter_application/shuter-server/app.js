/**
 * Aplication de gestion des volets.
 */

var url = require('url');
var http = require('http');
var fs = require('fs');
var port = 9000;

var voletFile = './volets.json';
var volets = require('./volets');
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
    } else {
		resultat = "Le chemin demand&eacute; est introuvable : " + pathname;

		response.writeHead(404, {
		    'Content-Type' : 'application/json',
		    'Charset' : 'utf-8'
		});
    }
    console.log(resultat);
    response.write(resultat);
    response.end();
}).listen(port);

var getVoletById = function(id) {
    var volet = null;
    for (var i = 0; i < voletsData.length; i++) {
		if (voletsData[i].id == id) {
		    volet = voletsData[i];
		}
    }
    return volet;
}

console.log("Serveur tourne sur http://localhost:" + port);
