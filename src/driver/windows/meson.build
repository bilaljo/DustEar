windows_source = files('udp.cpp')
project_sources += windows_source

compiler = meson.get_compiler('cpp')
win32api = compiler.find_library('ws2_32', required: true)

deps = [
    win32api
]

windows_lib = static_library('windows',
                             windows_source,
                             include_directories : inc,
                             dependencies : deps)
