filter_source = files('sinc.cpp', 'butterworth.cpp')

project_sources += filter_source

iir_dep = declare_dependency(link_with: iir_lib)


deps = [
    iir_dep,
]

filter_lib = static_library('filter',
                            filter_source,
                            include_directories : inc,
                            dependencies : deps)
