var addon = require('../build/Release/dmCreator');

console.log("Generating dm...");
var result = addon.generateDm({
	data: "hello, world!",
	path: "./"
});

console.log("Success: ", result.success);
if (result.success) {
    console.log("Path: "+result.path);
    var decodeRes = addon.decodeDm({
        path: result.path
    });
    for (var p in decodeRes) console.log("p: "+p+" "+decodeRes[p]);
    if (decodeRes.success) {
        console.log ("Decode successful");
    }
    else {
        console.log ("Failed to decode: "+decodeRes.error);
    }
}
