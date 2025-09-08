file(GLOB SUBDIRECTORIES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} */)

foreach(SUBDIR ${SUBDIRECTORIES})
    if(SUBDIR MATCHES "^_")
        continue()
    endif()
    
    if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${SUBDIR} AND
       EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${SUBDIR}/CMakeLists.txt)
        add_subdirectory(${SUBDIR})
    endif()
endforeach()
