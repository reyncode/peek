project('peek', 'c')

gnome = import('gnome')
i18n  = import('i18n')

data_dir = join_paths(meson.current_source_dir(), 'data')
po_dir = join_paths(meson.current_source_dir(), 'po')

prefix = get_option('prefix')
bindir = join_paths(prefix, get_option('bindir'))
datadir = join_paths(prefix, get_option('datadir'))
schemadir = join_paths(datadir, 'glib-2.0', 'schemas')

adwaita = dependency('libadwaita-1', version: '>= 1.0.0')
libgtop = dependency('libgtop-2.0')

deps = [
  adwaita,
  libgtop,
]

gnome.post_install(
  glib_compile_schemas: true,
  gtk_update_icon_cache: true,
)

subdir('src')
subdir('data')