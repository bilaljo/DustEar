tabs_source = files('distribution.cpp',
                    'time_series.cpp'
)

project_sources += tabs_source

qt6_dep = dependency('qt6', modules: ['Core', 'Gui', 'Widgets', 'openglwidgets'], required: true)

model_dep = declare_dependency(link_with: model_lib)
presenter_dep = declare_dependency(link_with: presenter_lib)
charts_dep =  declare_dependency(link_with: charts_lib)

deps = [
    qt6_dep,
    model_dep,
    presenter_dep,
    charts_dep
]

tabs_lib = static_library('tabs',
                          tabs_source,
                          include_directories : inc,
                          dependencies : deps)
