cmake_minimum_required (VERSION 3.1)

project (TrackRender C)

find_package(PNG REQUIRED)
include_directories(${PNG_INCLUDE_DIR} ../libIsoRender/src ../libImage/src /usr/local/Cellar/jansson/2.12/include)
link_directories(../libIsoRender ../libImage /usr/local/Cellar/jansson/2.12/lib/)
link_directories(/usr/local/Cellar/embree/3.6.1/lib)
link_directories(/usr/local/Cellar/assimp/5.0.0/lib)

add_executable(TrackRender src/main.c src/track.c src/track_sections.c src/sprites.c)
set_property(TARGET TrackRender PROPERTY C_STANDARD 99)
target_link_libraries (TrackRender Image IsoRender ${PNG_LIBRARY} embree3 assimp jansson m)
