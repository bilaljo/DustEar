settings_source = files('filter.cpp',
                        'calculation_method.cpp',
                        'measurement.cpp',
                        'general.cpp',
)

project_sources += settings_source

model_dep = declare_dependency(link_with: model_lib)

deps = [
    model_dep
]

settings_lib = static_library('settings',
                              settings_source,
                              include_directories : inc,
                              dependencies : deps)
