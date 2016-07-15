# Define some useful macro or function

# Here the list of macro defined
# - JOINLIST : join a list of item into an unique string with a separator
# - SUFFIXLIST : add a suffix to all list of item
# - install_if_not_exists : install a file without override if file exists
# - check_has_module : check if a cmake module exists

macro(JOINLIST list_name sep output)
	# create empty list - necessary?
	# prefix and suffix elements
	foreach(l ${${list_name}})
		if(NOT DEFINED list_name_TMP)
			SET(list_name_TMP ${l})
		else()
			SET(list_name_TMP ${list_name_TMP}${sep}${l})
		endif()
	endforeach()
	# replace list by tmp list
	SET(${output} ${list_name_TMP})
	UNSET(list_name_TMP)
endmacro(JOINLIST)

macro(SUFFIXLIST list_name suffix output)
	# create empty list - necessary?
	# prefix and suffix elements
	foreach(l ${${list_name}})
		SET(list_name_TMP ${list_name_TMP} ${l}${suffix})
	endforeach()
	# replace list by tmp list
	SET(${output} ${list_name_TMP})
	UNSET(list_name_TMP)
endmacro(SUFFIXLIST)

function(install_if_not_exists src dest)
  if(NOT IS_ABSOLUTE "${src}")
    set(src "${CMAKE_CURRENT_SOURCE_DIR}/${src}")
  endif()
  get_filename_component(src_name "${src}" NAME)
  if (NOT IS_ABSOLUTE "${dest}")
    set(dest "${CMAKE_INSTALL_PREFIX}/${dest}")
  endif()
  install(CODE "
    if(NOT EXISTS \"\$ENV{DESTDIR}${dest}/${src_name}\")
      #file(INSTALL \"${src}\" DESTINATION \"${dest}\")
      message(STATUS \"Installing: \$ENV{DESTDIR}${dest}/${src_name}\")
      execute_process(COMMAND \${CMAKE_COMMAND} -E copy \"${src}\"
                      \"\$ENV{DESTDIR}${dest}/${src_name}\"
                      RESULT_VARIABLE copy_result
                      ERROR_VARIABLE error_output)
      if(copy_result)
        message(FATAL_ERROR \${error_output})
      endif()
    else()
      message(STATUS \"Skipping  : \$ENV{DESTDIR}${dest}/${src_name}\")
    endif()
  ")
endfunction(install_if_not_exists)

function(check_has_module Module)
  find_package(${Module} QUIET)
  if(NOT DEFINED ${Module}_DIR)
    set(HAS_MODULE_${Module} TRUE PARENT_SCOPE)
  elseif(${Module}_DIR)
    set(HAS_MODULE_${Module} TRUE PARENT_SCOPE)
  else()
    set(HAS_MODULE_${Module} FALSE PARENT_SCOPE)
  endif()
endfunction()
