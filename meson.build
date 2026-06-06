project('absengine', 'cpp',
        version : '1.0.0',
        default_options : ['warning_level=3', 'cpp_std=c++20'])

# Находим raylib в системе
# Meson сам вызовет pkg-config и найдет все пути
raylib_dep = dependency('raylib', required : true)

# Исходники
src = [
    'src/main.cpp',
    'src/scenes/Scene.cpp',
    'src/scenes/SceneManager.cpp'
]

# Исполняемый файл
executable('game',
           src,
           dependencies : [raylib_dep]
)