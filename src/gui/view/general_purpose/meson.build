general_purpose_source = files('toolbar.cpp',
                               'logging.cpp',
                               'status_bar.cpp',
)

project_sources += general_purpose_source

qt6_dep = dependency('qt6', modules: ['Core', 'Gui', 'Widgets', 'openglwidgets'], required: true)

model_dep = declare_dependency(link_with: model_lib)
presenter_dep = declare_dependency(link_with: presenter_lib)

deps = [
    qt6_dep,
    model_dep,
    presenter_dep
]

general_purpose_lib = static_library('general_purpose',
                                     general_purpose_source,
                                     include_directories : inc,
                                     dependencies : deps)
