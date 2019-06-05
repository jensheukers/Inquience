cmake_minimum_required(VERSION 2.8.9)

set(CMAKE_SUPPRESS_REGENERATION true) 

project (Dustville)

# Main engine files
file(GLOB MAIN "engine/*.cpp" "engine/*.h" )
file(GLOB GRAPHICS "engine/graphics/*.cpp" "engine/graphics/*.h")
file(GLOB COMPONENTS "engine/components/*.cpp" "engine/components/*.h")
file(GLOB MATH "engine/math/*.cpp" "engine/math/*.h")
file(GLOB GAME "game/*.cpp" "game/*.h")
file(GLOB IMGUI "external/imgui/*.cpp" "external/imgui/*.h")

# Includes
set(GLFW_DIR "external/glfw")
set(GLEW_DIR "external/glew")
set(GLM_DIR "external/glm")
set(STEAM_DIR "external/steam")
set(FREETYPE_DIR "external/freetype")

include_directories(${GLFW_DIR}/include ${GLEW_DIR}/include ${GLM_DIR} ${STEAM_DIR} ${FREETYPE_DIR}/include)
link_directories(${GLFW_DIR}/lib-vc2015 ${GLEW_DIR}/lib/Win32 ${STEAM_DIR}/lib ${FREETYPE_DIR}/win32)

# Add Executable

add_executable(Dustville ${MAIN} ${GRAPHICS} ${GAME} ${COMPONENTS} ${MATH} ${IMGUI})

set(CMAKE_CXX_FLAGS_RELEASE "/MD")
set(CMAKE_CXX_FLAGS_DEBUG "/MD")

# Link libraries
target_link_libraries(Dustville glfw3.lib glfw3dll.lib opengl32.lib glew32.lib glew32s.lib sdkencryptedappticket.lib steam_api.lib freetype.lib)
SET (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SAFESEH:NO")
SET (CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /SAFESEH:NO")
SET (CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} /SAFESEH:NO")

# TODO: Set Commands
add_custom_command(TARGET Dustville POST_BUILD 
					COMMAND ${CMAKE_COMMAND} -E copy
						${CMAKE_SOURCE_DIR}/external/glfw/lib-vc2015/glfw3.dll $<TARGET_FILE_DIR:Dustville>)

add_custom_command(TARGET Dustville POST_BUILD 
					COMMAND ${CMAKE_COMMAND} -E copy
						${CMAKE_SOURCE_DIR}/external/glew/bin/Win32/glew32.dll $<TARGET_FILE_DIR:Dustville>)

add_custom_command(TARGET Dustville POST_BUILD 
					COMMAND ${CMAKE_COMMAND} -E copy
						${CMAKE_SOURCE_DIR}/external/steam/lib/sdkencryptedappticket.dll $<TARGET_FILE_DIR:Dustville>)
	
add_custom_command(TARGET Dustville POST_BUILD 
					COMMAND ${CMAKE_COMMAND} -E copy
						${CMAKE_SOURCE_DIR}/external/steam/lib/steam_api.dll $<TARGET_FILE_DIR:Dustville>)
						
add_custom_command(TARGET Dustville POST_BUILD 
					COMMAND ${CMAKE_COMMAND} -E copy
						${CMAKE_SOURCE_DIR}/external/freetype/win32/freetype.dll $<TARGET_FILE_DIR:Dustville>)						
	
add_custom_command(TARGET Dustville POST_BUILD 
					COMMAND ${CMAKE_COMMAND} -E make_directory
						$<TARGET_FILE_DIR:Dustville>/shaders)

add_custom_command(TARGET Dustville POST_BUILD 
					COMMAND ${CMAKE_COMMAND} -E copy_directory
						${CMAKE_SOURCE_DIR}/engine/shaders $<TARGET_FILE_DIR:Dustville>/shaders)
						
add_custom_command(TARGET Dustville POST_BUILD 
					COMMAND ${CMAKE_COMMAND} -E make_directory
						$<TARGET_FILE_DIR:Dustville>/res)

add_custom_command(TARGET Dustville POST_BUILD 
					COMMAND ${CMAKE_COMMAND} -E copy_directory
						${CMAKE_SOURCE_DIR}/game/res $<TARGET_FILE_DIR:Dustville>/res)
# Filter groups
source_group("engine" FILES ${MAIN})
source_group("graphics" FILES ${GRAPHICS})
source_group("components" FILES ${COMPONENTS})
source_group("game" FILES ${GAME})
source_group("math" FILES ${MATH})
source_group("imgui" FILES ${IMGUI})