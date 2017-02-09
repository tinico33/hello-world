/**
 * http://usejsdoc.org/
 */
var voletsApplication = new angular.module("voletsApplication", []);

var serveurVolet = 'http://localhost:9000';
//var serveurVolet = 'http://serveur-dell:9000';

function getStatusLabel(status) {
	var label;
	console.log(status);
	switch(parseInt(status)) {
	    case 0:
	        label = "Ouverture en cours";
	        break;
	    case 1:
	        label = "Volet ouvert";
	        break;
	    case 2:
	        label = "Fermeture en cours";
	        break;
	    case 3:
	        label = "Volet fermé";
	        break;
	    case 4:
	        label = "Volet arrêté";
	        break;
	    default:
	        label = "Etat inconnu";
	}
	return label;
}

var voletsController = voletsApplication.controller("voletsController", function($scope, $http) {

	$scope.messages = [];

    $http.get(serveurVolet + '/infos').then(function(response) {
		$scope.volets = response.data;
		for (var i = 0; i < $scope.volets.length; i++) {
			loadedStatus($scope.volets[i].id);
		}
    }, function(response) {
		alert(response.data+serveurVolet);
	});

    window.scope = $scope;

    var getShutterById = function(id) {
	    var volet = null;
	    for (var i = 0; i < $scope.volets.length; i++) {
			if ($scope.volets[i].id == id) {
			    volet = $scope.volets[i];
			}
	    }
	    return volet;
	}

	function loadedStatus(shutterId) {
    	var shutter = getShutterById(shutterId);
		getStatus(shutter, function successCallback(response) {
    		document.getElementById("loading-" + shutterId).setAttribute("hidden", "true");
    		document.getElementById("up-" + shutterId).removeAttribute("hidden");
    		document.getElementById("down-" + shutterId).removeAttribute("hidden");
    		$scope.messages[shutterId] = getStatusLabel(response.data);
		}, function errorCallback(response) {
    		loadedStatus(shutterId);
		});
	}

	function getStatus(shutter, successCallback, errorCallback) {
		var request = {
		    method : 'GET',
		    url : shutter.site+'/status',
		    headers : {
				'Content-Type' : undefined
		    }
		}
	    $http(request).then(successCallback, errorCallback);
	}

	var updateStatus = function(shutter) {
		getStatus(shutter, function successCallback(response) {
    		$scope.messages[shutter.id] = getStatusLabel(response.data);
		}, function errorCallback(response) {
    		$scope.messages[shutter.id]="ERREUR : "+response.config.url;
		});
	}

	var disableButtons = function(id) {
    	document.getElementById("up-"+id).setAttribute("disabled", "true");
    	document.getElementById("down-"+id).setAttribute("disabled", "true");
	}

	var enableButtons = function(id) {
    	document.getElementById("up-"+id).removeAttribute("disabled");
    	document.getElementById("down-"+id).removeAttribute("disabled");
	}

    $scope.executeRequest = function(request, shutterId) {
    	$scope.messages[shutterId]="En cours";
		$http(request).then(function successCallback(response) {
    		$scope.messages[shutterId]="Terminé";
		}, function errorCallback(response) {
    		$scope.messages[shutterId]="ERREUR : "+response.config.url;
		});
    }

    $scope.ouvrir = function(shutterId) {
    	var shutter = getShutterById(shutterId);
    	disableButtons(shutterId);

    	if(shutter != undefined) {
			var request = {
			    method : 'POST',
			    url : shutter.site+'/open',
			    headers : {
					'Content-Type' : undefined
			    }
			}
			$scope.messages[shutterId]="Ouverture en cours";
			$http(request).then(function successCallback(response) {
				var timer = parseInt(response.data) + 2000;
				setTimeout(function(){
					updateStatus(shutter);
    				enableButtons(shutterId);
				}, timer);
			}, function errorCallback(response) {
	    		$scope.messages[shutterId]="ERREUR : "+response.config.url;
    			enableButtons(shutterId);
			});
    	}
    }

    $scope.fermer = function(shutterId) {
    	var shutter = getShutterById(shutterId);
    	disableButtons(shutterId);

    	if(shutter != undefined) {
			var request = {
			    method : 'POST',
			    url : shutter.site+'/close',
			    headers : {
					'Content-Type' : undefined
			    }
			}
			$scope.messages[shutterId]="Fermeture en cours";
			$http(request).then(function successCallback(response) {
				var timer = parseInt(response.data) + 2000;
				setTimeout(function(){
					updateStatus(shutter);
    				enableButtons(shutterId);
				}, timer);
			}, function errorCallback(response) {
	    		$scope.messages[shutterId]="ERREUR : "+response.config.url;
    			enableButtons(shutterId);
			});
    	}
    }

    $scope.stop = function(shutterId) {
    	var shutter = getShutterById(shutterId);

    	if(shutter != undefined) {
			var request = {
			    method : 'POST',
			    url : shutter.site+'/stop',
			    headers : {
					'Content-Type' : undefined
			    }
			}
			$scope.executeRequest(request, shutterId);
    	}
    }
});
