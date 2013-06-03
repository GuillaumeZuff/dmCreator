var addon = require('../build/Release/dmCreator');
var result = addon.generateDm({
	data: "hello, world!",
	path: "./"
});

console.log("Success: ", result.success);
if (result.success) {
}
