cmake_minimum_required(VERSION 3.14)

# 项目名 相当于设置全局变量PROJECT_NAME
project(asio-tutorial)

# C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 库编译

# 查找库，todo
find_package(boost REQUIRED)

# 引入头文件
# include_directories(include)
include_directories(/usr/local/include)

# 引入源文件
file(GLOB SRC_FILES "*.cpp")

# 可执行文件编译
add_executable(${PROJECT_NAME}_timer1 timer1.cpp)
add_executable(${PROJECT_NAME}_timer2 timer2.cpp)
add_executable(${PROJECT_NAME}_timer3 timer3.cpp)
add_executable(${PROJECT_NAME}_timer4 timer4.cpp)
add_executable(${PROJECT_NAME}_timer5 timer5.cpp)

add_executable(${PROJECT_NAME}_daytime1 daytime1.cpp)
add_executable(${PROJECT_NAME}_daytime2 daytime2.cpp)
add_executable(${PROJECT_NAME}_daytime3 daytime3.cpp)

add_executable(${PROJECT_NAME}_daytime4 daytime4.cpp)
add_executable(${PROJECT_NAME}_daytime5 daytime5.cpp)
add_executable(${PROJECT_NAME}_daytime6 daytime6.cpp)

# 将搜索到的头文件路径添加到目标，格式：libname_INCLUDE_DIRS
# target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIRS})

# 引入三方库，格式：libname_LIBRARES
target_link_libraries(${PROJECT_NAME}_timer1 PRIVATE ${boost_LIBRARES})
target_link_libraries(${PROJECT_NAME}_timer2 PRIVATE ${boost_LIBRARES})
target_link_libraries(${PROJECT_NAME}_timer3 PRIVATE ${boost_LIBRARES})
target_link_libraries(${PROJECT_NAME}_timer4 PRIVATE ${boost_LIBRARES})
target_link_libraries(${PROJECT_NAME}_timer5 PRIVATE ${boost_LIBRARES})

target_link_libraries(${PROJECT_NAME}_daytime1 PRIVATE ${boost_LIBRARES})
target_link_libraries(${PROJECT_NAME}_daytime2 PRIVATE ${boost_LIBRARES})
target_link_libraries(${PROJECT_NAME}_daytime3 PRIVATE ${boost_LIBRARES})

target_link_libraries(${PROJECT_NAME}_daytime4 PRIVATE ${boost_LIBRARES})
target_link_libraries(${PROJECT_NAME}_daytime5 PRIVATE ${boost_LIBRARES})
target_link_libraries(${PROJECT_NAME}_daytime6 PRIVATE ${boost_LIBRARES})