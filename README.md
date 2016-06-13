# dmCreator

node.js package, wrapper around [libdmtx](http://libdmtx.sourceforge.net/) for creating and decoding datamatrix tags.

## Install
```
npm install dmcreator
```

## Usage
```
const dmCreator = require("dmcreator");
```

### Encoding
```
dmCreator.generateDm({data: "The text to encode in a datamatrix"});
```
It returns an object with the following properties:
* pixels: an array of pixels representing the datamatrix tag
* width: the width of the resulting pixels
* height: the height of the resulting pixels
* channels: the channels count of the resulting pixels

### Decoding
```
dmCreator.decodeDm({
  cols: colsCount,
  rows: rowsCount,
  channels: channelsCount 
}, data, timeout)
```
where data is a [Buffer](https://nodejs.org/api/buffer.html).

It returns an object with the following properties:
* success: true if a datamatrix tag was found and decoded properly
* test: the content of the datamatrix tag
