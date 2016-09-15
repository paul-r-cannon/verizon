# Paul R. Cannon
# paul_r_cannon@yahoo.com
# 240-344-6081

project(Verizon)
cmake_minimum_required(VERSION 2.8)

# get opencv library info
find_package(OpenCV REQUIRED)
message("OpenCV_INCLUDE_DIRS: " ${OpenCV_INCLUDE_DIRS})
message("OpenCV_LIBS: " ${OpenCV_LIBS})

# get opengl library info
#find_package(OpenGL REQUIRED)
# note: unfortunately, "/usr/lib/x86_64-linux-gnu/libGL.so" does not like my std::string declaration, so I had to set the list of libs manually in order to use "/usr/lib/nvidia-352/libGL.so" instead (see: http://stackoverflow.com/questions/30950191/declaration-of-stdstring-causes-a-segmentation-fault-with-opengl)
set(OPENGL_LIBRARIES "/usr/lib/x86_64-linux-gnu/libGLU.so;/usr/lib/nvidia-352/libGL.so;/usr/lib/x86_64-linux-gnu/libSM.so;/usr/lib/x86_64-linux-gnu/libICE.so;/usr/lib/x86_64-linux-gnu/libX11.so;/usr/lib/x86_64-linux-gnu/libXext.so")
message("OPENGL_INCLUDE_DIR: " ${OPENGL_INCLUDE_DIR})
message("OPENGL_LIBRARIES: " ${OPENGL_LIBRARIES})

# get glut library info
find_package(GLUT REQUIRED)
message("GLUT_INCLUDE_DIR: " ${GLUT_INCLUDE_DIR})
message("GLUT_LIBRARIES: " ${GLUT_LIBRARIES})

# include directories
include_directories(${OpenCV_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIR} ${GLUT_INCLUDE_DIR})

# library directories
#link_directories(${Boost_LIBRARY_DIRS} ${libssl_LIBRARY_DIRS})
#link_directories(${OpenCV_LIBRARY_DIRS})

# we must do this so that Qt Creator will include our header files as part of the project
#FILE(GLOB_RECURSE HeaderFiles "./*.hpp")

# targets

# display_image
add_executable(display_image display_image.cpp ${HeaderFiles})
target_link_libraries(display_image ${OpenCV_LIBRARIES})

# disparity_map
add_executable(disparity_map disparity_map.cpp ${HeaderFiles})
target_link_libraries(disparity_map ${OpenCV_LIBRARIES})

# generate_point_cloud
add_executable(generate_point_cloud generate_point_cloud.cpp ${HeaderFiles})
target_link_libraries(generate_point_cloud ${OpenCV_LIBRARIES})

# test_opengl
add_executable(test_opengl test_opengl.cpp ${HeaderFiles})
target_link_libraries(test_opengl ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})

# render_point_cloud
add_executable(render_point_cloud render_point_cloud.cpp ${HeaderFiles})
target_link_libraries(render_point_cloud ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})

# camera_calibration
add_executable(camera_calibration camera_calibration.cpp ${HeaderFiles})
target_link_libraries(camera_calibration ${OpenCV_LIBRARIES})

# stereo_rectify_images
add_executable(stereo_rectify_images stereo_rectify_images.cpp ${HeaderFiles})
target_link_libraries(stereo_rectify_images ${OpenCV_LIBRARIES})
