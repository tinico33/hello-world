/**
 * Respésentation d'une action.
 */

var Action = function(o) {
        this.volet = o.volet ? o.volet : 0;
        this.action = o.action ? o.action : 0;
};
