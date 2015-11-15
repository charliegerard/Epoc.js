"use strict"

var Epoc = require('bindings')('Epoc');
var sphero = require('sphero');
var orb = sphero("/dev/tty.Sphero-RBR-AMP-SPP");

console.log('waiting....')

orb.connect(function(){
	console.log("Sphero connected....")
	// spheroControl(orb);
})

// function spheroControl(sphero){
	Epoc.connect(function(event) {


		if(event.blink > 0){
			console.log("blink!")
			// sphero.roll(70, 0, 1);
		}

		if(event.lookingLeft > 0){
			console.log('LEFT')
		}

		if(event.lookingUp > 0){
			console.log('UP')
		}

		if(event.lookingDown > 0){
			console.log('DOWN')
		}

		if(event.lookingRight > 0){
			console.log('RIGHT')
		}

		if(event.smile > 0){
			console.log('SMILE')
			// sphero.roll(0, 0, 0);
		}
	});
// }
//
