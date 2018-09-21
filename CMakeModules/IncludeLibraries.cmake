# Do include for requested modules

include (CheckFunctionExists)

include (IncludeMacro)

#####################
### Compiler library
#####################

option(WITH_COMPILER_LIBRARIES "Enable build with compiler libraries" OFF)
if(WITH_COMPILER_LIBRARIES)
    if(WIN32)
        if(CMAKE_COMPILER_IS_GNUCXX)
            set(TMP_DLL_PATH "")
            # If libgmp is present we just need it
            if(HOST_WIN32)
                execute_process(COMMAND cmd.exe /c "${CMAKE_CURRENT_LIST_DIR}/find_file_in_path.bat libgmp-10.dll" OUTPUT_VARIABLE TMP_DLL_PATH OUTPUT_STRIP_TRAILING_WHITESPACE)
            endif()
            if(TMP_DLL_PATH STREQUAL "")
                # Otherwise we take libstdc++
                execute_process(COMMAND ${CMAKE_CXX_COMPILER} -print-file-name=libstdc++-6.dll OUTPUT_VARIABLE TMP_DLL_PATH OUTPUT_STRIP_TRAILING_WHITESPACE)
            else()
                install(FILES ${TMP_DLL_PATH} DESTINATION ${INSTALL_PATH_BIN})
            endif()
            if(NOT TMP_DLL_PATH STREQUAL "")
                install(FILES ${TMP_DLL_PATH} DESTINATION ${INSTALL_PATH_BIN})
            endif()
            if(NOT HOST_WIN32)
                execute_process(COMMAND ${CMAKE_CXX_COMPILER} -print-file-name=libgcc_s_sjlj-1.dll OUTPUT_VARIABLE TMP_DLL_PATH OUTPUT_STRIP_TRAILING_WHITESPACE)
                install(FILES ${TMP_DLL_PATH} DESTINATION ${INSTALL_PATH_BIN})
            endif(NOT HOST_WIN32)
        endif(CMAKE_COMPILER_IS_GNUCXX)
    
        if(MSVC)
            install(FILES C:/Windows/System32/msvcp100.dll DESTINATION ${INSTALL_PATH_BIN})
            install(FILES C:/Windows/System32/msvcr100.dll DESTINATION ${INSTALL_PATH_BIN})
        endif(MSVC)
    endif(WIN32)
endif(WITH_COMPILER_LIBRARIES)

# Add regex
option(WITH_REGEX "Enable GNU regex build" OFF)
if (WITH_REGEX)
    set(GNURX_STATIC ON CACHE BOOL "Build with static GNURX library" FORCE)
    find_package (GNURX)
    if(GNURX_FOUND)
        include_directories (${GNURX_INCLUDE_DIR})
        set (USE_REGEX 1)
    endif()
endif(WITH_REGEX)

# Find SSE
option(WITH_SIMD "Enable build with compiler SIMD instruction" OFF)
if (WITH_SIMD)
    include(FindSSE)
    FindSSE()

    set(SIMD_FLAGS "${SIMD_SSE_FLAGS}")
endif(WITH_SIMD)

# Find pthread
option(WITH_PTHREAD "Enable build with pthread library" OFF)
if(WITH_PTHREAD)
    if(WIN32)
        set(CMAKE_THREAD_LIBS_INIT "")
    else(WIN32)
        find_package(Threads REQUIRED)
        if(Threads_FOUND)
            set(USE_PTHREAD 1)
            add_definitions(-D_REENTRANT)
        endif(Threads_FOUND)
    endif(WIN32)
endif(WITH_PTHREAD)

#########################
### GUI modules
#########################

# Add QT
option(WITH_QT "Enable build with qt library" OFF)
if(WITH_QT)
    if(NOT QT4_COMPONENTS OR NOT QT5_COMPONENTS)
        message(FATAL_ERROR "Please define the QT4_COMPONENTS and QT5_COMPONENTS variables")
    endif(NOT QT4_COMPONENTS OR NOT QT5_COMPONENTS)

    if(QT_USE_VERSION)
        if(QT_USE_VERSION EQUAL 5)
            message(STATUS "Looking for Qt major version: 5")
            find_package (Qt5 5.1.0 COMPONENTS ${QT5_COMPONENTS})
            set(QT_COMPONENTS ${QT5_COMPONENTS})
        elseif(QT_USE_VERSION EQUAL 4)
            message(STATUS "Looking for Qt major version: 4")
            find_package (Qt4 4.7.0 COMPONENTS ${QT4_COMPONENTS})
            set(QT_COMPONENTS ${QT4_COMPONENTS})
        else()
            message(STATUS "Unsupported Qt major version requested: ${QT_USE_VERSION}")
        endif()
    else(QT_USE_VERSION)
        check_has_module(Qt5)
        if(HAS_MODULE_Qt5)
            message(STATUS "Looking for Qt major version: 5")
            find_package (Qt5 5.1.0 COMPONENTS ${QT5_COMPONENTS})
            set(QT_COMPONENTS ${QT5_COMPONENTS})
            set(QT_USE_VERSION 5)
        endif()
        if(NOT Qt5_FOUND)
            message(STATUS "Looking for Qt major version: 4")
            find_package (Qt4 4.7.0 COMPONENTS ${QT4_COMPONENTS})
            set(QT_COMPONENTS ${QT4_COMPONENTS})
            set(QT_USE_VERSION 4)
        endif()
    endif(QT_USE_VERSION)
    
    if(Qt5_FOUND AND NOT QT_FOUND)
        set(QT_FOUND ${Qt5_FOUND})
        # Qt version may not be defined (probably because there is no FindQt5.cmake)
        # If not defined we require at least qt 5.0. Find a better solution for qt6.
        if(NOT QT_VERSION_MAJOR)
            set(QT_VERSION_MAJOR 5)
            set(QT_VERSION_MINOR 0)
        endif()
    endif()

    if(QT_FOUND)
        set(USE_QT 1)

        if(QT_USE_VERSION LESS 5)
            include (${QT_USE_FILE})
            add_definitions (${QT_DEFINITIONS})

            # override function to generate moc
            function(qt_wrap_cpp arg1)
                QT4_WRAP_CPP(${arg1} ${ARGN})
                set(${arg1} ${${arg1}} PARENT_SCOPE)
            endfunction(qt_wrap_cpp)
            
            # override function to generate ts
            function(qt_create_translation arg1)
                qt4_create_translation(arg1 ${ARGN})
                set(${arg1} ${${arg1}} PARENT_SCOPE)
            endfunction(qt_create_translation)
            function(qt_add_translation arg1)
                qt4_add_translation(arg1 ${ARGN})
                set(${arg1} ${${arg1}} PARENT_SCOPE)
            endfunction(qt_add_translation)
            
            # override function to generate res
            function(qt_add_resources arg1)
                QT4_ADD_RESOURCES(${arg1} ${ARGN})
                set(${arg1} ${${arg1}} PARENT_SCOPE)
            endfunction(qt_add_resources)
        else()
        
            # option for Qt 5
            if (Qt5_POSITION_INDEPENDENT_CODE)
              set(CMAKE_POSITION_INDEPENDENT_CODE ON)
            endif()

            # override function to generate moc
            function(qt_wrap_cpp arg1)
                QT5_WRAP_CPP(${arg1} ${ARGN})
                set(${arg1} ${${arg1}} PARENT_SCOPE)
            endfunction(qt_wrap_cpp)

            # override function to generate ts
            find_package(Qt5LinguistTools)
            if(Qt5LinguistTools_FOUND)
                function(qt_create_translation arg1)
                    qt5_create_translation(arg1 ${ARGN})
                    set(${arg1} ${${arg1}} PARENT_SCOPE)
                endfunction(qt_create_translation)
                function(qt_add_translation arg1)
                    qt5_add_translation(arg1 ${ARGN})
                    set(${arg1} ${${arg1}} PARENT_SCOPE)
                endfunction(qt_add_translation)
            else()
                message(FATAL_ERROR "Qt5LinguistTools package is required")
            endif()
            
            # override function to generate res
            function(qt_add_resources arg1)
                QT5_ADD_RESOURCES(${arg1} ${ARGN})
                set(${arg1} ${${arg1}} PARENT_SCOPE)
            endfunction(qt_add_resources)
        endif()

        # Compute path on QT5
        if(WIN32)
            if(NOT QT_USE_VERSION LESS 5)
                list(GET Qt5Core_INCLUDE_DIRS 0 QT_INCLUDE_DIR)
                set(QT_BINARY_DIR ${QT_INCLUDE_DIR}../bin)
                set(QT_PLUGINS_DIR ${QT_INCLUDE_DIR}../plugins)
                set(QT_LIBRARY_DIR ${QT_INCLUDE_DIR}../lib)
            endif()
        endif()
        if(APPLE)
            if(NOT QT_USE_VERSION LESS 5)
                list(GET Qt5Core_INCLUDE_DIRS 0 QT_INCLUDE_DIR)
                set(QT_BINARY_DIR ${QT_INCLUDE_DIR}/../../bin)
                set(QT_PLUGINS_DIR ${QT_INCLUDE_DIR}/../../plugins)
                set(QT_LIBRARY_DIR ${QT_INCLUDE_DIR}/../../lib)
            endif()
        endif()

        if (WIN32)
            if(HOST_WIN32)
                set(QT_DLL_DIR ${QT_BINARY_DIR})
            else()
                set(QT_DLL_DIR ${QT_LIBRARY_DIR}) # When cross compiling DLL are here
            endif()

            # Common library
            if(CMAKE_COMPILER_IS_GNUCXX)
                INSTALL(FILES
                    "${QT_DLL_DIR}/libgcc_s_dw2-1.dll"
                    DESTINATION ${INSTALL_PATH_BIN}
                )
                # Since 4.8.6 use libwinpthread
                if(EXISTS "${QT_DLL_DIR}/libwinpthread-1.dll")
                    INSTALL(FILES
                        "${QT_DLL_DIR}/libwinpthread-1.dll"
                        DESTINATION ${INSTALL_PATH_BIN}
                    )
                else()
                    INSTALL(FILES
                        "${QT_DLL_DIR}/mingwm10.dll"
                        DESTINATION ${INSTALL_PATH_BIN}
                    )
                endif()

            endif(CMAKE_COMPILER_IS_GNUCXX)
        endif(WIN32)

        set(QT_DEPS_VERSION "${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}")

        # Qt Components
        foreach(TMP_COMPONENT ${QT_COMPONENTS})
            if(QT_USE_VERSION LESS 5)
                string(SUBSTRING ${TMP_COMPONENT} 2 -1 TMP_MODULE)
                string(TOLOWER ${TMP_MODULE} TMP_MODULE)
            else()        
                string(TOLOWER ${TMP_COMPONENT} TMP_MODULE)
            endif()

            if(NOT (QT_USE_VERSION LESS 5))
                include_directories(${Qt5${TMP_COMPONENT}_INCLUDE_DIRS})
                add_definitions (${Qt5${TMP_COMPONENT}_DEFINITIONS})
                
                set(QT_LIBRARIES ${QT_LIBRARIES} ${Qt5${TMP_COMPONENT}_LIBRARIES})
            endif()
            
            if (WIN32)
                if(QT_USE_VERSION LESS 5)
                    INSTALL(FILES "${QT_DLL_DIR}/${TMP_COMPONENT}${QT_VERSION_MAJOR}.dll"
                        DESTINATION ${INSTALL_PATH_BIN} CONFIGURATIONS Release)
                    INSTALL(FILES "${QT_DLL_DIR}/${TMP_COMPONENT}d${QT_VERSION_MAJOR}.dll"
                        DESTINATION ${INSTALL_PATH_BIN} CONFIGURATIONS Debug)
                else()
                    INSTALL(FILES "${QT_DLL_DIR}/Qt${QT_VERSION_MAJOR}${TMP_COMPONENT}.dll"
                        DESTINATION ${INSTALL_PATH_BIN} CONFIGURATIONS Release)
                    INSTALL(FILES "${QT_DLL_DIR}/Qt${QT_VERSION_MAJOR}${TMP_COMPONENT}d.dll"
                        DESTINATION ${INSTALL_PATH_BIN} CONFIGURATIONS Debug)
                endif()
            elseif(APPLE)
                if(INSTALL_PATH_FRAMEWORKS)
                    IF(CMAKE_BUILD_TYPE MATCHES Release)
                      INSTALL(DIRECTORY "${QT_LIBRARY_DIR}/Qt${TMP_COMPONENT}.framework"
                          DESTINATION ${INSTALL_PATH_FRAMEWORKS}
                          PATTERN "Qt${TMP_COMPONENT}_debug" EXCLUDE
                          PATTERN "*.prl" EXCLUDE
                      )
                    else()
                      INSTALL(DIRECTORY "${QT_LIBRARY_DIR}/Qt${TMP_COMPONENT}.framework"
                          DESTINATION ${INSTALL_PATH_FRAMEWORKS})
                    endif()
                else()
                    INSTALL(FILES "${QT_LIBRARY_DIR}/Qt${TMP_COMPONENT}.framework/Versions/${QT_VERSION_MAJOR}/Qt${TMP_COMPONENT}"
                        DESTINATION ${INSTALL_PATH_LIB})
                endif()
            else()
                if(QT_USE_VERSION LESS 5)
                    set(TMP_PACKAGE_NAME "libqt${QT_VERSION_MAJOR}-${TMP_MODULE}")
                else()
                    if(${TMP_MODULE} STREQUAL "core")
                        set(TMP_PACKAGE_NAME "libqt${QT_VERSION_MAJOR}${TMP_MODULE}${QT_VERSION_MAJOR}a")
                    else()
                        set(TMP_PACKAGE_NAME "libqt${QT_VERSION_MAJOR}${TMP_MODULE}${QT_VERSION_MAJOR}")
                    endif()
                endif()
                list(APPEND PACKAGE_DEPS "${TMP_PACKAGE_NAME} (>= ${QT_DEPS_VERSION})")
            endif(WIN32)
        endforeach(TMP_COMPONENT)

        if(INSTALL_PATH_PLUGIN)
            set(INSTALL_PATH_QTPLUGINS ${INSTALL_PATH_PLUGIN})
        else()
            set(INSTALL_PATH_QTPLUGINS ${INSTALL_PATH_BIN})
        endif()

        # Qt Platforms plugins
        foreach(TMP_COMPONENT ${QT_PLATFORMS_COMPONENTS})
            string(SUBSTRING ${TMP_COMPONENT} 1 -1 TMP_MODULE)
            string(TOLOWER ${TMP_MODULE} TMP_MODULE)
            
            if (WIN32)
                if(QT_USE_VERSION LESS 5)
                    INSTALL(FILES "${QT_PLUGINS_DIR}/platforms/${TMP_COMPONENT}${QT_VERSION_MAJOR}.dll"
                        DESTINATION ${INSTALL_PATH_QTPLUGINS}/platforms CONFIGURATIONS Release
                    )
                    INSTALL(FILES "${QT_PLUGINS_DIR}/platforms/${TMP_COMPONENT}d${QT_VERSION_MAJOR}.dll"
                        DESTINATION ${INSTALL_PATH_QTPLUGINS}/platforms CONFIGURATIONS Debug
                    )
                else()
                    INSTALL(FILES "${QT_PLUGINS_DIR}/platforms/${TMP_COMPONENT}.dll"
                        DESTINATION ${INSTALL_PATH_QTPLUGINS}/platforms CONFIGURATIONS Release
                    )
                    INSTALL(FILES "${QT_PLUGINS_DIR}/platforms/${TMP_COMPONENT}d.dll"
                        DESTINATION ${INSTALL_PATH_QTPLUGINS}/platforms CONFIGURATIONS Debug
                    )
                endif()
            endif(WIN32)
            if(APPLE)
                    INSTALL(FILES "${QT_PLUGINS_DIR}/platforms/lib${TMP_COMPONENT}.dylib"
                        DESTINATION ${INSTALL_PATH_QTPLUGINS}/platforms
                    )
                    INSTALL(FILES "${QT_PLUGINS_DIR}/platforms/lib${TMP_COMPONENT}_debug.dylib"
                        DESTINATION ${INSTALL_PATH_QTPLUGINS}/platforms CONFIGURATIONS Debug
                    )
            endif()
        endforeach(TMP_COMPONENT)

        # Qt ImageFormats plugins
        foreach(TMP_COMPONENT ${QT_IMAGEFORMATS_COMPONENTS})
            string(SUBSTRING ${TMP_COMPONENT} 1 -1 TMP_MODULE)
            string(TOLOWER ${TMP_MODULE} TMP_MODULE)
            
            if (WIN32)
                if(QT_USE_VERSION LESS 5)
                    INSTALL(FILES "${QT_PLUGINS_DIR}/imageformats/${TMP_COMPONENT}${QT_VERSION_MAJOR}.dll"
                        DESTINATION ${INSTALL_PATH_QTPLUGINS}/imageformats CONFIGURATIONS Release
                    )
                    INSTALL(FILES "${QT_PLUGINS_DIR}/imageformats/${TMP_COMPONENT}d${QT_VERSION_MAJOR}.dll"
                        DESTINATION ${INSTALL_PATH_QTPLUGINS}/imageformats CONFIGURATIONS Debug
                    )
                else()
                    INSTALL(FILES "${QT_PLUGINS_DIR}/imageformats/${TMP_COMPONENT}.dll"
                        DESTINATION ${INSTALL_PATH_QTPLUGINS}/imageformats CONFIGURATIONS Release
                    )
                    INSTALL(FILES "${QT_PLUGINS_DIR}/imageformats/${TMP_COMPONENT}d.dll"
                        DESTINATION ${INSTALL_PATH_QTPLUGINS}/imageformats CONFIGURATIONS Debug
                    )
                endif()
            endif(WIN32)
            if(APPLE)
                    INSTALL(FILES "${QT_PLUGINS_DIR}/imageformats/lib${TMP_COMPONENT}.dylib"
                        DESTINATION ${INSTALL_PATH_QTPLUGINS}/imageformats
                    )
                    INSTALL(FILES "${QT_PLUGINS_DIR}/imageformats/lib${TMP_COMPONENT}_debug.dylib"
                        DESTINATION ${INSTALL_PATH_QTPLUGINS}/imageformats CONFIGURATIONS Debug
                    )
            endif()
        endforeach(TMP_COMPONENT)

        # Qt Sql plugins
        foreach(TMP_COMPONENT ${QT_SQL_COMPONENTS})

            if(TMP_COMPONENT STREQUAL "qsqlmysql")
                set(TMP_MODULE "mysql")
            elseif(TMP_COMPONENT STREQUAL "qsqlpsql")
                set(TMP_MODULE "psql")
            else()
                string(SUBSTRING ${TMP_COMPONENT} 1 -1 TMP_MODULE)
                string(TOLOWER ${TMP_MODULE} TMP_MODULE)
            endif()
            
            if (WIN32)
                if(QT_USE_VERSION LESS 5)
                    SET(TMPDLL "${QT_PLUGINS_DIR}/sqldrivers/${TMP_COMPONENT}${QT_VERSION_MAJOR}.dll")
                    if(EXISTS ${TMPDLL})
                        INSTALL(FILES "${TMPDLL}"
                            DESTINATION ${INSTALL_PATH_QTPLUGINS}/sqldrivers CONFIGURATIONS Release
                        )
                    endif()
                    SET(TMPDLL "${QT_PLUGINS_DIR}/sqldrivers/${TMP_COMPONENT}d${QT_VERSION_MAJOR}.dll")
                    if(EXISTS ${TMPDLL})
                        INSTALL(FILES "${TMPDLL}"
                            DESTINATION ${INSTALL_PATH_QTPLUGINS}/sqldrivers CONFIGURATIONS Debug
                        )
                    endif()
                else()
                    SET(TMPDLL "${QT_PLUGINS_DIR}/sqldrivers/${TMP_COMPONENT}.dll")
                    if(EXISTS ${TMPDLL})
                        INSTALL(FILES "${TMPDLL}"
                            DESTINATION ${INSTALL_PATH_QTPLUGINS}/sqldrivers CONFIGURATIONS Release
                        )
                    endif()
                    SET(TMPDLL "${QT_PLUGINS_DIR}/sqldrivers/${TMP_COMPONENT}d}.dll")
                    if(EXISTS ${TMPDLL})
                        INSTALL(FILES "${TMPDLL}"
                            DESTINATION ${INSTALL_PATH_QTPLUGINS}/sqldrivers CONFIGURATIONS Debug
                        )
                    endif()
                endif()
            elseif(APPLE)
                    INSTALL(FILES "${QT_PLUGINS_DIR}/sqldrivers/lib${TMP_COMPONENT}.dylib"
                        DESTINATION ${INSTALL_PATH_QTPLUGINS}/sqldrivers
                    )
                    INSTALL(FILES "${QT_PLUGINS_DIR}/sqldrivers/lib${TMP_COMPONENT}_debug.dylib"
                        DESTINATION ${INSTALL_PATH_QTPLUGINS}/sqldrivers CONFIGURATIONS Debug
                    )
            else()
                if(QT_USE_VERSION LESS 5)
                    set(TMP_PACKAGE_NAME "libqt${QT_VERSION_MAJOR}-sql-${TMP_MODULE}")
                else()
                    set(TMP_PACKAGE_NAME "libqt${QT_VERSION_MAJOR}sql${QT_VERSION_MAJOR}-${TMP_MODULE}")
                endif()
                list(APPEND PACKAGE_DEPS "${TMP_PACKAGE_NAME} (>= ${QT_DEPS_VERSION})")
            endif()
        endforeach(TMP_COMPONENT)
        
        if(NOT QT_USE_VERSION LESS 5)
            set(QT_QTMAIN_LIBRARY ${Qt5Core_QTMAIN_LIBRARIES})
        endif()
        
    endif(QT_FOUND)
endif(WITH_QT)

# Add QT Mobility (Linux only)
option(WITH_QTMOBILITY "Enable build with qtmobility library" OFF)
if(WITH_QT AND NOT QT_USE_VERSION EQUAL 4)
    # Disable qt mobility if using qt>=5
    set(WITH_QTMOBILITY OFF)
endif()
if(WITH_QTMOBILITY)
    find_package (QtMobility 1.2.0 REQUIRED ${QT_MOBILITY_COMPONENTS})
    if(QTMOBILITY_FOUND)
        set(USE_QTMOBILITY 1)
        set(QTMOBILITY_DEPS_VERSION "1.2")
        include_directories (${QTMOBILITY_INCLUDE_DIR} ${QTMOBILITY_INCLUDE_DIRS})
        
        # Add component
        foreach(TMP_COMPONENT ${QT_MOBILITY_COMPONENTS})
            string(TOLOWER ${TMP_COMPONENT} TMP_MODULE)
            string(TOUPPER ${TMP_COMPONENT} TMP_MODULEU)
            
            if(TMP_MODULE STREQUAL "multimediakit")
                set(USE_QTMULTIMEDIA 1)
            else()
                set(USE_QT${TMP_MODULEU} 1)
            endif()
            
            if (NOT WIN32)
                list(APPEND PACKAGE_DEPS "libqt${TMP_MODULE}1 (>= ${QTMOBILITY_DEPS_VERSION})")
            endif(NOT WIN32)
        endforeach(TMP_COMPONENT)
        
        if (WIN32)
            if(CMAKE_HOST_SYSTEM MATCHES Linux)
                set(QTMOBILITY_DLL_DIR ${QTMOBILITY_LIBRARY_DIR}) # When cross compiling DLL are here
            else()
                set(QTMOBILITY_DLL_DIR ${QTMOBILITY_LIBRARY_DIR})
            endif()
        endif(WIN32)
    endif(QTMOBILITY_FOUND)
endif(WITH_QTMOBILITY)

