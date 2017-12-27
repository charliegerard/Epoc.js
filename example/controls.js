"use strict";

var Epoc = require('bindings')('index');
var pastAction;
var action = "?";

Epoc.connectToLiveData("/Users/<username>/Library/Application Support/Emotiv/Profiles/<filename>.emu", function(event){
	if(event){
		pastAction = action;

		if(event.blink > 0){
			action = 'blink';
		}

		if(event.lookingLeft > 0){
			action = 'looking left'
		}

		if(event.lookingUp > 0){
			action = 'looking up'
		}

		if(event.lookingDown > 0){
			action = 'looking down'
		}

		if(event.lookingRight > 0){
			action = 'looking right'
		}

		if(event.smile > 0){
			action = 'smile'
		}

		if(event.cognitivAction > 0 && event.cognitivActionPower > 0){
			switch(event.cognitivAction){
				case 1:
					action = "neutral";
					break;
				case 2:
					action = 'push';
					break;
				case 4:
					action = 'pull';
					break;
				case 8:
					action = 'lift';
					break;
				case 16:
					action = 'drop';
					break;
				case 32:
					action = 'left';
					break;
				case 64:
					action = 'right';
					break;
				case 128:
					action = 'rotate left';
					break;
				case 256:
					action = 'rotate right';
					break;
				case 512:
					action = 'rotate clockwise';
					break;
				case 1024:
					action = 'rotate counter clockwise';
					break;
				case 2048:
					action = 'rotate forwards';
					break;
				case 4096:
					action = 'rotate reverse';
					break;
				case 8192:
					action = 'disappear';
					break;
			}
		}

		if(event.gyroX){
			console.log("gyroX: ", event.gyroX);
		}

		if(event.gyroY){
			console.log("gyroY: ", event.gyroY);
		}

		// at the moment will only handle 1 event at a time. Need to fix that
		if(pastAction !== action){
			console.log('action', action)
		}
	}
})
