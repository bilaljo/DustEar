general_purpose_source = files('logging.cpp',
                               'toolbar.cpp',
                               'progress_bar.cpp',
                               'status_bar.cpp',
)

project_sources += general_purpose_source

model_dep = declare_dependency(link_with: model_lib)

deps = [
    model_dep
]

general_purpose_lib = static_library('general_purpose',
                                     general_purpose_source,
                                     include_directories : inc,
                                     dependencies : deps)
