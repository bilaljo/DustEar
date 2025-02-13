iir_source = files('Biquad.cpp',
                          'Butterworth.cpp',
                          'Cascade.cpp',
                          'Common.cpp',
                          'PoleFilter.cpp',
)

project_sources += iir_source

deps = [
]

iir_lib = static_library('iir',
                         iir_source,
                         include_directories : inc,
                         dependencies: deps)
