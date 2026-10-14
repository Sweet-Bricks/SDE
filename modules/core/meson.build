core_inc = include_directories('include')

core_lib = shared_library(
    'core',
    'src/engine.cpp',
    'src/system.cpp',
    include_directories: core_inc,
    dependencies: [sdl3_dep, eigen3_dep],
    cpp_args: '-DSDE_CORE_BUILD',
    gnu_symbol_visibility: 'hidden',
    install: true,
)

core_dep = declare_dependency(
    include_directories: core_inc,
    link_with: core_lib,
    dependencies: [sdl3_dep, eigen3_dep],
)
