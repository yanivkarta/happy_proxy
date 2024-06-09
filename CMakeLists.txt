cmake_minimum_required(VERSION 3.0)

#set the project 
project(happy_proxy-${_system_name}_${_host_id})    

set(ASM_PLATFORM "x86_64")
#set the version
set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
set(VERSION_PATCH 0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
#with ASM support
set(KERNEL_INCLUDE_DIR /usr/src/linux-headers-6.5.0-25-generic/include)

set(KERNEL_TOOLCHAIN_DIR /usr/src/linux-headers-6.5.0-25-generic/tools)

set(KERNEL_ASM_DIR /usr/src/linux-headers-6.5.0-25-generic/arch/x86/include/asm-generic)
#set the build type
set(CMAKE_BUILD_TYPE Debug)

#set the compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -D_GLIBCXX_USE_CXX11_ABI=0 -Wall -Wextra -Wpedantic -Werror")

#set the include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include  ${CMAKE_CURRENT_SOURCE_DIR}/include/util ${CMAKE_CURRENT_SOURCE_DIR}/include/data ${CMAKE_CURRENT_SOURCE_DIR}/include/glue ${CMAKE_CURRENT_SOURCE_DIR}/include/decision_engine  ${CMAKE_CURRENT_SOURCE_DIR}/include/parsers ${CMAKE_CURRENT_SOURCE_DIR}/include/helpers ${CMAKE_CURRENT_SOURCE_DIR}/include/statistics ${CMAKE_CURRENT_SOURCE_DIR}/include/third_party ${CMAKE_CURRENT_SOURCE_DIR}/include/conntrack ${CMAKE_CURRENT_SOURCE_DIR}/include/strategies ${CMAKE_CURRENT_SOURCE_DIR}/include/conntrack ${CMAKE_CURRENT_SOURCE_DIR}/include/conntrack/policies ${CMAKE_CURRENT_SOURCE_DIR}/include   )   



#set HAPPY_PROXY_SOURCES
set(HAPPY_PROXY_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/util/logger.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/util/exception.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/util/interface_info.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/strategies/strategy.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/strategies/strategy_factory.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/proxy.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/proxy_util.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/proxy_config.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/proxy_client.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/proxy_server.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/proxy_acl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/proxy_session.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/proxy_session_manager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/proxy_ev_queues.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/conntrack/conntrack_listener.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/conntrack/conntrack_policy_maker.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/conntrack/iptables_helper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/tls/tls_helper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/util/basic_encoding.cpp 
    #${CMAKE_CURRENT_SOURCE_DIR}/src/sampler/statcollector.cpp 
    #${CMAKE_CURRENT_SOURCE_DIR}/src/conntrack/netlink_helper.cpp
    

   # ${CMAKE_CURRENT_SOURCE_DIR}/src/proxy_ev_loop.cpp
   # ${CMAKE_CURRENT_SOURCE_DIR}/src/proxy_ev_timer.cpp
   # ${CMAKE_CURRENT_SOURCE_DIR}/src/proxy_ev_io.cpp
   # ${CMAKE_CURRENT_SOURCE_DIR}/src/proxy_ev_signal.cpp
   # ${CMAKE_CURRENT_SOURCE_DIR}/src/proxy_ev_child.cpp
   # ${CMAKE_CURRENT_SOURCE_DIR}/src/proxy_ev_idle.cpp
   # ${CMAKE_CURRENT_SOURCE_DIR}/src/proxy_ev_prepare.cpp
   # ${CMAKE_CURRENT_SOURCE_DIR}/src/proxy_ev_check.cpp
   # ${CMAKE_CURRENT_SOURCE_DIR}/src/proxy_ev_async.cpp
   # ${CMAKE_CURRENT_SOURCE_DIR}/src/proxy_ev_stat.cpp
   # ${CMAKE_CURRENT_SOURCE_DIR}/src/proxy_ev_io_watcher.cpp
    
) 

#set the source files 
set(SOURCE_FILES ${HAPPY_PROXY_SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

#set the executable 
add_executable(happy_proxy ${SOURCE_FILES})

#set the libraries add libnetfilter_conntrack, gnutls, libevent, libsqlite3, libmnl, libnftnl 
target_link_libraries(happy_proxy pthread crypto ssl elf nfnetlink netfilter_queue sqlite3 event kqueue aio netfilter_conntrack mnl nftnl gnutls ) 

#set the install target
install(TARGETS happy_proxy DESTINATION bin)

#add compilation flags for the test
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11  -Wno-unused-local-typedefs   -Wall -Wextra -Wno-deprecated  -Werror -fprofile-arcs -ftest-coverage")   

#add tests 
add_subdirectory(tests) 
#add the test target
add_custom_target(test COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure) 
#add the coverage target 
add_custom_target(coverage COMMAND ${CMAKE_COMMAND} -E env CTEST_OUTPUT_ON_FAILURE=1 ${CMAKE_CTEST_COMMAND} --output-on-failure) 

#set the cpack
set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_NAME "happy_proxy")
set(CPACK_PACKAGE_VERSION_MAJOR ${VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${VERSION_PATCH})
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Provallo Team")
set(CPACK_DEBIAN_PACKAGE_DESCRIPTION "Happy Proxy Engine")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libssl-dev, libcrypto++-dev, libelf-dev, libnetfilter-queue-dev, libnfnetlink-dev libsqlite3-dev")

include(CPack)
