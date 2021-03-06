cmake_minimum_required(VERSION 2.8.3)
project(auv_navigation)

## Compile as C++11, supported in ROS Kinetic and newer
add_compile_options(-std=c++11)


find_package(catkin REQUIRED COMPONENTS
  auv_core
  roscpp
  geometry_msgs
  tf
  eigen_conversions
)

find_package(Eigen3 REQUIRED)

###########
## Build ##
###########

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES ${PROJECT_NAME}
  CATKIN_DEPENDS roscpp
)

include_directories(
  include 
  ${catkin_INCLUDE_DIRS} 
  ${EIGEN3_INCLUDE_DIRS} 
)

add_executable(test_node src/test_node.cpp)
target_link_libraries(test_node ${catkin_LIBRARIES})
add_dependencies(test_node ${catkin_EXPORTED_TARGETS})

add_library(${PROJECT_NAME} SHARED
  src/kalman_filter.cpp
  src/translation_ekf.cpp
)

target_link_libraries(${PROJECT_NAME} 
  ${catkin_LIBRARIES} 
  ${EIGEN3_LIBRARIES} 
  ${CERES_LIBRARIES}
)

#############
## Install ##
#############

install(TARGETS test_node
        RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

install(DIRECTORY launch
        DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)

install(DIRECTORY include/${PROJECT_NAME}/
        DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
)

install(TARGETS ${PROJECT_NAME}    
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)