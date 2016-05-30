{
    "targets": [{
        "target_name": "dmCreator",
        "sources": ["src/dmCreator.cc", "src/datamatrix.cpp"],
        "include_dirs" : [
            "<!(node -e \"require('nan')\")"
        ],
        "libraries": [
            "-Wl","-rpath","-llibdmtx"
        ],
        'conditions': [
            ['OS=="mac"', {
                "include_dirs": [
                    "/usr/local/include/"
                ],
                "libraries": [
                    "-L/usr/local/lib"
                ]
            }],
        ]
    }],
}
