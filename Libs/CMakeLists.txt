# --------------------------------------- #
#               CMAKE CONFIG              #
# --------------------------------------- #

# ----------- GLFW ----------- #
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(glfw)



# ----------- GLM ----------- #
set(DGLM_BUILD_TESTS OFF CACHE BOOL "" FORCE)
add_subdirectory(glm)

# ----------- SPDLOG ----------- #
add_subdirectory(spdlog)

# ----------- GLAD ----------- #
add_library(glad STATIC
   glad/src/glad.c
)

target_include_directories(glad SYSTEM PUBLIC
    glad/include
)