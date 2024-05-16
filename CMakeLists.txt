cmake_minimum_required(VERSION 3.5.0)
project(pxd_vulkan_proj VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

################################################################################
## Function DEFINITIONS

function(set_conf conf_name)
    if(NOT MSVC)
        set(CMAKE_BUILD_TYPE ${conf_name})
    endif()
endfunction()

function(set_debug)
    set_conf(Debug)
endfunction()

function(set_release)
    set_conf(Release)
endfunction(set_release)

################################################################################
## GLOBAL DEFINITIONS

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
endif(NOT MSVC)

SET_DEBUG()

set(PXD_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/includes)
set(PXD_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/sources)
set(PXD_THIRD_PARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third-party)
set(PXD_SHADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/shaders)
set(PXD_COMPILED_SHADER_DIR ${PXD_SHADER_DIR}/compiled)
set(PXD_COMPILED_SHADER_INCLUDE_DIR ${PXD_INCLUDE_DIR}/shaders)

set(PXD_OPTIMIZE_SHADERS "ON")

################################################################################
## Submodule DEFINITIONS

set(BUILD_SHARED_LIBS OFF)

set(GLM_ENABLE_CXX_20 ON)
set(GLM_ENABLE_SIMD_AVX2 ON)

set(ASSIMP_INSTALL OFF)
set(ASSIMP_BUILD_TESTS OFF)
set(ASSIMP_NO_EXPORT ON)
set(ASSIMP_WARNINGS_AS_ERRORS OFF)

find_package(Vulkan REQUIRED)
find_program(GLSL_VALIDATOR glslangValidator HINTS /usr/bin /usr/local/bin $ENV{VULKAN_SDK}/Bin/ $ENV{VULKAN_SDK}/Bin32/)
find_program(SPIRV_OPT spirv-opt HINTS /usr/bin /usr/local/bin $ENV{VULKAN_SDK}/Bin/ $ENV{VULKAN_SDK}/Bin32/)


################################################################################
## Project DEFINITIONS

set(PXD_HEADER_FILES
    ${PXD_INCLUDE_DIR}/debug.h
    ${PXD_INCLUDE_DIR}/utility.h
    ${PXD_INCLUDE_DIR}/logger.hpp
    ${PXD_INCLUDE_DIR}/gui_config.h

    # third party headers
    ${PXD_THIRD_PARTY_DIR}/meshoptimizer/src/meshoptimizer.h
    ${PXD_THIRD_PARTY_DIR}/SIMDString/SIMDString.h
    ${PXD_THIRD_PARTY_DIR}/glm/glm/glm.hpp
    ${PXD_THIRD_PARTY_DIR}/glm/glm/gtc/type_ptr.hpp
    ${PXD_THIRD_PARTY_DIR}/volk/volk.h
    ${PXD_THIRD_PARTY_DIR}/vma/include/vk_mem_alloc.h
    ${PXD_THIRD_PARTY_DIR}/glfw/include/GLFW/glfw3.h
    ${PXD_THIRD_PARTY_DIR}/fmt/include/fmt/core.h
    ${PXD_THIRD_PARTY_DIR}/fmt/include/fmt/os.h

    ${PXD_THIRD_PARTY_DIR}/imgui/imgui.h
    ${PXD_THIRD_PARTY_DIR}/imgui/imgui_internal.h
    ${PXD_THIRD_PARTY_DIR}/imgui/imconfig.h
    ${PXD_THIRD_PARTY_DIR}/imgui/imstb_truetype.h
    ${PXD_THIRD_PARTY_DIR}/imgui/imstb_rectpack.h
    ${PXD_THIRD_PARTY_DIR}/imgui/imstb_textedit.h
    ${PXD_THIRD_PARTY_DIR}/imgui/backends/imgui_impl_glfw.h
    ${PXD_THIRD_PARTY_DIR}/imgui/backends/imgui_impl_vulkan.h
)

set(PXD_SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp

    ${PXD_SOURCE_DIR}/debug.cpp
    ${PXD_SOURCE_DIR}/logger.cpp

    ${PXD_THIRD_PARTY_DIR}/SIMDString/SIMDString.cpp

    ${PXD_THIRD_PARTY_DIR}/imgui/imgui.cpp
    ${PXD_THIRD_PARTY_DIR}/imgui/imgui_widgets.cpp
    ${PXD_THIRD_PARTY_DIR}/imgui/imgui_tables.cpp
    ${PXD_THIRD_PARTY_DIR}/imgui/imgui_draw.cpp
    ${PXD_THIRD_PARTY_DIR}/imgui/backends/imgui_impl_glfw.cpp
    ${PXD_THIRD_PARTY_DIR}/imgui/backends/imgui_impl_vulkan.cpp

    ${PXD_HEADER_FILES}
)

include_directories(
    ${PXD_INCLUDE_DIR}
    ${PXD_THIRD_PARTY_DIR}/fmt/include/fmt
    ${PXD_THIRD_PARTY_DIR}/glm/glm
    ${PXD_THIRD_PARTY_DIR}/assimp/include/assimp
    ${PXD_THIRD_PARTY_DIR}/meshoptimizer/src
    ${PXD_THIRD_PARTY_DIR}/glfw/include/GLFW
    ${PXD_THIRD_PARTY_DIR}/volk
    ${PXD_THIRD_PARTY_DIR}/vma/include
    ${PXD_THIRD_PARTY_DIR}/SIMDString
    ${PXD_THIRD_PARTY_DIR}/imgui
    ${PXD_THIRD_PARTY_DIR}/imgui/backends
    ${Vulkan_INCLUDE_DIRS}
)

add_compile_definitions(IMGUI_USER_CONFIG="${PXD_INCLUDE_DIR}/gui_config.h")

SET_RELEASE()
add_subdirectory(${PXD_THIRD_PARTY_DIR}/assimp)
add_subdirectory(${PXD_THIRD_PARTY_DIR}/glfw)
add_subdirectory(${PXD_THIRD_PARTY_DIR}/glm)
add_subdirectory(${PXD_THIRD_PARTY_DIR}/meshoptimizer)
add_subdirectory(${PXD_THIRD_PARTY_DIR}/volk)
add_subdirectory(${PXD_THIRD_PARTY_DIR}/fmt)
SET_DEBUG()

set(PXD_LIBS_TO_LINK
    glm
    assimp::assimp
    glfw
    meshoptimizer
    volk
    fmt::fmt
)

add_executable(${PROJECT_NAME} ${PXD_SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE ${PXD_LIBS_TO_LINK})

################################################################################
## Compile Shaders

if(GLSL_VALIDATOR)
    set(TARGET_ENV vulkan1.3)
    set(GLSL_VERSION 460)

    file(GLOB_RECURSE PXD_GLSL_FILES
        "${PXD_SHADER_DIR}/*.frag"
        "${PXD_SHADER_DIR}/*.vert"
        "${PXD_SHADER_DIR}/*.comp"
        "${PXD_SHADER_DIR}/*.mesh"
        "${PXD_SHADER_DIR}/*.task"
        "${PXD_SHADER_DIR}/*.tesc"
        "${PXD_SHADER_DIR}/*.tese"
        "${PXD_SHADER_DIR}/*.glsl"
        "${PXD_SHADER_DIR}/*.geom"
        "${PXD_SHADER_DIR}/*.rgen"
        "${PXD_SHADER_DIR}/*.rint"
        "${PXD_SHADER_DIR}/*.rahit"
        "${PXD_SHADER_DIR}/*.rchit"
        "${PXD_SHADER_DIR}/*.rmiss"
        "${PXD_SHADER_DIR}/*.rcall"
    )

    foreach(GLSL_FILE ${PXD_GLSL_FILES})
        get_filename_component(GLSL_FILENAME ${GLSL_FILE} NAME_WE)

        set(SPIRV_FILE "${PXD_COMPILED_SHADER_DIR}/${GLSL_FILENAME}.spv")
        
        set(SPIRV_OPT_NAME "${GLSL_FILENAME}_opt")
        set(SPIRV_OPT_FILE "${PXD_COMPILED_SHADER_DIR}/${SPIRV_OPT_NAME}.spv")

        set(SPIRV_C_NAME "${GLSL_FILENAME}_spv")
        set(SPIRV_C_HEADER_FILE "${PXD_COMPILED_SHADER_INCLUDE_DIR}/${SPIRV_C_NAME}.h")

        add_custom_target(
            SPIRV_TARGET
            COMMAND ${GLSL_VALIDATOR} -V ${GLSL_FILE} -o ${SPIRV_FILE} -e main -t --glsl-version ${GLSL_VERSION} --enhanced-msgs --target-env ${TARGET_ENV} -I${PXD_SHADER_DIR}
        )

        add_custom_target(
            SPIRV_C_TARGET
            COMMAND ${GLSL_VALIDATOR} -V ${GLSL_FILE} -o ${SPIRV_C_HEADER_FILE} -e main -t --glsl-version ${GLSL_VERSION} --enhanced-msgs --target-env ${TARGET_ENV} -I${PXD_SHADER_DIR} --vn ${SPIRV_C_NAME}
        )

        add_dependencies(${PROJECT_NAME} SPIRV_TARGET SPIRV_C_TARGET)

        if(SPIRV_OPT_TARGET AND (${PXD_OPTIMIZE_SHADERS} EQUAL "ON"))
            add_custom_target(
                SPIRV_OPT_TARGET
                COMMAND ${SPIRV_OPT} ${SPIRV_FILE} -O -o ${SPIRV_OPT_FILE} --strip-debug --target-env=${TARGET_ENV} --upgrade-memory-model --remove-duplicates --amd-ext-to-khr
            )

            add_dependencies(${PROJECT_NAME} SPIRV_OPT_TARGET)
        endif()
    endforeach()
else()
    message(STATUS "Cant find the GLSL Validator program so skipping building shaders")
endif()


target_precompile_headers(
    ${PROJECT_NAME} PRIVATE
    ${PXD_HEADER_FILES}
    <filesystem>
    <fstream>
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
