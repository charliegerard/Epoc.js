{
  "targets": [
    {
      "target_name": "index",
      "sources": [ "epoc.cc"],
      "include_dirs" : [
        "<!(node -e \"require('nan')\")"
      ],
      "conditions": [
                ['OS=="mac"', {
                    "cflags": [ "-m64" ],
                    "ldflags": [ "-m64" ],
                   "xcode_settings": {
                     "OTHER_CFLAGS": ["-ObjC++"],
                     "ARCHS": [ "x86_64" ]
                   },
                    "link_settings": {
                        "libraries": [
                         "/Library/Frameworks/edk.framework/edk"
                        ],
                        "include_dirs": ["./lib/includes/", "./lib/"]
                    }
                }]
             ]
    }
  ]
}
