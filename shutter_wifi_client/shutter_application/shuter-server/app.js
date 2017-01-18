/**
 * Aplication de gestion des volets.
 */

var url = require('url');
var http = require('http');
var fs = require('fs');
var port = 9000;
var voletFile = './volets.json';
var actionFile = './actions.json';

var volets = require('./volets');
var voletsJson = fs.readFileSync(voletFile).toString();
var voletsData = JSON.parse(voletsJson);

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

    if (pathname != undefined && pathname.startsWith('/volets/action')) {
	console.log(req.method);
	if (req.method == 'GET') {
	    var actionJson = getActions();
	    if (actionJson == null) {
		console.log("Aucune action à réaliser");
	    } else {
		resultat = JSON.stringify(actionJson);
		deleteActions();
	    }
	} else if (req.method == 'POST') {
	    req.on('data', function(chunk) {
		console.log("Data reçue : " + chunk.toString());
		postActions(chunk);
	    });
	} else if (req.method == 'DELETE') {
	    deleteActions();
	} else {
	    resultat = "Erreur : la m&eacute;thode " + req.method + " n'est pas trait&eacute;e.";
	}
	res.writeHead(200, {
	    'Content-Type' : 'application/json',
	    'Charset' : 'utf-8'
	});
    } else if (pathname != undefined && pathname.startsWith('/volets/infos')) {
	resultat = voletsJson;
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

var getActions = function() {
    try {
	fs.accessSync(actionFile, fs.F_OK);
	var actionsJson = fs.readFileSync(actionFile);
	return JSON.parse(actionsJson.toString());
    } catch (e) {
	// File isn't accessible
    }
    return null;
}

var postActions = function(action) {
    try {
	fs.accessSync(actionFile, fs.F_OK);
    } catch (e) {
	fs.appendFileSync(actionFile, action);
    }
    fs.writeFileSync(actionFile, action);
}

var deleteActions = function() {
    try {
	fs.accessSync(actionFile, fs.F_OK);
	fs.unlinkSync(actionFile);
	console.log('Le fichier ' + actionFile + ' a été supprimé.')
    } catch (e) {
	// File isn't accessible
    }
}

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
