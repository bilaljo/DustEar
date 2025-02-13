# meson files are based on
# https://github.com/tiernemi/meson-sample-project/tree/master

project('dustear', ['cpp', 'c'],
        version : '1.0',
        license : [ 'MIT'],
        meson_version : '>= 0.64.0',
        default_options : ['warning_level=3', 'buildtype=release', 'cpp_std=c++20', 'optimization=3',
                           'b_ndebug=true', 'b_lto=true']
)

cpp_args = []


# Variables tracking sources and libraries
library_path = []
project_sources = []
project_header_files = []
project_test_sources = []
project_benchmark_sources = []

inc = [include_directories('include'), include_directories('third_party/include/cminpack')]

subdir('include')
subdir('third_party')


# This is where you should add in include directories
# This triggers the builds of sources
subdir('src')
# This links all the static libs into the main source file to form a binary


qt6 = import('qt6')
moc_headers = files('include/gui/view/widgets/animated_toggle.hpp',
                    'include/gui/view/widgets/toggle.hpp',
)

moc_files = qt6.compile_moc(headers: moc_headers,
                            extra_args: ['-DMAKES_MY_MOC_HEADER_COMPILE'],
                            include_directories: inc,
                            dependencies: qt6_dep)

deps = [
    qt6_dep,
]

libs = [
        driver_lib,
        processing_lib,
        view_lib,
]


if host_machine.system() == 'windows'
    windows = import('windows')
    win = windows.compile_resources('assets/dustear.rc')
    main_source += win
endif



sample_cpp_project_bin = executable('dustear',
                                    main_source,
                                    moc_files,
                                    include_directories : inc,
                                    dependencies : deps,
                                    link_with : libs,
                                    cpp_args : cpp_args,
)

subdir('tests')

# This regex excludes any sources from the third_party, tests, benchmarks
# and gtest related files.
regex = '^((?!(third_party|tests|benchmarks|gtest)).)*$'

#run_target('tidy',
#           command : ['run-clang-tidy.py','-fix', '-j' , '8', 'files', regex ,'-format', '-p='+ meson.build_root()])
