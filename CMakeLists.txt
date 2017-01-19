cmake_minimum_required (VERSION 2.6)
project (UE4)
set(CMAKE_CXX_STANDARD 11)

include("/Users/tanis/Documents/Unreal Projects/Test2DPlatformer/Intermediate/ProjectFiles/IncludeDirectories.cmake")
include("/Users/tanis/Documents/Unreal Projects/Test2DPlatformer/Intermediate/ProjectFiles/Definitions.cmake")
include("/Users/tanis/Documents/Unreal Projects/Test2DPlatformer/Intermediate/ProjectFiles/UE4Editor.cmake")

# Custom target for UE4Editor project, DebugGame configuration
set(MONO_ROOT_PATH "/Users/Shared/UnrealEngine/4.14/Engine/Binaries/DotNET")
set(BUILD cd "${MONO_ROOT_PATH}")

add_custom_target(UE4Editor-Mac-DebugGame ${BUILD} && mono UnrealBuildTool.exe UE4Editor Mac DebugGame -project="/Users/tanis/Documents/Unreal Projects/Test2DPlatformer/Test2DPlatformer.uproject" -game)
add_custom_target(UE4Editor-Mac-DebugGame-clean ${BUILD} && mono UnrealBuildTool.exe UE4Editor Mac DebugGame -project="/Users/tanis/Documents/Unreal Projects/Test2DPlatformer/Test2DPlatformer.uproject" -clean)


# Custom target for UE4Editor project, Development configuration
add_custom_target(UE4Editor ${BUILD} && mono UnrealBuildTool.exe UE4Editor Mac Development -project="/Users/tanis/Documents/Unreal Projects/Test2DPlatformer/Test2DPlatformer.uproject" -game)
add_custom_target(UE4Editor-clean ${BUILD} && mono UnrealBuildTool.exe UE4Editor Mac Development -project="/Users/tanis/Documents/Unreal Projects/Test2DPlatformer/Test2DPlatformer.uproject" -clean)


# Custom target for UE4Editor project, Shipping configuration
add_custom_target(UE4Editor-Mac-Shipping ${BUILD} && mono UnrealBuildTool.exe UE4Editor Mac Shipping -project="/Users/tanis/Documents/Unreal Projects/Test2DPlatformer/Test2DPlatformer.uproject" -game)
add_custom_target(UE4Editor-Mac-Shipping-clean ${BUILD} && mono UnrealBuildTool.exe UE4Editor Mac Shipping -project="/Users/tanis/Documents/Unreal Projects/Test2DPlatformer/Test2DPlatformer.uproject" -clean)

include("/Users/tanis/Documents/Unreal Projects/Test2DPlatformer/Intermediate/ProjectFiles/UE4Game.cmake")

# Custom target for UE4Game project, DebugGame configuration
set(MONO_ROOT_PATH "/Users/Shared/UnrealEngine/4.14/Engine/Binaries/DotNET")
set(BUILD cd "${MONO_ROOT_PATH}")

add_custom_target(UE4Game-Mac-DebugGame ${BUILD} && mono UnrealBuildTool.exe UE4Game Mac DebugGame -project="/Users/tanis/Documents/Unreal Projects/Test2DPlatformer/Test2DPlatformer.uproject" -game)
add_custom_target(UE4Game-Mac-DebugGame-clean ${BUILD} && mono UnrealBuildTool.exe UE4Game Mac DebugGame -project="/Users/tanis/Documents/Unreal Projects/Test2DPlatformer/Test2DPlatformer.uproject" -clean)


# Custom target for UE4Game project, Development configuration
add_custom_target(UE4Game ${BUILD} && mono UnrealBuildTool.exe UE4Game Mac Development -project="/Users/tanis/Documents/Unreal Projects/Test2DPlatformer/Test2DPlatformer.uproject" -game)
add_custom_target(UE4Game-clean ${BUILD} && mono UnrealBuildTool.exe UE4Game Mac Development -project="/Users/tanis/Documents/Unreal Projects/Test2DPlatformer/Test2DPlatformer.uproject" -clean)


# Custom target for UE4Game project, Shipping configuration
add_custom_target(UE4Game-Mac-Shipping ${BUILD} && mono UnrealBuildTool.exe UE4Game Mac Shipping -project="/Users/tanis/Documents/Unreal Projects/Test2DPlatformer/Test2DPlatformer.uproject" -game)
add_custom_target(UE4Game-Mac-Shipping-clean ${BUILD} && mono UnrealBuildTool.exe UE4Game Mac Shipping -project="/Users/tanis/Documents/Unreal Projects/Test2DPlatformer/Test2DPlatformer.uproject" -clean)

include("/Users/tanis/Documents/Unreal Projects/Test2DPlatformer/Intermediate/ProjectFiles/Test2DPlatformer.cmake")

# Custom target for Test2DPlatformer project, DebugGame configuration
set(MONO_ROOT_PATH "/Users/Shared/UnrealEngine/4.14/Engine/Binaries/DotNET")
set(BUILD cd "${MONO_ROOT_PATH}")

add_custom_target(Test2DPlatformer-Mac-DebugGame ${BUILD} && mono UnrealBuildTool.exe Test2DPlatformer Mac DebugGame -project="/Users/tanis/Documents/Unreal Projects/Test2DPlatformer/Test2DPlatformer.uproject" -game)
add_custom_target(Test2DPlatformer-Mac-DebugGame-clean ${BUILD} && mono UnrealBuildTool.exe Test2DPlatformer Mac DebugGame -project="/Users/tanis/Documents/Unreal Projects/Test2DPlatformer/Test2DPlatformer.uproject" -clean)


# Custom target for Test2DPlatformer project, Development configuration
add_custom_target(Test2DPlatformer ${BUILD} && mono UnrealBuildTool.exe Test2DPlatformer Mac Development -project="/Users/tanis/Documents/Unreal Projects/Test2DPlatformer/Test2DPlatformer.uproject" -game)
add_custom_target(Test2DPlatformer-clean ${BUILD} && mono UnrealBuildTool.exe Test2DPlatformer Mac Development -project="/Users/tanis/Documents/Unreal Projects/Test2DPlatformer/Test2DPlatformer.uproject" -clean)


# Custom target for Test2DPlatformer project, Shipping configuration
add_custom_target(Test2DPlatformer-Mac-Shipping ${BUILD} && mono UnrealBuildTool.exe Test2DPlatformer Mac Shipping -project="/Users/tanis/Documents/Unreal Projects/Test2DPlatformer/Test2DPlatformer.uproject" -game)
add_custom_target(Test2DPlatformer-Mac-Shipping-clean ${BUILD} && mono UnrealBuildTool.exe Test2DPlatformer Mac Shipping -project="/Users/tanis/Documents/Unreal Projects/Test2DPlatformer/Test2DPlatformer.uproject" -clean)

include("/Users/tanis/Documents/Unreal Projects/Test2DPlatformer/Intermediate/ProjectFiles/Test2DPlatformerEditor.cmake")

# Custom target for Test2DPlatformerEditor project, DebugGame configuration
set(MONO_ROOT_PATH "/Users/Shared/UnrealEngine/4.14/Engine/Binaries/DotNET")
set(BUILD cd "${MONO_ROOT_PATH}")

add_custom_target(Test2DPlatformerEditor-Mac-DebugGame ${BUILD} && mono UnrealBuildTool.exe Test2DPlatformerEditor Mac DebugGame -project="/Users/tanis/Documents/Unreal Projects/Test2DPlatformer/Test2DPlatformer.uproject" -game)
add_custom_target(Test2DPlatformerEditor-Mac-DebugGame-clean ${BUILD} && mono UnrealBuildTool.exe Test2DPlatformerEditor Mac DebugGame -project="/Users/tanis/Documents/Unreal Projects/Test2DPlatformer/Test2DPlatformer.uproject" -clean)


# Custom target for Test2DPlatformerEditor project, Development configuration
add_custom_target(Test2DPlatformerEditor ${BUILD} && mono UnrealBuildTool.exe Test2DPlatformerEditor Mac Development -project="/Users/tanis/Documents/Unreal Projects/Test2DPlatformer/Test2DPlatformer.uproject" -game)
add_custom_target(Test2DPlatformerEditor-clean ${BUILD} && mono UnrealBuildTool.exe Test2DPlatformerEditor Mac Development -project="/Users/tanis/Documents/Unreal Projects/Test2DPlatformer/Test2DPlatformer.uproject" -clean)


# Custom target for Test2DPlatformerEditor project, Shipping configuration
add_custom_target(Test2DPlatformerEditor-Mac-Shipping ${BUILD} && mono UnrealBuildTool.exe Test2DPlatformerEditor Mac Shipping -project="/Users/tanis/Documents/Unreal Projects/Test2DPlatformer/Test2DPlatformer.uproject" -game)
add_custom_target(Test2DPlatformerEditor-Mac-Shipping-clean ${BUILD} && mono UnrealBuildTool.exe Test2DPlatformerEditor Mac Shipping -project="/Users/tanis/Documents/Unreal Projects/Test2DPlatformer/Test2DPlatformer.uproject" -clean)

set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
set(CMAKE_C_COMPILER "/usr/bin/clang")

add_executable(Test2DPlatformerEditorFake ${Test2DPlatformerEditor_FILES})
