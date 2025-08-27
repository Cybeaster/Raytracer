cmake_minimum_required(VERSION 3.25)

# Auto-detect vcpkg toolchain if not explicitly provided
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    if(DEFINED ENV{VCPKG_ROOT} AND EXISTS "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
        set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE FILEPATH "Vcpkg toolchain file")
    elseif(DEFINED ENV{VCPKG_INSTALLATION_ROOT} AND EXISTS "$ENV{VCPKG_INSTALLATION_ROOT}/scripts/buildsystems/vcpkg.cmake")
        set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_INSTALLATION_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE FILEPATH "Vcpkg toolchain file")
    endif()
endif()

project(Raytracer)

set(CMAKE_CXX_STANDARD 20)

# Ensure new behavior regarding FindBoost removal under vcpkg wrapper.
if(POLICY CMP0167)
    cmake_policy(SET CMP0167 NEW)
endif()

# Prefer Boost via CONFIG mode (vcpkg), fall back to module, then fallback to header-only include discovery
find_package(Boost CONFIG QUIET COMPONENTS qvm)

if(NOT Boost_FOUND)
    find_package(Boost QUIET)
endif()

if(NOT Boost_FOUND)
    # Try to locate Boost headers installed by vcpkg manifest mode in common locations on windows
    set(_VCPKG_TRIPLETS x64-windows x86-windows arm64-windows)
    set(_BOOST_CANDIDATE_DIRS)
    foreach(_t ${_VCPKG_TRIPLETS})
        list(APPEND _BOOST_CANDIDATE_DIRS
            "${CMAKE_BINARY_DIR}/vcpkg_installed/${_t}"
            "${CMAKE_SOURCE_DIR}/vcpkg_installed/${_t}"
        )
    endforeach()
    if(EXISTS "${CMAKE_SOURCE_DIR}/out/build")
        file(GLOB _OUT_BUILD_VCPKG_DIRS "${CMAKE_SOURCE_DIR}/out/build/*/vcpkg_installed/*")
        list(APPEND _BOOST_CANDIDATE_DIRS ${_OUT_BUILD_VCPKG_DIRS})
    endif()

    set(_BOOST_HEADERS_FOUND FALSE)
    foreach(_base ${_BOOST_CANDIDATE_DIRS})
        if(EXISTS "${_base}/include/boost/qvm.hpp")
            set(Boost_INCLUDE_DIRS "${_base}/include")
            set(_BOOST_HEADERS_FOUND TRUE)
            message(STATUS "Found Boost headers at: ${Boost_INCLUDE_DIRS}")
            if(NOT TARGET Boost::boost)
                add_library(Boost::boost INTERFACE IMPORTED)
                set_target_properties(Boost::boost PROPERTIES
                    INTERFACE_INCLUDE_DIRECTORIES "${Boost_INCLUDE_DIRS}"
                )
            endif()
            if(NOT TARGET Boost::qvm)
                add_library(Boost::qvm INTERFACE IMPORTED)
                set_target_properties(Boost::qvm PROPERTIES
                    INTERFACE_LINK_LIBRARIES Boost::boost
                    INTERFACE_INCLUDE_DIRECTORIES "${Boost_INCLUDE_DIRS}"
                )
            endif()
            break()
        endif()
    endforeach()

    if(_BOOST_HEADERS_FOUND)
        set(Boost_FOUND TRUE)
    endif()
endif()

if(NOT Boost_FOUND)
    message(FATAL_ERROR
        "Boost not found. Set CMAKE_TOOLCHAIN_FILE to your vcpkg toolchain "
        "or install Boost and/or set Boost_INCLUDE_DIR(S). If using vcpkg manifest mode, "
        "ensure the vcpkg toolchain is active."
    )
endif()

file(GLOB_RECURSE RAYTRACER_SRC CONFIGURE_DEPENDS
        "${CMAKE_SOURCE_DIR}/Application/**/*.cpp"
        "${CMAKE_SOURCE_DIR}/Application/*.cpp"
        "${CMAKE_SOURCE_DIR}/Objects/**/*.cpp"
        "${CMAKE_SOURCE_DIR}/Types/**/*.cpp"
        "${CMAKE_SOURCE_DIR}/Utils/**/*.cpp"
)

# Optionally collect headers for IDE visibility (not required to build)
file(GLOB_RECURSE RAYTRACER_HEADERS CONFIGURE_DEPENDS
        "${CMAKE_SOURCE_DIR}/Application/**/*.h*"
        "${CMAKE_SOURCE_DIR}/Objects/**/*.h*"
        "${CMAKE_SOURCE_DIR}/Types/**/*.h*"
        "${CMAKE_SOURCE_DIR}/Utils/**/*.h*"
)

# Exclude tests from main target sources
# Only gather sources from your tests directory
file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/Test/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Test/*.cxx"
    "${CMAKE_CURRENT_SOURCE_DIR}/Test/*.cc"
)

# Build core library once; link it where needed
add_library(raytracer_core STATIC)
target_sources(raytracer_core PRIVATE ${RAYTRACER_SRC} ${RAYTRACER_HEADERS}
)

target_include_directories(raytracer_core PUBLIC
        ${CMAKE_SOURCE_DIR} # project includes
)
# Link Boost to core so all dependents inherit it
if(TARGET Boost::boost)
    target_link_libraries(raytracer_core PUBLIC Boost::boost)
endif()
if(TARGET Boost::qvm)
    target_link_libraries(raytracer_core PUBLIC Boost::qvm)
endif()
if(Boost_INCLUDE_DIRS)
    target_include_directories(raytracer_core PUBLIC ${Boost_INCLUDE_DIRS})
endif()

# Main app: just main.cpp, link the core
add_executable(Raytracer main.cpp)
target_link_libraries(Raytracer PRIVATE raytracer_core)

option(TESTING_ENABLED "Build tests" ON)
if(TESTING_ENABLED)
    if(TEST_SOURCES)
        add_executable(RaytracerTests ${TEST_SOURCES})
        target_link_libraries(RaytracerTests PRIVATE raytracer_core)
        include(CTest)
        enable_testing()
        add_test(NAME AllTests COMMAND RaytracerTests)
    else()
        message(STATUS "No test sources found in '${CMAKE_CURRENT_SOURCE_DIR}/tests'; skipping RaytracerTests target.")
    endif()
endif()