"use strict"

var Epoc = require('bindings')('index');

Epoc.connect(function(event) {

	if(event.blink > 0){
		console.log('blink')
	}

	if(event.lookingLeft > 0){
		console.log('looking left')
	}

	if(event.lookingUp > 0){
		console.log('looking up')
	}

	if(event.lookingDown > 0){
		console.log('looking down')
	}

	if(event.lookingRight > 0){
		console.log('looking right')
	}

	if(event.smile > 0){
		console.log('smile')
	}
});
