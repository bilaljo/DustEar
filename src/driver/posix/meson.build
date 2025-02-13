posix_source = files('udp.cpp')
project_sources += posix_source

deps = [
]

posix_lib = static_library('posix',
                           posix_source,
                           include_directories : inc,
                           dependencies : deps)
