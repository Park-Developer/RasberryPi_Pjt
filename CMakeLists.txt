# 요구 CMake 최소 버전
CMAKE_MINIMUM_REQUIRED ( VERSION 2.8 )
 
# 프로젝트 이름 및 버전
PROJECT ( "Wono_o_Bot" )
SET ( PROJECT_VERSION_MAJOR 1 )
SET ( PROJECT_VERSION_MINOR 0 )
 
# 빌드 형상(Configuration) 및 주절주절 Makefile 생성 여부
SET ( CMAKEwh_BUILD_TYPE Debug )
SET ( CMAKE_VERBOSE_MAKEFILE true )
 
# 빌드 대상 바이너리 파일명 및 소스 파일 목록
SET ( OUTPUT_ELF
        "${CMAKE_PROJECT_NAME}-${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.out"
        )
        
SET ( SRC_FILES
        communi.c
        initial_set.c
        interruption.c
        motion.c
        sensor.c
        main.c
        )
 
# 공통 컴파일러
SET ( CMAKE_C_COMPILER "gcc" )
# 공통 헤더 파일 Include 디렉토리 (-I)
INCLUDE_DIRECTORIES ( include )
ADD_COMPILE_OPTIONS( -g -Wall -lwiringPi -lpthread -lm)
# 빌드 대상 바이너리 추가
ADD_EXECUTABLE( ${OUTPUT_ELF} ${SRC_FILES} )


# Link against wiringPi
# gcc -o main main.c motion.c sensor.c interruption.c initial_set.c -lwiringPi -lpthread -lm
find_library(WIRINGPI_LIBRARIES NAMES wiringPi)
find_library(PTHREAD_LIBRARIES NAMES pthread)
find_library(M_LIBRARIES NAMES m)

target_link_libraries(${OUTPUT_ELF} ${WIRINGPI_LIBRARIES})
target_link_libraries(${OUTPUT_ELF} ${PTHREAD_LIBRARIES})
target_link_libraries(${OUTPUT_ELF} ${M_LIBRARIES})