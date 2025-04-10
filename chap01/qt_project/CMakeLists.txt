cmake_minimum_required(VERSION 3.16)

project(chap_01 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

add_executable(chap_01
  main.cpp
  englishalphabet.h englishalphabet.cpp
  alphabettools.h alphabettools.cpp
  alphabet.h alphabet.cpp
  exec_01.cpp
  exec_01.h
  exec_05.cpp
  exec_05.h
  book.txt count_1w.txt
  shiftattack.h shiftattack.cpp
  vigenereattack.h vigenereattack.cpp
  helper.h helper.cpp

)
target_link_libraries(chap_01 Qt${QT_VERSION_MAJOR}::Core)

include(GNUInstallDirs)
install(TARGETS chap_01
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
