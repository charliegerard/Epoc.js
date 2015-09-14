var BrainSensor = require('bindings')('BrainSensor');

BrainSensor.connect(function(event) {
	console.log("Callback working with event: ", event);
	if(event.blink > 0){
		console.log("blink!")
	}
});


