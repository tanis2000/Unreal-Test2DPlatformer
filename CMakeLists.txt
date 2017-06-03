cmake_minimum_required (VERSION 2.6)
project (UE4)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
set(CMAKE_C_COMPILER "/usr/bin/clang")
set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

include("/Users/tanis/Documents/Unreal Projects/Test2DPlatformer/Intermediate/ProjectFiles/IncludeDirectories.cmake")
include("/Users/tanis/Documents/Unreal Projects/Test2DPlatformer/Intermediate/ProjectFiles/Definitions.cmake")
include("/Users/tanis/Documents/Unreal Projects/Test2DPlatformer/Intermediate/ProjectFiles/Test2DPlatformerEditor.cmake")

set(MONO_ROOT_PATH "/Users/Shared/Epic Games/UE_4.15/Engine/Binaries/DotNET")
set(BUILD cd "${MONO_ROOT_PATH}")

# Custom target for Test2DPlatformerEditor project, Development configuration
add_custom_target(Test2DPlatformerEditor-Development ${BUILD} && mono UnrealBuildTool.exe Test2DPlatformerEditor Mac Development -project="/Users/tanis/Documents/Unreal Projects/Test2DPlatformer/Test2DPlatformer.uproject" -game)
add_custom_target(Test2DPlatformerEditor-Development-CLEAN ${BUILD} && mono UnrealBuildTool.exe Test2DPlatformerEditor Mac Development -project="/Users/tanis/Documents/Unreal Projects/Test2DPlatformer/Test2DPlatformer.uproject" -clean)

add_executable(PleaseIgnoreMe ${Test2DPlatformerEditor_FILES})