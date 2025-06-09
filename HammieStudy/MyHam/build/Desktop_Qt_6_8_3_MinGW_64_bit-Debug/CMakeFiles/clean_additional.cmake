# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\MyHam_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\MyHam_autogen.dir\\ParseCache.txt"
  "MyHam_autogen"
  )
endif()
