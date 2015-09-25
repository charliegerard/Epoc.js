var Epoc = require('bindings')('Epoc');

Epoc.connect(function(event) {
	console.log("Callback working with event: ", event);
	if(event.blink > 0){
		console.log("blink!")
	}
});


