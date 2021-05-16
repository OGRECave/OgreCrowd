cmake_minimum_required(VERSION 3.4.3)
project(OgreCrowd)
find_package(OGRE REQUIRED CONFIG)

option(BUILD_DEMO "Build demo" ON)

# recast lib versions
SET(SOVERSION 1)
SET(VERSION 1.0.0)

include_directories(include/ 
  Samples/include
  recastnavigation/RecastDemo/Contrib/fastlz/
  /usr/include/ois)

add_subdirectory("recastnavigation/Recast")
add_subdirectory("recastnavigation/Detour")
add_subdirectory("recastnavigation/DebugUtils")
add_subdirectory("recastnavigation/DetourCrowd")
add_subdirectory("recastnavigation/DetourTileCache")

file(GLOB ogrecrowdcpp "${PROJECT_SOURCE_DIR}/src/*.cpp")
file(GLOB fastlzcpp "${PROJECT_SOURCE_DIR}/recastnavigation/RecastDemo/Contrib/fastlz/*.c")

add_library(OgreCrowd ${ogrecrowdcpp} ${fastlzcpp})
target_link_libraries(OgreCrowd ${OGRE_LIBRARIES} Recast Detour DebugUtils DetourCrowd DetourTileCache)

INSTALL(TARGETS OgreCrowd
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)

if(BUILD_DEMO)
  file(COPY OgreRecast.cfg DESTINATION ${PROJECT_BINARY_DIR})
  configure_file(resources.cfg.in ${PROJECT_BINARY_DIR}/resources.cfg)

  file(GLOB democpp "${PROJECT_SOURCE_DIR}/Samples/src/*.cpp")
  add_executable(OgreCrowdDemo ${democpp})
  target_link_libraries(OgreCrowdDemo OgreCrowd)
endif()