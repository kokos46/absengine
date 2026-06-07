project('absengine', 'cpp',
        version : '1.0.0',
        default_options : ['warning_level=3', 'cpp_std=c++20'])

# Находим raylib в системе
# Meson сам вызовет pkg-config и найдет все пути
raylib_dep = dependency('raylib', required : true)
qt6_dep = dependency('qt6',
                     modules : ['Core', 'Gui', 'Widgets'],
                     required : get_option('level_editor'))
copy_program = find_program('cp')

# Исходники
src = [
    'src/main.cpp',
    'src/scenes/Scene.cpp',
    'src/scenes/SceneManager.cpp',
    'src/entities/Entity.cpp',
    'src/entities/Player.cpp',
    'src/animations/Animator.cpp'
]

# Исполняемый файл
executable('game',
           src,
           dependencies : [raylib_dep]
)

if qt6_dep.found()
    executable('level_editor',
               'src/editor/main.cpp',
               dependencies : [qt6_dep])
endif

custom_target('copy_player_sprite',
              input : 'assets/npc01_spritesheet.png',
              output : 'npc01_spritesheet.png',
              command : [copy_program, '@INPUT@', '@OUTPUT@'],
              build_by_default : true)
