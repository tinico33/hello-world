/**
 * Aplication de gestion des volets.
 */

var url = require('url');
var http = require('http');
var fs = require('fs');
var port = 9100;

http.createServer(function(req, res) {
    // Website you wish to allow to connect
    res.setHeader('Access-Control-Allow-Origin', '*');

    // Request methods you wish to allow
    res.setHeader('Access-Control-Allow-Methods', 'GET, POST, OPTIONS, PUT, PATCH, DELETE');

    // Request headers you wish to allow
    res.setHeader('Access-Control-Allow-Headers', 'X-Requested-With,content-type');

    // Set to true if you need the website to include cookies in the requests
    // sent to the API (e.g. in case you use sessions)
    res.setHeader('Access-Control-Allow-Credentials', true);

    var pathname = req.url;

    var resultat = "";

    if (pathname != undefined && pathname.endsWith('/open')) {
        resultat = "2000";
    } else if (pathname != undefined && pathname.endsWith('/close')) {
        resultat = "100";
    } else if (pathname != undefined && pathname.endsWith('/status')) {
        resultat = "Terminé";
    } else {
		resultat = "Le chemin demand&eacute; est introuvable : " + pathname;

		res.writeHead(404, {
		    'Content-Type' : 'application/json',
		    'Charset' : 'utf-8'
		});
    }
    console.log(resultat);
    res.write(resultat);
    res.end();
}).listen(port);

console.log("Serveur tourne sur http://localhost:" + port);
