
file (GLOB_RECURSE headers "*.h") ## retrieve all header files in current directory
file (GLOB_RECURSE sources "*.cpp")     ## retrieve all source files in current directory



SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

set(mfast_coder_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/../../../tinyxml2/tinyxml2.cpp ${sources} ${headers})

add_library(mfast_coder_static STATIC ${mfast_coder_SRCS})

if (UNIX)
	set_target_properties(mfast_coder_static PROPERTIES OUTPUT_NAME mfast_coder)
endif()
set_target_properties(mfast_coder_static PROPERTIES COMPILE_FLAGS -DMFAST_CODER_STATIC_DEFINE)

install(TARGETS mfast_coder_static
        EXPORT  mFASTTargets
        RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin
		ARCHIVE DESTINATION "${INSTALL_LIB_DIR}" COMPONENT lib)


if (BUILD_SHARED_LIBS)
  add_library(mfast_coder SHARED ${mfast_coder_SRCS})
  if (CMAKE_COMPILER_IS_GNUCXX OR ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang"))
	set_target_properties(mfast_coder PROPERTIES COMPILE_FLAGS -fvisibility=hidden)
  endif()
  target_link_libraries(mfast_coder mfast)

  install(TARGETS mfast_coder
          EXPORT  mFASTTargets
          RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin
  	      LIBRARY DESTINATION "${INSTALL_LIB_DIR}" COMPONENT lib)

  SET(MFAST_DYNAMIC_COMPONENTS ${MFAST_DYNAMIC_COMPONENTS} coder CACHE INTERNAL "")
endif()

SET(MFAST_STATIC_COMPONENTS ${MFAST_STATIC_COMPONENTS} coder_static CACHE INTERNAL "")

install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        DESTINATION "${INSTALL_INCLUDE_DIR}/mfast"
        FILES_MATCHING PATTERN "*.h")
