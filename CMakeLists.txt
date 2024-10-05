# Beetle/CMakeLists.txt
# Created by Robin Rowe 2024-10-05
# License Open Source GPL3

cmake_minimum_required(VERSION 3.8)
set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(Beetle)
message("Configuring ${CMAKE_PROJECT_NAME}...")

if(NOT WIN32 AND NOT APPLE)
	link_libraries(rt pthread)
endif(NOT WIN32 AND NOT APPLE)

option(SUPPRESS "Enable suppression of code warnings" true)
if(SUPPRESS)
	if(WIN32)
		add_definitions(-D_CRT_SECURE_NO_WARNINGS /wd4244 /wd4305 /wd4018 /wd26451 /wd6031 /wd4267)
	endif(WIN32)
endif(SUPPRESS)

option(UNISTD "Enable libunistd" true)
if(UNISTD)
	if(WIN32)
#		message("CMAKE_GENERATOR_PLATFORM = ${CMAKE_GENERATOR_PLATFORM}")
		set(LIBUNISTD_PATH /code/github/libunistd)
		include_directories(${LIBUNISTD_PATH}/unistd)
		set (UNISTD_LINK_DIRECTORIES ${LIBUNISTD_PATH}/build/${CMAKE_GENERATOR_PLATFORM}/Release)
		message(libunistd = ${UNISTD_LINK_DIRECTORIES}/libunistd.lib)
		link_directories(${UNISTD_LINK_DIRECTORIES})
		link_libraries(libunistd Ws2_32 Imm32)
	endif(WIN32)
endif(UNISTD)

option(FLTK "Enable FLTK" false)
if(FLTK)
	set(FLTK_PATH /Code/fltk)
	include_directories(${FLTK_PATH} ${FLTK_PATH}/fltk-build ${FLTK_PATH}/fltk-src/)
	link_directories(${FLTK_PATH}/fltk-src/build/lib/Debug)
	set(FLTK_LIBS
		fltk_formsd
		fltk_gld
		fltk_imagesd
		fltk_jpegd
		fltk_pngd
		fltk_zd
		fltkd
	)
	link_libraries(${FLTK_LIBS} comctl32)
endif(FLTK)

option(GTK1 "Enable GTK1" false)
if(GTK1)
	set(GTK_INCLUDE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/gtk1/gtk1win)
	set(GTK_INCLUDE_DIRS 
		${GTK_INCLUDE_PATH}
		${GTK_INCLUDE_PATH}/glib 
		${GTK_INCLUDE_PATH}/gdk)
	include_directories(${GTK_INCLUDE_DIRS})
endif(GTK1)
#add_subdirectory(./doc)
add_subdirectory(./src)
#add_subdirectory(./tests)
link_libraries(libbeetle)
add_executable(beetle ./src/main.c)

