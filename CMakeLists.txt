cmake_minimum_required(VERSION 2.6)
project(SHADOW_ENGINE)

include_directories(
src
contrib
)

if(MSVC)
  include_directories(
    contrib/msvc-windows-x86/glew/include
    contrib/msvc-windows-x86/openal-soft/include
    contrib/msvc-windows-x86/ogg/include
    contrib/msvc-windows-x86/vorbis/include
    contrib/msvc-windows-x86/GLFW/include
  )

  link_directories(
    ${CMAKE_SOURCE_DIR}/contrib/msvc-windows-x86/glew/lib
    ${CMAKE_SOURCE_DIR}/contrib/msvc-windows-x86/openal-soft/lib
    ${CMAKE_SOURCE_DIR}/contrib/msvc-windows-x86/ogg/lib
    ${CMAKE_SOURCE_DIR}/contrib/msvc-windows-x86/vorbis/lib
	${CMAKE_SOURCE_DIR}/contrib/msvc-windows-x86/GLFW/lib
  )
endif()

add_library(VanillaEngine
	src/VanillaEngine/VanillaEngine.h
	src/VanillaEngine/Application.h
	src/VanillaEngine/Application.cpp
	src/VanillaEngine/Environment.h
	src/VanillaEngine/Environment.cpp
	src/VanillaEngine/Entity.h
	src/VanillaEngine/Entity.cpp
	src/VanillaEngine/Component.h
	src/VanillaEngine/Component.cpp
	src/VanillaEngine/ShaderProgram.h
	src/VanillaEngine/ShaderProgram.cpp
	src/VanillaEngine/VertexArray.h
	src/VanillaEngine/VertexArray.cpp
	src/VanillaEngine/VertexBuffer.h
	src/VanillaEngine/VertexBuffer.cpp
	src/VanillaEngine/Exception.h
	src/VanillaEngine/Exception.cpp
	src/VanillaEngine/Camera.h
	src/VanillaEngine/Camera.cpp
	src/VanillaEngine/Texture.h
	src/VanillaEngine/Texture.cpp
	src/VanillaEngine/stb_image.h
	src/VanillaEngine/stb_image.cpp
	src/VanillaEngine/Renderer.h
	src/VanillaEngine/Renderer.cpp
	src/VanillaEngine/Resource.h
	src/VanillaEngine/Resource.cpp
	src/VanillaEngine/Resources.h
	src/VanillaEngine/Resources.cpp
	src/VanillaEngine/Transform.h
	src/VanillaEngine/Transform.cpp
	src/VanillaEngine/Lights.h
	src/VanillaEngine/Lights.cpp
	src/VanillaEngine/PointLight.h
	src/VanillaEngine/PointLight.cpp
	src/VanillaEngine/Spotlight.h
	src/VanillaEngine/Spotlight.cpp
	src/VanillaEngine/Input.h
	src/VanillaEngine/Input.cpp
	src/VanillaEngine/ShadowBox.h
	src/VanillaEngine/ShadowBox.cpp
	src/VanillaEngine/ShadowManager.h
	src/VanillaEngine/ShadowManager.cpp
	src/VanillaEngine/ShadowShader.h
	src/VanillaEngine/ShadowShader.cpp
)

if(MSVC)
  target_link_libraries(VanillaEngine
    glew32 opengl32 OpenAL32 ogg vorbis vorbisfile glfw3
  )
else()
  target_link_libraries(VanillaEngine
    GLU GLEW GL AL ogg vorbis vorbisfile glfw3
  )
endif()

add_executable(game
	src/game/main.cpp
	src/game/FirstPersonCameraControls.h
	src/game/FirstPersonCameraControls.cpp
)

target_link_libraries(game VanillaEngine)

if(MSVC)
  add_custom_command(TARGET game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
	  "${CMAKE_SOURCE_DIR}/contrib/msvc-windows-x86/glew/bin/glew32.dll"
	  "${CMAKE_SOURCE_DIR}/contrib/msvc-windows-x86/openal-soft/bin/OpenAL32.dll"
      "${CMAKE_SOURCE_DIR}/contrib/msvc-windows-x86/GLFW/bin/glfw3.dll"
      "$<TARGET_FILE_DIR:game>"
  )
endif()