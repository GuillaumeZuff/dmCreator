{
    "targets": [{
        "target_name": "dmCreator",
        "sources": ["src/dmCreator.cc", "src/datamatrix.cpp"],
        "include_dirs" : [
            "<!(node -e \"require('nan')\")"
        ],
        'conditions': [
            ['OS=="mac"', {
                "include_dirs": [
                    "/usr/local/include/"
                ],
                "libraries": [
                    "-L/usr/local/lib",
                    "libdmtx.dylib"
                ]
            }],
            ['OS=="linux"', {
		"libraries": [
			"/usr/lib/libdmtx.so"
		]
	    }],
        ]
    }],
}
