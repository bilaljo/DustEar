tabs_source = files('distribution.cpp',
                    'time_series.cpp')

project_sources += tabs_source

model_dep = declare_dependency(link_with: model_lib)

deps = [
    model_dep,
]

tabs_lib = static_library('tabs',
                          tabs_source,
                          include_directories : inc,
                          dependencies : deps)
