project('ctimer', 'cpp', 'c')

# complier = meson.get_compiler('cpp')

incl = include_directories('include')

raylib = dependency('raylib')
spdlog = dependency('spdlog')

incls = [ incl ]
deps = [ raylib, spdlog ]

src = files(
    'src/main.cpp',
    'src/screen_timer.cpp',
)

extra_args = []
if get_option('buildtype') == 'release'
    extra_args += [ '-DRELEASE_BUILD' ]
endif

executable('ctimer', src, cpp_args: extra_args, include_directories: incls, dependencies: deps)
