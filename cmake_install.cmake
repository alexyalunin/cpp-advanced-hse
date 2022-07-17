# Install script for directory: /home/dnsost/cpp-advanced-hse

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "ASAN")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/dnsost/cpp-advanced-hse/tasks/tasks/multiplication/cmake_install.cmake")
  include("/home/dnsost/cpp-advanced-hse/tasks/tasks/harakiri/cmake_install.cmake")
  include("/home/dnsost/cpp-advanced-hse/tasks/tasks/deque/cmake_install.cmake")
  include("/home/dnsost/cpp-advanced-hse/tasks/tasks/dedup/cmake_install.cmake")
  include("/home/dnsost/cpp-advanced-hse/tasks/tasks/cow-vector/cmake_install.cmake")
  include("/home/dnsost/cpp-advanced-hse/tasks/tasks/compressed_pair/cmake_install.cmake")
  include("/home/dnsost/cpp-advanced-hse/tasks/tasks/lru-cache/cmake_install.cmake")
  include("/home/dnsost/cpp-advanced-hse/tasks/tasks/string-operations/cmake_install.cmake")
  include("/home/dnsost/cpp-advanced-hse/tasks/tasks/intrusive-list/cmake_install.cmake")
  include("/home/dnsost/cpp-advanced-hse/tasks/tasks/list/cmake_install.cmake")
  include("/home/dnsost/cpp-advanced-hse/tasks/tasks/string-view/cmake_install.cmake")
  include("/home/dnsost/cpp-advanced-hse/tasks/tasks/bind_front/cmake_install.cmake")
  include("/home/dnsost/cpp-advanced-hse/tasks/tasks/curry/cmake_install.cmake")
  include("/home/dnsost/cpp-advanced-hse/tasks/tasks/stdflib/cmake_install.cmake")
  include("/home/dnsost/cpp-advanced-hse/tasks/tasks/dungeon/cmake_install.cmake")
  include("/home/dnsost/cpp-advanced-hse/tasks/tasks/fold/cmake_install.cmake")
  include("/home/dnsost/cpp-advanced-hse/tasks/tasks/grep/cmake_install.cmake")
  include("/home/dnsost/cpp-advanced-hse/tasks/tasks/safe-transform/cmake_install.cmake")
  include("/home/dnsost/cpp-advanced-hse/tasks/tasks/tryhard/cmake_install.cmake")
  include("/home/dnsost/cpp-advanced-hse/tasks/tasks/defer/cmake_install.cmake")
  include("/home/dnsost/cpp-advanced-hse/tasks/tasks/algo_spec/cmake_install.cmake")
  include("/home/dnsost/cpp-advanced-hse/tasks/tasks/transform_tuple/cmake_install.cmake")
  include("/home/dnsost/cpp-advanced-hse/tasks/tasks/reflection/cmake_install.cmake")
  include("/home/dnsost/cpp-advanced-hse/tasks/tasks/pipes/cmake_install.cmake")
  include("/home/dnsost/cpp-advanced-hse/tasks/tasks/smart-ptrs/cmake_install.cmake")
  include("/home/dnsost/cpp-advanced-hse/tasks/tasks/brainfuck/cmake_install.cmake")
  include("/home/dnsost/cpp-advanced-hse/tasks/tasks/solve_or_die/cmake_install.cmake")
  include("/home/dnsost/cpp-advanced-hse/tasks/tasks/gcd/cmake_install.cmake")
  include("/home/dnsost/cpp-advanced-hse/tasks/tasks/rule-of-5/cmake_install.cmake")
  include("/home/dnsost/cpp-advanced-hse/tasks/tasks/reallol/cmake_install.cmake")
  include("/home/dnsost/cpp-advanced-hse/tasks/tasks/bad-hash/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/dnsost/cpp-advanced-hse/tasks/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
