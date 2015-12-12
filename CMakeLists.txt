cmake_minimum_required(VERSION 2.8)
project(mgf)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH "./libs")

set(SOURCE_FILES
	main.cpp
	src/input/BaseInput.cpp
	src/input/BaseInput.h
	src/input/IInput.h
	src/input/InputFPS.cpp
	src/input/InputFPS.h
	src/input/InputTopDown.cpp
	src/input/InputTopDown.h
	src/collision/MouseRay.cpp
	src/collision/MouseRay.h
	src/collision/RayPlane.cpp
	src/collision/RayPlane.h
	src/scene/SimpleLoader.cpp
	src/scene/SimpleLoader.h
	src/scene/OverlayNode.cpp
	src/scene/OverlayNode.h
	src/scene/LODNode.cpp
	src/scene/LoaderTransparent.cpp
	src/scene/MeshTransparentNode.cpp
	src/scene/LoaderLOD.cpp
	src/renderer/LightManager.cpp
	src/renderer/LightManager.h
	src/overlay/Button.cpp
	src/overlay/Button.h
	src/overlay/IOverlayElement.h
	src/overlay/Label.cpp
	src/overlay/Label.h
	src/overlay/Moveable.cpp
	src/overlay/Moveable.h
	src/overlay/Overlay.cpp
	src/overlay/Overlay.h
	src/camera/BaseCamera.cpp
	src/camera/BaseCamera.h
	src/camera/CameraFlightQuat.cpp
	src/camera/CameraFlightQuat.h
	src/camera/CameraFPSEuler.cpp
	src/camera/CameraFPSEuler.h
	src/camera/CameraFPSQuat.cpp
	src/camera/CameraFPSQuat.h
	src/camera/CameraTopDown.cpp
	src/camera/CameraTopDown.h
	src/camera/ICamera.h
	src/data/Data.cpp
	src/data/Data.h
	src/data/Light.cpp
	src/data/Light.h
	src/data/Material.cpp
	src/data/Material.h
	src/data/Mesh.cpp
	src/data/Mesh.h
	src/data/Texture.cpp
	src/data/Texture.h
	src/helper/Helper.cpp
	src/helper/Helper.h
	src/lib/mlog.h
	src/renderer/Renderer.cpp
	src/renderer/Renderer.h
	src/scene/Loader.cpp
	src/scene/Loader.h
	src/scene/MeshNode.cpp
	src/scene/MeshNode.h
	src/scene/LightNode.cpp
	src/scene/LightNode.h
	src/scene/Node.cpp
	src/scene/Node.h
	src/scene/Positionable.cpp
	src/scene/Positionable.h
	src/util/ShaderProgram.cpp
	src/util/ShaderProgram.h
	src/util/Window.cpp
	src/util/Window.h
	src/Include.h
	src/mgf.cpp
	src/mgf.h)

add_executable(mgf ${SOURCE_FILES})

INCLUDE(FindPkgConfig)

PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(mgf ${SDL2_LIBRARIES})

PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image)
INCLUDE_DIRECTORIES(${SDL2IMAGE_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(mgf ${SDL2IMAGE_LIBRARIES})

PKG_SEARCH_MODULE(SDL2TTF REQUIRED SDL2_ttf)
INCLUDE_DIRECTORIES(${SDL2TTF_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(mgf ${SDL2TTF_LIBRARIES})

PKG_SEARCH_MODULE(GLEW REQUIRED glew)
INCLUDE_DIRECTORIES(${GLEW_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(mgf ${GLEW_LIBRARIES})


PKG_SEARCH_MODULE(ASSIMP REQUIRED assimp)
INCLUDE_DIRECTORIES(${ASSIMP_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(mgf ${ASSIMP_LIBRARIES})

