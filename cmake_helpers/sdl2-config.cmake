IF (WIN32)
	set(SDL2_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/../3rdparty/SDL2/include")

	# Support both 32 and 64 bit builds
	if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
	  set(SDL2_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/../3rdparty/SDL2/lib/x64/SDL2.lib;${CMAKE_CURRENT_LIST_DIR}/../3rdparty/SDL2/lib/x64/SDL2main.lib")
	else ()
	  set(SDL2_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/../3rdparty/SDL2/lib/x86/SDL2.lib;${CMAKE_CURRENT_LIST_DIR}/../3rdparty/SDL2/lib/x86/SDL2main.lib")
	endif ()

	string(STRIP "${SDL2_LIBRARIES}" SDL2_LIBRARIES)
ENDIF()