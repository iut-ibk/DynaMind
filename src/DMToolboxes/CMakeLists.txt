FILE(GLOB MODULE_CPPS *.cpp)
FILE(GLOB MODULE_HEADER *.h)

INCLUDE_DIRECTORIES(.)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

ADD_LIBRARY(dynamindtoolbox SHARED ${MODULE_CPPS} ${MODULE_HEADER})

IF(${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
        SET_TARGET_PROPERTIES(dynamindtoolbox PROPERTIES COMPILE_FLAGS "-frounding-math")
ENDIF()

TARGET_LINK_LIBRARIES(dynamindtoolbox dynamindcore)

INSTALL(TARGETS dynamindtoolbox
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
		PERMISSIONS OWNER_WRITE OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

IF(WIN32 OR WIN64)
    INSTALL(FILES ${OUTPUT_DIR}/dynamindtoolbox.lib
            DESTINATION lib
            PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)
ENDIF()

INSTALL(FILES ${MODULE_HEADER}
        DESTINATION "include/dynamindcore"
        PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)
