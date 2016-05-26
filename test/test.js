'use strict';

const getPixels = require("get-pixels");
const savePixels = require("save-pixels")
const dmCreator = require("../build/Release/dmCreator");
const ndarray = require("ndarray");
const fs = require("fs");

describe("encode", function() {
    it("should encode correctly", function(done) {
        const res = dmCreator.generateDm({
            data: "hello, world"
        });

        res.width.should.equal(100);
        res.height.should.equal(100);
        res.channels.should.equal(3);
        res.pixels.length.should.equal(100*100*3);

        done();
    });
});

const decodePixels = function (err, pixels) {
    if (err) {
        err.should.be.null();
    } else {
        var data = new Buffer(pixels.data);
        return dmCreator.decodeDm({
            cols: pixels.shape[0],
            rows: pixels.shape[1],
            channels: pixels.shape[2] 
        }, data);
    }
}

describe("decode", function() {
    it("should decode correctly", function(done) {
        getPixels("./test/image.png", function(err, pixels) {
        //getPixels("http://datamatrix.kaywa.com/img.php?s=6&d=Hello%20DmCreator", function(err, pixels) {
            const res = decodePixels(err, pixels);
            res.success.should.be.true();
            res.text.should.equal("Hello DmCreator");
            done();
        });
    });

    it("should decode what it encoded", function(done) {
        const encode = dmCreator.generateDm({
            data: "hello"
        });
        var data = new Buffer(encode.pixels);
        var image = ndarray(encode.pixels, [encode.width, encode.height, encode.channels])
        var output = fs.createWriteStream("test/output.png");
        var stream = savePixels(image, "png").pipe(output);
        stream.on("finish", function() {
            getPixels("./test/output.png", function(err, pixels) {
                const res = decodePixels(err, pixels);
                res.success.should.be.true();
                res.text.should.equal("hello");
                done();
            });
        });
    });
});
