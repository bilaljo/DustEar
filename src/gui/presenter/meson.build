subdir('general_purpose')
subdir('settings')
subdir('tabs')

presenter_source = files('main_app.cpp')
project_sources += presenter_source

model_dep = declare_dependency(link_with: model_lib)
general_purpose_dep = declare_dependency(link_with: general_purpose_lib)
settings_dep = declare_dependency(link_with: settings_lib)
tabs_dep = declare_dependency(link_with: tabs_lib)

deps = [
    model_dep,
    general_purpose_dep,
    settings_dep,
    tabs_dep
]

presenter_lib = static_library('presenter',
                                presenter_source,
                                include_directories : inc,
                                dependencies : deps)
