cmake_minimum_required(VERSION 3.13)
project(bayer_bilinear)

set(CMAKE_CXX_STANDARD 11)

# Najdi Python 3
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)

# Použití pybind11 s Python 3
find_package(pybind11 REQUIRED CONFIG)

# Přidání modulu
pybind11_add_module(bayer MODULE bayer.cpp)

# Zjištění cesty pro instalaci Python modulů (site-packages) pro Python 3
execute_process(
    # COMMAND ${Python3_EXECUTABLE} -c "import sysconfig; print(sysconfig.get_paths()['purelib'])"
    COMMAND ${Python3_EXECUTABLE} -c "import site; print(site.getusersitepackages())"
    OUTPUT_VARIABLE PYTHON_SITE_PACKAGES
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Instalace do site-packages
install(TARGETS bayer
        LIBRARY DESTINATION ${PYTHON_SITE_PACKAGES})

# Další nastavení pro různé platformy
if(APPLE)
    set(CMAKE_INSTALL_NAME_DIR "@rpath")
    set(CMAKE_MACOSX_RPATH 1)
endif()

# Volitelně: Pokud chcete knihovnu nainstalovat i do systémových knihoven
#install(TARGETS bayer
#        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})