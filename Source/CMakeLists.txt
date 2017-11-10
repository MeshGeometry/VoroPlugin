# Set CMake modules search path
set (CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake/Modules)

# Include Urho3D Cmake common module
include (Urho3D-CMake-common)

set(CMAKE_DEBUG_POSTFIX "_d")

set (TARGET_NAME VoroPlugin)

# Find Urho3D library
find_package (Urho3D REQUIRED)
include_directories (${URHO3D_INCLUDE_DIRS})
include_directories(${IOGRAM_SDK}/Core)
include_directories(${IOGRAM_SDK}/Geometry)
include_directories(${IOGRAM_SDK}/ThirdParty/Eigen)
include_directories(${IOGRAM_SDK}/ThirdParty/Libigl/include)
include_directories(../Voro++)

define_source_files()

set(RESOURCE_DIRS "")

setup_library(SHARED)


# Setup target with resource copying
if (APPLE)
target_link_libraries(${TARGET_NAME} debug /Users/Mesh/Dev/IogramEditor/build/Source/Core/Debug/libCore_d.a optimized ${IOGRAM_SDK}/../Build/Win64/Source/Core/Release/Core.lib )
target_link_libraries(${TARGET_NAME} debug /Users/Mesh/Dev/IogramEditor/build/Source/Geometry/Debug/libGeometry_d.a  optimized ${IOGRAM_SDK}/../Build/Win64/Source/Geometry/Release/Geometry.lib )
target_link_libraries(${TARGET_NAME} debug ${CMAKE_SOURCE_DIR}/Build/gcc_build/Voro++/Debug/libvoro-cpp.a optimized ${CMAKE_SOURCE_DIR}/Build/Win64/Voro++/Release/voro-cpp.lib )

  else()
target_link_libraries(${TARGET_NAME} debug ${IOGRAM_SDK}/../Build/Win64/Source/Core/Debug/Core_d.lib optimized ${IOGRAM_SDK}/../Build/Win64/Source/Core/Release/Core.lib )
target_link_libraries(${TARGET_NAME} debug ${IOGRAM_SDK}/../Build/Win64/Source/Geometry/Debug/Geometry_d.lib  optimized ${IOGRAM_SDK}/../Build/Win64/Source/Geometry/Release/Geometry.lib )
target_link_libraries(${TARGET_NAME} debug ${CMAKE_SOURCE_DIR}/Build/Win64/Voro++/Debug/voro-cpp.lib optimized ${CMAKE_SOURCE_DIR}/Build/Win64/Voro++/Release/voro-cpp.lib )

endif()