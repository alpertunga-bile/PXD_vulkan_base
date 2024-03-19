cmake_minimum_required(VERSION 3.5.0)
project(vulkan_proj VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

################################################################################
## GLOBAL DEFINITIONS

set(CMAKE_BUILD_TYPE Debug)
set(BUILD_SHARED_LIBS OFF)

list(APPEND CMAKE_CXX_COMPILER_LAUNCHER "ccache")

set(GLM_ENABLE_CXX_20 ON)
set(GLM_ENABLE_SIMD_AVX2 ON)

set(ASSIMP_INSTALL OFF)
set(ASSIMP_BUILD_TESTS OFF)
set(ASSIMP_NO_EXPORT ON)
set(ASSIMP_WARNINGS_AS_ERRORS OFF)

find_package(Vulkan REQUIRED)

set(HEADER_FILES
    # third party headers
    third-party/meshoptimizer/src/meshoptimizer.h
    third-party/SIMDString/SIMDString.h
    third-party/glm/glm/glm.hpp
    third-party/glm/glm/gtc/type_ptr.hpp
    third-party/volk/volk.h
    third-party/vma/include/vk_mem_alloc.h
    third-party/glfw/include/GLFW/glfw3.h
)

set(SOURCE_FILES
    main.cpp
    third-party/SIMDString/SIMDString.cpp
    ${HEADER_FILES}
)

include_directories(
    includes
    third-party/glm/glm
    third-party/assimp/include/assimp
    third-party/meshoptimizer/src
    third-party/glfw/include/GLFW
    third-party/volk
    third-party/vma/include
    third-party/SIMDString
    ${Vulkan_INCLUDE_DIRS}
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(
    ${PROJECT_NAME} PRIVATE
    glm
    assimp::assimp
    glfw
    meshoptimizer
    volk
    ${Vulkan_LIBRARIES}
)

set(CMAKE_BUILD_TYPE Release)
add_subdirectory(third-party/assimp)
add_subdirectory(third-party/glfw)
add_subdirectory(third-party/glm)
add_subdirectory(third-party/meshoptimizer)
add_subdirectory(third-party/volk)
set(CMAKE_BUILD_TYPE Debug)

target_precompile_headers(
    ${PROJECT_NAME} PRIVATE
    ${HEADER_FILES}
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
