project('peek', 'c')

gnome = import('gnome')

data_dir = join_paths(meson.current_source_dir(), 'data')

prefix = get_option('prefix')
datadir = join_paths(prefix, get_option('datadir'))

adwaita = dependency('libadwaita-1', version: '>= 1.0.0')
libgtop = dependency('libgtop-2.0')

deps = [
  adwaita,
  libgtop,
]

subdir('data')
subdir('src')
