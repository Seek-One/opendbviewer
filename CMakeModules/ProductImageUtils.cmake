
# Look for image magick needed
find_program(IMAGEMAGICK_CONVERT NAMES magick convert PATHS "C:/Program Files/ImageMagick*")
if(NOT IMAGEMAGICK_CONVERT)
	message(FATAL_ERROR "Product: ImageMagick convert program is not found")
else()
	message(STATUS "Product: ImageMagick convert program is found: ${IMAGEMAGICK_CONVERT}")
endif()

#if(APPLE)
#	find_program(PNG2ICNS png2icns)
#	if(NOT PNG2ICNS)
#		message(FATAL_ERROR "png2icns program is not found")
#	else()
#		message(STATUS "Product: png2icns program is found: ${PNG2ICNS}")
#	endif()
#endif()

if(APPLE)
	find_program(SIPS sips)
	if(NOT SIPS)
		message(FATAL_ERROR "Product: sips program is not found")
	else()
		message(STATUS "Product: sips program is found: ${SIPS}")
	endif()
endif()

###########################
# Create product logo about
###########################
macro (product_image_make_logo_about SRC_ICON_PATH DST_PATH)
	message(STATUS "Product: creating logo about file from ${SRC_ICON_PATH}")
	# For application resources
	file(COPY ${SRC_ICON_PATH} DESTINATION ${DST_PATH})
endmacro (product_image_make_logo_about)

#########################
# Create application logo
#########################
macro (product_image_make_logo PRODUCT_NAME SRC_PNG_ICON_PATH DST_PATH)
	message(STATUS "Product: creating logo file from ${SRC_PNG_ICON_PATH}")
	# For application resources
	configure_file(${SRC_PNG_ICON_PATH} ${DST_PATH}/${PRODUCT_NAME}.png COPYONLY)
	# Create icon for Windows
	if(WIN32)
		execute_process(COMMAND ${IMAGEMAGICK_CONVERT} ${SRC_PNG_ICON_PATH} -resize 128x128 ${DST_PATH}/${PRODUCT_NAME}.ico)
	endif()
	# Create icon for Linux
	if(UNIX)
		if(APPLE)
			# Create icon for Mac
			#execute_process(COMMAND ${PNG2ICNS} ${DST_PATH}/${PRODUCT_NAME}.icns ${SRC_PNG_ICON_PATH})
			execute_process(COMMAND ${SIPS} -z 512 512 -s format icns ${SRC_PNG_ICON_PATH} --out ${DST_PATH}/${PRODUCT_NAME}.icns)
		else()
			execute_process(COMMAND ${IMAGEMAGICK_CONVERT} -background none ${SRC_PNG_ICON_PATH} -resize 16x16 ${DST_PATH}/hicolor_16x16_apps_${PRODUCT_NAME}.png)
			execute_process(COMMAND ${IMAGEMAGICK_CONVERT} -background none ${SRC_PNG_ICON_PATH} -resize 22x22 ${DST_PATH}/hicolor_22x22_apps_${PRODUCT_NAME}.png)
			execute_process(COMMAND ${IMAGEMAGICK_CONVERT} -background none ${SRC_PNG_ICON_PATH} -resize 24x24 ${DST_PATH}/hicolor_24x24_apps_${PRODUCT_NAME}.png)
			execute_process(COMMAND ${IMAGEMAGICK_CONVERT} -background none ${SRC_PNG_ICON_PATH} -resize 32x32 ${DST_PATH}/hicolor_32x32_apps_${PRODUCT_NAME}.png)
			execute_process(COMMAND ${IMAGEMAGICK_CONVERT} -background none ${SRC_PNG_ICON_PATH} -resize 48x48 ${DST_PATH}/hicolor_48x48_apps_${PRODUCT_NAME}.png)
			execute_process(COMMAND ${IMAGEMAGICK_CONVERT} -background none ${SRC_PNG_ICON_PATH} -resize 64x64 ${DST_PATH}/hicolor_64x64_apps_${PRODUCT_NAME}.png)
			execute_process(COMMAND ${IMAGEMAGICK_CONVERT} -background none ${SRC_PNG_ICON_PATH} -resize 128x128 ${DST_PATH}/hicolor_128x128_apps_${PRODUCT_NAME}.png)
		endif()
	endif()
	# Create favicon
	execute_process(COMMAND ${IMAGEMAGICK_CONVERT} ${SRC_PNG_ICON_PATH} -resize 32x32 ${DST_PATH}/${PRODUCT_NAME}-favicon.ico)
endmacro (product_image_make_logo)

#############################
# Create application logo web
#############################
macro (product_image_make_logo_web PRODUCT_NAME SRC_PNG_ICON_PATH DST_PATH)
	message(STATUS "Product: creating logo web file from ${SRC_PNG_ICON_PATH}")
	# For application resources
	configure_file(${SRC_PNG_ICON_PATH} ${DST_PATH}/${PRODUCT_NAME}-web.png COPYONLY)
	# Create icon for Windows and all plaforms
	execute_process(COMMAND ${IMAGEMAGICK_CONVERT} ${SRC_PNG_ICON_PATH} -resize 128x128 ${DST_PATH}/${PRODUCT_NAME}-web.ico)
	execute_process(COMMAND ${IMAGEMAGICK_CONVERT} ${SRC_PNG_ICON_PATH} -resize 32x32 ${DST_PATH}/${PRODUCT_NAME}-web-favicon.ico)
	# Create icon for Linux
	if(UNIX)
		execute_process(COMMAND ${IMAGEMAGICK_CONVERT} -background none ${SRC_PNG_ICON_PATH} -resize 16x16 ${DST_PATH}/hicolor_16x16_apps_${PRODUCT_NAME}-web.png)
		execute_process(COMMAND ${IMAGEMAGICK_CONVERT} -background none ${SRC_PNG_ICON_PATH} -resize 22x22 ${DST_PATH}/hicolor_22x22_apps_${PRODUCT_NAME}-web.png)
		execute_process(COMMAND ${IMAGEMAGICK_CONVERT} -background none ${SRC_PNG_ICON_PATH} -resize 24x24 ${DST_PATH}/hicolor_24x24_apps_${PRODUCT_NAME}-web.png)
		execute_process(COMMAND ${IMAGEMAGICK_CONVERT} -background none ${SRC_PNG_ICON_PATH} -resize 32x32 ${DST_PATH}/hicolor_32x32_apps_${PRODUCT_NAME}-web.png)
		execute_process(COMMAND ${IMAGEMAGICK_CONVERT} -background none ${SRC_PNG_ICON_PATH} -resize 48x48 ${DST_PATH}/hicolor_48x48_apps_${PRODUCT_NAME}-web.png)
		execute_process(COMMAND ${IMAGEMAGICK_CONVERT} -background none ${SRC_PNG_ICON_PATH} -resize 64x64 ${DST_PATH}/hicolor_64x64_apps_${PRODUCT_NAME}-web.png)
		execute_process(COMMAND ${IMAGEMAGICK_CONVERT} -background none ${SRC_PNG_ICON_PATH} -resize 128x128 ${DST_PATH}/hicolor_128x128_apps_${PRODUCT_NAME}-web.png)
	endif()
	# Create icon for Mac
	if(APPLE)
		#execute_process(COMMAND ${PNG2ICNS} ${DST_PATH}/${PRODUCT_NAME}.icns ${SRC_PNG_ICON_PATH})
		execute_process(COMMAND ${SIPS} -s format icns ${SRC_PNG_ICON_PATH} --out ${DST_PATH}/${PRODUCT_NAME}-web.icns)
	endif()
endmacro (product_image_make_logo_web)

################################
# Create application logo viewer
################################
macro (product_image_make_logo_viewer PRODUCT_NAME SRC_PNG_ICON_PATH DST_PATH)
	message(STATUS "Product: creating logo viewer file from ${SRC_PNG_ICON_PATH}")
	# For application resources
	configure_file(${SRC_PNG_ICON_PATH} ${DST_PATH}/${PRODUCT_NAME}-viewer.png COPYONLY)
	execute_process(COMMAND ${IMAGEMAGICK_CONVERT} -background none ${SRC_PNG_ICON_PATH} -resize 16x16 ${DST_PATH}/${PRODUCT_NAME}-viewer.png)
endmacro (product_image_make_logo_viewer)

#############################
# Create application logo ios
#############################
macro (product_image_make_logo_ios PRODUCT_NAME SRC_PNG_ICON_PATH DST_PATH)
	message(STATUS "Product: creating logo iOS file from ${SRC_PNG_ICON_PATH}")
	# For application resources
	configure_file(${SRC_PNG_ICON_PATH} ${DST_PATH}/${PRODUCT_NAME}-ios.png COPYONLY)
	execute_process(COMMAND ${IMAGEMAGICK_CONVERT} -background none ${SRC_PNG_ICON_PATH} -resize 16x16 ${DST_PATH}/${PRODUCT_NAME}-ios.png)
endmacro (product_image_make_logo_ios)

#################################
# Create application logo android
#################################
macro (product_image_make_logo_android PRODUCT_NAME SRC_PNG_ICON_PATH DST_PATH)
	message(STATUS "Product: creating logo android file from ${SRC_PNG_ICON_PATH}")
	# For application resources
	configure_file(${SRC_PNG_ICON_PATH} ${DST_PATH}/${PRODUCT_NAME}-android.png COPYONLY)
	execute_process(COMMAND ${IMAGEMAGICK_CONVERT} -background none ${SRC_PNG_ICON_PATH} -resize 16x16 ${DST_PATH}/${PRODUCT_NAME}-android.png)
endmacro (product_image_make_logo_android)


########################
# Create customized icon
########################
macro (product_image_make_customized_icon SRC_PATH DST_PATH)
	message(STATUS "Product: creating customized icon")

	file(GLOB SVGIN_FILES RELATIVE ${SRC_PATH} ${SRC_PATH}/*.svg.in)

	set(ICON_SIZES 16 22 24 32 48 64 128)
	set(ICON_SPECIAL_SIZES 112x70:64 144x90:64 192x120:64)

	foreach(ICON_IN ${SVGIN_FILES})
		get_filename_component(ICON ${ICON_IN} NAME_WE)

		string(REGEX REPLACE "([a-zA-Z0-9]+)_(.*)" "\\1" ICON_CATEGORY ${ICON})
		string(REGEX REPLACE "([a-zA-Z0-9]+)_(.*)" "\\2" ICON_NAME ${ICON})

		message(STATUS "Product: customizing svg icon ${ICON_NAME}")

		set(SVG_IN_ICON_PATH ${SRC_PATH}/${ICON_IN})
		set(SVG_ICON_PATH ${DST_PATH}/${ICON}.svg)

		configure_file(${SVG_IN_ICON_PATH} ${SVG_ICON_PATH})

		# Create icon for all size
		foreach(ICON_SIZE ${ICON_SIZES})
			set(DST_ICON_PATH ${DST_PATH}/theme/${ICON_CATEGORY}/${ICON_SIZE})
			file(MAKE_DIRECTORY ${DST_ICON_PATH})
			execute_process(COMMAND ${IMAGEMAGICK_CONVERT} -background none ${SVG_ICON_PATH} -resize ${ICON_SIZE}x${ICON_SIZE} ${DST_ICON_PATH}/${ICON_NAME}.png)
		endforeach(ICON_SIZE)

		# Create special sizes derived from 184px icon
		foreach(ICON_SPECIAL_SIZE ${ICON_SPECIAL_SIZES})

			string(REGEX REPLACE "([0-9]+x[0-9]+):([0-9]+)" "\\1" SIZE_DIR ${ICON_SPECIAL_SIZE})
			string(REGEX REPLACE "([0-9]+)x([0-9]+)" "\\1" SIZE_WIDTH ${SIZE_DIR})
			string(REGEX REPLACE "([0-9]+)x([0-9]+)" "\\2" SIZE_HEIGHT ${SIZE_DIR})
			string(REGEX REPLACE "([0-9]+x[0-9]+):([0-9]+)" "\\2" ICON_SIZE ${ICON_SPECIAL_SIZE})

			# message(STATUS "Product: customizing svg icon ${ICON_NAME} to ${SIZE_WIDTH}x${SIZE_HEIGHT}, ${ICON_SIZE}")

			set(DST_ICON_PATH ${DST_PATH}/theme/${ICON_CATEGORY}/${SIZE_DIR})
			file(MAKE_DIRECTORY ${DST_ICON_PATH})
			execute_process(COMMAND ${IMAGEMAGICK_CONVERT} -background none ${SVG_ICON_PATH} -resize ${ICON_SIZE}x${ICON_SIZE} -gravity center -extent ${SIZE_WIDTH}x${SIZE_HEIGHT} -flatten ${DST_ICON_PATH}/${ICON_NAME}.png)
		endforeach(ICON_SPECIAL_SIZE)

		# TODO MP: clean YYxYY icons
		# create svg directory and copy the svg file into
		set(SVG_DEST_DIR "${DST_PATH}/theme/${ICON_CATEGORY}/svg")
		file(MAKE_DIRECTORY ${SVG_DEST_DIR})
		file(COPY ${SVG_ICON_PATH} DESTINATION ${SVG_DEST_DIR})
		file(RENAME "${SVG_DEST_DIR}/${ICON_CATEGORY}_${ICON_NAME}.svg" "${SVG_DEST_DIR}/${ICON_NAME}.svg")

	endforeach(ICON_IN)
endmacro(product_image_make_customized_icon)

###########################
# Create androidcv logo about
###########################
macro (product_image_make_androidcv_logo_about SRC_ICON_PATH DST_PATH)
	message(STATUS "Product: creating android logo about file from ${SRC_ICON_PATH}")
	configure_file(${SRC_ICON_PATH} ${DST_PATH}/logo_about.jpg COPYONLY)
endmacro (product_image_make_androidcv_logo_about)

#######################
# Create androidcv logo
#######################
macro (product_image_make_androidcv_logo PRODUCT_NAME SRC_PNG_ICON_PATH DST_PATH)
	message(STATUS "Product: creating android logo file from ${SRC_PNG_ICON_PATH}")
	execute_process(COMMAND ${IMAGEMAGICK_CONVERT} ${SRC_PNG_ICON_PATH} -resize 36x36 ${DST_PATH}/drawable-ldpi/icon.png)
	execute_process(COMMAND ${IMAGEMAGICK_CONVERT} ${SRC_PNG_ICON_PATH} -resize 48x48 ${DST_PATH}/drawable-mdpi/icon.png)
	execute_process(COMMAND ${IMAGEMAGICK_CONVERT} ${SRC_PNG_ICON_PATH} -resize 72x72 ${DST_PATH}/drawable-hdpi/icon.png)
	execute_process(COMMAND ${IMAGEMAGICK_CONVERT} ${SRC_PNG_ICON_PATH} -resize 96x96 ${DST_PATH}/drawable-xhdpi/icon.png)
	execute_process(COMMAND ${IMAGEMAGICK_CONVERT} ${SRC_PNG_ICON_PATH} -resize 144x144 ${DST_PATH}/drawable-xxhdpi/icon.png)
endmacro (product_image_make_androidcv_logo)

###################
# Create ioscv logo
###################
macro (product_image_make_ioscv_logo PRODUCT_NAME SRC_PNG_ICON_PATH DST_PATH)
	message(STATUS "Product: creating ios logo file from ${SRC_PNG_ICON_PATH}")
	execute_process(COMMAND ${IMAGEMAGICK_CONVERT} ${SRC_PNG_ICON_PATH} -resize 29x29 -background white -alpha remove -alpha off ${DST_PATH}/appicon-29.png)
	execute_process(COMMAND ${IMAGEMAGICK_CONVERT} ${SRC_PNG_ICON_PATH} -resize 58x58 -background white -alpha remove -alpha off ${DST_PATH}/appicon-29@2x.png)
	execute_process(COMMAND ${IMAGEMAGICK_CONVERT} ${SRC_PNG_ICON_PATH} -resize 87x87 -background white -alpha remove -alpha off ${DST_PATH}/appicon-29@3x.png)
	execute_process(COMMAND ${IMAGEMAGICK_CONVERT} ${SRC_PNG_ICON_PATH} -resize 40x40 -background white -alpha remove -alpha off ${DST_PATH}/appicon-40.png)
	execute_process(COMMAND ${IMAGEMAGICK_CONVERT} ${SRC_PNG_ICON_PATH} -resize 80x80 -background white -alpha remove -alpha off ${DST_PATH}/appicon-40@2x.png)
	execute_process(COMMAND ${IMAGEMAGICK_CONVERT} ${SRC_PNG_ICON_PATH} -resize 120x120 -background white -alpha remove -alpha off ${DST_PATH}/appicon-40@3x.png)
	execute_process(COMMAND ${IMAGEMAGICK_CONVERT} ${SRC_PNG_ICON_PATH} -resize 50x50 -background white -alpha remove -alpha off ${DST_PATH}/appicon-50.png)
	execute_process(COMMAND ${IMAGEMAGICK_CONVERT} ${SRC_PNG_ICON_PATH} -resize 100x100 -background white -alpha remove -alpha off ${DST_PATH}/appicon-50@2x.png)
	execute_process(COMMAND ${IMAGEMAGICK_CONVERT} ${SRC_PNG_ICON_PATH} -resize 57x57 -background white -alpha remove -alpha off ${DST_PATH}/appicon-57.png)
	execute_process(COMMAND ${IMAGEMAGICK_CONVERT} ${SRC_PNG_ICON_PATH} -resize 114x114 -background white -alpha remove -alpha off ${DST_PATH}/appicon-57@2x.png)
	execute_process(COMMAND ${IMAGEMAGICK_CONVERT} ${SRC_PNG_ICON_PATH} -resize 120x120 -background white -alpha remove -alpha off ${DST_PATH}/appicon-60@2x.png)
	execute_process(COMMAND ${IMAGEMAGICK_CONVERT} ${SRC_PNG_ICON_PATH} -resize 180x180 -background white -alpha remove -alpha off ${DST_PATH}/appicon-60@3x.png)
	execute_process(COMMAND ${IMAGEMAGICK_CONVERT} ${SRC_PNG_ICON_PATH} -resize 72x72 -background white -alpha remove -alpha off ${DST_PATH}/appicon-72.png)
	execute_process(COMMAND ${IMAGEMAGICK_CONVERT} ${SRC_PNG_ICON_PATH} -resize 144x144 -background white -alpha remove -alpha off ${DST_PATH}/appicon-72@2x.png)
	execute_process(COMMAND ${IMAGEMAGICK_CONVERT} ${SRC_PNG_ICON_PATH} -resize 76x76 -background white -alpha remove -alpha off ${DST_PATH}/appicon-76.png)
	execute_process(COMMAND ${IMAGEMAGICK_CONVERT} ${SRC_PNG_ICON_PATH} -resize 152x152 -background white -alpha remove -alpha off ${DST_PATH}/appicon-76@2x.png)
	execute_process(COMMAND ${IMAGEMAGICK_CONVERT} ${SRC_PNG_ICON_PATH} -resize 167x167 -background white -alpha remove -alpha off ${DST_PATH}/appicon-83.5@2x.png)
	execute_process(COMMAND ${IMAGEMAGICK_CONVERT} ${SRC_PNG_ICON_PATH} -resize 1024x1024 -background white -alpha remove -alpha off ${DST_PATH}/appicon-1024.png)
endmacro (product_image_make_ioscv_logo)

############################
# Create ioscv logo about
###########################
macro (product_image_make_ioscv_logo_about SRC_ICON_PATH DST_PATH)
	message(STATUS "Product: creating ios logo about file from ${SRC_ICON_PATH}")
	configure_file(${SRC_ICON_PATH} ${DST_PATH}/logo_about.jpg COPYONLY)
endmacro (product_image_make_ioscv_logo_about)

##########################
# Create ioscv launchimage
##########################
macro (product_image_make_ioscv_launchimage PRODUCT_NAME SRC_PNG_ICON_PATH DST_PATH)
	message(STATUS "Product: creating ios launchimage file from ${SRC_PNG_ICON_PATH}")
	execute_process(COMMAND ${IMAGEMAGICK_CONVERT} ${SRC_PNG_ICON_PATH} -geometry 640x -background white -gravity center -extent 640x1136 -flatten ${DST_PATH}/launchimage-default@2x.png)
	execute_process(COMMAND ${IMAGEMAGICK_CONVERT} ${SRC_PNG_ICON_PATH} -geometry 320x -background white -gravity center -extent 320x480 -flatten ${DST_PATH}/launchimage-portrait-320x480pt.png)
	execute_process(COMMAND ${IMAGEMAGICK_CONVERT} ${SRC_PNG_ICON_PATH} -geometry 640x -background white -gravity center -extent 640x960 -flatten ${DST_PATH}/launchimage-portrait-640x960pt.png)
	execute_process(COMMAND ${IMAGEMAGICK_CONVERT} ${SRC_PNG_ICON_PATH} -geometry 768x -background white -gravity center -extent 768x1004 -flatten ${DST_PATH}/launchimage-portrait-768x1004pt.png)
	execute_process(COMMAND ${IMAGEMAGICK_CONVERT} ${SRC_PNG_ICON_PATH} -geometry 768x -background white -gravity center -extent 768x1024 -flatten ${DST_PATH}/launchimage-portrait-768x1024pt.png)
	execute_process(COMMAND ${IMAGEMAGICK_CONVERT} ${SRC_PNG_ICON_PATH} -geometry 1536x -background white -gravity center -extent 1536x2048 -flatten ${DST_PATH}/launchimage-portrait-1536x2048pt.png)
	execute_process(COMMAND ${IMAGEMAGICK_CONVERT} ${SRC_PNG_ICON_PATH} -geometry 750x -background white -gravity center -extent 750x1334 -flatten ${DST_PATH}/launchimage-portrait-iphone-ios47-750x1334pt.png)
	execute_process(COMMAND ${IMAGEMAGICK_CONVERT} ${SRC_PNG_ICON_PATH} -geometry 1242x -background white -gravity center -extent 1242x2208 -flatten ${DST_PATH}/launchimage-portrait-iphone-ios56-736h@3x.png)
endmacro (product_image_make_ioscv_launchimage)


########################
# Create ioscv customized icons
########################
macro (product_image_make_ioscv_customized_icon ICON_NAME SRC_PATH DST_PATH)
	file(GLOB SVGIN_FILES RELATIVE ${SRC_PATH} ${SRC_PATH}/${ICON_NAME}.svg.in)

	set(ICON_DESCS 64:1 128:2 192:3)

	message(STATUS "Product: customizing svg icon ${ICON_NAME}")

	configure_file(${SRC_PATH}/${SVGIN_FILES} ${SRC_PATH}/${ICON_NAME}.svg)

	# Create icon for all size
	foreach(ICON_DESC ${ICON_DESCS})
		string(REPLACE ":" ";" LIST ${ICON_DESC})
		list(GET LIST 0 ICON_SIZE)
		list(GET LIST 1 ICON_SCALE)

		set(DST_ICON_PATH ${DST_PATH}/${ICON_NAME}.imageset)
		execute_process(COMMAND ${IMAGEMAGICK_CONVERT} -background none ${SRC_PATH}/${ICON_NAME}.svg -resize ${ICON_SIZE}x${ICON_SIZE} ${DST_ICON_PATH}/${ICON_NAME}@${ICON_SCALE}x.png)
	endforeach(ICON_DESC)

endmacro(product_image_make_ioscv_customized_icon)
