ICONNAME=opendbviewer
IMAGEMAGICK_CONVERT=convert
SIPS=sips
PNG2ICNS=png2icns
DST_PATH=.

${IMAGEMAGICK_CONVERT} -background none ${ICONNAME}.svg -resize 16x16 ${DST_PATH}/hicolor_16x16_apps_${ICONNAME}.png
${IMAGEMAGICK_CONVERT} -background none ${ICONNAME}.svg -resize 22x22 ${DST_PATH}/hicolor_22x22_apps_${ICONNAME}.png
${IMAGEMAGICK_CONVERT} -background none ${ICONNAME}.svg -resize 24x24 ${DST_PATH}/hicolor_24x24_apps_${ICONNAME}.png
${IMAGEMAGICK_CONVERT} -background none ${ICONNAME}.svg -resize 32x32 ${DST_PATH}/hicolor_32x32_apps_${ICONNAME}.png
${IMAGEMAGICK_CONVERT} -background none ${ICONNAME}.svg -resize 48x48 ${DST_PATH}/hicolor_48x48_apps_${ICONNAME}.png
${IMAGEMAGICK_CONVERT} -background none ${ICONNAME}.svg -resize 64x64 ${DST_PATH}/hicolor_64x64_apps_${ICONNAME}.png
${IMAGEMAGICK_CONVERT} -background none ${ICONNAME}.svg -resize 128x128 ${DST_PATH}/hicolor_128x128_apps_${ICONNAME}.png
${IMAGEMAGICK_CONVERT} -background none ${ICONNAME}.svg -resize 128x128 ${ICONNAME}.png
${IMAGEMAGICK_CONVERT} -background none ${ICONNAME}.svg -resize 128x128 ${ICONNAME}.ico

${PNG2ICNS} ${ICONNAME}.icns ${ICONNAME}.png
#${SIPS} -z 512 512 -s format icns ${ICONNAME}.png --out ${ICONNAME}.icns
