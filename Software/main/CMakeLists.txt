file(GLOB_RECURSE SOURCES "*.cpp" "*.c")

idf_component_register(SRCS "main.cpp" "${SOURCES}"
                    INCLUDE_DIRS ".")
target_compile_options(${COMPONENT_LIB} PRIVATE -std=gnu++23)
