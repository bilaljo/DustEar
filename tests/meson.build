test_source = files('main.cpp',
                    'doctest.h',
)

deps = [
]

test_executable = executable('dustear_tests',
                    test_source,
                    include_directories : inc,
                    dependencies : deps,
                    link_with: [processing_lib]
)

test('tests', test_executable)
