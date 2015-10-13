"use strict"

var Epoc = require('bindings')('Epoc');
var sphero = require('sphero');
var orb = sphero("/dev/rfcomm0");

require('events').EventEmitter.prototype._maxListeners = 0;

Epoc.connect(function(event) {
	// console.log("Callback working with event: ", event);
	// if(event.blink > 0){
	// 	console.log("blink!")
	// }

	orb.connect(function(){
		console.log("Sphero connected....")
		if(event.gyroX > 0){
			//Roll Sphero forward
			orb.roll(0, 150);
		} else if(event.gyroX < 0){
			//Roll Sphero backwards
			orb.roll(180, 150);
		}
	})
});
