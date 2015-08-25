var addon = require('bindings')('addon');
// module.exports = require('./build/Release/emojs');
// var addon = require('./build/Release/addon');

var obj = new addon.NodeEPOCDriver();
var _ = require('underscore');

obj.connect('', _.throttle(function(e){
	obj.rezero();
	console.log('this is working and is connected')
}, 1000));

// obj.connect(function(){
// 	obj.disconnect();
// 	console.log('this is working and is connected')
// })

console.log('blablbalablbala')

console.log('end of program')

// obj.Reconnect(1);