# Set CMake modules search path
set (CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake/Modules)
# Include Urho3D Cmake common module
include (Urho3D-CMake-common)

set(CMAKE_DEBUG_POSTFIX "_d")

set (TARGET_NAME IoPluginDemo)

# Find Urho3D library
find_package (Urho3D REQUIRED)
include_directories (${URHO3D_INCLUDE_DIRS})
include_directories(${IOGRAM_SDK}/include/Core)
include_directories(${IOGRAM_SDK}/include/Geometry)
include_directories(${IOGRAM_SDK}/include/ThirdParty/Eigen)
include_directories(${IOGRAM_SDK}/include/ThirdParty/Libigl/include)

define_source_files()

set(RESOURCE_DIRS "")

setup_library(SHARED)

target_link_libraries(${TARGET_NAME} debug ${IOGRAM_SDK}/lib/Core_d.lib optimized ${IOGRAM_SDK}/lib/Core.lib )
target_link_libraries(${TARGET_NAME} debug ${IOGRAM_SDK}/lib/Geometry_d.lib  optimized ${IOGRAM_SDK}/lib/Geometry.lib )

