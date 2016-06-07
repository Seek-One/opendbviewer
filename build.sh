#/bin/sh

# Tutorial for Eclipse is here : http://www.cmake.org/Wiki/Eclipse_CDT4_Generator
# Import the created project file into Eclipse
# - Import project using Menu File->Import
# - Select General->Existing projects into workspace:
# - Select the build directory. Keep "Copy projects into workspace" unchecked.
# -DECLIPSE_CDT4_GENERATE_SOURCE_PROJECT=TRUE if using separate src and build dir

# Define constants
IDE_NONE=0
IDE_ECLIPSE=1
MAKE_CMD=make

# Build options
USE_IDE=${IDE_ECLIPSE}
SRC_PATH=`pwd`
BIN_PATH=.

# Perform actions
if [ ! -d ${BIN_PATH} ]; then
	echo ++ Creating build directory ${BIN_PATH}
	mkdir --parents ${BIN_PATH}
fi

SRC_ABS_PATH=`cd ${SRC_PATH}; pwd`
BIN_ABS_PATH=`cd ${BIN_PATH}; pwd`

if [ "${SRC_ABS_PATH}" = "${BIN_ABS_PATH}" ]; then
	echo ++ Using in sources build: ${BIN_ABS_PATH}
	IN_SOURCES_BUILD=1
else
	echo ++ Using out of sources build: ${BIN_ABS_PATH}
	IN_SOURCES_BUILD=0
fi

# By default we just do a build
DO_CLEAN=0
DO_REGEN=0
DO_BUILD=1

if [ "$1" = "regen" ]; then
	DO_CLEAN=1
	DO_REGEN=1
	DO_BUILD=0
elif [ "$1" = "clean" ]; then
	DO_CLEAN=1
	DO_BUILD=0
fi

# Display operation
echo ++ Clean=${DO_CLEAN} Regen=${DO_REGEN} Build=${DO_BUILD}

# Destroy old directory
if [ ${DO_CLEAN} -eq 1 ]; then
	if [ ${IN_SOURCES_BUILD} -eq 0 ]; then
		if [ -d ${BIN_ABS_PATH} ]; then
			echo ++ Clean: Removing old directory ${BIN_ABS_PATH}
			rm -rf ${BIN_ABS_PATH}
		fi
	else
		(
			cd ${BIN_ABS_PATH}
			if [ -f Makefile ]; then
				echo "++ Clean: Do make clean"
				${MAKE_CMD} clean
			fi
			echo "++ Clean: Deleting all Makefile"
			find . -name "Makefile" -print0 | xargs -0 rm -rf
			echo "++ Clean: Deleting all CMakeFiles"
			find . -name "CMakeFiles" -print0 | xargs -0 rm -rf
			echo "++ Clean: Deleting all cmake_install.cmake"
			find . -name "cmake_install.cmake" -print0 | xargs -0 rm -rf
			echo "++ Clean: Deleting CMakeCache.txt"
			rm -f CMakeCache.txt
		)
	fi
fi

# Build 
if [ ${DO_REGEN} -eq 1 ]; then
	if [ ! -d ${BIN_ABS_PATH} ]; then
		echo ++ Creating build directory
		mkdir --parents ${BIN_ABS_PATH}
	fi

	(
		cd ${BIN_ABS_PATH}
		if [ ${DO_REGEN} -eq 1 ]; then
			if [ ${USE_IDE} -eq ${IDE_ECLIPSE} ]; then
				cmake -G"Eclipse CDT4 - Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_ECLIPSE_GENERATE_LINKED_RESOURCES=FALSE ${SRC_ABS_PATH}
			else
				cmake -G"MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug ${SRC_ABS_PATH}
			fi
		fi
	)
fi

if [ ${DO_BUILD} -eq 1 ]; then
	(
		cd ${BIN_ABS_PATH}
		echo ++ Building the project
		${MAKE_CMD}
	)
fi
