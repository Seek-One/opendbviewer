# Mandatory options
if(NOT QT4_COMPONENTS AND NOT QT5_COMPONENTS AND NOT QT6_COMPONENTS)
	message(FATAL_ERROR "Please define the QT4_COMPONENTS, QT5_COMPONENTS or QT6_COMPONENTS variables")
endif(NOT QT4_COMPONENTS AND NOT QT5_COMPONENTS AND NOT QT6_COMPONENTS)

# Look for correct QT version
if(QT_USE_VERSION)
	if(QT_USE_VERSION EQUAL 6)
		message(STATUS "Looking for Qt major version: 6")
		find_package (Qt6 6.5.0 COMPONENTS ${QT6_COMPONENTS})
		set(QT_COMPONENTS ${QT6_COMPONENTS})
	elseif(QT_USE_VERSION EQUAL 5)
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
	check_has_module(Qt6)
	if(HAS_MODULE_Qt6)
		message(STATUS "Looking for Qt major version: 6")
		find_package (Qt6 6.5.0 COMPONENTS ${QT6_COMPONENTS})
		set(QT_COMPONENTS ${QT6_COMPONENTS})
		set(QT_USE_VERSION 6)
	else()
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
	endif()
endif(QT_USE_VERSION)

if(Qt6_FOUND AND NOT QT_FOUND)
	set(QT_FOUND ${Qt6_FOUND})
	# Qt version may not be defined (probably because there is no FindQt5.cmake)
	# If not defined we require at least qt 5.0. Find a better solution for qt6.
	if(NOT QT_VERSION_MAJOR)
		set(QT_VERSION_MAJOR 6)
		set(QT_VERSION_MINOR 0)
	endif()
endif()
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

	if(QT_USE_VERSION GREATER_EQUAL 6)
		# option for Qt 6
		if (Qt6_POSITION_INDEPENDENT_CODE)
			set(CMAKE_POSITION_INDEPENDENT_CODE ON)
		endif()

		if (SINGLE_UNIT_COMPILATION)
			set(CMAKE_AUTOMOC ON)
		endif ()

		# override function to generate moc
		function(qtx_wrap_cpp arg1)
			if (NOT SINGLE_UNIT_COMPILATION)
				qt_wrap_cpp(${arg1} ${ARGN})
				set(${arg1} ${${arg1}} PARENT_SCOPE)
			endif ()
		endfunction(qtx_wrap_cpp)

		# override function to generate ts
		find_package(Qt${QT_VERSION_MAJOR}LinguistTools)
		if(Qt${QT_VERSION_MAJOR}LinguistTools_FOUND)
			function(qtx_create_translation arg1)
				qt_create_translation(arg1 ${ARGN})
				set(${arg1} ${${arg1}} PARENT_SCOPE)
			endfunction(qtx_create_translation)
			function(qtx_add_translation arg1)
				qt_add_translation(arg1 ${ARGN})
				set(${arg1} ${${arg1}} PARENT_SCOPE)
			endfunction(qtx_add_translation)
		else()
			message(FATAL_ERROR "Qt${QT_VERSION_MAJOR}LinguistTools package is required")
		endif()

		# override function to generate res
		function(qtx_add_resources arg1)
			qt_add_resources(${arg1} ${ARGN})
			set(${arg1} ${${arg1}} PARENT_SCOPE)
		endfunction(qtx_add_resources)
	elseif(QT_USE_VERSION EQUAL 5)
		# option for Qt 5
		if (Qt5_POSITION_INDEPENDENT_CODE)
			set(CMAKE_POSITION_INDEPENDENT_CODE ON)
		endif()

		if (SINGLE_UNIT_COMPILATION)
			set(CMAKE_AUTOMOC ON)
		endif ()

		# override function to generate moc
		function(qtx_wrap_cpp arg1)
			if (NOT SINGLE_UNIT_COMPILATION)
				qt5_wrap_cpp(${arg1} ${ARGN})
				set(${arg1} ${${arg1}} PARENT_SCOPE)
			endif ()
		endfunction(qtx_wrap_cpp)

		# override function to generate ts
		find_package(Qt${QT_VERSION_MAJOR}LinguistTools)
		if(Qt${QT_VERSION_MAJOR}LinguistTools_FOUND)
			function(qtx_create_translation arg1)
				qt5_create_translation(arg1 ${ARGN})
				set(${arg1} ${${arg1}} PARENT_SCOPE)
			endfunction(qtx_create_translation)
			function(qtx_add_translation arg1)
				qt5_add_translation(arg1 ${ARGN})
				set(${arg1} ${${arg1}} PARENT_SCOPE)
			endfunction(qtx_add_translation)
		else()
			message(FATAL_ERROR "Qt${QT_VERSION_MAJOR}LinguistTools package is required")
		endif()

		# override function to generate res
		function(qtx_add_resources arg1)
			qt5_add_resources(${arg1} ${ARGN})
			set(${arg1} ${${arg1}} PARENT_SCOPE)
		endfunction(qtx_add_resources)
	else()
		include (${QT_USE_FILE})
		add_definitions (${QT_DEFINITIONS})

		if (SINGLE_UNIT_COMPILATION)
			set(CMAKE_AUTOMOC ON)
		endif ()

		# override function to generate moc
		function(qtx_wrap_cpp arg1)
			if (NOT SINGLE_UNIT_COMPILATION)
				qt4_wrap_cpp(${arg1} ${ARGN})
				set(${arg1} ${${arg1}} PARENT_SCOPE)
			endif ()
		endfunction(qtx_wrap_cpp)

		# override function to generate ts
		function(qtx_create_translation arg1)
			qt4_create_translation(arg1 ${ARGN})
			set(${arg1} ${${arg1}} PARENT_SCOPE)
		endfunction(qtx_create_translation)
		function(qtx_add_translation arg1)
			qt4_add_translation(arg1 ${ARGN})
			set(${arg1} ${${arg1}} PARENT_SCOPE)
		endfunction(qtx_add_translation)

		# override function to generate res
		function(qtx_add_resources arg1)
			qt4_add_resources(${arg1} ${ARGN})
			set(${arg1} ${${arg1}} PARENT_SCOPE)
		endfunction(qtx_add_resources)
	endif()

	# Compute path on QT5
	if(QT_USE_VERSION GREATER_EQUAL 5)
		list(GET Qt${QT_VERSION_MAJOR}Core_INCLUDE_DIRS 0 QT_INCLUDE_DIR)
		# Get library dir
		if(EXISTS ${QT_INCLUDE_DIR}../lib)
			set(QT_LIBRARY_DIR ${QT_INCLUDE_DIR}../lib/)
		elseif(EXISTS ${QT_INCLUDE_DIR}/../../lib)
			set(QT_LIBRARY_DIR ${QT_INCLUDE_DIR}/../../lib)
		else()
			find_library(QtCore_LIBRARY_PATH libQt${QT_VERSION_MAJOR}Core.so)
			if(QtCore_LIBRARY_PATH)
				get_filename_component(QT_LIBRARY_DIR ${QtCore_LIBRARY_PATH} DIRECTORY)
			endif()
		endif()
		if(NOT QT_LIBRARY_DIR)
			message(FATAL_ERROR "Qt library path not found (include path: ${QT_INCLUDE_DIR})")
		endif()
		# Get root dir
		if(EXISTS ${QT_LIBRARY_DIR}../qt.conf)
			set(QT_ROOT_DIR ${QT_LIBRARY_DIR}/..)
		elseif(EXISTS ${QT_LIBRARY_DIR}/../bin/qt.conf)
			set(QT_ROOT_DIR ${QT_LIBRARY_DIR}/..)
		elseif(EXISTS ${QT_LIBRARY_DIR}/../bin/qmake)
			set(QT_ROOT_DIR ${QT_LIBRARY_DIR}/..)
		elseif(EXISTS ${QT_LIBRARY_DIR}/qt${QT_VERSION_MAJOR})
			set(QT_ROOT_DIR ${QT_LIBRARY_DIR}/qt${QT_VERSION_MAJOR})
		endif()
		if(NOT QT_ROOT_DIR)
			message(FATAL_ERROR "Qt root path not found (include path: ${QT_INCLUDE_DIR})")
		endif()
		set(QT_BINARY_DIR ${QT_ROOT_DIR}/bin)
		set(QT_PLUGINS_DIR ${QT_ROOT_DIR}/plugins)
		set(QT_QML_DIR ${QT_ROOT_DIR}/qml)
		if(QT_USE_VERSION GREATER_EQUAL 6)
			set(QT_TRANSLATIONS_DIR ${QT_ROOT_DIR}/translations)
 		endif()
	endif()
	if(NOT QT_TRANSLATIONS_DIR)
		if(QT_USE_VERSION EQUAL 5)
			# If Qt5+, we get QT_TRANSLATIONS_DIR from the executable path
			get_target_property(QT${QT_VERSION_MAJOR}_QMAKE_EXECUTABLE Qt${QT_VERSION_MAJOR}::qmake IMPORTED_LOCATION)
			execute_process(COMMAND ${QT${QT_VERSION_MAJOR}_QMAKE_EXECUTABLE} -query QT_INSTALL_TRANSLATIONS
				OUTPUT_VARIABLE qt_translations_dir OUTPUT_STRIP_TRAILING_WHITESPACE)
			file(TO_CMAKE_PATH "${qt_translations_dir}" qt_translations_dir)
			set(QT_TRANSLATIONS_DIR ${qt_translations_dir} CACHE PATH "The location of the Qt translations" FORCE)
		endif()
	endif()
	message(STATUS "QT root path: ${QT_ROOT_DIR}")
	message(STATUS "QT include path: ${QT_INCLUDE_DIR}")
	message(STATUS "QT library path: ${QT_LIBRARY_DIR}")
	message(STATUS "QT binary path: ${QT_BINARY_DIR}")
	message(STATUS "QT plugins path: ${QT_PLUGINS_DIR}")
	message(STATUS "QT translations path: ${QT_TRANSLATIONS_DIR}")
	message(STATUS "QT qml path: ${QT_QML_DIR}")

	set(QT_USE_DEBUG_DLL FALSE)

	if (WIN32)
		# Find Qt dll directory
		find_path(QT_DLL_DIR NAMES QtCore4.dll QtCored4.dll Qt${QT_VERSION_MAJOR}Core.dll Qt${QT_VERSION_MAJOR}Cored.dll PATHS ${QT_BINARY_DIR} {QT_LIBRARY_DIR})
		if(NOT QT_DLL_DIR)
			message(FATAL "Qt dll directory not found")
		endif()
		find_path(QT_DLL_DEBUG_DIR NAMES QtCored4.dll Qt${QT_VERSION_MAJOR}Cored.dll Qt6Cored.dll PATHS ${QT_BINARY_DIR} {QT_LIBRARY_DIR})
		if(QT_DLL_DEBUG_DIR)
			set(QT_USE_DEBUG_DLL TRUE)
		endif()
		message(STATUS "Using Qt debug dll: ${QT_USE_DEBUG_DLL}")

		function(install_qt_dll qt_dll_dir qt_component qt_version_major install_path)
			if(QT_USE_VERSION LESS 5)
				INSTALL(FILES "${qt_dll_dir}/${qt_component}${qt_version_major}.dll"
						DESTINATION ${install_path} CONFIGURATIONS Release)
				INSTALL(FILES "${qt_dll_dir}/${qt_component}d${qt_version_major}.dll"
						DESTINATION ${install_path} CONFIGURATIONS Debug)
			elseif(QT_USE_DEBUG_DLL)
				INSTALL(FILES "${qt_dll_dir}/Qt${qt_version_major}${qt_component}.dll"
						DESTINATION ${install_path} CONFIGURATIONS Release)
				INSTALL(FILES "${qt_dll_dir}/Qt${qt_version_major}${qt_component}d.dll"
						DESTINATION ${install_path} CONFIGURATIONS Debug)
			else()
				INSTALL(FILES "${qt_dll_dir}/Qt${qt_version_major}${qt_component}.dll"
						DESTINATION ${install_path})
			endif()
		endfunction()

		function(install_qt_plugins_dll qt_dll_dir qt_component qt_version_major install_path)
			if(QT_USE_DEBUG_DLL)
				INSTALL(FILES "${qt_dll_dir}/${qt_component}.dll"
						DESTINATION ${install_path} CONFIGURATIONS Release)
				INSTALL(FILES "${qt_dll_dir}/${qt_component}d.dll"
						DESTINATION ${install_path} CONFIGURATIONS Debug)
			else()
				INSTALL(FILES "${qt_dll_dir}/${qt_component}.dll"
						DESTINATION ${install_path})
			endif()
		endfunction()

		set(QT_TRANSLATIONS_DIR ${QT_DLL_DIR}/../translations)

		# Common library
		if(CMAKE_COMPILER_IS_GNUCXX)
			if(EXISTS "${QT_DLL_DIR}/libgcc_s_dw2-1.dll")
				INSTALL(FILES
					"${QT_DLL_DIR}/libgcc_s_dw2-1.dll"
					DESTINATION ${INSTALL_PATH_BIN}
				)
			endif()
			# Since 4.8.6 use libwinpthread
			if(EXISTS "${QT_DLL_DIR}/libwinpthread-1.dll")
				INSTALL(FILES
					"${QT_DLL_DIR}/libwinpthread-1.dll"
					DESTINATION ${INSTALL_PATH_BIN}
				)
			endif()
			if(EXISTS "${QT_DLL_DIR}/mingwm10.dll")
				INSTALL(FILES
					"${QT_DLL_DIR}/mingwm10.dll"
					DESTINATION ${INSTALL_PATH_BIN}
				)
			endif()
			# libgomp
			execute_process(COMMAND ${CMAKE_CXX_COMPILER} -print-file-name=libgomp-1.dll OUTPUT_VARIABLE TMP_DLL_PATH OUTPUT_STRIP_TRAILING_WHITESPACE)
			if(NOT TMP_DLL_PATH STREQUAL "" AND NOT TMP_DLL_PATH STREQUAL "libgomp-1.dll")
				install(FILES ${TMP_DLL_PATH} DESTINATION ${INSTALL_PATH_BIN})
			endif()
			# libwinpthread
			execute_process(COMMAND ${CMAKE_CXX_COMPILER} -print-file-name=libwinpthread-1.dll OUTPUT_VARIABLE TMP_DLL_PATH OUTPUT_STRIP_TRAILING_WHITESPACE)
			if(NOT TMP_DLL_PATH STREQUAL "" AND NOT TMP_DLL_PATH STREQUAL "libwinpthread-1.dll")
				install(FILES ${TMP_DLL_PATH} DESTINATION ${INSTALL_PATH_BIN})
			endif()
			# zlib
			execute_process(COMMAND ${CMAKE_CXX_COMPILER} -print-file-name=zlib1.dll OUTPUT_VARIABLE TMP_DLL_PATH OUTPUT_STRIP_TRAILING_WHITESPACE)
			if(NOT TMP_DLL_PATH STREQUAL "" AND NOT TMP_DLL_PATH STREQUAL "zlib1.dll")
				install(FILES ${TMP_DLL_PATH} DESTINATION ${INSTALL_PATH_BIN})
			endif()
		endif(CMAKE_COMPILER_IS_GNUCXX)

		# Add platform specific informations
		if(QT_USE_VERSION LESS 5)

		else()
			set(QT_PLATFORMS_COMPONENTS ${QT_PLATFORMS_COMPONENTS} qwindows)
		endif()

	endif(WIN32)

	set(QT_DEPS_VERSION "${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}")

	# Qt Components
	foreach(TMP_COMPONENT ${QT_COMPONENTS})
		if(QT_USE_VERSION LESS 5)
			string(SUBSTRING ${TMP_COMPONENT} 2 -1 TMP_MODULE)
			string(TOLOWER ${TMP_MODULE} TMP_MODULE)
		else()
			string(TOLOWER ${TMP_COMPONENT} TMP_MODULE)
			# Qt adds a "-" each time a module's name ends with a digit
			if(TMP_MODULE MATCHES "[(+0-9)]$")
				set(TMP_MODULE "${TMP_MODULE}-")
			endif()
		endif()

		if(NOT (QT_USE_VERSION LESS 5))
			include_directories(${Qt${QT_VERSION_MAJOR}${TMP_COMPONENT}_INCLUDE_DIRS})
			add_definitions (${Qt${QT_VERSION_MAJOR}${TMP_COMPONENT}_DEFINITIONS})

			set(QT_LIBRARIES ${QT_LIBRARIES} ${Qt${QT_VERSION_MAJOR}${TMP_COMPONENT}_LIBRARIES})
		endif()

		if(WIN32)
			install_qt_dll(${QT_DLL_DIR} ${TMP_COMPONENT} ${QT_VERSION_MAJOR} ${INSTALL_PATH_BIN})
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

			file(GLOB DBUS_LIBRARIES_INSTALL /usr/local/opt/dbus/lib/*.dylib)
			install(FILES ${DBUS_LIBRARIES_INSTALL} DESTINATION ${INSTALL_PATH_LIB})
		else()
			if(AUTONOMOUS_PACKAGE_BUILD)
				set(QT_LIBSO_NAME libQt${QT_VERSION_MAJOR}${TMP_COMPONENT}.so)
				find_install_lib_so(${QT_LIBSO_NAME} ${INSTALL_PATH_LIB} TRUE)
				if("${TMP_COMPONENT}" STREQUAL "Gui")
					set(QT_LIBSO_NAME libQt${QT_VERSION_MAJOR}XcbQpa.so.${QT_VERSION_MAJOR})
					find_install_lib_so(${QT_LIBSO_NAME} ${INSTALL_PATH_LIB} TRUE)
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
			endif()
		endif()
	endforeach(TMP_COMPONENT)

	if(INSTALL_PATH_PLUGIN)
		set(INSTALL_PATH_QTPLUGINS ${INSTALL_PATH_PLUGIN})
	else()
		set(INSTALL_PATH_QTPLUGINS ${INSTALL_PATH_BIN})
	endif()

	if(INSTALL_PATH_QML)
		set(INSTALL_PATH_QTQML ${INSTALL_PATH_QML})
	else()
		set(INSTALL_PATH_QTQML ${INSTALL_PATH_BIN})
	endif()

	# Qt Languages
	if(QT_USE_VERSION LESS 5)
		list(APPEND PACKAGE_DEPS "qtcore4-l10n")
	else()
		if(AUTONOMOUS_PACKAGE_BUILD)
			set(QT_USED_MODULES "qt;qtbase")
			set(QT_USED_LANG "fr;sv")
			install_qm_files(${QT_TRANSLATIONS_DIR} "${QT_USED_MODULES}" "${QT_USED_LANG}" ${INSTALL_PATH_SHARE}/translations)
		else()
			list(APPEND PACKAGE_DEPS "qttranslations5-l10n")
		endif()
	endif()

	# Get QML module files directory
	function(qt_qml_module_directory qml_module qml_module_dir)
		if(${qml_module} STREQUAL "qtquick2")
			set(${qml_module_dir} "QtQuick.2" PARENT_SCOPE)
		elseif(${qml_module} STREQUAL "qtquick-layouts")
			set(${qml_module_dir} "QtQuick/Layouts" PARENT_SCOPE)
		elseif(${qml_module} STREQUAL "qtquick-controls")
			set(${qml_module_dir} "QtQuick/Controls" PARENT_SCOPE)
		elseif(${qml_module} STREQUAL "qtquick-dialogs")
			set(${qml_module_dir} "QtQuick/Dialogs" PARENT_SCOPE)
		elseif(${qml_module} STREQUAL "qtquick-dialogs/private")
			set(${qml_module_dir} "QtQuick/Dialogs/Private" PARENT_SCOPE)
		elseif(${qml_module} STREQUAL "qtquick-controls2")
			set(${qml_module_dir} "QtQuick/Controls.2" PARENT_SCOPE)
		elseif(${qml_module} STREQUAL "qtquick-controls2/material")
			set(${qml_module_dir} "QtQuick/Controls.2/Material" PARENT_SCOPE)
		elseif(${qml_module} STREQUAL "qtquick-privatewidgets")
			set(${qml_module_dir} "QtQuick/PrivateWidgets" PARENT_SCOPE)
		elseif(${qml_module} STREQUAL "qtquick-window2")
			set(${qml_module_dir} "QtQuick/Window.2" PARENT_SCOPE)
		elseif(${qml_module} STREQUAL "qtquick-templates2")
			set(${qml_module_dir} "QtQuick/Templates.2" PARENT_SCOPE)
		elseif(${qml_module} STREQUAL "qtgraphicaleffects")
			set(${qml_module_dir} "QtGraphicalEffects" PARENT_SCOPE)
		elseif(${qml_module} STREQUAL "qtgraphicaleffects/private")
			set(${qml_module_dir} "QtGraphicalEffects/private" PARENT_SCOPE)
		elseif(${qml_module} STREQUAL "qtlocation")
			set(${qml_module_dir} "QtLocation" PARENT_SCOPE)
		elseif(${qml_module} STREQUAL "qtpositioning")
			set(${qml_module_dir} "QtPositioning" PARENT_SCOPE)
		elseif(${qml_module} STREQUAL "qtqml-models2")
			set(${qml_module_dir} "QtQml/Models.2" PARENT_SCOPE)
		elseif(${qml_module} STREQUAL "qt-labs-folderlistmodel")
			set(${qml_module_dir} "Qt/labs/folderlistmodel" PARENT_SCOPE)
		elseif(${qml_module} STREQUAL "qt-labs-platform")
			set(${qml_module_dir} "Qt/labs/platform" PARENT_SCOPE)
		elseif(${qml_module} STREQUAL "qt-labs-settings")
			set(${qml_module_dir} "Qt/labs/settings" PARENT_SCOPE)
		elseif(${qml_module} STREQUAL "qt-labs-qmlmodels")
			set(${qml_module_dir} "Qt/labs/qmlmodels" PARENT_SCOPE)
		endif()
	endfunction(qt_qml_module_directory)

	# Get QML module plugin name
	function(qt_qml_module_plugin_name qml_module qml_plugin_name)
		if(${qml_module} STREQUAL "qtquick2")
			set(${qml_plugin_name} "qtquick2plugin" PARENT_SCOPE)
		elseif(${qml_module} STREQUAL "qtquick-layouts")
			set(${qml_plugin_name} "qquicklayoutsplugin" PARENT_SCOPE)
		elseif(${qml_module} STREQUAL "qtquick-controls")
			set(${qml_plugin_name} "qtquickcontrolsplugin" PARENT_SCOPE)
		elseif(${qml_module} STREQUAL "qtquick-dialogs")
			set(${qml_plugin_name} "dialogplugin" PARENT_SCOPE)
		elseif(${qml_module} STREQUAL "qtquick-dialogs/private")
			set(${qml_plugin_name} "dialogsprivateplugin" PARENT_SCOPE)
		elseif(${qml_module} STREQUAL "qtquick-controls2")
			set(${qml_plugin_name} "qtquickcontrols2plugin" PARENT_SCOPE)
		elseif(${qml_module} STREQUAL "qtquick-controls2/material")
			if(MODULE_Qt${QT_VERSION_MAJOR}_VERSION VERSION_LESS 5.15.3)
				set(${qml_plugin_name} "qtquickcontrols2materialstyleplugin" PARENT_SCOPE)
			else()
    				set(${qml_plugin_name} "qqc2materialstyleplugin" PARENT_SCOPE)
			endif()
		elseif(${qml_module} STREQUAL "qtquick-privatewidgets")
			set(${qml_plugin_name} "widgetsplugin" PARENT_SCOPE)
		elseif(${qml_module} STREQUAL "qtquick-window2")
			set(${qml_plugin_name} "windowplugin" PARENT_SCOPE)
		elseif(${qml_module} STREQUAL "qtquick-templates2")
			set(${qml_plugin_name} "qtquicktemplates2plugin" PARENT_SCOPE)
		elseif(${qml_module} STREQUAL "qtgraphicaleffects")
			set(${qml_plugin_name} "qtgraphicaleffectsplugin" PARENT_SCOPE)
		elseif(${qml_module} STREQUAL "qtgraphicaleffects/private")
			set(${qml_plugin_name} "qtgraphicaleffectsprivate" PARENT_SCOPE)
		elseif(${qml_module} STREQUAL "qtlocation")
			set(${qml_plugin_name} "declarative_location" PARENT_SCOPE)
		elseif(${qml_module} STREQUAL "qtpositioning")
			set(${qml_plugin_name} "declarative_positioning" PARENT_SCOPE)
		elseif(${qml_module} STREQUAL "qtqml-models2")
			set(${qml_plugin_name} "modelsplugin" PARENT_SCOPE)
		elseif(${qml_module} STREQUAL "qt-labs-folderlistmodel")
			set(${qml_plugin_name} "qmlfolderlistmodelplugin" PARENT_SCOPE)
		elseif(${qml_module} STREQUAL "qt-labs-platform")
			set(${qml_plugin_name} "qtlabsplatformplugin" PARENT_SCOPE)
		elseif(${qml_module} STREQUAL "qt-labs-settings")
			set(${qml_plugin_name} "qmlsettingsplugin" PARENT_SCOPE)
		elseif(${qml_module} STREQUAL "qt-labs-qmlmodels")
			set(${qml_plugin_name} "labsmodelsplugin" PARENT_SCOPE)
		endif()
	endfunction(qt_qml_module_plugin_name)

	# Get QML module has package
	function(qt_qml_module_has_package qml_module qml_has_package)
		if(${qml_module} MATCHES ".*/.*")
			set(${qml_has_package} FALSE PARENT_SCOPE)
		else()
			set(${qml_has_package} TRUE PARENT_SCOPE)
		endif()
	endfunction(qt_qml_module_has_package)

	# Get QML module has package
	function(qt_qml_module_has_plugins_qmltypes qml_module qml_has_plugins_qmltypes)
		if(${qml_module} MATCHES ".*/private")
			set(${qml_has_plugins_qmltypes} FALSE PARENT_SCOPE)
		else()
			set(${qml_has_plugins_qmltypes} TRUE PARENT_SCOPE)
		endif()
	endfunction(qt_qml_module_has_plugins_qmltypes)

	# Qt QML modules
	foreach(TMP_QML_MODULE ${QT_QML_MODULES})
		qt_qml_module_directory(${TMP_QML_MODULE} QML_MODULE_${TMP_QML_MODULE}_DIRECTORY)
		qt_qml_module_plugin_name(${TMP_QML_MODULE} QML_MODULE_${TMP_QML_MODULE}_PLUGIN_NAME)
		qt_qml_module_has_package(${TMP_QML_MODULE} QML_MODULE_${TMP_QML_MODULE}_HAS_PACKAGE)
		qt_qml_module_has_plugins_qmltypes(${TMP_QML_MODULE} QML_MODULE_${TMP_QML_MODULE}_HAS_PLUGINS_QMLTYPES)

		set(TMP_QML_MODULE_DIRECTORY ${QML_MODULE_${TMP_QML_MODULE}_DIRECTORY})
		set(TMP_QML_MODULE_PLUGIN_NAME ${QML_MODULE_${TMP_QML_MODULE}_PLUGIN_NAME})
		set(TMP_QML_MODULE_HAS_PACKAGE ${QML_MODULE_${TMP_QML_MODULE}_HAS_PACKAGE})
		set(TMP_QML_MODULE_HAS_PLUGINS_QMLTYPES ${QML_MODULE_${TMP_QML_MODULE}_HAS_PLUGINS_QMLTYPES})
		set(TMP_QML_MODULE_INSTALL_FILES FALSE)

		if(WIN32)
			# Install DLL files
			if(QT_USE_DEBUG_DLL)
				install(FILES "${QT_QML_DIR}/${TMP_QML_MODULE_DIRECTORY}/${TMP_QML_MODULE_PLUGIN_NAME}.dll"
					DESTINATION ${INSTALL_PATH_QTQML}/${TMP_QML_MODULE_DIRECTORY} CONFIGURATIONS Release
				)
				install(FILES "${QT_QML_DIR}/${TMP_QML_MODULE_DIRECTORY}/${TMP_QML_MODULE_PLUGIN_NAME}d.dll"
					DESTINATION ${INSTALL_PATH_QTQML}/${TMP_QML_MODULE_DIRECTORY} CONFIGURATIONS Debug
				)
			else()
				install(FILES "${QT_QML_DIR}/${TMP_QML_MODULE_DIRECTORY}/${TMP_QML_MODULE_PLUGIN_NAME}.dll"
					DESTINATION ${INSTALL_PATH_QTQML}/${TMP_QML_MODULE_DIRECTORY}
				)
			endif()
			set(TMP_QML_MODULE_INSTALL_FILES TRUE)
		elseif(APPLE)
			# Install dylib files
			install(FILES "${QT_QML_DIR}/${TMP_QML_MODULE_DIRECTORY}/lib${TMP_QML_MODULE_PLUGIN_NAME}.dylib"
				DESTINATION ${INSTALL_PATH_QTQML}/${TMP_QML_MODULE_DIRECTORY}
			)
			if(EXISTS "${QT_QML_DIR}/${TMP_QML_MODULE_DIRECTORY}/lib${TMP_QML_MODULE_PLUGIN_NAME}_debug.dylib")
				install(FILES "${QT_QML_DIR}/${TMP_QML_MODULE_DIRECTORY}/lib${TMP_QML_MODULE_PLUGIN_NAME}_debug.dylib"
					DESTINATION ${INSTALL_PATH_QTQML}/${TMP_QML_MODULE_DIRECTORY} CONFIGURATIONS Debug
				)
			endif()
			set(TMP_QML_MODULE_INSTALL_FILES TRUE)
		else()
			if(AUTONOMOUS_PACKAGE_BUILD)
				# Install SO files
				set(TMP_SO_FILE "${QT_QML_DIR}/${TMP_QML_MODULE_DIRECTORY}/lib${TMP_QML_MODULE_PLUGIN_NAME}.so")
				message("-- Installing SO file: ${TMP_SO_FILE}")
				install(FILES "${TMP_SO_FILE}" DESTINATION ${INSTALL_PATH_QTQML}/${TMP_QML_MODULE_DIRECTORY})
				set(TMP_QML_MODULE_INSTALL_FILES TRUE)
			else()
				if(TMP_QML_MODULE_HAS_PACKAGE)
					list(APPEND PACKAGE_DEPS "qml-module-${TMP_QML_MODULE}")
				endif()
			endif()
		endif()

		if(TMP_QML_MODULE_INSTALL_FILES)
			# Install qmldir file
			install(FILES "${QT_QML_DIR}/${TMP_QML_MODULE_DIRECTORY}/qmldir"
				DESTINATION ${INSTALL_PATH_QTQML}/${TMP_QML_MODULE_DIRECTORY}
			)
			# Install plugins.qmltypes file
			if(TMP_QML_MODULE_HAS_PLUGINS_QMLTYPES)
				install(FILES "${QT_QML_DIR}/${TMP_QML_MODULE_DIRECTORY}/plugins.qmltypes"
					DESTINATION ${INSTALL_PATH_QTQML}/${TMP_QML_MODULE_DIRECTORY}
				)
			endif()
		endif()
	endforeach(TMP_QML_MODULE)

	foreach(TMP_COMPONENT ${QT_QML_MODULES_FILES})
		if(WIN32 OR APPLE OR AUTONOMOUS_PACKAGE_BUILD)
			set(TMP_QML_SRC_DIR ${QT_QML_DIR}/${TMP_COMPONENT})
			# Compute directory name
			string(FIND ${TMP_COMPONENT} "/" POSITION REVERSE)
			string(SUBSTRING ${TMP_COMPONENT} 0 ${POSITION} DIR)
			if(IS_DIRECTORY "${QT_QML_DIR}/${TMP_COMPONENT}")
				install_qml_directory("${TMP_QML_SRC_DIR}" "${INSTALL_PATH_QTQML}/${DIR}")
			else()
				install(FILES "${TMP_QML_SRC_DIR}" DESTINATION "${INSTALL_PATH_QTQML}/${DIR}")
			endif()
		endif()
	endforeach(TMP_COMPONENT)

	# Create function for plugins installation
	function(install_qt_plugins category components)
		set(SRC_DIR ${QT_PLUGINS_DIR}/${category})
		set(DST_DIR ${INSTALL_PATH_QTPLUGINS}/${category})
		foreach(TMP_COMPONENT ${components})
			if (WIN32)
				install_qt_plugins_dll(${SRC_DIR} ${TMP_COMPONENT} ${QT_VERSION_MAJOR} ${DST_DIR})
			elseif(APPLE)
				install(FILES "${SRC_DIR}/lib${TMP_COMPONENT}.dylib" DESTINATION ${DST_DIR})
				if(EXISTS "${SRC_DIR}/lib${TMP_COMPONENT}_debug.dylib")
					install(FILES "${SRC_DIR}/lib${TMP_COMPONENT}_debug.dylib" DESTINATION ${DST_DIR} CONFIGURATIONS Debug)
				endif()
			else()
				if(AUTONOMOUS_PACKAGE_BUILD)
					install(FILES "${SRC_DIR}/lib${TMP_COMPONENT}.so" DESTINATION ${DST_DIR})
				endif()
			endif()
		endforeach(TMP_COMPONENT)
	endfunction(install_qt_plugins)

	# Create function for plugins installation
	function(install_qt_plugins_directory category)
		set(SRC_DIR ${QT_PLUGINS_DIR}/${category})
		set(DST_DIR ${INSTALL_PATH_QTPLUGINS})
		install(DIRECTORY "${SRC_DIR}" DESTINATION ${DST_DIR})
	endfunction(install_qt_plugins_directory)

	# Qt Platforms plugins (Qt5+ only)
	if(NOT QT_USE_VERSION LESS 5)
		install_qt_plugins("platforms" "${QT_PLATFORMS_COMPONENTS}")
	endif()

	# Qt Platforms plugins (Qt5+ only)
	if(NOT QT_USE_VERSION LESS 5)
		install_qt_plugins("platforms" "${QT_PLATFORMS_COMPONENTS}")
	endif()

	if("${QT_PLATFORMS_COMPONENTS}" MATCHES "qxcb")
		install_qt_plugins_directory("xcbglintegrations")
	endif()

	if("${QT_PLATFORMS_COMPONENTS}" MATCHES "wayland")
		install_qt_plugins_directory("wayland-shell-integration")
		install_qt_plugins_directory("wayland-graphics-integration-client")
		install_qt_plugins_directory("wayland-decoration-client")
	endif()

	# Qt Styles plugins (Qt5+ only)
	if(NOT QT_USE_VERSION LESS 5)
		install_qt_plugins("styles" "${QT_STYLES_COMPONENTS}")
	endif()

	# Qt ImageFormats plugins
	install_qt_plugins("imageformats" "${QT_IMAGEFORMATS_COMPONENTS}")

	# Qt GeoServices plugins
	if(NOT QT_USE_VERSION LESS 5)
		install_qt_plugins("geoservices" "${QT_GEOSERVICES_COMPONENTS}")
	endif()

	# Qt Sql plugins
	foreach(TMP_COMPONENT ${QT_SQL_COMPONENTS})

		if(TMP_COMPONENT STREQUAL "qsqlmysql")
			set(TMP_MODULE "mysql")
		else()
			string(SUBSTRING ${TMP_COMPONENT} 1 -1 TMP_MODULE)
			string(TOLOWER ${TMP_MODULE} TMP_MODULE)
		endif()

		if (WIN32)
			install_qt_plugins_dll("${QT_PLUGINS_DIR}/sqldrivers" ${TMP_COMPONENT} ${QT_VERSION_MAJOR} "${INSTALL_PATH_QTPLUGINS}/sqldrivers")
		elseif(APPLE)
			INSTALL(FILES "${QT_PLUGINS_DIR}/sqldrivers/lib${TMP_COMPONENT}.dylib"
				DESTINATION ${INSTALL_PATH_QTPLUGINS}/sqldrivers
			)
			if(EXISTS "${QT_PLUGINS_DIR}/sqldrivers/lib${TMP_COMPONENT}_debug.dylib")
				INSTALL(FILES "${QT_PLUGINS_DIR}/sqldrivers/lib${TMP_COMPONENT}_debug.dylib"
					DESTINATION ${INSTALL_PATH_QTPLUGINS}/sqldrivers CONFIGURATIONS Debug
				)
			endif()
		else()
			if(AUTONOMOUS_PACKAGE_BUILD)
				install(FILES "${QT_PLUGINS_DIR}/sqldrivers/lib${TMP_COMPONENT}.so"
					DESTINATION ${INSTALL_PATH_QTPLUGINS}/sqldrivers
				)
			else()
				if(QT_USE_VERSION LESS 5)
					set(TMP_PACKAGE_NAME "libqt${QT_VERSION_MAJOR}-sql-${TMP_MODULE}")
				else()
					set(TMP_PACKAGE_NAME "libqt${QT_VERSION_MAJOR}sql${QT_VERSION_MAJOR}-${TMP_MODULE}")
				endif()
				list(APPEND PACKAGE_DEPS "${TMP_PACKAGE_NAME} (>= ${QT_DEPS_VERSION})")
			endif()
		endif()
	endforeach(TMP_COMPONENT)

	# Qt Audio plugins
	if(NOT QT_USE_VERSION LESS 5)
		install_qt_plugins("audio" "${QT_AUDIO_COMPONENTS}")
	endif()

	if(NOT QT_USE_VERSION LESS 5)
		set(QT_QTMAIN_LIBRARY ${Qt${QT_VERSION_MAJOR}Core_QTMAIN_LIBRARIES})
	endif()

endif(QT_FOUND)
