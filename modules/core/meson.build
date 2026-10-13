core_inc = include_directories('include')

core_lib = static_library(
    'core',
    'src/engine.cpp',
    'src/sdl_main.cpp',
    'src/system.cpp',
    include_directories : core_inc,
    dependencies : [sdl3_dep],
)

core_dep = declare_dependency(
    include_directories: core_inc,
    link_with: core_lib,
    dependencies: [sdl3_dep]
)
