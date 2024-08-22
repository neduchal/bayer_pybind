cmake_minimum_required(VERSION 3.13)
project(bayer_bilinear)

set(CMAKE_CXX_STANDARD 11)

find_package(pybind11 REQUIRED)

# Přidání modulu
pybind11_add_module(bayer MODULE bayer.cpp)

# Specifikace instalační cesty
install(TARGETS bayer
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

# Instalace do pythonového site-packages (volitelné)
install(TARGETS bayer
        LIBRARY DESTINATION ${PYTHON_SITE_PACKAGES}
        ARCHIVE DESTINATION ${PYTHON_SITE_PACKAGES})