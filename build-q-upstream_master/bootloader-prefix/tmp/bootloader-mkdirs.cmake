# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file LICENSE.rst or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "/Users/bash/esp/esp-idf/components/bootloader/subproject")
  file(MAKE_DIRECTORY "/Users/bash/esp/esp-idf/components/bootloader/subproject")
endif()
file(MAKE_DIRECTORY
  "/Users/bash/dev/src/github.com/bash0C7/R2P2-ESP32/build-q-upstream_master/bootloader"
  "/Users/bash/dev/src/github.com/bash0C7/R2P2-ESP32/build-q-upstream_master/bootloader-prefix"
  "/Users/bash/dev/src/github.com/bash0C7/R2P2-ESP32/build-q-upstream_master/bootloader-prefix/tmp"
  "/Users/bash/dev/src/github.com/bash0C7/R2P2-ESP32/build-q-upstream_master/bootloader-prefix/src/bootloader-stamp"
  "/Users/bash/dev/src/github.com/bash0C7/R2P2-ESP32/build-q-upstream_master/bootloader-prefix/src"
  "/Users/bash/dev/src/github.com/bash0C7/R2P2-ESP32/build-q-upstream_master/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/bash/dev/src/github.com/bash0C7/R2P2-ESP32/build-q-upstream_master/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/bash/dev/src/github.com/bash0C7/R2P2-ESP32/build-q-upstream_master/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
