/**
 * http://usejsdoc.org/
 */
var voletsApplication = new angular.module("voletsApplication", []);

var serveurVolet = 'http://bureau:9000/volets';

var voletsController = voletsApplication.controller("voletsController", function($scope, $http) {

    $http.get(serveurVolet + '/infos').then(function(response) {
	$scope.volets = response.data;
    }, function(response) {
	alert(response.data);
    });

    window.scope = $scope;

    $scope.action = function(methode, idVolet, actionVolet) {
	var request = {
	    method : methode,
	    url : serveurVolet + '/action',
	    headers : {
		'Content-Type' : undefined
	    },
	    data : {
		id : idVolet,
		action : actionVolet
	    }
	}
	$http(request).then(function successCallback(response) {
	    // alert(response.data);
	}, function errorCallback(response) {
	    // alert(response.data);
	});
    }

    $scope.ouvrir = function(idVolet) {
	$scope.action('POST', idVolet, 'U');
    }

    $scope.fermer = function(idVolet) {
	$scope.action('POST', idVolet, 'D');
    }

    $scope.deleteAction = function() {
	$scope.action('DELETE');
    }
});