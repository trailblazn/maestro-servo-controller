{
  "targets": [
    {
      "target_name": "maestro",
      "sources": [
        "src/maestro.cc",
        "src/maestro_servo_controller.cc"
      ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")"
      ],
      "cflags": [
        "-std=c++11",
        "-fexceptions",
        "-Wall",
        "-O3"
      ],
      "cflags_cc": [
        "-std=c++11",
        "-fexceptions",
        "-Wall",
        "-O3"
      ],
      "xcode_settings": {
        "OTHER_CPLUSPLUSFLAGS": [
          "-std=c++11",
          "-stdlib=libc++",
          "-fexceptions",
          "-Wall",
          "-O3"
        ],
        "MACOSX_DEPLOYMENT_TARGET": "10.7"
      }
    }
  ]
}
