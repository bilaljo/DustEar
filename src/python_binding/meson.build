py = import('python').find_installation(pure: false)

inc_dirs = include_directories('../../include', '../../third_party/include',
                               '../../third_party/include/cminpack')

processing_dep = declare_dependency(link_with: processing_lib)

#torch_dep = dependency('torch', required: true)

pydustear_module = py.extension_module('pydustear',
                                       files('python.cpp'),
                                       include_directories: inc_dirs,
                                       install: true,
                                       dependencies : [py.dependency(), processing_dep],
                                       cpp_args: host_machine.system() == 'windows' ? ['/O2'] : ['-O3'],
)
