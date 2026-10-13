game_inc = include_directories('include')

executable(
    'game',
    'src/game_entry.cpp',
    'src/game_system.cpp',
    include_directories : game_inc,
    dependencies: [trampoline_dep],
    install: true,
)
