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

	switch(event.cognitivAction){
		case 2:
			console.log('push')
			break;
		case 4:
			console.log('pull')
			break;
		case 8:
			console.log('lift')
			break;
		case 16:
			console.log('drop')
			break;
		case 32:
			console.log('left')
			break;
		case 64:
			console.log('right')
			break;
		case 128:
			console.log('rotate left')
			break;
		case 256:
			console.log('rotate right')
			break;
		case 512:
			console.log('rotate clockwise')
			break;
		case 1024:
			console.log('rotate counter clockwise')
			break;
		case 2048:
			console.log('rotate forwards')
			break;
		case 4096:
			console.log('rotate reverse')
		case 8192:
			console.log('disappear')
	}

	// if(event.cognitiviActionConfidence > 0){
	// 	console.log('cognitive action confidence')
	// }

	// if(event.push > 0){
	// 	console.log('push')
	// }
});
