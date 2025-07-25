cmake_minimum_required(VERSION 3.13)

project(medic)

set(_OS_NAME "")
 if (WIN32)
    set(_OS_NAME Windows)
elseif(APPLE)
    set(_OS_NAME macOS)
elseif(UNIX)
    set(_OS_NAME linux)
endif()


set(MEDIC_VERSION_MAJOR 1)
set(MEDIC_VERSION_MINOR 5)
set(MEDIC_VERSION_PATCH 5)

set(CMAKE_CXX_STANDARD 11)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(maya_cmake)

file(GLOB CORE_SRCS src/core/*.cpp)
add_library(medic SHARED ${CORE_SRCS})
target_include_directories(medic PUBLIC include)
setup_maya(medic)

execute_process(COMMAND ${MAYAPY_PATH} -c "import sys; sys.path.append('cython'); sys.argv = ['cython', '-I', 'include', '--cplus', '--embed-positions', '-3', '-o', '${CMAKE_CURRENT_SOURCE_DIR}/src/py/_medic.cpp', '${CMAKE_CURRENT_SOURCE_DIR}/src/py/_medic.pyx']; from Cython.Compiler.Main import main; main(command_line = 1)")
add_library(_medic SHARED src/py/_medic.cpp)
if (WIN32)
    set_target_properties(_medic PROPERTIES PREFIX "" SUFFIX ".pyd")
endif()
target_link_libraries(_medic PRIVATE medic)
setup_py(_medic)
setup_maya(_medic)


set(_PY_TESTERS "")
foreach (_tester_path IN ITEMS plugins/Tester ${MEDIC_TESTER_PATH})
    file(GLOB _testers ${_tester_path}/*.cpp)
    file(GLOB _pytesters ${_tester_path}/*.py)
    list(APPEND _PY_TESTERS ${_pytesters})

    foreach(_tester IN LISTS _testers)
        get_filename_component(_tester_name "${_tester}" NAME_WE)
        add_library(${_tester_name} SHARED ${_tester})
        if (APPLE)
            set_target_properties(${_tester_name} PROPERTIES PREFIX "" SUFFIX ".bundle")
        endif()

        target_link_libraries(${_tester_name} PUBLIC medic)
        setup_maya(${_tester_name})

        if (WIN32)
            install(TARGETS ${_tester_name} RUNTIME DESTINATION medic/plugins/Tester)
        else()
            install(TARGETS ${_tester_name} LIBRARY DESTINATION medic/plugins/Tester)
        endif()

    endforeach()

endforeach()

file(GLOB _KARTES plugins/Karte/*.karte)
if (WIN32)
    install(TARGETS medic RUNTIME DESTINATION medic/bin)
    install(TARGETS _medic RUNTIME DESTINATION medic/py)
else()
    install(TARGETS medic LIBRARY DESTINATION medic/lib)
    install(TARGETS _medic LIBRARY DESTINATION medic/py)
endif()

install(FILES src/py/medic.py DESTINATION medic/py)
install(FILES ${_PY_TESTERS} DESTINATION medic/plugins/Tester)
install(FILES ${_KARTES} DESTINATION medic/plugins/Karte)
install(DIRECTORY python/medicUI DESTINATION medic/py)
install(DIRECTORY include/medic DESTINATION medic/include)
install(FILES medic.mod DESTINATION .)

set(CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}/medic_${MEDIC_VERSION_MAJOR}.${MEDIC_VERSION_MINOR}.${MEDIC_VERSION_PATCH}_${_OS_NAME}_maya${MAYA_VERSION}_${CMAKE_SYSTEM_PROCESSOR}")

if (APPLE)
    set_target_properties(_medic PROPERTIES LINK_FLAGS "-Wl,-rpath,@loader_path/../lib")
elseif (UNIX)
    set_target_properties(_medic PROPERTIES LINK_FLAGS "-Wl,-rpath,$$ORIGIN/../lib")
endif()
