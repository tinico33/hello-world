/**
 * Respésentation d'un volet.
 */

var Volet = function(o) {
        this.id = o.id ? o.id : 0;
        this.description = o.description ? o.description : 0;
};

Volet.prototype.ouvrir = function() {
        this.etat = 'OFF';
}
