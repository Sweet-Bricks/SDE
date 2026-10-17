requires = ['core', 'doctest']
dependencies = []
foreach name : requires
    dependencies += component_deps[name]
endforeach

unittest_inc = include_directories('include')

unittest_exe = executable(
    'unittest',
    'src/main.cpp',
    'src/obj_loader.unittest.cpp',
    include_directories : unittest_inc,
    dependencies: dependencies,
    cpp_pch: '../../build/include/std_pch.hpp',
    install: true,
    install_dir: get_option('prefix') / 'unittest',
    install_tag: 'runtime',
)

test('unittest', unittest_exe, verbose: true)

unittest_runtime = [fs.replace_suffix(unittest_exe.full_path(), '.pdb')]

components += [{
    'name': 'unittest',
    'requires': requires,
    'source_dir': meson.current_source_dir(),
    'runtime': unittest_runtime,
    'program': true,
    'build_dir': meson.current_build_dir(),
}]
