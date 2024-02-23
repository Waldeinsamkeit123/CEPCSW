# Install script for directory: /afs/ihep.ac.cn/users/z/zhaoxiao/cms/CEPCSW/Digitisers/FanEcalDigi

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/afs/ihep.ac.cn/users/z/zhaoxiao/cms/CEPCSW/InstallArea")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "RelWithDebInfo")
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
  set(CMAKE_INSTALL_SO_NO_EXE "0")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libFanEcalDigi.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libFanEcalDigi.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libFanEcalDigi.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE MODULE FILES "/afs/ihep.ac.cn/users/z/zhaoxiao/cms/CEPCSW/Digitisers/FanEcalDigi/libFanEcalDigi.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libFanEcalDigi.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libFanEcalDigi.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libFanEcalDigi.so"
         OLD_RPATH "/afs/ihep.ac.cn/users/z/zhaoxiao/cms/CEPCSW/.plugins:/cvmfs/cepcsw.ihep.ac.cn/prototype/releases/externals/98.0.0/k4FWCore/lib:/cvmfs/cepcsw.ihep.ac.cn/prototype/releases/externals/98.0.0/Gaudi/lib:/cvmfs/sft.cern.ch/lcg/releases/clhep/2.4.1.3-78165/x86_64-centos7-gcc8-opt/lib:/cvmfs/cepcsw.ihep.ac.cn/prototype/releases/externals/98.0.0/GEAR/lib:/cvmfs/cepcsw.ihep.ac.cn/prototype/releases/externals/98.0.0/LCIO/lib:/cvmfs/sft.cern.ch/lcg/views/LCG_98python3/x86_64-centos7-gcc8-opt/lib:/cvmfs/sft.cern.ch/lcg/releases/ROOT/v6.22.00-be0a0/x86_64-centos7-gcc8-opt/lib:/cvmfs/cepcsw.ihep.ac.cn/prototype/releases/externals/98.0.0/EDM4hep/lib64:/cvmfs/cepcsw.ihep.ac.cn/prototype/releases/externals/98.0.0/podio/lib64:/cvmfs/sft.cern.ch/lcg/views/LCG_98python3/x86_64-centos7-gcc8-opt/lib64:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/cvmfs/sft.cern.ch/lcg/releases/binutils/2.30-e5b21/x86_64-centos7/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libFanEcalDigi.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/python/FanEcalDigi" TYPE FILE FILES
    "/afs/ihep.ac.cn/users/z/zhaoxiao/cms/CEPCSW/Digitisers/FanEcalDigi/genConf/FanEcalDigi/FanEcalDigiConf.py"
    "/afs/ihep.ac.cn/users/z/zhaoxiao/cms/CEPCSW/Digitisers/FanEcalDigi/genConf/FanEcalDigi/__init__.py"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libFanEcalDigi.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libFanEcalDigi.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libFanEcalDigi.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE MODULE FILES "/afs/ihep.ac.cn/users/z/zhaoxiao/cms/CEPCSW/Digitisers/FanEcalDigi/libFanEcalDigi.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libFanEcalDigi.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libFanEcalDigi.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libFanEcalDigi.so"
         OLD_RPATH "/afs/ihep.ac.cn/users/z/zhaoxiao/cms/CEPCSW/.plugins:/cvmfs/cepcsw.ihep.ac.cn/prototype/releases/externals/98.0.0/k4FWCore/lib:/cvmfs/cepcsw.ihep.ac.cn/prototype/releases/externals/98.0.0/Gaudi/lib:/cvmfs/sft.cern.ch/lcg/releases/clhep/2.4.1.3-78165/x86_64-centos7-gcc8-opt/lib:/cvmfs/cepcsw.ihep.ac.cn/prototype/releases/externals/98.0.0/GEAR/lib:/cvmfs/cepcsw.ihep.ac.cn/prototype/releases/externals/98.0.0/LCIO/lib:/cvmfs/sft.cern.ch/lcg/views/LCG_98python3/x86_64-centos7-gcc8-opt/lib:/cvmfs/sft.cern.ch/lcg/releases/ROOT/v6.22.00-be0a0/x86_64-centos7-gcc8-opt/lib:/cvmfs/cepcsw.ihep.ac.cn/prototype/releases/externals/98.0.0/EDM4hep/lib64:/cvmfs/cepcsw.ihep.ac.cn/prototype/releases/externals/98.0.0/podio/lib64:/cvmfs/sft.cern.ch/lcg/views/LCG_98python3/x86_64-centos7-gcc8-opt/lib64:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/cvmfs/sft.cern.ch/lcg/releases/binutils/2.30-e5b21/x86_64-centos7/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libFanEcalDigi.so")
    endif()
  endif()
endif()

