project('peek', 'c')

gnome = import('gnome')

data_dir = join_paths(meson.current_source_dir(), 'data')

prefix = get_option('prefix')
datadir = join_paths(prefix, get_option('datadir'))

gtk = dependency('gtk+-3.0', version: '>= 3.22.0')
procps = dependency('libprocps')

deps = [
  gtk,
  procps,
]

subdir('data')
subdir('src')
