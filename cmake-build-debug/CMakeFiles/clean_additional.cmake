# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "BartmossCloud_autogen"
  "CMakeFiles/BartmossCloud_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/BartmossCloud_autogen.dir/ParseCache.txt"
  "CMakeFiles/C_Library_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/C_Library_autogen.dir/ParseCache.txt"
  "C_Library_autogen"
  )
endif()
