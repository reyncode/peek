desktop_file = i18n.merge_file(
  input: 'com.github.reyncode.peek.desktop.in',
  output: 'com.github.reyncode.peek.desktop',
  type: 'desktop',
  po_dir: po_dir,
  install: true,
  install_dir: join_paths(datadir, 'applications'),
)

install_data(
  'org.gnome.Adwaita1.Demo.svg',
  install_dir: datadir / 'icons' / 'hicolor' / 'scalable' / 'apps'
)

install_data(
  'com.github.reyncode.peek.gschema.xml',
  install_dir: schemadir,
)
