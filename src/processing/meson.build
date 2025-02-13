subdir('filter')
subdir('detection')
subdir('parameter_calculation')

processing_source = files('pipeline.cpp', 'event_bus.cpp')

project_sources += processing_source

detection_dep = declare_dependency(link_with: detection_lib)
filter_dep = declare_dependency(link_with: filter_lib)
estimation_dep = declare_dependency(link_with: estimation_lib)
estimation_dep = declare_dependency(link_with: estimation_lib)
cminpack_dep = declare_dependency(link_with: cminpack_lib)

#torch_dep = dependency('torch', required: true)

deps = [
    detection_dep,
    filter_dep,
    estimation_dep,
    cminpack_dep,
    #torch_dep,
]

processing_lib = static_library('processing',
                                processing_source,
                                include_directories : inc,
                                dependencies : deps)

