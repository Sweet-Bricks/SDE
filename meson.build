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
    ],
)

add_project_arguments(
    # Turn off useless wrnings
    '-Wno-c++98-compat-pedantic',
    '-Wno-padded',
    '-Wno-unsafe-buffer-usage-in-container',
    language: 'cpp',
)

subdir('third-party')
subdir('modules')
subdir('programs')
