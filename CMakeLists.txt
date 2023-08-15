cmake_minimum_required(VERSION 3.25)

project(Hook VERSION 0.0.1)

set(EXE_NAME "Hook")
set(SRC_DIR "src")
set(INCLUDE_DIR "include")
set(SONGS_DIR "songs")

file(GLOB C_FILES "${SRC_DIR}/*.c")
file(GLOB H_FILES "${INCLUDE_DIR}/*.h")

configure_file(${INCLUDE_DIR}/config.in ${INCLUDE_DIR}/config.h)

add_executable(${EXE_NAME} ${C_FILES})

target_include_directories(${EXE_NAME} PUBLIC ${INCLUDE_DIR})

if(WIN32)
    # ws2_32
    target_link_libraries(${EXE_NAME} PRIVATE ws2_32)
endif(WIN32)


install(TARGETS ${EXE_NAME} RUNTIME DESTINATION bin)
install(FILES ${H_FILES} DESTINATION include)
install(FILES "${PROJECT_BINARY_DIR}/${INCLUDE_DIR}/config.h" DESTINATION include)
install(DIRECTORY ${SONGS_DIR} DESTINATION bin)

include(InstallRequiredSystemLibraries)