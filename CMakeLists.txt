# CMakeLists files in this project can
# refer to the root source directory of the project as ${MAIN_SOURCE_DIR} and
# to the root binary directory of the project as ${MAIN_BINARY_DIR}.
cmake_minimum_required(VERSION 3.5)

# https://cmake.org/cmake/help/v3.0/command/project.html
project(h265nal VERSION 0.44)

# std::make_ptr requires C++14
set(CMAKE_CXX_STANDARD 14)

# set up clang-tidy
# set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-format-style='google'")
if(WIN32)
  # Designated initializers throughout project requires C++20 on windows
  set(CMAKE_CXX_STANDARD 20)
  if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++20")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
  endif()
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
endif()

enable_testing()

# add executable
#add_executable(main main.cc)

option(H265NAL_SMALL_FOOTPRINT, "small footprint build")

if(H265NAL_SMALL_FOOTPRINT)
  message(STATUS "src: small footprint selected")
  add_compile_definitions(SMALL_FOOTPRINT)
else()
  message(STATUS "src: normal footprint selected")
  add_compile_definitions(FDUMP_DEFINE)
  add_compile_definitions(RTP_DEFINE)
  add_compile_definitions(FPRINT_ERRORS)
endif()

add_library(h265nal)
file(GLOB H265NAL_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")
if(H265NAL_SMALL_FOOTPRINT)
  target_sources(h265nal PRIVATE
      src/rtc_common.cc
      src/h265_common.cc
      src/h265_utils.cc
      src/h265_profile_tier_level_parser.cc
      src/h265_sub_layer_hrd_parameters_parser.cc
      src/h265_hrd_parameters_parser.cc
      src/h265_vps_parser.cc
      src/h265_vui_parameters_parser.cc
      src/h265_pred_weight_table_parser.cc
      src/h265_st_ref_pic_set_parser.cc
      src/h265_sps_multilayer_extension_parser.cc
      src/h265_sps_3d_extension_parser.cc
      src/h265_sps_range_extension_parser.cc
      src/h265_sps_scc_extension_parser.cc
      src/h265_scaling_list_data_parser.cc
      src/h265_sps_parser.cc
      src/h265_pps_scc_extension_parser.cc
      src/h265_pps_parser.cc
      src/h265_aud_parser.cc
      src/h265_sei_parser.cc
      src/h265_slice_parser.cc
      src/h265_bitstream_parser_state.cc
      src/h265_bitstream_parser.cc
      src/h265_nal_unit_header_parser.cc
      src/h265_nal_unit_payload_parser.cc
      src/h265_nal_unit_parser.cc
      src/h265_configuration_box_parser.cc
)
else()
  target_sources(h265nal PRIVATE
      src/rtc_common.cc
      src/h265_common.cc
      src/h265_utils.cc
      src/h265_profile_tier_level_parser.cc
      src/h265_sub_layer_hrd_parameters_parser.cc
      src/h265_hrd_parameters_parser.cc
      src/h265_vps_parser.cc
      src/h265_vui_parameters_parser.cc
      src/h265_pred_weight_table_parser.cc
      src/h265_st_ref_pic_set_parser.cc
      src/h265_sps_multilayer_extension_parser.cc
      src/h265_sps_3d_extension_parser.cc
      src/h265_sps_range_extension_parser.cc
      src/h265_sps_scc_extension_parser.cc
      src/h265_scaling_list_data_parser.cc
      src/h265_sps_parser.cc
      src/h265_pps_scc_extension_parser.cc
      src/h265_pps_parser.cc
      src/h265_aud_parser.cc
      src/h265_sei_parser.cc
      src/h265_rtp_ap_parser.cc
      src/h265_rtp_fu_parser.cc
      src/h265_rtp_single_parser.cc
      src/h265_rtp_parser.cc
      src/h265_slice_parser.cc
      src/h265_bitstream_parser_state.cc
      src/h265_bitstream_parser.cc
      src/h265_nal_unit_header_parser.cc
      src/h265_nal_unit_payload_parser.cc
      src/h265_nal_unit_parser.cc
      src/h265_configuration_box_parser.cc
)
endif()

target_include_directories(
  h265nal
  INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

if (WIN32)
  target_link_libraries(h265nal PUBLIC wsock32 ws2_32)
endif()

# configure_file(config.h.in config.h)

target_sources(
  h265nal
  PUBLIC FILE_SET HEADERS
  BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include"
  FILES ${H265NAL_HEADERS}
)

# target_sources(
#   h265nal
#   PUBLIC FILE_SET HEADERS
#   BASE_DIRS "${CMAKE_CURRENT_BINARY_DIR}"
#   FILES "${CMAKE_CURRENT_BINARY_DIR}/config.h"
# )

# https://cmake.org/cmake/help/latest/guide/tutorial/Adding%20Export%20Configuration.html
install(TARGETS h265nal EXPORT h265nalTargets FILE_SET HEADERS)
install(EXPORT h265nalTargets FILE h265nalTargets.cmake DESTINATION lib/cmake/h265nal)
include(CMakePackageConfigHelpers)
# generate the config file that includes the exports
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/h265nalConfig.cmake"
  INSTALL_DESTINATION "lib/cmake/h265nal"
  NO_SET_AND_CHECK_MACRO
  NO_CHECK_REQUIRED_COMPONENTS_MACRO
)
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/h265nalConfigVersion.cmake"
  VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}"
  COMPATIBILITY AnyNewerVersion
)
install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/h265nalConfig.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/h265nalConfigVersion.cmake
  DESTINATION lib/cmake/h265nal
)


# Recurse into source code subdirectories.
add_subdirectory(tools)
# add_subdirectory(test)

option(BUILD_CLANG_FUZZER "Build clang fuzzer sanitizer targets" ON)

if(BUILD_CLANG_FUZZER)
  add_subdirectory(fuzz)
endif()
