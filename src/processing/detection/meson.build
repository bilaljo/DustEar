detection_source = files('trigger_level.cpp',
)

project_sources += detection_source

deps = [
]

detection_lib = static_library('detection',
                                detection_source,
                                include_directories : inc,
                                dependencies : deps)
