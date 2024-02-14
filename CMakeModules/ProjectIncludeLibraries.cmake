# Do include for requested modules

include (CheckFunctionExists)

include (ProjectUsefulMacros)

option(SINGLE_UNIT_COMPILATION "Enable single unit compilation" OFF)
option(AUTONOMOUS_PACKAGE_BUILD "Enable build of autonomous package without dependencies" OFF)

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
			execute_process(COMMAND ${CMAKE_CXX_COMPILER} -print-file-name=libgcc_s_sjlj-1.dll OUTPUT_VARIABLE TMP_DLL_PATH OUTPUT_STRIP_TRAILING_WHITESPACE)
			if(NOT TMP_DLL_PATH STREQUAL "" AND NOT TMP_DLL_PATH STREQUAL "libgcc_s_sjlj-1.dll")
				install(FILES ${TMP_DLL_PATH} DESTINATION ${INSTALL_PATH_BIN})
			endif()
			execute_process(COMMAND ${CMAKE_CXX_COMPILER} -print-file-name=libgcc_s_seh-1.dll OUTPUT_VARIABLE TMP_DLL_PATH OUTPUT_STRIP_TRAILING_WHITESPACE)
			if(NOT TMP_DLL_PATH STREQUAL "" AND NOT TMP_DLL_PATH STREQUAL "libgcc_s_seh-1.dll")
				install(FILES ${TMP_DLL_PATH} DESTINATION ${INSTALL_PATH_BIN})
			endif()
		endif(CMAKE_COMPILER_IS_GNUCXX)

		if(MSVC)
			install(FILES C:/Windows/System32/msvcp100.dll DESTINATION ${INSTALL_PATH_BIN})
			install(FILES C:/Windows/System32/msvcr100.dll DESTINATION ${INSTALL_PATH_BIN})
			install(FILES C:/Windows/System32/vcomp140.dll DESTINATION ${INSTALL_PATH_BIN})
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
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${SIMD_FLAGS}")
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
			set(PTHREAD_LIBRARIES -lpthread)
		endif(Threads_FOUND)
	endif(WIN32)
endif(WITH_PTHREAD)

option(WITH_OPENMP "Enable build with OpenMP library" OFF)
if(WITH_OPENMP)
	find_package(OpenMP)
	if(OPENMP_FOUND)
		set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
		set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
	endif()
endif(WITH_OPENMP)

# Add gdb
if(CMAKE_BUILD_TYPE MATCHES Debug)
	if(NOT AUTONOMOUS_PACKAGE_BUILD)
		list(APPEND PACKAGE_DEPS "gdb")
	endif()
endif()

option(WITH_ALEAKD "Enable build with ALeakD library" OFF)
if(WITH_ALEAKD)
	#set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "../libaleakd/release/usr/local")
	find_package(ALeakD)
	if(ALeakD_FOUND)
		include_directories (${ALeakD_INCLUDE_DIR})
	endif(ALeakD_FOUND)
endif(WITH_ALEAKD)

#####################
### Security
#####################

# Add OpenSSL
option(WITH_OPENSSL "Enable build with openssl library" OFF)
if(WITH_OPENSSL)
	find_package (OpenSSL)
	include_directories (${OPENSSL_INCLUDE_DIR})
	if(OPENSSL_FOUND)
		message(STATUS "OpenSSL found: ${OPENSSL_VERSION}")
		set(USE_OPENSSL 1)

		if(WIN32)
			if(MSVC)
				find_install_dll("libcrypto" ${INSTALL_PATH_BIN} TRUE)
				find_install_dll("libssl" ${INSTALL_PATH_BIN} TRUE)
			else()
				find_install_dll("crypto" ${INSTALL_PATH_BIN} TRUE)
				find_install_dll("ssl" ${INSTALL_PATH_BIN} TRUE)
			endif()
		elseif(NOT APPLE)
			if(AUTONOMOUS_PACKAGE_BUILD)
				install_libs_so("${OPENSSL_LIBRARIES}" ${INSTALL_PATH_LIB})
			endif(AUTONOMOUS_PACKAGE_BUILD)
		endif(WIN32)

	endif(OPENSSL_FOUND)
endif(WITH_OPENSSL)

if(QT_OPENSSL_USE)
	if(MSVC)
		# Workaround to get Qt Openssl v1 when vcpkg use v3
		message(STATUS "Using Qt Open SSL version from ${QT_OPENSSL_PATH}")
		find_install_dll_in_path("libcrypto" ${QT_OPENSSL_PATH}/bin ${INSTALL_PATH_BIN} TRUE)
		find_install_dll_in_path("libssl" ${QT_OPENSSL_PATH}/bin ${INSTALL_PATH_BIN} TRUE)
	endif()
endif()

#####################
### Security
#####################

# Database
option(WITH_MYSQL "Enable build with mysql library" OFF)
if(WITH_MYSQL)
	find_package (MySQL)
	if(MySQL_FOUND)
		set(USE_MYSQL 1)
		if(WIN32)
			install(FILES ${MySQL_RUNTIME_LIBRARY_PATH}/libmysql.dll DESTINATION ${INSTALL_PATH_BIN})
		else()
			
		endif(WIN32)
	endif(MySQL_FOUND)
endif(WITH_MYSQL)

#####################
### Access module
#####################

# Add Curl
option(WITH_CURL "Enable build with curl library" OFF)
if(WITH_CURL)
	set(CURL_STATIC OFF CACHE BOOL "Build with static CURL library" FORCE)
	find_package (CURL 7.0)
	include_directories (${CURL_INCLUDE_DIRS})
	if(CURL_FOUND)
		set(USE_CURL 1)
		if(NOT CURL_STATIC)
			if(WIN32)
				if(MSVC)
					install(FILES ${CURL_RUNTIME_LIBRARY_PATH}/libcurl.dll DESTINATION ${INSTALL_PATH_BIN})
					#install(FILES ${CURL_RUNTIME_LIBRARY_PATH}/curllib.dll DESTINATION ${INSTALL_PATH_BIN})
					#install(FILES ${CURL_RUNTIME_LIBRARY_PATH}/ssleay32.dll DESTINATION ${INSTALL_PATH_BIN})
					#install(FILES ${CURL_RUNTIME_LIBRARY_PATH}/openldap.dll DESTINATION ${INSTALL_PATH_BIN})
				else(MSVC)
					if(EXISTS ${CURL_RUNTIME_LIBRARY_PATH}/libcurl-4.dll)
						install(FILES ${CURL_RUNTIME_LIBRARY_PATH}/libcurl-4.dll DESTINATION ${INSTALL_PATH_BIN})
					else()
						install(FILES ${CURL_RUNTIME_LIBRARY_PATH}/libcurl.dll DESTINATION ${INSTALL_PATH_BIN})
					endif()

					# Must be imported from OpenSSL
					if(EXISTS ${CURL_RUNTIME_LIBRARY_PATH}/libcrypto-1_1.dll)
						install(FILES ${CURL_RUNTIME_LIBRARY_PATH}/libcrypto-1_1.dll DESTINATION ${INSTALL_PATH_BIN})
					endif()
					if(EXISTS ${CURL_RUNTIME_LIBRARY_PATH}/libssl-1_1.dll)
						install(FILES ${CURL_RUNTIME_LIBRARY_PATH}/libssl-1_1.dll DESTINATION ${INSTALL_PATH_BIN})
					endif()

					if(EXISTS ${CURL_RUNTIME_LIBRARY_PATH}/libidn-11.dll)
						install(FILES ${CURL_RUNTIME_LIBRARY_PATH}/libidn-11.dll DESTINATION ${INSTALL_PATH_BIN})
					endif()
					if(EXISTS ${CURL_RUNTIME_LIBRARY_PATH}/libeay32.dll)
						install(FILES ${CURL_RUNTIME_LIBRARY_PATH}/libeay32.dll DESTINATION ${INSTALL_PATH_BIN})
					endif()
					if(EXISTS ${CURL_RUNTIME_LIBRARY_PATH}/ssleay32.dll)
						install(FILES ${CURL_RUNTIME_LIBRARY_PATH}/ssleay32.dll DESTINATION ${INSTALL_PATH_BIN})
					endif()
				endif(MSVC)
			elseif(APPLE)
				list(GET CURL_LIBRARIES 0 CURL_LIBRARY_PATH_0)
				get_filename_component(CURL_LIBRARY_PATH ${CURL_LIBRARY_PATH_0} DIRECTORY)
				get_filename_component(CURL_LIBRARY_ROOT_PATH ${CURL_LIBRARY_PATH}/../.. REALPATH)
				if(EXISTS ${CURL_LIBRARY_ROOT_PATH}/openssl)
					file(GLOB OPENSSL_INSTALL
						${CURL_LIBRARY_ROOT_PATH}/openssl/lib/libssl*.dylib
						${CURL_LIBRARY_ROOT_PATH}/openssl/lib/libcrypto*.dylib
					)
					install(FILES ${OPENSSL_INSTALL} DESTINATION ${INSTALL_PATH_LIB})
				endif()
				file(GLOB CURL_LIBRARIES_INSTALL
					${CURL_LIBRARY_PATH}/libcurl*.dylib
				)
				install(FILES ${CURL_LIBRARIES_INSTALL} DESTINATION ${INSTALL_PATH_LIB})
			else()
				if(AUTONOMOUS_PACKAGE_BUILD)
					install_libs_so("${CURL_LIBRARIES}" ${INSTALL_PATH_LIB})
				else(AUTONOMOUS_PACKAGE_BUILD)
					if("${CURL_VERSION_STRING}" VERSION_LESS "7.60.0")
						list(APPEND PACKAGE_DEPS "libcurl3 (>= 7.25)")
					else()
						list(APPEND PACKAGE_DEPS "libcurl4 (>= 7.60)")
					endif()
				endif(AUTONOMOUS_PACKAGE_BUILD)
			endif()
		endif(NOT CURL_STATIC)
	endif(CURL_FOUND)
endif(WITH_CURL)

# Add livemedia
option(WITH_LIVEMEDIA "Enable build with livemedia library" OFF)
if(WITH_LIVEMEDIA)
	set(LIVEMEDIA_STATIC OFF CACHE BOOL "Build with static LiveMedia library" FORCE)
	find_package (LiveMedia 7)
	include_directories (${LIVEMEDIA_INCLUDE_DIRS})
	if(LIVEMEDIA_FOUND)
		set(USE_LIVEMEDIA 1)
		if(NOT LIVEMEDIA_STATIC)
			# Linux
			if (UNIX AND NOT APPLE)
				if(AUTONOMOUS_PACKAGE_BUILD OR WITH_LIVEMEDIA_CUSTOM)
					install_libs_so("${LIVEMEDIA_LIBRARIES}" ${INSTALL_PATH_LIB})
				else()
					if(LIVEMEDIA_LIBRARY_NAMES)
						foreach(library_name ${LIVEMEDIA_LIBRARY_NAMES})
							string(TOLOWER ${library_name} TMP_LIBRARY_NAME_LOWER)
							list(APPEND PACKAGE_DEPS lib${TMP_LIBRARY_NAME_LOWER}${LIVEMEDIA_${library_name}_VERSION_MAJOR})
						endforeach()
					endif()
				endif()
			endif()
			# Windows
			if(WIN32 AND NOT MSVC)
				install(FILES ${LIVEMEDIA_RUNTIME_LIBRARY_PATH}/livemedia.dll DESTINATION ${INSTALL_PATH_BIN})
			endif(WIN32 AND NOT MSVC)
		endif()
	endif(LIVEMEDIA_FOUND)
endif(WITH_LIVEMEDIA)

# Add HikVisionSDK
option(WITH_HIKVISIONSDK "Enable build with HikVisionSDK library" OFF)
if(WITH_HIKVISIONSDK)
	if(HIKVISIONSDK_ROOT)
		if(WIN32)
		  if("${CMAKE_SIZEOF_VOID_P}" STREQUAL "4")
			  set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${HIKVISIONSDK_ROOT}/win32)
		  else()
			  set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${HIKVISIONSDK_ROOT}/win64)
		  endif()
		else(WIN32)
		  if("${CMAKE_SIZEOF_VOID_P}" STREQUAL "4")
			  set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${HIKVISIONSDK_ROOT}/linux32)
		  else()
			  set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${HIKVISIONSDK_ROOT}/linux64)
		  endif()
		endif(WIN32)
	endif(HIKVISIONSDK_ROOT)

	find_package (HikVisionSDK)
	include_directories (${HIKVISIONSDK_INCLUDE_DIRS})
	if(HIKVISIONSDK_FOUND)
		set(USE_HIKVISIONSDK 1)
		if(WIN32)
			install(FILES ${HIKVISIONSDK_RUNTIME_LIBRARY_PATH}/HCNetSDK.dll DESTINATION ${INSTALL_PATH_BIN})
		else(WIN32)
			# Install the shared libraries within the archive
			if(HIKVISIONSDK_ROOT)
				foreach(lib ${HIKVISIONSDK_LIBRARIES})
					install(FILES ${lib} DESTINATION ${INSTALL_PATH_LIB})
				endforeach()
			endif()
		endif(WIN32)
	endif(HIKVISIONSDK_FOUND)
endif(WITH_HIKVISIONSDK)

# Add Bonjour
option(WITH_BONJOUR "Enable build with Bonjour library" OFF)
if(WITH_BONJOUR)
	find_package(Bonjour REQUIRED)
	if(Bonjour_FOUND)
		include_directories(${Bonjour_INCLUDE_DIR})
		# Linux
		if (UNIX AND NOT APPLE)
			if(AUTONOMOUS_PACKAGE_BUILD)
				install_libs_so("${Bonjour_LIBRARIES}" ${INSTALL_PATH_LIB})
			else()
				list(APPEND PACKAGE_DEPS "libavahi-compat-libdnssd1")
			endif()
		endif()
		set(USE_BONJOUR 1)
	endif(Bonjour_FOUND)
endif(WITH_BONJOUR)

#####################
### Mux/Demux modules
#####################

# Add AVFormat
option(WITH_AVFORMAT "Enable build with avformat library" OFF)
if(WITH_AVFORMAT)
	find_package (AVFormat 54)
	if(AVFORMAT_FOUND)
		set(USE_AVFORMAT 1)
		include_directories (${AVFORMAT_INCLUDE_DIRS})
		if(WIN32)
			install(DIRECTORY ${AVFORMAT_RUNTIME_LIBRARY_PATH}/ DESTINATION ${INSTALL_PATH_BIN} FILES_MATCHING PATTERN "avformat-*.dll" PATTERN "avutil-*.dll")
		elseif(APPLE)
			list(GET AVFORMAT_LIBRARIES 0 AVFORMAT_LIBRARY_PATH_0)
			get_filename_component(AVFORMAT_LIBRARY_PATH ${AVFORMAT_LIBRARY_PATH_0} DIRECTORY)
			get_filename_component(AVFORMAT_LIBRARY_ROOT_PATH ${AVFORMAT_LIBRARY_PATH}/../.. REALPATH)
			file(GLOB AVFORMAT_LIBRARIES_INSTALL
				${AVFORMAT_LIBRARY_PATH}/libavformat*.dylib
				${AVFORMAT_LIBRARY_PATH}/libavutil*.dylib
			)
			install(FILES ${AVFORMAT_LIBRARIES_INSTALL} DESTINATION ${INSTALL_PATH_LIB})
	else()
		if(AUTONOMOUS_PACKAGE_BUILD)
			install_libs_so("${AVFORMAT_LIBRARIES}" ${INSTALL_PATH_LIB})
		else()
				list(APPEND PACKAGE_DEPS "libavformat${AVFORMAT_VERSION_MAJOR}")
 		endif()
		endif()
	endif(AVFORMAT_FOUND)
endif(WITH_AVFORMAT)

#####################
### Codec modules
#####################

# Add libjpeg
option(WITH_JPEG "Enable build with jpeg library" OFF)
if(WITH_JPEG)
	find_package (JPEG 8)
	if(JPEG_FOUND)
		set(USE_JPEG 1)
		include_directories (${JPEG_INCLUDE_DIR})
		list(APPEND PACKAGE_DEPS "libjpeg8 (>= 8)")
	endif(JPEG_FOUND)
endif(WITH_JPEG)

# Add libjpeg-turbo
# WARNING: Static linkage fail with Qt
option(WITH_JPEGTURBO "Enable build with jpeg-turbo library" OFF)
if(WITH_JPEGTURBO)
	if(WIN32 AND NOT UNIX)
		set(JPEGTURBO_STATIC OFF CACHE BOOL "Build with static libjpeg-turbo library" FORCE)
	else(WIN32 AND NOT UNIX)
		set(JPEGTURBO_STATIC ON CACHE BOOL "Build with static libjpeg-turbo library" FORCE)
	endif(WIN32 AND NOT UNIX)
	find_package (JPEGTurbo 8)
	if(JPEGTURBO_FOUND)
		set(USE_JPEGTURBO 1)
		include_directories (${JPEGTURBO_INCLUDE_DIR})

		set(CMAKE_REQUIRED_INCLUDES ${JPEGTURBO_INCLUDE_DIR})
		set(CMAKE_REQUIRED_LIBRARIES ${JPEGTURBO_TJPEG_LIBRARY})
		CHECK_FUNCTION_EXISTS (tjDecompressToYUV2 HAVE_TJDECOMPRESSTOYUV2)
		CHECK_FUNCTION_EXISTS (tjCompressFromYUV HAVE_TJCOMPRESSFROMYUV)
		CHECK_FUNCTION_EXISTS (tjCompressFromYUVPlanes HAVE_TJCOMPRESSFROMYUVPLANES)
		include (CheckPrototypeDefinition)
		CHECK_PROTOTYPE_DEFINITION(tjCompress2
				"DLLEXPORT int DLLCALL tjCompress2(tjhandle handle, const unsigned char *srcBuf, int width, int pitch, int height, int pixelFormat, unsigned char **jpegBuf, unsigned long *jpegSize, int jpegSubsamp, int jpegQual, int flags)"
		 "0"
		 "turbojpeg.h"
		 HAVE_TJCONSTBUFFER)

		if(WIN32 AND NOT JPEGTURBO_STATIC)
			if(MSVC)
				install(FILES ${JPEGTURBO_RUNTIME_TJPEG_LIBRARY_PATH}/turbojpeg.dll DESTINATION ${INSTALL_PATH_BIN})
			else(MSVC)
				if(EXISTS ${JPEGTURBO_RUNTIME_TJPEG_LIBRARY_PATH}/libturbojpeg-0.dll)
					install(FILES ${JPEGTURBO_RUNTIME_TJPEG_LIBRARY_PATH}/libturbojpeg-0.dll DESTINATION ${INSTALL_PATH_BIN})
				else()
					install(FILES ${JPEGTURBO_RUNTIME_TJPEG_LIBRARY_PATH}/libturbojpeg.dll DESTINATION ${INSTALL_PATH_BIN})
				endif()
			endif(MSVC)
		endif(WIN32 AND NOT JPEGTURBO_STATIC)
		if(NOT JPEGTURBO_STATIC)
			list(APPEND PACKAGE_DEPS "libjpeg-turbo (>= 1.2)")
		endif()
	endif(JPEGTURBO_FOUND)
endif(WITH_JPEGTURBO)

# Add IPP
option(WITH_IPP "Enable build with ipp library" OFF)
if(WITH_IPP)
	find_package (IPP 7)
endif(WITH_IPP)

# Add AVCodec
option(WITH_AVCODEC "Enable build with avcodec library" OFF)
if(WITH_AVCODEC)
	find_package (AVCodec 54)
	if(AVCODEC_FOUND)
		set(USE_AVCODEC 1)
		include_directories (${AVCODEC_INCLUDE_DIRS})
		if(WIN32)
			install(DIRECTORY ${AVCODEC_RUNTIME_LIBRARY_PATH}/ DESTINATION ${INSTALL_PATH_BIN} FILES_MATCHING PATTERN "avcodec-*.dll" PATTERN "avutil-*.dll" PATTERN "avresample-*.dll")
			find_install_dll("openh264" ${INSTALL_PATH_BIN} FALSE)
			if(MSVC)
				find_install_dll("libx265" ${INSTALL_PATH_BIN} FALSE)
				find_install_dll("libx264" ${INSTALL_PATH_BIN} FALSE)
			else()
				find_install_dll("x265" ${INSTALL_PATH_BIN} FALSE)
				find_install_dll("x264" ${INSTALL_PATH_BIN} FALSE)
			endif()
		elseif(APPLE)
			list(GET AVCODEC_LIBRARIES 0 AVCODEC_LIBRARY_PATH_0)
			get_filename_component(AVCODEC_LIBRARY_PATH ${AVCODEC_LIBRARY_PATH_0} DIRECTORY)
			get_filename_component(AVCODEC_LIBRARY_ROOT_PATH ${AVCODEC_LIBRARY_PATH}/../.. REALPATH)
			if(EXISTS ${AVCODEC_LIBRARY_ROOT_PATH}/x265)
				file(GLOB X265_INSTALL
					${AVCODEC_LIBRARY_ROOT_PATH}/x265/lib/libx265*.dylib
				)
				install(FILES ${X265_INSTALL} DESTINATION ${INSTALL_PATH_LIB})
			endif()
			if(EXISTS ${AVCODEC_LIBRARY_ROOT_PATH}/libass)
				file(GLOB LIBASS_INSTALL
					${AVCODEC_LIBRARY_ROOT_PATH}/libass/lib/libass*.dylib
					${AVCODEC_LIBRARY_ROOT_PATH}/fribidi/lib/libfribidi*.dylib
					${AVCODEC_LIBRARY_ROOT_PATH}/harfbuzz/lib/libharfbuzz*.dylib
					${AVCODEC_LIBRARY_ROOT_PATH}/graphite2/lib/libgraphite2*.dylib
					${AVCODEC_LIBRARY_ROOT_PATH}/glib/lib/libglib*.dylib
					${AVCODEC_LIBRARY_ROOT_PATH}/gettext/lib/libintl*.dylib
					${AVCODEC_LIBRARY_ROOT_PATH}/pcre/lib/libpcre*.dylib
				)
				install(FILES ${LIBASS_INSTALL} DESTINATION ${INSTALL_PATH_LIB})
			endif()
			if(EXISTS ${AVCODEC_LIBRARY_ROOT_PATH}/fontconfig)
				file(GLOB FONTCONFIG_INSTALL
					${AVCODEC_LIBRARY_ROOT_PATH}/fontconfig/lib/libfontconfig*.dylib
				)
				install(FILES ${FONTCONFIG_INSTALL} DESTINATION ${INSTALL_PATH_LIB})
			endif()
			if(EXISTS ${AVCODEC_LIBRARY_ROOT_PATH}/freetype)
				file(GLOB LIBFREETYPE_INSTALL
					${AVCODEC_LIBRARY_ROOT_PATH}/freetype/lib/libfreetype*.dylib
					${AVCODEC_LIBRARY_ROOT_PATH}/libpng/lib/libpng1*.dylib
				)
				install(FILES ${LIBFREETYPE_INSTALL} DESTINATION ${INSTALL_PATH_LIB})
			endif()
			if(EXISTS ${AVCODEC_LIBRARY_ROOT_PATH}/xz)
				file(GLOB LIBLZMA_INSTALL
					${AVCODEC_LIBRARY_ROOT_PATH}/xz/lib/liblzma*.dylib
				)
				install(FILES ${LIBLZMA_INSTALL} DESTINATION ${INSTALL_PATH_LIB})
			endif()
			file(GLOB AVCODEC_LIBRARIES_INSTALL
				${AVCODEC_LIBRARY_PATH}/libavcodec*.dylib
				${AVCODEC_LIBRARY_PATH}/libavutil*.dylib
				${AVCODEC_LIBRARY_ROOT_PATH}/x264/lib/libx264*.dylib
				${AVCODEC_LIBRARY_ROOT_PATH}/lame/lib/libmp3lame*.dylib
			)
			install(FILES ${AVCODEC_LIBRARIES_INSTALL} DESTINATION ${INSTALL_PATH_LIB})
		else()
			if(AUTONOMOUS_PACKAGE_BUILD)
				install_libs_so("${AVCODEC_LIBRARIES}" ${INSTALL_PATH_LIB})
				find_install_lib_so("openh264" ${INSTALL_PATH_LIB} FALSE)
				find_install_lib_so("x264" ${INSTALL_PATH_LIB} FALSE)
				find_install_lib_so("x265" ${INSTALL_PATH_LIB} FALSE)
				find_install_lib_so("swresample" ${INSTALL_PATH_LIB} FALSE)
			else()
				list(APPEND PACKAGE_DEPS "libavcodec${AVCODEC_VERSION_MAJOR}")
			endif()
		endif()
	endif(AVCODEC_FOUND)
endif(WITH_AVCODEC)

option(WITH_DE265 "Enable build with avcodec library" OFF)
if(WITH_DE265)
	find_package (DE265 REQUIRED)
	if (DE265_FOUND)
		set(USE_DE265 1)
		include_directories (${DE265_INCLUDE_DIRS})
	endif()
endif(WITH_DE265)

#########################
### Video filter modules
#########################

# Add SWScale
option(WITH_SWSCALE "Enable build with swscale library" OFF)
if(WITH_SWSCALE)
	find_package (SWScale 2)
	if(SWSCALE_FOUND)
		set(USE_SWSCALE 1)
		if(WIN32)
			install(DIRECTORY ${SWSCALE_RUNTIME_LIBRARY_PATH}/ DESTINATION ${INSTALL_PATH_BIN} FILES_MATCHING PATTERN "swscale-*.dll")
		elseif(APPLE)
			 list(GET SWSCALE_LIBRARIES 0 SWSCALE_LIBRARY_PATH_0)
			 get_filename_component(SWSCALE_LIBRARY_PATH ${SWSCALE_LIBRARY_PATH_0} DIRECTORY)
			 install(DIRECTORY ${SWSCALE_LIBRARY_PATH}/ DESTINATION ${INSTALL_PATH_LIB} FILES_MATCHING PATTERN "libswscale*.dylib")
		else()
			if(AUTONOMOUS_PACKAGE_BUILD)
				install_libs_so("${SWSCALE_LIBRARIES}" ${INSTALL_PATH_LIB})
			else()
				list(APPEND PACKAGE_DEPS "libswscale${SWSCALE_VERSION_MAJOR}")
			endif()
		endif()
	endif(SWSCALE_FOUND)
endif(WITH_SWSCALE)

# Add AVFilter
option(WITH_AVFILTER "Enable build with avfilter library" OFF)
if(WITH_AVFILTER)
	find_package (AVFilter 3)
	if(AVFILTER_FOUND)
		set(USE_AVFILTER 1)
		include_directories (${AVFILTER_INCLUDE_DIRS})
		if(WIN32)
			install(DIRECTORY ${AVFILTER_RUNTIME_LIBRARY_PATH}/ DESTINATION ${INSTALL_PATH_BIN} FILES_MATCHING PATTERN "avfilter-*.dll" PATTERN "avformat-*.dll" PATTERN "avcodec-*.dll" PATTERN "avutil-*.dll" PATTERN "swresample-*.dll" PATTERN "postproc-*.dll" PATTERN "swscale-*.dll" PATTERN "libpng*.dll")
			if (LIB_VCPKG_INSTALL)
				if (CMAKE_BUILD_TYPE STREQUAL "Debug")
					install(DIRECTORY ${LIB_VCPKG_INSTALL}/bin/ DESTINATION ${INSTALL_PATH_BIN} FILES_MATCHING PATTERN "bz2.dll")
					install(DIRECTORY ${LIB_VCPKG_INSTALL}/debug/bin/ DESTINATION ${INSTALL_PATH_BIN} FILES_MATCHING PATTERN "ass*.dll" PATTERN "fribidi*.dll" PATTERN "libprotobufd.dll" PATTERN "bz2.dll" PATTERN "brotlidec.dll" PATTERN "freetyped.dll" PATTERN "harfbuzz.dll" PATTERN "bz2d.dll" PATTERN "libpng16d.dll" PATTERN "brotlicommon.dll" PATTERN "zlib1.dll" PATTERN "zlibd1.dll")
				elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
					install(DIRECTORY ${LIB_VCPKG_INSTALL}/bin/ DESTINATION ${INSTALL_PATH_BIN} FILES_MATCHING PATTERN "bz2.dll" PATTERN "ass*.dll" PATTERN "fribidi*.dll" PATTERN "libprotobuf.dll" PATTERN "bz2.dll" PATTERN "brotlidec.dll" PATTERN "harfbuzz.dll" PATTERN "brotlicommon.dll")
				else()
					message(FATAL_ERROR "Unsupported build type")
				endif()
			endif()
		elseif(APPLE)
			list(GET AVFILTER_LIBRARIES 0 AVFILTER_LIBRARY_PATH_0)
			get_filename_component(AVFILTER_LIBRARY_PATH ${AVFILTER_LIBRARY_PATH_0} DIRECTORY)
			get_filename_component(AVFILTER_LIBRARY_ROOT_PATH ${AVFILTER_LIBRARY_PATH}/../.. REALPATH)
			file(GLOB AVFILTER_LIBRARIES_INSTALL
				${AVFILTER_LIBRARY_PATH}/libavfilter*.dylib
				${AVFILTER_LIBRARY_PATH}/libavformat*.dylib
				${AVFILTER_LIBRARY_PATH}/libavcodec*.dylib
				${AVFILTER_LIBRARY_PATH}/libavresample*.dylib
				${AVFILTER_LIBRARY_PATH}/libswresample*.dylib
				${AVFILTER_LIBRARY_PATH}/libswscale*.dylib
				${AVFILTER_LIBRARY_PATH}/libpostproc*.dylib
				${AVFILTER_LIBRARY_PATH}/libfreetype*.dylib
				${AVFILTER_LIBRARY_PATH}/libzlib*.dylib
				${AVFILTER_LIBRARY_ROOT_PATH}/x264/lib/libx264*.dylib
				${AVFILTER_LIBRARY_ROOT_PATH}/lame/lib/libmp3lame*.dylib
			)
			install(FILES ${AVFILTER_LIBRARIES_INSTALL} DESTINATION ${INSTALL_PATH_LIB})
		else()
			if(AUTONOMOUS_PACKAGE_BUILD)
				install_libs_so("${AVFILTER_LIBRARIES}" ${INSTALL_PATH_LIB})
			else()
				list(APPEND PACKAGE_DEPS "libavfilter${AVFILTER_VERSION_MAJOR}")
			endif()
		endif()
	endif(AVFILTER_FOUND)
endif(WITH_AVFILTER)

#########################
### Video output module
#########################

# Add DirectX
option(WITH_DIRECTX "Enable build with DirectX library" OFF)
if(WITH_DIRECTX)
	set(DIRECTX_EXTRA_INCLUDE_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/directx/include")
	find_package (DirectX COMPONENTS DirectDraw DirectWrite Direct2D Direct3D11)
	#find_package (DirectX COMPONENTS DirectDraw Direct2D Direct3D)
	if(DIRECTX_DIRECTDRAW_FOUND)
		set(USE_DIRECTX_DIRECTDRAW 1)
	endif()
	if (DIRECTX_DIRECTWRITE_FOUND)
		set(USE_DIRECTX_DIRECTWRITE 1)
	endif()
	if (DIRECTX_DIRECT2D_FOUND)
		set(USE_DIRECTX_DIRECT2D 1)
	endif()
	if (DIRECTX_DIRECT3D11_FOUND)
		set(USE_DIRECTX_DIRECT3D11 1)
	endif()
	if(DIRECTX_FOUND)
		set(USE_DIRECTX 1)
		include_directories(${DIRECTX_INCLUDE_DIRS})
	endif()
	if (WIN32 AND DIRECTX_DIRECT3D11_FOUND)
		# Dirty hack to copy d3dcompiler dll
		if(CUSTOM_DIRECTX_DLL_PATH)
			install(FILES ${CUSTOM_DIRECTX_DLL_PATH}/d3dcompiler_47.dll DESTINATION ${INSTALL_PATH_BIN})
		endif()
	endif()
endif(WITH_DIRECTX)

# Add XCB
option(WITH_XCB "Enable build with XCB library" OFF)
if(WITH_XCB)
	find_package (XCB)
	if(XCB_FOUND)
		set(USE_XCB 1)
	endif()
	if(XCB_VIDEO_FOUND)
		set(USE_XCB_XV 1)
		include_directories(${XCB_VIDEO_INCLUDE_DIR})
	endif()
endif(WITH_XCB)

##########################
### Video analyser modules
##########################

# Add OpenCV
option(WITH_OPENCV "Enable build with opencv library" OFF)
if(WITH_OPENCV)
	find_package(OpenCV QUIET COMPONENTS ml imgproc core OPTIONAL_COMPONENTS dnn)
	if(OpenCV_FOUND)
		message(STATUS "OpenCV ${OpenCV_VERSION_MAJOR}.${OpenCV_VERSION_MINOR} found")
		#message(STATUS "OpenCV components: ${OpenCV_LIB_COMPONENTS}")
		#if(OPENCV_CORE_FOUND)
		#	message(STATUS "OpenCV core module found")
		#else()
		#	message(STATUS "OpenCV code module not found" )
		#endif()
		#if(OPENCV_IMGPROC_FOUND)
		#	message(STATUS "OpenCV imgproc module found")
		#else()
		#	message(STATUS "OpenCV imgproc module not found" )
		#endif()
		#if(OPENCV_ML_FOUND)
		#	message(STATUS "OpenCV ml module found")
		#else()
		#	message(STATUS "OpenCV ml module not found" )
		#endif()
		if(OPENCV_DNN_FOUND)
			message(STATUS "OpenCV dnn module found")
			set(USE_OPENCV_DNN 1)
			if(${OpenCV_VERSION_MAJOR} GREATER 2)
				set(USE_OPENCV_OBJECT_RECOGNITION 1)
			endif()
		else()
			message(STATUS "OpenCV dnn module not found" )
		endif()

		include_directories(${OpenCV_INCLUDE_DIRS})
		set(USE_OPENCV 1)
		list(GET OpenCV_INCLUDE_DIRS 0 OpenCV_INCLUDE_DIR_0)
		if(WIN32)
			# Add libprotobuf since 3.3.1
			if(NOT ${OpenCV_VERSION} VERSION_LESS 3.3.1 AND NOT OpenCV_SHARED)
				set(OpenCV_LIBS ${OpenCV_LIBS} libprotobuf)
			endif()

			if(MSVC)
			# Custom build
			file(GLOB OPENCV_LIBRARIES_INSTALL
					${OpenCV_INCLUDE_DIR_0}/../x86/vc10/bin/opencv_core*.dll
					${OpenCV_INCLUDE_DIR_0}/../bin/opencv_core*.dll
					${OpenCV_INCLUDE_DIR_0}/../debug/bin/opencv_core*.dll
					${OpenCV_INCLUDE_DIR_0}/../x86/vc10/bin/opencv_imgproc*.dll
					${OpenCV_INCLUDE_DIR_0}/../bin/opencv_imgproc*.dll
					${OpenCV_INCLUDE_DIR_0}/../debug/bin/opencv_imgproc*.dll
					${OpenCV_INCLUDE_DIR_0}/../x86/vc10/bin/opencv_ml*.dll
					${OpenCV_INCLUDE_DIR_0}/../bin/opencv_ml*.dll
					${OpenCV_INCLUDE_DIR_0}/../debug/bin/opencv_ml*.dll
					if(OPENCV_DNN_FOUND)
						${OpenCV_INCLUDE_DIR_0}/../x64/vc10/bin/opencv_dnn*.dll
						${OpenCV_INCLUDE_DIR_0}/../bin/opencv_dnn*.dll
						${OpenCV_INCLUDE_DIR_0}/../debug/bin/opencv_dnn*.dll
					endif()
					#if(OPENCV_HIGHGUI_FOUND)
					#	${OpenCV_INCLUDE_DIR_0}/../x64/vc12/bin/opencv_highgui*.dll
					#endif()
				)
			else()
				file(GLOB OPENCV_LIBRARIES_INSTALL
					${OpenCV_INCLUDE_DIR_0}/../x86/mingw/bin/libopencv_core*.dll
					${OpenCV_INCLUDE_DIR_0}/../bin/libopencv_core*.dll
					${OpenCV_INCLUDE_DIR_0}/../x86/mingw/bin/libopencv_imgproc*.dll
					${OpenCV_INCLUDE_DIR_0}/../bin/libopencv_imgproc*.dll
					${OpenCV_INCLUDE_DIR_0}/../x86/mingw/bin/libopencv_ml*.dll
					${OpenCV_INCLUDE_DIR_0}/../bin/libopencv_ml*.dll
					if(OPENCV_DNN_FOUND)
						${OpenCV_INCLUDE_DIR_0}/../bin/libopencv_dnn*.dll
						${OpenCV_INCLUDE_DIR_0}/../x86/mingw/bin/libopencv_dnn*.dll
					endif()
					#if(OPENCV_HIGHGUI_FOUND)
					#	${OpenCV_INCLUDE_DIR_0}/../bin/libopencv_highgui*.dll
					#	${OpenCV_INCLUDE_DIR_0}/../x86/mingw/bin/libopencv_highgui*.dll
					#endif()
				)
			endif()
			install(FILES ${OPENCV_LIBRARIES_INSTALL} DESTINATION ${INSTALL_PATH_LIB})
		elseif(APPLE)
			file(GLOB OPENCV_LIBRARIES_INSTALL
				${OpenCV_INCLUDE_DIR_0}/../../lib/libopencv_core*.dylib
				${OpenCV_INCLUDE_DIR_0}/../../lib/libopencv_imgproc*.dylib
				${OpenCV_INCLUDE_DIR_0}/../../lib/libopencv_ml*.dylib
			)
			install(FILES ${OPENCV_LIBRARIES_INSTALL} DESTINATION ${INSTALL_PATH_LIB})
		else()
			if(OpenCV_SHARED)
				if(AUTONOMOUS_PACKAGE_BUILD)
					find_install_libs_so("${OpenCV_LIBS}" ${INSTALL_PATH_LIB} TRUE)
				else()
					list(APPEND PACKAGE_DEPS "libopencv-core${OpenCV_VERSION_MAJOR}.${OpenCV_VERSION_MINOR} (>= ${OpenCV_VERSION}) | libopencv-core${OpenCV_VERSION_MAJOR}.${OpenCV_VERSION_MINOR}v5")
					list(APPEND PACKAGE_DEPS "libopencv-imgproc${OpenCV_VERSION_MAJOR}.${OpenCV_VERSION_MINOR} (>= ${OpenCV_VERSION}) | libopencv-imgproc${OpenCV_VERSION_MAJOR}.${OpenCV_VERSION_MINOR}v5")
					list(APPEND PACKAGE_DEPS "libopencv-ml${OpenCV_VERSION_MAJOR}.${OpenCV_VERSION_MINOR} (>= ${OpenCV_VERSION}) | libopencv-ml${OpenCV_VERSION_MAJOR}.${OpenCV_VERSION_MINOR}v5")
				endif()
			endif()
			if(OPENCV_DNN_FOUND)
				# Mandatory for debian bullseye
				if(NOT AUTONOMOUS_PACKAGE_BUILD)
				    list(APPEND PACKAGE_DEPS "libopencv-dnn${OpenCV_VERSION_MAJOR}.${OpenCV_VERSION_MINOR} (>= ${OpenCV_VERSION})")
				endif()
			#	list(APPEND PACKAGE_DEPS "nvidia-cuda-toolkit (>= 9.1)")
			endif()
		endif()
	endif(OpenCV_FOUND)
endif(WITH_OPENCV)

# Add AlexeyABDarknet
option(WITH_ALEXEYABDARKNET "Enable build with AlexeyAB Darknet library" OFF)
if(WITH_ALEXEYABDARKNET)
	if(OpenCV_FOUND)
		find_package (AlexeyABDarknet)
		if(ALEXEYABDARKNET_FOUND)
			include_directories(${ALEXEYABDARKNET_INCLUDE_DIRS})
			if(WIN32)
				install(FILES ${ALEXEYABDARKNET_RUNTIME_LIBRARY_PATH}/yolo_cpp_dll.dll DESTINATION ${INSTALL_PATH_BIN})
			endif()
			if(UNIX)
				set(USE_ALEXEYABDARKNET 1)
			else()
				if(MSVC_VERSION GREATER 1700)
					set(USE_ALEXEYABDARKNET 1)
				else()
					message(FATAL "AlexeyAB Darknet need VC >= 12")
				endif()
			endif()
		endif(ALEXEYABDARKNET_FOUND)
	else()
		message(FATAL "AlexeyAB Darknet need OpenCV")
	endif()
endif()

#########################
### SystemInfos modules
#########################

# Add PsAPI
option(WITH_PSAPI "Enable build with psapi library" OFF)
if(WITH_PSAPI)
	if(WIN32)
		find_package (PSAPI REQUIRED)
	else(WIN32)
		set(PSAPI_LIBRARIES "")
	endif(WIN32)
endif(WITH_PSAPI)

# Add ShlwAPI
option(WITH_SHLWAPI "Enable build with shlwapi library" OFF)
if(WITH_SHLWAPI)
	if(WIN32)
		find_package (SHLWAPI REQUIRED)
	else(WIN32)
		set(SHLWAPI_LIBRARIES "")
	endif(WIN32)
endif(WITH_SHLWAPI)

# Add Winsock2
option(WITH_WINSOCK2 "Enable build with Winsock2 library" OFF)
if(WITH_WINSOCK2)
	if(WIN32)
		find_package (WINSOCK2 REQUIRED)
	else(WIN32)
		set(WINSOCK2_LIBRARIES "")
	endif(WIN32)
endif(WITH_WINSOCK2)

# Add Iphlpapi
option(WITH_IPHLPAPI "Enable build with Iphlpapi library" OFF)
if(WITH_IPHLPAPI)
	if(WIN32)
		find_package (IPHLPAPI REQUIRED)
	else(WIN32)
		set(IPHLPAPI_LIBRARIES "")
	endif(WIN32)
endif(WITH_IPHLPAPI)

# Add Iphlpapi
option(WITH_WTSAPI32 "Enable build with WtsApi32 library" OFF)
if(WITH_WTSAPI32)
	if(WIN32)
		find_package (WTSAPI32 REQUIRED)
	else(WIN32)
		set(WTSAPI32_LIBRARIES "")
	endif(WIN32)
endif(WITH_WTSAPI32)

# Add WBEM
option(WITH_WBEM "Enable build with Wbem library" OFF)
if(WITH_WBEM)
	if(WIN32)
		find_package (WBEM REQUIRED)
	else(WIN32)
		set(WBEM_LIBRARIES "")
	endif(WIN32)
endif(WITH_WBEM)

# Add ConsoleKit
option(WITH_CKCONNECTOR "Enable build with ConsoleKit library" OFF)
if(WITH_CKCONNECTOR)
	if(WIN32)
		set(CKCONNECTOR_LIBRARIES "")
	else(WIN32)
		find_package (CKConnector)
		if(CKCONNECTOR_FOUND)
			list(APPEND PACKAGE_DEPS "libck-connector0")
		endif(CKCONNECTOR_FOUND)
	endif(WIN32)
endif(WITH_CKCONNECTOR)

# Add Elogind

option(WITH_ELOGIND "Enable build with Elogind library" OFF)
if(WITH_ELOGIND)
	if(WIN32)
		set(ELOGIND_LIBRARIES "")
	else(WIN32)
		find_package (Elogind)
		if(ELOGIND_FOUND)
			list(APPEND PACKAGE_DEPS "libelogind0")
		endif(ELOGIND_FOUND)
	endif(WIN32)
endif(WITH_ELOGIND)

#########################
### Device modules
#########################

# Add Winmm
option(WITH_WINMM "Enable build with winmm library" OFF)
if(WITH_WINMM)
	if(WIN32)
		link_libraries (winmm)
	endif(WIN32)
endif(WITH_WINMM)

# Add CoreFoundation
option(WITH_CoreFoundation "Enable build with CoreFoundation library" OFF)
if(WITH_CoreFoundation)
	if(APPLE)
		find_package(CoreFoundation REQUIRED)
		if(CoreFoundation_FOUND)
			include_directories(CoreFoundation_INCLUDE_DIR)
		endif(CoreFoundation_FOUND)
	endif(APPLE)
endif(WITH_CoreFoundation)

# Add IOKit
option(WITH_IOKit "Enable build with IOKit library" OFF)
if(WITH_IOKit)
	if(APPLE)
		find_package(IOKit REQUIRED)
		if(IOKit_FOUND)
			include_directories(IOKit_INCLUDE_DIR)
		endif(IOKit_FOUND)
	endif(APPLE)
endif(WITH_IOKit)

#########################
### GUI modules
#########################

# Add QT
option(WITH_QT "Enable build with qt library" OFF)
if(WITH_QT)
	include (ProjectIncludeLibrariesQt)

	# Add dependencies libraries
	if (UNIX AND NOT APPLE)
		if(AUTONOMOUS_PACKAGE_BUILD)
			find_install_lib_so("icui18n" ${INSTALL_PATH_LIB} FALSE)
			find_install_lib_so("icuuc" ${INSTALL_PATH_LIB} FALSE)
			find_install_lib_so("icudata" ${INSTALL_PATH_LIB} FALSE)
			find_install_lib_so("double-conversion" ${INSTALL_PATH_LIB} FALSE)
			list(APPEND PACKAGE_DEPS "libpcre2-16-0")
		endif()
	endif()

	# Add dependencies libraries
	set(HAS_QTGUI ((NOT Qt${QT_VERSION_MAJOR}_FOUND AND NOT QT_DONT_USE_QTGUI) OR Qt${QT_VERSION_MAJOR}Gui_FOUND))
	if(HAS_QTGUI)
		if(WIN32)
			if(QT_CUSTOM_PATH AND QT_USE_VERSION EQUAL 5)
				install(FILES ${QT_CUSTOM_PATH}/bin/libEGL.dll DESTINATION ${INSTALL_PATH_BIN} CONFIGURATIONS Release)
				install(FILES ${QT_CUSTOM_PATH}/bin/libGLESv2.dll DESTINATION ${INSTALL_PATH_BIN} CONFIGURATIONS Release)
				install(FILES ${QT_CUSTOM_PATH}/bin/libEGLd.dll DESTINATION ${INSTALL_PATH_BIN} CONFIGURATIONS Debug)
				install(FILES ${QT_CUSTOM_PATH}/bin/libGLESv2d.dll DESTINATION ${INSTALL_PATH_BIN} CONFIGURATIONS Debug)
			endif()
		endif()
		if(AUTONOMOUS_PACKAGE_BUILD)
			list(APPEND PACKAGE_DEPS "libx11-6")
			list(APPEND PACKAGE_DEPS "libxcb-xinput0")
			list(APPEND PACKAGE_DEPS "libxcb-icccm4")
			list(APPEND PACKAGE_DEPS "libxcb-image0")
			list(APPEND PACKAGE_DEPS "libxcb-keysyms1")
			list(APPEND PACKAGE_DEPS "libxcb-randr0")
			list(APPEND PACKAGE_DEPS "libxcb-render-util0")
			list(APPEND PACKAGE_DEPS "libxcb-xinerama0")
			list(APPEND PACKAGE_DEPS "libxcb-xkb1")
			list(APPEND PACKAGE_DEPS "libxkbcommon-x11-0")
			list(APPEND PACKAGE_DEPS "libgl1")
			list(APPEND PACKAGE_DEPS "libvdpau1")
			list(APPEND PACKAGE_DEPS "libharfbuzz0b")
			# Qt modules without cmake files
			find_install_lib_so("Qt${QT_VERSION_MAJOR}XcbQpa" ${INSTALL_PATH_LIB} FALSE)
			find_install_lib_so("jpeg" ${INSTALL_PATH_LIB} FALSE)
		endif()
	endif()

endif()

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

# Add QxT library
option(WITH_QXT "Enable build with qxt library" OFF)
if(WITH_QXT)
	set(USE_QXT 1)
	find_package (Qxt COMPONENTS QxtWeb)
	if(USE_QXT)
		if(WIN32 AND FALSE)
			# Release install
			INSTALL(FILES
				"${QXT_BIN_DIR}/QxtCore.dll"
				"${QXT_BIN_DIR}/QxtNetwork.dll"
				"${QXT_BIN_DIR}/QxtWeb.dll"
				DESTINATION ${INSTALL_PATH_BIN}
				CONFIGURATIONS Release
			)
			# Debug install
			INSTALL(FILES
				"${QXT_BIN_DIR}/QxtCored.dll"
				"${QXT_BIN_DIR}/QxtNetworkd.dll"
				"${QXT_BIN_DIR}/QxtWebd.dll"
				DESTINATION ${INSTALL_PATH_BIN}
				CONFIGURATIONS Debug
			)
		else(WIN32 AND FALSE)
			list(APPEND PACKAGE_DEPS "libqxt-web0")
		endif(WIN32 AND FALSE)
	endif(USE_QXT)
endif()

# zlib
if(WITH_CURL OR WITH_AVCODEC OR WITH_AVFILTER)
	if(WIN32)
		find_install_dll("zlib" ${INSTALL_PATH_BIN} FALSE)
		find_install_dll("z" ${INSTALL_PATH_BIN} FALSE)
	elseif(APPLE)
		# Todo
	else()
		if(AUTONOMOUS_PACKAGE_BUILD)
			if(WITH_ZLIB_SYSTEM)
				list(APPEND PACKAGE_DEPS "zlib1g")
			else()
				find_install_lib_so("z" ${INSTALL_PATH_LIB} FALSE)
			endif()
		endif()
	endif()
endif()

# freetype
if(WITH_AVCODEC OR WITH_AVFILTER OR (WITH_QT AND HAS_QTGUI))
	if(WIN32)
		find_install_dll("freetype" ${INSTALL_PATH_BIN} FALSE)
		find_install_dll("libfreetype" ${INSTALL_PATH_BIN} FALSE)
	elseif(APPLE)
		# Todo
	else()
		if(AUTONOMOUS_PACKAGE_BUILD)
			if(WITH_FREETYPE_SYSTEM)
				list(APPEND PACKAGE_DEPS "libfreetype6")
			else()
				find_install_lib_so("freetype" ${INSTALL_PATH_LIB} FALSE)
			endif()
		endif()
	endif()
endif()

# fontconfig
if(WITH_QT AND HAS_QTGUI)
	if(AUTONOMOUS_PACKAGE_BUILD)
		if(WITH_FONTCONFIG_SYSTEM)
			list(APPEND PACKAGE_DEPS "fontconfig")
		else()
			find_package(Fontconfig REQUIRED)
			if(Fontconfig_FOUND)
				get_filename_component(FONTCONFIG_ROOT ${Fontconfig_INCLUDE_DIRS}/.. ABSOLUTE)
				if(EXISTS ${FONTCONFIG_ROOT}/etc)
					set(FONTCONFIG_CONFIG_PATH ${FONTCONFIG_ROOT}/etc)
				else()
					set(FONTCONFIG_CONFIG_PATH /etc)
				endif()
				install_libs_so(${Fontconfig_LIBRARIES} ${INSTALL_PATH_LIB} FALSE)
				list(APPEND PACKAGE_DEPS "libexpat1")
        
				install(FILES ${FONTCONFIG_CONFIG_PATH}/fonts/fonts.conf DESTINATION ${INSTALL_PATH_ETC}/fontconfig/)
				install(DIRECTORY ${FONTCONFIG_ROOT}/share/fontconfig/conf.avail/ DESTINATION ${INSTALL_PATH_ETC}/fontconfig/conf.d)
			endif()
		endif()
	endif()
endif()

# libpng
if(WITH_AVCODEC OR WITH_AVFILTER OR (WITH_QT AND HAS_QTGUI))
	if(WIN32)
		if(MSVC)
			find_install_dll("libpng" ${INSTALL_PATH_BIN} FALSE)
		else()
			find_install_dll("png" ${INSTALL_PATH_BIN} FALSE)
		endif()
	elseif(APPLE)
		# Todo
	else()
		if(AUTONOMOUS_PACKAGE_BUILD)
			if(WITH_LIBPNG_SYSTEM)
				list(APPEND PACKAGE_DEPS "libpng16-16")
			else()
				find_install_lib_so("png" ${INSTALL_PATH_LIB} FALSE)
			endif()
		endif()
	endif()
endif()

if(WITH_ALSA)
	find_package(ALSA)
	if(ALSA_FOUND)
		include_directories (${ALSA_INCLUDE_DIRS})
		list(APPEND PACKAGE_DEPS "libasound2")
	endif()
endif()

# Add OpenGL library
option(WITH_OPENGL "Enable build with OpenGL library" OFF)
if(WITH_OPENGL)
	set(OpenGL_GL_PREFERENCE LEGACY)
	find_package(OpenGL REQUIRED)

	if(OpenGL_FOUND)
		set(USE_OPENGL 1)
	# Sometimes, OpenGL_FOUND is undefined but should be TRUE...
	elseif(NOT ${OPENGL_gl_LIBRARY} STREQUAL "")
		set(USE_OPENGL 1)
	endif()
endif(WITH_OPENGL)

# Add GTKmm library
option(WITH_GTKMM "Enable build with gtkmm library" OFF)
if(WITH_GTKMM)
	find_package (GTKmm 3.0)
	if(GTKmm_FOUND)
		set(USE_GTKMM 1)
	endif(GTKmm_FOUND)
endif(WITH_GTKMM)

#####################
### Hardwares modules
#####################

# Add VDPAU (linux only) - Codec/filter/GUI
option(WITH_VDPAU "Enable build with VDPAU API" OFF)
if(WITH_VDPAU AND UNIX)
	find_package (VDPAU)
	find_package (X11)
	if(VDPAU_FOUND AND X11_FOUND)
		set(USE_VDPAU 1)
		include_directories (${VDPAU_INCLUDE_DIRS} ${X11_INCLUDE_DIR})
		list(APPEND PACKAGE_DEPS "libvdpau${VDPAU_VERSION_MAJOR}, libx11-${X11_VERSION_MAJOR}")
	endif(VDPAU_FOUND AND X11_FOUND)
endif(WITH_VDPAU AND UNIX)
