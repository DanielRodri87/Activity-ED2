# Readme temporarios com tutorial reaproveitado

1. GCC
```
sudo apt install build-essential
```
2. Meson
```
sudo apt-get -y install meson
```
3. Ninja
```
sudo apt-get -y install ninja-build
```
4. GTK
```
sudo apt-get -y install libgtk-3-dev
```
5. Bib-Gtk
```
sudo apt-get install libgdk-pixbuf2.0-dev
```
6. setup
```
meson setup builddir
```
7. build
```
ninja -C builddir
```
8. run
```
cd builddir
./main
```