requires = ['sdl3', 'eigen3', 'spdlog']
dependencies = []
foreach name : requires
    dependencies += component_deps[name]
endforeach

core_inc = include_directories('include')

core_lib = shared_library(
    'core',
    'src/engine.cpp',
    'src/system.cpp',
    'src/obj_loader.cpp',
    'src/resource_path.cpp',
    'src/resource_provider.cpp',
    include_directories: core_inc,
    dependencies: dependencies,
    cpp_args: '-DSDE_CORE_BUILD',
    gnu_symbol_visibility: 'hidden',
    install: false,
)

core_dep = declare_dependency(
    include_directories: core_inc,
    link_with: core_lib,
    dependencies: dependencies,
)

core_runtime = [core_lib.full_path()]
core_runtime += fs.replace_suffix(core_lib.full_path(), '.pdb')

components += [{
    'name': 'core',
    'requires': requires,
    'source_dir': meson.current_source_dir(),
    'runtime': core_runtime,
}]
component_deps += {'core': core_dep}
