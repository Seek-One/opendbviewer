
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

########################
# Create customized icon
########################
macro (product_image_generate_icon SRC_PATH DST_PATH)
	message(STATUS "Generating icon")

	file(GLOB SVGIN_FILES RELATIVE ${SRC_PATH} ${SRC_PATH}/*.svg)
	
	set(ICON_SIZES 16 22 24 32 48 64 128) 

	foreach(ICON_IN ${SVGIN_FILES})
		get_filename_component(ICON ${ICON_IN} NAME_WE)

		string(REGEX REPLACE "([a-zA-Z0-9]+)_(.*)" "\\1" ICON_CATEGORY ${ICON})
		string(REGEX REPLACE "([a-zA-Z0-9]+)_(.*)" "\\2" ICON_NAME ${ICON})

		# Create icon for all size
		foreach(ICON_SIZE ${ICON_SIZES})
			set(DST_ICON_PATH ${DST_PATH}/${ICON_CATEGORY}/${ICON_SIZE})
			file(MAKE_DIRECTORY ${DST_ICON_PATH})
			execute_process(COMMAND ${IMAGEMAGICK_CONVERT} -background none ${SRC_PATH}/${ICON}.svg -resize ${ICON_SIZE}x${ICON_SIZE} ${DST_ICON_PATH}/${ICON_NAME}.png)
		endforeach(ICON_SIZE)

	endforeach(ICON_IN)
endmacro(product_image_make_customized_icon)
