project(
    'sde',
    'cpp',
    version: '0.1.0',
    meson_version: '>=1.12',
    subproject_dir: 'third-party/subprojects',
    default_options: [
        'buildtype=debug',
        'cpp_std=c++23',
        'warning_level=everything',
        'werror=true',
        'wrap_mode=forcefallback',
    ],
)

add_project_arguments(
    # Turn off compat requirment for all standarts
    '-Wno-c++98-compat-pedantic',
    '-Wno-padded',
    language: 'cpp',
)

subdir('third-party')
subdir('modules')
subdir('programs')
