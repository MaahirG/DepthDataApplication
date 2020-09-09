cmake_minimum_required(VERSION 3.5.1)

# set the project name
project(recording)

find_package( OpenCV REQUIRED )
find_package( realsense2 REQUIRED )

add_executable(${PROJECT_NAME}.out ./src/getFrames.cpp)
set_property(TARGET ${PROJECT_NAME}.out PROPERTY CXX_STANDARD 11)

# Link libraries
target_link_libraries( ${PROJECT_NAME}.out ${realsense2_LIBRARY})
target_link_libraries( ${PROJECT_NAME}.out ${OpenCV_LIBS} )

#set_target_properties (${PROJECT_NAME}.out PROPERTIES
#	FOLDER "Examples/OpenCV"
#)

install(
	TARGETS

	${PROJECT_NAME}.out

	RUNTIME DESTINATION
	${CMAKE_INSTALL_PREFIX}/bin
)