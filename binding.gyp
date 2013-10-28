{
  "targets": [{
    "target_name": "dmCreator",
    "sources": ["src/dmCreator.cc", "src/datamatrix.cpp"],
		'conditions': [
			['OS=="mac"', {
                "include_dirs": [
                    "/usr/local/include/"
                ],
    		"libraries": [
                "/usr/local/lib/libdmtx.dylib",
        	"/usr/lib/libopencv_core.dylib",
        	"/usr/lib/libopencv_highgui.dylib"
    		]
			}],
			['OS=="linux"', {
    		"libraries": [
					"/usr/lib/libdmtx.so",
        	"/usr/lib/libopencv_core.so",
        	"/usr/lib/libopencv_highgui.so"
    		]
			}],

		]
  }],
}
