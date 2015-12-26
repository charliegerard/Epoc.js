"use strict"

var Epoc = require('bindings')('Epoc');
var sphero = require('sphero');
var orb = sphero("/dev/tty.Sphero-RBR-AMP-SPP");

console.log('waiting....')

orb.connect(function(){
	console.log("Sphero connected....")
	spheroControl(orb);
})

function spheroControl(sphero){
	Epoc.connect(function(event) {
		var eyeMovement = '';

		if(event.blink > 0){
			eyeMovement != "blink" ? eyeMovement = "blink" : break;
		}

		if(event.lookingLeft > 0){
			eyeMovement != "left" ? eyeMovement = "left" : break;
		}

		if(event.lookingUp > 0){
			eyeMovement != "up" ? eyeMovement = "up" : break;
		}

		if(event.lookingDown > 0){
			eyeMovement != "down" ? eyeMovement = "down" : break;
		}

		if(event.lookingRight > 0){
			eyeMovement != "right" ? eyeMovement = "right" : break;
		}

		if(event.smile > 0){
			eyeMovement != "smile" ? eyeMovement = "smile" : break;
		}

		switch(eyeMovement){
			case 'left':
				sphero.roll(70, 0, 1);
				break;
			case 'right':
				sphero.roll(70, 90, 1);
				break;
			case 'smile':
				sphero.roll(0, 0, 1);
				break;
		}

		console.log('Direction: ', eyeMovement);
	});
}
