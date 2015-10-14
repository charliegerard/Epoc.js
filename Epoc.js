"use strict"

var Epoc = require('bindings')('Epoc');
var sphero = require('sphero');
var orb = sphero("/dev/rfcomm0");

require('events').EventEmitter.prototype._maxListeners = 0;
console.log('waiting for sphero connection')
Epoc.connect(function(event) {
	// console.log("Callback working with event: ", event);
	// if(event.blink > 0){
	// 	console.log("blink!")
	// }

	function gyroControl(sphero){
		if(event.gyroX > 0){
			//Roll Sphero forward
			sphero.roll(0, 150);
		} else if(event.gyroX < 0){
			//Roll Sphero backwards
			sphero.roll(180, 150);
		}
	}
});

orb.connect(function(){
	console.log("Sphero connected....")
	gyroControl(orb);
})
