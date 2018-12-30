{
  'targets': [
    {
      'target_name': 'smodem',
      'sources': [
      		 'src/main.cpp',
		 'src/driver/signals.cpp'
#		 '/usr/local/share/verilator/include/verilated.cpp'
      		 ],
      'include_dirs': ["<!@(node -p \"require('node-addon-api').include\")",
      "src/driver"
      # "/usr/local/share/verilator/include"
      ],
#     'libraries': ["../obj_dir/verilator_global_libs.a"],
      'dependencies': ["<!(node -p \"require('node-addon-api').gyp\")"],
      'cflags!': [ '-fno-exceptions' ],
      'cflags_cc!': [ '-fno-exceptions', '-MMD', '-MP', '-DVL_DEBUG=1' ],
#      'cflags!': [ '-fno-exceptions', '-fPIC'],
#      'cflags_cc!': [ '-fno-exceptions', '-fPIC'],

      'xcode_settings': {
        'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
        'CLANG_CXX_LIBRARY': 'libc++',
        'MACOSX_DEPLOYMENT_TARGET': '10.7'
      },
      'msvs_settings': {
        'VCCLCompilerTool': { 'ExceptionHandling': 1 },
      }
    }
  ]
}