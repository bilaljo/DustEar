measurement_source = files('processing.cpp',
)

project_sources += measurement_source


deps = [
]


measurement_lib = static_library('measurement',
                                 measurement_source,
                                 include_directories : inc,
                                 dependencies : deps)
