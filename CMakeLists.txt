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
    includes/debug.h
    includes/utility.h

    # third party headers
    third-party/meshoptimizer/src/meshoptimizer.h
    third-party/SIMDString/SIMDString.h
    third-party/glm/glm/glm.hpp
    third-party/glm/glm/gtc/type_ptr.hpp
    third-party/volk/volk.h
    third-party/vma/include/vk_mem_alloc.h
    third-party/glfw/include/GLFW/glfw3.h

    #[[
    third-party/imgui/imgui.h
    third-party/imgui/imgui_internal.h
    third-party/imgui/imconfig.h
    third-party/imgui/imstb_truetype.h
    third-party/imgui/imstb_rectpack.h
    third-party/imgui/imstb_textedit.h
    third-party/imgui/backends/imgui_impl_glfw.h
    third-party/imgui/backends/imgui_impl_vulkan.h
    #]]
)

set(SOURCE_FILES
    main.cpp

    sources/debug.cpp

    third-party/SIMDString/SIMDString.cpp

    #[[
    third-party/imgui/imgui.cpp
    third-party/imgui/imgui_widgets.cpp
    third-party/imgui/imgui_tables.cpp
    third-party/imgui/imgui_draw.cpp
    third-party/imgui/backends/imgui_impl_glfw.cpp
    third-party/imgui/backends/imgui_impl_vulkan.cpp
    #]]

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
    third-party/imgui
    third-party/imgui/backends
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
    <filesystem>
    <format>
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
