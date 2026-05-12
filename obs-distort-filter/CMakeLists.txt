cmake_minimum_required(VERSION 3.16...3.30)

project(obs-distort-filter VERSION 1.0.0)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Option 1: Build inside OBS source tree (libobs target already exists)
# Option 2: Standalone build with find_package(libobs)
# Option 3: Standalone build with LIBOBS_INCLUDE_DIR / LIBOBS_LIB set
add_library(obs-distort-filter MODULE)
target_sources(obs-distort-filter PRIVATE obs-distort-filter.c)

if(TARGET libobs)
  target_link_libraries(obs-distort-filter PRIVATE libobs)
elseif(DEFINED LIBOBS_INCLUDE_DIR AND DEFINED LIBOBS_LIB)
  target_include_directories(obs-distort-filter PRIVATE ${LIBOBS_INCLUDE_DIR})
  target_link_libraries(obs-distort-filter PRIVATE ${LIBOBS_LIB})
else()
  find_package(libobs REQUIRED)
  target_link_libraries(obs-distort-filter PRIVATE OBS::libobs)
endif()

if(WIN32)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/windows/obs-module.rc.in
    ${CMAKE_CURRENT_BINARY_DIR}/obs-distort-filter.rc
  )
  target_sources(obs-distort-filter PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/obs-distort-filter.rc)
endif()

set_target_properties(
  obs-distort-filter
  PROPERTIES
    FOLDER plugins
    PREFIX ""
)

# Install the plugin binary
install(TARGETS obs-distort-filter
  LIBRARY DESTINATION obs-plugins/64bit
  RUNTIME DESTINATION obs-plugins/64bit
)

# Install data files
install(FILES data/distort_filter.effect
  DESTINATION data/obs-plugins/obs-distort-filter
)
install(DIRECTORY data/locale
  DESTINATION data/obs-plugins/obs-distort-filter
)
