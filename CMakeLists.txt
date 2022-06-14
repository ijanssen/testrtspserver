cmake_minimum_required(VERSION 3.5)

project(rtsp-server LANGUAGES C)

find_package(PkgConfig REQUIRED)

pkg_check_modules(GSTREAMER REQUIRED gstreamer-1.0)
pkg_check_modules(GSTRTSP REQUIRED gstreamer-rtsp-server-1.0)
pkg_check_modules(GSTREAMER_PLUGINS REQUIRED gstreamer-plugins-base-1.0)
pkg_check_modules(GSTREAMER_APP REQUIRED gstreamer-app-1.0)
pkg_check_modules(GSTREAMER_PB_UTILS REQUIRED gstreamer-pbutils-1.0)
pkg_check_modules(GSTREAMER_VIDEO REQUIRED gstreamer-video-1.0)
pkg_check_modules(GSTREAMER_RTP REQUIRED gstreamer-rtp-1.0)

include_directories(
    ${GSTREAMER_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(rtsp-server main.c)

target_link_libraries(rtsp-server
    ${GSTREAMER_VIDEO_LIBRARIES}
    ${GSTREAMER_RTP_LIBRARIES}
    ${GSTREAMER_LIBRARIES}
    ${GSTRTSP_LIBRARIES})
