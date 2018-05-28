FIND_PACKAGE(VTK REQUIRED)
#IF(NOT VTK_USE_RENDERING)
#  MESSAGE(FATAL_ERROR "Example ${PROJECT_NAME} requires VTK_USE_RENDERING.")
#ENDIF(NOT VTK_USE_RENDERING)
INCLUDE(${VTK_USE_FILE})

if("${VTK_MAJOR_VERSION}" GREATER 5)
  set(VTK_RENDERING_LIBS vtkRenderingImage vtkInteractionStyle vtkInteractionImage vtkImagingColor
      vtkRenderingFreeType) # VTK 6.x
  set(VTK_GL_LIBS vtkRenderingOpenGL vtkRenderingFreeTypeOpenGL
      vtkRenderingOpenGL2 vtkRenderingFreeTypeOpenGL2)
  foreach(TMP_LIB ${VTK_GL_LIBS})
    list(FIND VTK_LIBRARIES ${TMP_LIB} TMP_INDEX)
    if(TMP_INDEX GREATER -1)
      set(VTK_RENDERING_LIBS ${VTK_RENDERING_LIBS} ${TMP_LIB})
    endif()
  endforeach()
else()
  set(VTK_RENDERING_LIBS vtkRendering) # VTK 5.x
endif()

set(BASE_LIBS vtkDICOM ${VTK_LIBS})



if(VTK_MAJOR_VERSION GREATER 5 OR VTK_MINOR_VERSION GREATER 9)
add_executable(TestMapToWLC TestMapToWLC.cxx vtkXpImageMapToWindowLevelColors.cxx)
target_compile_definitions(TestMapToWLC PRIVATE 
    VTK_XP_IMAGINGCOLOR_STATIC_DEFINE 
)
target_link_libraries(TestMapToWLC ${BASE_LIBS} ${VTK_RENDERING_LIBS})

# declare BUILD_TESTING by default ON
# if don't want call test use: -DBUILD_TESTING=OFF 
include(CTest)
if(BUILD_TESTING) 
    message ("BUILD_TESTING ON")
    enable_testing()
    #download demo files in zip on configuration state
if (NOT EXISTS "${CMAKE_CURRENT_BINARY_DIR}/demo.zip")
file(DOWNLOAD "http://deckard.mc.duke.edu/~samei/tg18_files/TG18-QC-1k-dcm.zip"
    "${CMAKE_CURRENT_BINARY_DIR}/TG18-QC-1k-dcm.zip" 
    SHOW_PROGRESS 
    EXPECTED_MD5;03d2eb1b6c089ba87faf89932305d9a1
    STATUS dl_status)
message("try... download TG18-QC-1k-dcm.zip - ${dl_status}")
add_custom_command(TARGET TestMapToWLC
  COMMAND ${CMAKE_COMMAND} -E tar xzf "${CMAKE_CURRENT_BINARY_DIR}/TG18-QC-1k-dcm.zip"
  WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
  DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/TG18-QC-1k-dcm.zip"
  COMMENT "Unpacking TG18-QC-1k-dcm.zip"
  VERBATIM
)
endif()
else(BUILD_TESTING)
    message ("BUILD_TESTING OFF")
endif(BUILD_TESTING)

ADD_TEST(do_test TestMapToWLC "${CMAKE_CURRENT_BINARY_DIR}/Tg18-qc/TG18-QC-1k-01.dcm")

ADD_TEST(file_comp_1 ${CMAKE_COMMAND} -E compare_files bmp0.bmp bmp1.bmp)
set_property(TEST file_comp_1 APPEND PROPERTY DEPENDS do_test)

ADD_TEST(file_comp_2 ${CMAKE_COMMAND} -E compare_files bmpCorr0.bmp bmpCorr0.bmp)
set_property(TEST file_comp_1 APPEND PROPERTY DEPENDS do_test)

endif()
