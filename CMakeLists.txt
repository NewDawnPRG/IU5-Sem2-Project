cmake_minimum_required(VERSION 3.23)
project(IU5-SEM2-PROJECT)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML REQUIRED COMPONENTS Graphics Window System)

add_subdirectory(board)
add_subdirectory(graphics)
add_subdirectory(piece)

add_executable(${PROJECT_NAME} 
    main.cpp
)

target_link_libraries(${PROJECT_NAME} 
    PRIVATE graphics board piece
    sfml-graphics 
    sfml-window 
    sfml-system
)