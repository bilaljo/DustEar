settings_source = files('window.cpp',
                        'filter.cpp',
                        'calculation_method.cpp',
                        'measurement.cpp',
                        'general.cpp',
)

project_sources += settings_source

qt6_dep = dependency('qt6', modules: ['Core', 'Gui', 'Widgets', 'openglwidgets'], required: true)

deps = [
    qt6_dep
]

settings_lib = static_library('settings',
                              settings_source,
                              include_directories : inc,
                              dependencies : deps)
