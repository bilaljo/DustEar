cminpack_source = files(
    'chkder.c',
    'covar.c',
    'covar1.c',
    'dogleg.c',
    'dpmpar.c',
    'enorm.c',
    'fdjac1.c',
    'fdjac2.c',
    'hybrd.c',
    'hybrd1.c',
    'hybrj.c',
    'hybrj1.c',
    'lmder.c',
    'lmder1.c',
    'lmdif.c',
    'lmdif1.c',
    'lmpar.c',
    'lmstr.c',
    'lmstr1.c',
    'qform.c',
    'qrfac.c',
    'qrsolv.c',
    'r1mpyq.c',
    'r1updt.c',
    'rwupdt.c'
)

project_sources += cminpack_source

deps = [
]

cminpack_lib = static_library('cminpack',
                         cminpack_source,
                         include_directories : inc,
                         dependencies: deps,
                         c_args: ['-D__cminpack_double__'])
