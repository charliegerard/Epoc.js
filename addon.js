var BrainSensor = require('bindings')('BrainSensor');
// module.exports = require('./build/Release/emojs');
// var addon = require('./build/Release/addon');

// var obj = new addon.NodeEPOCDriver();
// var _ = require('underscore');



BrainSensor.connect(function() {
	console.log("errrmaggahhd! A callback!");
})


// obj.connect('/Users/cgerard/Library/Application Support/Emotiv/Profiles/charlie.emu', _.throttle(function(e){
	// obj.connect('', function(e){
	// obj.rezero();
	// console.log(e);
	// if (e.smile) console.log('Smile: ', e.smile);

	// console.log('this is working and is connected');
	// console.log('gyro', e.gyroX+':'+ e.gyroY);
// }, 1000));

