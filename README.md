# BAYER PYBIND11

Repozitář s PYBIND funkce pro debayerizaci obrazu do RGB.

## Návod na instalaci 

```
git clone git@github.com:neduchal/bayer_pybind.git
cd bayer_pybind
mkdir build
cd build
make
sudo make install
```

Instaluje do site-packages. Instalace nefunguje s prostredím conda. Nutno pro použití deaktivovat, nebo nainstalovat manualne

## Použití

```
import bayer

bayer.bayerBillinearUINT32(bayer_data, rgb_image, width, height, "BG")
```
