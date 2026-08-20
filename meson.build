project('ctimer', 'cpp', 'c')

complier = meson.get_compiler('cpp')
third_dep_dir = 'dependencies'
third_dep_dir_ab = meson.current_source_dir()/third_dep_dir

incl = include_directories('include')

src = files(
    'src/main.cpp'
)

raylib = dependency('raylib')
spdlog = dependency('spdlog')

incls = [ incl ]
deps = [ raylib, spdlog ]

extra_args = []
if get_option('buildtype') == 'release'
    extra_args += [ '-DRELEASE_BUILD' ]
endif

executable('ctimer', src, cpp_args: extra_args, include_directories: incls, dependencies: deps)
