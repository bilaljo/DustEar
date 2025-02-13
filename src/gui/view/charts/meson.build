charts_source = files('histogram.cpp',
                      'chart.cpp',
                      'line_chart.cpp',
                      #'heatmap.cpp',
)

project_sources += charts_source

qt6_dep = dependency('qt6', modules: ['Core', 'Gui', 'Widgets', 'openglwidgets'], required: true)
opengl_dep = dependency('opengl', required: true)

model_dep = declare_dependency(link_with: model_lib)
presenter_dep = declare_dependency(link_with: presenter_lib)

deps = [
    qt6_dep,
    model_dep,
    presenter_dep,
    opengl_dep
]

charts_lib = static_library('charts',
                            charts_source,
                            include_directories : inc,
                            dependencies : deps)
