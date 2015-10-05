var Epoc = require('bindings')('Epoc');

Epoc.connect(function(event) {
	// console.log("Callback working with event: ", event);
	// console.log(event.gyroX)
	if(event.blink > 0){
		console.log("blink!")
	}
});


