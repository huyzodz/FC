# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "E:/K49 CTU tdh/Big_project/luanvan/FC/tmp/Target_1/default/main+Target_1")
  file(MAKE_DIRECTORY "E:/K49 CTU tdh/Big_project/luanvan/FC/tmp/Target_1/default/main+Target_1")
endif()
file(MAKE_DIRECTORY
  "E:/K49 CTU tdh/Big_project/luanvan/FC/tmp/Target_1/default/1"
  "E:/K49 CTU tdh/Big_project/luanvan/FC/tmp/Target_1/default/main+Target_1"
  "E:/K49 CTU tdh/Big_project/luanvan/FC/tmp/Target_1/default/main+Target_1/tmp"
  "E:/K49 CTU tdh/Big_project/luanvan/FC/tmp/Target_1/default/main+Target_1/src/main+Target_1-stamp"
  "E:/K49 CTU tdh/Big_project/luanvan/FC/tmp/Target_1/default/main+Target_1/src"
  "E:/K49 CTU tdh/Big_project/luanvan/FC/tmp/Target_1/default/main+Target_1/src/main+Target_1-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "E:/K49 CTU tdh/Big_project/luanvan/FC/tmp/Target_1/default/main+Target_1/src/main+Target_1-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "E:/K49 CTU tdh/Big_project/luanvan/FC/tmp/Target_1/default/main+Target_1/src/main+Target_1-stamp${cfgdir}") # cfgdir has leading slash
endif()
