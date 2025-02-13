subdir('settings')
subdir('general_purpose')
subdir('charts')
subdir('tabs')

view_source = files('main_window.cpp',
)

project_sources += view_source

tabs_dep = declare_dependency(link_with: tabs_lib)
settings_dep = declare_dependency(link_with: settings_lib)
presenter_dep = declare_dependency(link_with: presenter_lib)
general_purpose_dep = declare_dependency(link_with: general_purpose_lib)

qt6_dep = dependency('qt6', modules: ['Core', 'Gui', 'Widgets', 'openglwidgets'], required: true)



deps = [
    tabs_dep,
    settings_dep,
    general_purpose_dep,
    presenter_dep,
    qt6_dep
]

view_lib = static_library('view',
                          view_source,
                          include_directories : inc,
                          dependencies : deps)
