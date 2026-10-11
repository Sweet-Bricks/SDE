project(
    'sde',
    'cpp',
    version: '0.1.0',
    meson_version: '>=1.12',
    subproject_dir: 'third-party',
    default_options: [
        'buildtype=debug',
        'cpp_std=none',
        'warning_level=everything',
        'werror=true',
    ],
)

subdir('build-logic')

sdl3_dep = dependency(
    'sdl3',
    include_type: 'system',
    default_options: ['warning_level=0', 'werror=false', 'default_library=static'],
)

subdir('modules/core')
subdir('programs/game')
