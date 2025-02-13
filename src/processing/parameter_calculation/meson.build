subdir('lsq')

estimation_source = files('estimation.cpp')

project_sources += estimation_source

lsq_dep = declare_dependency(link_with: lsq_lib)

deps = [
    lsq_dep,
]

estimation_lib = static_library('estimation',
                                estimation_source,
                                include_directories : inc,
                                dependencies : deps)
