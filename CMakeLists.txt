cmake_minimum_required(VERSION 3.7...3.29)

option(KIERO_INCLUDE_D3D9 "Enable support for D3D9" OFF)
option(KIERO_INCLUDE_D3D10 "Enable support for D3D10" OFF)
option(KIERO_INCLUDE_D3D11 "Enable support for D3D11" OFF)
option(KIERO_INCLUDE_D3D12 "Enable support for D3D12" OFF)
option(KIERO_INCLUDE_OPENGL "Enable support for OpenGL" OFF)
option(KIERO_INCLUDE_VULKAN "Enable support for Vulkan" OFF)
option(KIERO_USE_MINHOOK "Enables \"kiero::bind\" as a wrapper around MinHook" OFF)
option(KIERO_MINHOOK_EXTERNAL "Use an externally provided MinHook instead of the bundled" OFF)

project(kiero)

add_library(kiero STATIC
    src/kiero.cpp
)

if (NOT (KIERO_INCLUDE_D3D9 OR KIERO_INCLUDE_D3D10 OR KIERO_INCLUDE_D3D11 OR KIERO_INCLUDE_D3D12 OR KIERO_INCLUDE_OPENGL OR KIERO_INCLUDE_VULKAN))
    message(FATAL_ERROR "At least one graphics API must be specified")
endif()

if (KIERO_INCLUDE_OPENGL)
    find_package(OpenGL REQUIRED)
    target_link_libraries(kiero PRIVATE OpenGL::GL)
endif()

if (KIERO_INCLUDE_VULKAN)
    find_package(Vulkan REQUIRED)
    target_link_libraries(kiero PRIVATE Vulkan::Vulkan)
endif()

if (KIERO_USE_MINHOOK)
    if (NOT KIERO_MINHOOK_EXTERNAL)
        include(FetchContent)
        FetchContent_Declare(
            minhook
            GIT_REPOSITORY https://github.com/TsudaKageyu/minhook.git
            GIT_TAG        f5485b8454544c2f034c78f8f127c1d03dea3636
        )
        FetchContent_MakeAvailable(minhook)
    elseif (NOT TARGET minhook)
        message(FATAL_ERROR "minhook target not present")
    endif()
    target_link_libraries(kiero PRIVATE minhook)
endif()

set_target_properties(kiero PROPERTIES CXX_STANDARD 14)

target_compile_definitions(kiero PUBLIC
    "$<$<BOOL:${KIERO_INCLUDE_D3D9}>:KIERO_INCLUDE_D3D9>"
    "$<$<BOOL:${KIERO_INCLUDE_D3D10}>:KIERO_INCLUDE_D3D10>"
    "$<$<BOOL:${KIERO_INCLUDE_D3D11}>:KIERO_INCLUDE_D3D11>"
    "$<$<BOOL:${KIERO_INCLUDE_D3D12}>:KIERO_INCLUDE_D3D12>"
    "$<$<BOOL:${KIERO_INCLUDE_OPENGL}>:KIERO_INCLUDE_OPENGL>"
    "$<$<BOOL:${KIERO_INCLUDE_VULKAN}>:KIERO_INCLUDE_VULKAN>"
    "$<$<BOOL:${KIERO_USE_MINHOOK}>:KIERO_USE_MINHOOK>"
)

target_include_directories(kiero PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
    $<INSTALL_INTERFACE:include>
)
