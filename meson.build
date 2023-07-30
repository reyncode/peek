project('peek', 'c')

gnome = import('gnome')
i18n  = import('i18n')

prefix = get_option('prefix')
datadir = join_paths(prefix, get_option('datadir'))

adwaita = dependency('libadwaita-1', version: '>= 1.0.0')
libgtop = dependency('libgtop-2.0')


resources = gnome.compile_resources(
  'resources',
  'peek.gresource.xml',
  c_name: 'resources'
)

subdir('data')

deps = [
  adwaita,
  libgtop,
]

subdir('src')

gnome.post_install(
  gtk_update_icon_cache: true,
  update_desktop_database: true,
)