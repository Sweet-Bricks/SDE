trampoline_inc = include_directories('include')

trampoline_lib = static_library(
    'trampoline',
    'src/trampoline.cpp',
    include_directories: trampoline_inc,
    dependencies: [core_dep],
)

trampoline_dep = declare_dependency(
    include_directories: trampoline_inc,
    link_with: trampoline_lib,
    dependencies: [core_dep],
)
