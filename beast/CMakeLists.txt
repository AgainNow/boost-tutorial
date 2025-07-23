cmake_minimum_required(VERSION 3.14)

# 项目名 相当于设置全局变量PROJECT_NAME
project(beast-test)

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
add_executable(${PROJECT_NAME}_http_sync_client http_sync_client.cpp)

# 将搜索到的头文件路径添加到目标，格式：libname_INCLUDE_DIRS
# target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIRS})

# 引入三方库，格式：libname_LIBRARES
target_link_libraries(${PROJECT_NAME}_http_sync_client PRIVATE ${boost_LIBRARES})