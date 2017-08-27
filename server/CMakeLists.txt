cmake_minimum_required (VERSION 2.6)
project (UDoHttp)
link_libraries(ws2_32 Shlwapi)


file(GLOB HTML_FILES "${CMAKE_HOME_DIRECTORY}/../resource/web/html/*.html")
file(GLOB LOGO "${CMAKE_HOME_DIRECTORY}/../resource/images/LOGO/*.png")
install(FILES ${HTML_FILES} DESTINATION "${CMAKE_BINARY_DIR}/resource/web/html/")
install(FILES ${HTML_FILES} DESTINATION "${CMAKE_BINARY_DIR}/resource/web/html/")
install(FILES ${LOGO} DESTINATION "${CMAKE_BINARY_DIR}/resource/images/LOGO/")

FILE(GLOB_RECURSE MAIN_SRC "./src/*")
FILE(GLOB HTTP_SRC "./src/http/*")
FILE(GLOB COMMON_CONFIG_SRC "./src/common/config/*")
FILE(GLOB COMMON_ERROR_SRC "./src/common/error/*")
FILE(GLOB MAIN_FILE "./src/*.c")


source_group("src\\" FILES ${MAIN_FILE})
source_group("src\\common\\config" FILES ${COMMON_CONFIG_SRC})
source_group("src\\common\\error" FILES ${COMMON_ERROR_SRC})
source_group("src\\http" FILES ${HTTP_SRC})
add_executable(UDoHttp ${MAIN_SRC})