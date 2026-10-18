project(
    'sde',
    'cpp',
    version: '0.1.0',
    meson_version: '>=1.12',
    subproject_dir: 'third-party/subprojects',
    default_options: [
        'buildtype=debugoptimized',
        'cpp_std=c++23',
        'cpp_eh=none',
        'warning_level=everything',
        'werror=true',
        'wrap_mode=forcefallback',
        'default_library=shared',
    ],
)

add_project_arguments(
    # Turn off useless wrnings
    '-Wno-c++98-compat-pedantic',
    '-Wno-padded',
    '-Wno-unsafe-buffer-usage-in-container',
    '-Wno-unsafe-buffer-usage',
    '-Wno-covered-switch-default',
    '-Wno-newline-eof',
    language: 'cpp',
)

add_global_arguments(
    '-DSPDLOG_NO_EXCEPTIONS',
    language: 'cpp'
)

fs = import('fs')
python = import('python').find_installation()

components = [] # List of maps for asset system
component_deps = {} # Modules depth registry for collecting transitive modules

subdir('third-party')
subdir('modules')
subdir('programs')

# Finalization magic
subdir('build')
