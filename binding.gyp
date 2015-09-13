{
  "targets": [
    {
      "target_name": "BrainSensor",
      "sources": [ "BrainSensor.cc", "Functions.cc" ],
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
                         "/usr/local/lib/EmotivXavier-PREMIUM/libedk.dylib",
                         "/usr/local/lib/EmotivXavier-PREMIUM/libedk.1.dylib",
                         "/usr/local/lib/EmotivXavier-PREMIUM/libedk_ultils_mac.dylib",
                         "/usr/local/lib/EmotivXavier-PREMIUM/libiomp5.dylib"
                        ]
                    }
                }],
                ['OS=="linux"', {
                     "cflags": [ "-m64" ],
                     "ldflags": [ "-m64" ],
                    "xcode_settings": {
                      "ARCHS": [ "x86_64" ]
                    },
                     "link_settings": {
                         "libraries": [
                          "/usr/local/lib/libedk.so.1",
                          "/usr/local/lib/libhal.so.1",
                          "/usr/local/lib/libedk_utils.so",
                          "/usr/local/lib/libqwt.so.5"
                         ]
                     }
                 }]
             ]
    }
  ]
}

