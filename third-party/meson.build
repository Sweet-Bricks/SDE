# SDL3
sdl3_project = subproject(
    'sdl3',
    default_options: ['warning_level=0', 'werror=false'],
)
sdl3_dep = dependency('sdl3', include_type: 'system')
sdl3_lib = sdl3_project.get_variable('sdl_lib')
sdl3_runtime = [sdl3_lib.full_path()]
sdl3_runtime += fs.replace_suffix(sdl3_lib.full_path(), '.pdb')

# Eigen
eigen3_dep = dependency(
    'eigen3',
    include_type: 'system',
    default_options: ['warning_level=0', 'werror=false'],
)

# Doctest
doctest_dep = dependency(
    'doctest',
    include_type: 'system',
    default_options: ['warning_level=0', 'werror=false'],
)

# spdlog
spdlog_project = subproject(
    'spdlog',
    default_options: ['warning_level=0', 'werror=false', 'tests=disabled', 'cpp_args=-DSPDLOG_NO_EXCEPTIONS'],
)
spdlog_dep = dependency('spdlog', include_type: 'system')
spdlog_lib = spdlog_project.get_variable('spdlog_lib')
spdlog_runtime = [spdlog_lib.full_path()]
spdlog_runtime += fs.replace_suffix(spdlog_lib.full_path(), '.pdb')

# Fill component tables
component_deps += {
    'sdl3': sdl3_dep,
    'eigen3': eigen3_dep,
    'doctest': doctest_dep,
    'spdlog': spdlog_dep,
}
components += [
    {'name': 'sdl3', 'requires': [], 'runtime': sdl3_runtime},
    {'name': 'eigen3', 'requires': [], 'runtime': []},
    {'name': 'doctest', 'requires': [], 'runtime': []},
    {'name': 'spdlog', 'requires': [], 'runtime': spdlog_runtime},
]
