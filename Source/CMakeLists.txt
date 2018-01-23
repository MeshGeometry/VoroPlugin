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
include_directories(${IOGRAM_SDK}/ThirdParty/Libigl/igl)
include_directories(../Voro++)

define_source_files()

set(RESOURCE_DIRS "")

setup_library(SHARED)


# Setup target with resource copying
if (APPLE)
#get_filename_component(IOGRAM_HOME ${IOGRAM_SDK} PATH)
target_link_libraries(${TARGET_NAME} debug ${IOGRAM_SDK}/build/Source/Core/Debug/libCore_d.a optimized ${IOGRAM_SDK}/build/Source/Core/Release/libCore.a )
target_link_libraries(${TARGET_NAME} debug ${IOGRAM_SDK}/build/Source/Geometry/Debug/libGeometry_d.a  optimized ${IOGRAM_SDK}/build/Source/Geometry/Release/libGeometry.a )
target_link_libraries(${TARGET_NAME} debug ${CMAKE_SOURCE_DIR}/Build/gcc_build/Voro++/Debug/libvoro-cpp.a optimized ${CMAKE_SOURCE_DIR}/Build/gcc_build/Voro++/Release/libvoro-cpp.a)
target_link_libraries(${TARGET_NAME} "-framework SystemConfiguration") 
  else()
target_link_libraries(${TARGET_NAME} debug ${IOGRAM_SDK}/Build/Win64/lib/Core_d.lib optimized ${IOGRAM_SDK}/Build/Win64/lib/Core.lib )
target_link_libraries(${TARGET_NAME} debug ${IOGRAM_SDK}/Build/Win64/lib/Geometry_d.lib  optimized ${IOGRAM_SDK}/Build/Win64/lib/Geometry.lib )
target_link_libraries(${TARGET_NAME} debug ${CMAKE_SOURCE_DIR}/Build/Win64/Voro++/Debug/voro-cpp.lib optimized ${CMAKE_SOURCE_DIR}/Build/Win64/Voro++/Release/voro-cpp.lib )

endif()