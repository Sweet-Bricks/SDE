requires = ['core']
dependencies = []
foreach name : requires
    dependencies += component_deps[name]
endforeach

trampoline_inc = include_directories('include')

trampoline_lib = static_library(
    'trampoline',
    'src/trampoline.cpp',
    include_directories: trampoline_inc,
    dependencies: dependencies,
)

trampoline_dep = declare_dependency(
    include_directories: trampoline_inc,
    link_with: trampoline_lib,
    dependencies: dependencies,
)

components += [{
    'name': 'trampoline',
    'requires': requires,
    'source_dir': meson.current_source_dir(),
    'runtime': [],
}]
component_deps += {'trampoline': trampoline_dep}
