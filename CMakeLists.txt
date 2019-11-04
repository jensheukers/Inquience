cmake_minimum_required(VERSION 2.8.9)

set(CMAKE_SUPPRESS_REGENERATION true) 
SET (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SAFESEH:NO")
SET (CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /SAFESEH:NO")
SET (CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} /SAFESEH:NO")

#------------------------------------------------------
#	Create new project
#------------------------------------------------------
project (Inquience)

# Get Headers
file (GLOB ENGINE_HEADERS "engine/*.h" "engine/component/*.h" "engine/graphics/*h" "engine/math/*.h")

#------------------------------------------------------
#	Include directories
#------------------------------------------------------
include_directories(
	${CMAKE_SOURCE_DIR}/external/glfw/include
	${CMAKE_SOURCE_DIR}/external/glew/include
	${CMAKE_SOURCE_DIR}/external/glm
	${CMAKE_SOURCE_DIR}/external/steam
	${CMAKE_SOURCE_DIR}/external/freetype/include
	${CMAKE_SOURCE_DIR}/external/lua/include
	${CMAKE_SOURCE_DIR}/external/openal/include
	${CMAKE_SOURCE_DIR}/external/oggvorbis/include
	${CMAKE_SOURCE_DIR}/external/imgui
	${CMAKE_SOURCE_DIR}/engine
)

link_directories (
	${CMAKE_SOURCE_DIR}/external/glfw/lib-vc2015 
	${CMAKE_SOURCE_DIR}/external/glew/lib/Win32 
	${CMAKE_SOURCE_DIR}/external/steam/lib 
	${CMAKE_SOURCE_DIR}/external/freetype/win32 
	${CMAKE_SOURCE_DIR}/external/lua/lib 
	${CMAKE_SOURCE_DIR}/external/openal/libs/Win32 
	${CMAKE_SOURCE_DIR}/external/oggvorbis/lib/Win32
)

set ( ALL_LIBS
	glfw3.lib 
	glfw3dll.lib 
	opengl32.lib 
	glew32.lib 
	glew32s.lib 
	sdkencryptedappticket.lib 
	steam_api.lib 
	freetype.lib 
	LuaLib.lib
	OpenAL32.lib 
	libogg.lib 
	libvorbis.lib 
	libvorbisfile.lib
)

#------------------------------------------------------
#	Add Engine
#------------------------------------------------------
add_library(Engine
	engine/camera.cpp
	engine/collisionmanager.cpp
	engine/component.cpp
	engine/component_register.cpp
	engine/core.cpp
	engine/debug.cpp
	engine/editor.cpp
	engine/entity.cpp
	engine/input.cpp
	engine/luascript.cpp
	engine/renderer.cpp
	engine/scene.cpp
	engine/scenemanager.cpp
	engine/sound.cpp
	engine/soundmanager.cpp
	engine/texture.cpp
	engine/unique_types.cpp
	engine/components/animator.cpp
	engine/components/collider.cpp
	engine/components/rigidbody.cpp
	engine/components/sprite.cpp
	engine/components/ui.cpp
	engine/graphics/font.cpp
	engine/graphics/shader.cpp
	engine/math/physics.cpp
	engine/math/vec2.cpp
	
	#ImGui
	external/imgui/imgui.cpp
	external/imgui/imgui_draw.cpp
	external/imgui/imgui_impl_glfw.cpp
	external/imgui/imgui_impl_opengl3.cpp
	external/imgui/imgui_widgets.cpp
)

add_custom_command(TARGET Engine POST_BUILD 
					COMMAND ${CMAKE_COMMAND} -E make_directory
						$<TARGET_FILE_DIR:Engine>/shaders)

add_custom_command(TARGET Engine POST_BUILD 
					COMMAND ${CMAKE_COMMAND} -E copy_directory
						${CMAKE_SOURCE_DIR}/engine/shaders $<TARGET_FILE_DIR:Engine>/shaders)
						
add_custom_command(TARGET Engine POST_BUILD 
					COMMAND ${CMAKE_COMMAND} -E copy
						${CMAKE_SOURCE_DIR}/external/glfw/lib-vc2015/glfw3.dll $<TARGET_FILE_DIR:Engine>)

add_custom_command(TARGET Engine POST_BUILD 
					COMMAND ${CMAKE_COMMAND} -E copy
						${CMAKE_SOURCE_DIR}/external/glew/bin/Win32/glew32.dll $<TARGET_FILE_DIR:Engine>)

add_custom_command(TARGET Engine POST_BUILD 
					COMMAND ${CMAKE_COMMAND} -E copy
						${CMAKE_SOURCE_DIR}/external/steam/lib/sdkencryptedappticket.dll $<TARGET_FILE_DIR:Engine>)
	
add_custom_command(TARGET Engine POST_BUILD 
					COMMAND ${CMAKE_COMMAND} -E copy
						${CMAKE_SOURCE_DIR}/external/steam/lib/steam_api.dll $<TARGET_FILE_DIR:Engine>)
						
add_custom_command(TARGET Engine POST_BUILD 
					COMMAND ${CMAKE_COMMAND} -E copy
						${CMAKE_SOURCE_DIR}/external/freetype/win32/freetype.dll $<TARGET_FILE_DIR:Engine>)						
	
add_custom_command(TARGET Engine POST_BUILD 
					COMMAND ${CMAKE_COMMAND} -E copy
						${CMAKE_SOURCE_DIR}/external/openal/bin/Win32/OpenAL32.dll $<TARGET_FILE_DIR:Engine>)

add_custom_command(TARGET Engine POST_BUILD 
					COMMAND ${CMAKE_COMMAND} -E copy_directory
						${CMAKE_SOURCE_DIR}/external/oggvorbis/bin $<TARGET_FILE_DIR:Engine>)
						
#------------------------------------------------------
#	INQUIENCE
#------------------------------------------------------
add_executable( Inquience
	inquience/main.cpp
)

target_link_libraries(Inquience
	Engine
	${ALL_LIBS}
)

add_custom_command(TARGET Inquience POST_BUILD 
					COMMAND ${CMAKE_COMMAND} -E make_directory
						$<TARGET_FILE_DIR:Inquience>/res)

add_custom_command(TARGET Inquience POST_BUILD 
					COMMAND ${CMAKE_COMMAND} -E copy_directory
						${CMAKE_SOURCE_DIR}/inquience/res $<TARGET_FILE_DIR:Inquience>/res)