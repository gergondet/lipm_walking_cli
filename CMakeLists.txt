cmake_minimum_required(VERSION 3.1)

find_package(mc_rtc REQUIRED)

add_executable(sn_walking_cli src/main.cpp)
target_link_libraries(sn_walking_cli PRIVATE mc_rtc::mc_control_client)
install(TARGETS sn_walking_cli DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
