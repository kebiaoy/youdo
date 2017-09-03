cmake_minimum_required (VERSION 2.6)
project (UDoHttp)


file(GLOB HTML_FILES "${CMAKE_HOME_DIRECTORY}/../resource/web/html/*")
file(GLOB IMAGES "${CMAKE_HOME_DIRECTORY}/../resource/images/*")
file(GLOB DLL_BINARYS "${CMAKE_HOME_DIRECTORY}/../bin/dll/*.dll")

install(FILES ${HTML_FILES} DESTINATION "${CMAKE_BINARY_DIR}/resource/web/html/")
install(FILES ${IMAGES} DESTINATION "${CMAKE_BINARY_DIR}/resource/images/")
install(FILES ${DLL_BINARYS} DESTINATION "${CMAKE_BINARY_DIR}/Debug/")
 
include_directories("${CMAKE_HOME_DIRECTORY}/../include/")
 
link_directories("${CMAKE_HOME_DIRECTORY}/../bin/lib/")
link_libraries(ws2_32 Shlwapi pcre2-8 pcre2-posix wpcap Packet IPHLPAPI)

Add_Definitions(-DPCRE2_CODE_UNIT_WIDTH=8)
Add_Definitions(-DHAVE_REMOTE)
Add_Definitions(-DUDO_MAC_ADDR_LEN=6)
Add_Definitions(-DUDO_MAC_ADDR_FORMAT_LEN=17)
Add_Definitions(-DUDO_MAC_ADDR_FORMAT_CHAR=':')
Add_Definitions(-DUDO_IP_ADDR_LEN=4)
Add_Definitions(-DUDO_IP_ADDR_FORMAT_LEN=16)
Add_Definitions(-DUDO_ETHERNET_IP=0x0800)
Add_Definitions(-DUDO_ETHERNET_IP=0x0800)
Add_Definitions(-DUDO_LINK_LAYER_LEN=14)

FILE(GLOB_RECURSE MAIN_SRC "./src/*")
FILE(GLOB HTTP_SRC "./src/http/*")
FILE(GLOB SOCK_SRC "./src/sock/*")
FILE(GLOB COMMON_CONFIG_SRC "./src/common/config/*")
FILE(GLOB COMMON_ERROR_SRC "./src/common/error/*")
FILE(GLOB COMMON_STRING_SRC "./src/common/string/*")
FILE(GLOB COMMON_CORE_SRC "./src/common/core/*")
FILE(GLOB MAIN_FILE "./src/*.c")


source_group("src\\" FILES ${MAIN_FILE})
source_group("src\\common\\config" FILES ${COMMON_CONFIG_SRC})
source_group("src\\common\\error" FILES ${COMMON_ERROR_SRC})
source_group("src\\common\\string" FILES ${COMMON_STRING_SRC})
source_group("src\\common\\core" FILES ${COMMON_CORE_SRC})
source_group("src\\http" FILES ${HTTP_SRC})
source_group("src\\sock" FILES ${SOCK_SRC})
add_executable(UDoHttp ${MAIN_SRC})