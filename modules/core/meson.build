core_inc = include_directories('include')

core_lib = static_library(
    'core',
    'src/math.cpp',
    include_directories: core_inc,
)

core_dep = declare_dependency(
    include_directories: core_inc,
    link_with: core_lib,
)
