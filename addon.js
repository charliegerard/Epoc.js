var addon = require('bindings')('addon');
// module.exports = require('./build/Release/emojs');
// var addon = require('./build/Release/addon');

var obj = new addon.NodeEPOCDriver();
var _ = require('underscore');


obj.connect('/Users/cgerard/Library/Application Support/Emotiv/Profiles/charlie.emu', function(e){
	obj.rezero();

	console.log('this is working and is connected');
	console.log('gyro', e.gyroX+':'+ e.gyroY);
}, 500);

