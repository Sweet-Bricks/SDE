requires = ['trampoline']
dependencies = []
foreach name : requires
    dependencies += component_deps[name]
endforeach

game_inc = include_directories('include')

game_exe = executable(
    'game',
    'src/game_entry.cpp',
    'src/game_system.cpp',
    include_directories : game_inc,
    dependencies: dependencies,
    cpp_pch: '../../build/include/std_pch.hpp',
    install: true,
    install_dir: get_option('prefix') / 'game',
    install_tag: 'runtime',
)

game_runtime = [fs.replace_suffix(game_exe.full_path(), '.pdb')]

components += [{
    'name': 'game',
    'requires': requires,
    'source_dir': meson.current_source_dir(),
    'runtime': game_runtime,
    'program': true,
    'build_dir': meson.current_build_dir(),
}]
