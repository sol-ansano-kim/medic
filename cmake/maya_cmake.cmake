macro(find_maya)
    if (NOT DEFINED MAYA_ROOT)
        if (NOT DEFINED MAYA_VERSION)
            if(WIN32)
                execute_process(
                    COMMAND powershell -Command "Get-ChildItem -Path 'C:\\Program Files\\Autodesk' -Directory | ForEach-Object { $_.Name }"
                    OUTPUT_VARIABLE ADSK_PRGRMS
                    OUTPUT_STRIP_TRAILING_WHITESPACE
                )

                foreach(adsk_dir ${ADSK_PRGRMS})
                    string(REGEX MATCH "[mM]aya[0-9.]+" MATCH_RES ${adsk_dir})

                    if (MATCH_RES)
                        set(MAYA_ROOT "C:/Program Files/Autodesk/${MATCH_RES}")
                        break()
                    endif()
                endforeach()

            elseif(APPLE)
                execute_process(
                    COMMAND find /Applications/Autodesk -mindepth 1 -maxdepth 1 -type d
                    OUTPUT_VARIABLE DIR_NAMES
                    OUTPUT_STRIP_TRAILING_WHITESPACE
                )

                foreach(adsk_dir ${DIR_NAMES})
                    string(REGEX MATCH "[mM]aya[0-9.]+" MATCH_RES ${adsk_dir})

                    if (MATCH_RES)
                        set(MAYA_ROOT "/Applications/Autodesk/${MATCH_RES}")
                        break()
                    endif()
                endforeach()
            else()
                execute_process(
                    COMMAND find /usr/autodesk -mindepth 1 -maxdepth 1 -type d
                    OUTPUT_VARIABLE DIR_NAMES
                    OUTPUT_STRIP_TRAILING_WHITESPACE
                )

                foreach(adsk_dir ${DIR_NAMES})
                    string(REGEX MATCH "[mM]aya[0-9.]+" MATCH_RES ${adsk_dir})

                    if (MATCH_RES)
                        set(MAYA_ROOT "/usr/autodesk/${MATCH_RES}")
                        break()
                    endif()
                endforeach()
            endif()
        else()
            if (WIN32)
                set(MAYA_ROOT "C:/Program Files/Autodesk/Maya${MAYA_VERSION}")
            elseif(APPLE)
                set(MAYA_ROOT "/Applications/Autodesk/maya${MAYA_VERSION}")
            elseif(UNIX)
                set(MAYA_ROOT "/usr/autodesk/maya${MAYA_VERSION}")
            endif()
        endif()
    endif()

    if (NOT DEFINED MAYA_ROOT)
        message(FATAL_ERROR "Please set MAYA_ROOT or MAYA_VERSION")
    endif()

    if (NOT DEFINED MAYA_VERSION)
        if (MAYA_ROOT MATCHES "[mM]aya([0-9.]+)")
            set(MAYA_VERSION ${CMAKE_MATCH_1})
        else()
            message(FATAL_ERROR "Please set MAYA_VERSION")
        endif()
    endif()

    if (APPLE)
        find_path(_mayapy_dir
            NAMES mayapy
            PATHS ${MAYA_ROOT}/Maya.app/Contents/MacOS)
    elseif (WIN32)
        find_path(_mayapy_dir
            NAMES bin/mayapy.exe
            PATHS ${MAYA_ROOT})
    endif()

    if (_mayapy_dir)
        set(MAYAPY_PATH "${_mayapy_dir}/mayapy")
    else()
        message(FATAL_ERROR "NO MAYAPY")
    endif()

    message(STATUS "> Maya Version : ${MAYA_VERSION}")
    message(STATUS "> Maya Root : ${MAYA_ROOT}")
    message(STATUS "> Maya Python : ${MAYAPY_PATH}")

    if (WIN32)
        set(FOUNDATION Foundation.lib)
        set(OPEN_MAYA OpenMaya.lib)
        set(LIB_SUFFIX "lib/")
    elseif(APPLE)
        set(FOUNDATION libFoundation.dylib)
        set(OPEN_MAYA libOpenMaya.dylib)
        set(LIB_SUFFIX "Maya.app/Contents/MacOS")
    elseif(UNIX)
        set(FOUNDATION libFoundation.so)
        set(OPEN_MAYA libOpenMaya.so)
        set(LIB_SUFFIX "lib/")
    endif()

    find_path(MAYA_INCLUDE_DIR
            maya/MFn.h
        HINTS
            ${MAYA_ROOT}
        PATH_SUFFIXES
            "include/"
        )
    find_path(MAYA_LIBRARY_DIR
            ${FOUNDATION}
        HINTS
            ${MAYA_ROOT}
        PATH_SUFFIXES
            ${LIB_SUFFIX}
        DOC
            "Maya's libraries path"
        )

    if (APPLE)
        set(MAYA_PY_INCLUDE_DIR ${MAYA_ROOT}/Maya.app/Contents/Frameworks/Python.framework/Headers)
    elseif (WIN32)
        execute_process(
            COMMAND powershell -Command "Get-ChildItem -Path '${MAYA_ROOT}/include' -Directory | ForEach-Object { $_.Name }"
            OUTPUT_VARIABLE _includes
            OUTPUT_STRIP_TRAILING_WHITESPACE)

        foreach(_inc_dir ${_includes})
            string(REGEX MATCH "Python[0-9.]+" MATCH_RES ${_inc_dir})

            if (MATCH_RES)
                set(MAYA_PY_INCLUDE_DIR "${MAYA_ROOT}/include/${MATCH_RES}/Python")
                break()
            endif()
        endforeach()

    endif()

    if (NOT MAYA_INCLUDE_DIR OR NOT MAYA_LIBRARY_DIR)
        message(FATAL_ERROR "Could not find maya include or lib directory")
    endif()

    message(STATUS "> Include dir : ${MAYA_INCLUDE_DIR}")
    message(STATUS "> Libs dir : ${MAYA_LIBRARY_DIR}")

    if (MAYA_WANT_CPP_17)
        set(CMAKE_CXX_STANDARD 17)
    else()
        set(CMAKE_CXX_STANDARD 14)
    endif()

endmacro()


macro(setup_py target)
    if (NOT DEFINED MAYA_PY_INCLUDE_DIR)
        find_maya()
    endif()

    set_property(TARGET ${target} PROPERTY CXX_VISIBILITY_PRESET hidden)
    set_property(TARGET ${target} PROPERTY VISIBILITY_INLINES_HIDDEN ON)

    if(WIN32)
       target_compile_definitions(${target} PUBLIC -D_BOOL -DNT_PLUGIN)
       target_compile_options(${target} PUBLIC /Zi)
       target_link_options(${target} PUBLIC /DEBUG /IGNORE:4099)
    elseif(APPLE)
        set_property(TARGET ${target} PROPERTY XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
        target_compile_definitions(${target} PUBLIC -D_BOOL -DDMAC_PLUGIN )
        target_compile_options(${target} PUBLIC -fPIC -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-unused-private-field)
        target_link_options(_medic PUBLIC -undefined dynamic_lookup)
        set_target_properties(_medic PROPERTIES PREFIX "" SUFFIX ".so")
    else()
        target_compile_definitions(${target} PUBLIC -D_BOOL)
        target_compile_options(${target} PUBLIC -fPIC -Wextra -Wno-unused-parameter -Wno-unused-variable )
    endif()

    target_include_directories(${target} PUBLIC ${MAYA_PY_INCLUDE_DIR})

endmacro()


macro(setup_maya target)
    if (NOT DEFINED MAYA_LIBRARY_DIR OR NOT DEFINED MAYA_INCLUDE_DIR OR NOT DEFINED MAYAPY_PATH)
        find_maya()
    endif()

    cmake_parse_arguments(_tgt "" "" "LINK_LIBRARIES" ${ARGN})

    # hmm.. TODO
    # set_property(TARGET ${target} PROPERTY CXX_VISIBILITY_PRESET hidden)
    # set_property(TARGET ${target} PROPERTY VISIBILITY_INLINES_HIDDEN ON)

    if(WIN32)
       target_compile_definitions(${target} PUBLIC -D_BOOL -DNT_PLUGIN)
       target_compile_options(${target} PUBLIC /Zi)
       target_link_options(${target} PUBLIC /DEBUG /IGNORE:4099)
    elseif(APPLE)
        set_property(TARGET ${target} PROPERTY XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
        target_compile_definitions(${target} PUBLIC -D_BOOL -DDMAC_PLUGIN)
        target_compile_options(${target} PUBLIC -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-unused-private-field)
    else()
        target_compile_definitions(${target} PUBLIC -D_BOOL)
        target_compile_options(${target} PUBLIC -Wextra -Wno-unused-parameter -Wno-unused-variable )
    endif()

    target_link_directories(${target} BEFORE PRIVATE ${MAYA_LIBRARY_DIR})
    target_include_directories(${target} BEFORE PRIVATE ${MAYA_INCLUDE_DIR})

    if (NOT _tgt_LINK_LIBRARIES)
        set(_tgt_LINK_LIBRARIES OpenMaya OpenMayaAnim OpenMayaFX OpenMayaRender OpenMayaUI Foundation)
    endif()

    target_link_libraries(${target} PRIVATE ${_tgt_LINK_LIBRARIES})

endmacro()