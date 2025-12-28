cmake_minimum_required(VERSION 3.29.0)

project(BaboonRT VERSION 1.0.0 LANGUAGES C CXX)

# --------------------------------------- #
#              FIND PACKAGES              #
# --------------------------------------- #
# Output directories
set(OUTPUT_DIR_BASE "${CMAKE_SOURCE_DIR}/Builds/${CMAKE_CXX_COMPILER_ID}")

message(STATUS "[${PROJECT_NAME}]: Base output directory: ${OUTPUT_DIR_BASE}")

# Use generator expressions directly in output variables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR_BASE}/$<CONFIG>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_DIR_BASE}/$<CONFIG>/libs")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_DIR_BASE}/$<CONFIG>/libs")

#---------------------------------------#
#				SOURCES					#
#---------------------------------------#
file(GLOB_RECURSE SOURCES "Code/src/*.cpp")

add_executable(RayTracer ${SOURCES})

target_include_directories(RayTracer PRIVATE "Code/include")

#---------------------------------------#
#                COMPILE MODE           #
#---------------------------------------#

# Always define BABOON_DEBUG in Debug builds, or if user forces the option
target_compile_definitions(RayTracer PUBLIC
    $<$<CONFIG:Debug>:BABOON_DEBUG>
)

if(CMAKE_CONFIGURATION_TYPES)
    message(STATUS "BABOON_DEBUG_DEFINE: OFF by default, ON when building Debug configuration")
else()
    message(STATUS "BABOON_DEBUG_DEFINE: ${CMAKE_BUILD_TYPE}")
endif()


target_compile_features(RayTracer PUBLIC cxx_std_23)

if (MSVC)
    set(CMAKE_DEBUG_POSTFIX "d")
    target_compile_options(RayTracer PRIVATE /W4 /WX)
else ()
    target_compile_options(RayTracer PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif ()

# Build position independent code.
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Disable C and C++ compiler extensions.
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


#---------------------------------------#
#               LINK LIBRARIES          #
#---------------------------------------#
add_subdirectory(Libs)

find_package(OpenGL REQUIRED)

target_link_libraries(RayTracer PUBLIC glm::glm glfw spdlog OpenGL::GL KQUEUE)
target_link_libraries(RayTracer PRIVATE glad)
target_compile_definitions(RayTracer PUBLIC GLAD_GL_IMPLEMENTATION)

add_custom_command(TARGET RayTracer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/Assets"
            "$<TARGET_FILE_DIR:RayTracer>/Assets"
)