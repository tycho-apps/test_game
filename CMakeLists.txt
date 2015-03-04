cmake_minimum_required (VERSION 2.8)

set(link_libs "tyengine;tyui;tymirror;tycore;tyinput;tygraphics")

# TODO : why does it link to d3dx??
if(ty_build_dx9)
    include_directories("${CMAKE_SOURCE_DIR}/3rdparty/pc/dx9")
    link_directories("${CMAKE_SOURCE_DIR}/3rdparty/pc/dx9/lib")
    list(APPEND link_libs "tydx9")
endif()

tycho_add_application(test_game "${link_libs}")
