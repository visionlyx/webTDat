#cmake file version 1.0
#author behnam sabaghi
#change time 22 / 7 / 96


cmake_minimum_required(VERSION 3.1)

PROJECT(webTDat_viewer)

IF(COMMAND CMAKE_POLICY)
	   CMAKE_POLICY(SET CMP0071 NEW)
ENDIF(COMMAND CMAKE_POLICY)


FIND_PACKAGE(TIFF)
IF (TIFF_FOUND)
	include_directories(${TIFF_INCLUDE_DIR})
ELSE(TIFF_FOUND)
	MESSAGE(FATAL_ERROR "Cannot find libtiff.  Please set TIFF_DIR.")
ENDIF(TIFF_FOUND)

find_package( OpenCV REQUIRED )
include_directories(${OpenCV_INCLUDE_DIRS})



INCLUDE_DIRECTORIES(
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${VTK_INCLUDE_DIRS}
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5Gui_INCLUDE_DIRS}
    ${QT_INCLUDE_DIR}
)

SET(SRC_FILES 
main.cpp 
mainwindow.cpp
./Render/VTKmain.cpp
./Render/TDatStyle.cpp
./TDatypes/ROI.cpp
./TDatypes/TDataset.cpp
./IOThreader/Volmereader.cpp
./IOThreader/Volmer2Actor.cpp
./IOThreader/webtiffreader.cpp
./dock/datamanagerdock.cpp
) 
#同理，查找当前文件夹中所有的头文件
SET( HEAD_FILES 
mainwindow.h
./Render/VTKmain.h
./Render/TDatStyle.h
./TDatypes/ROI.h
./TDatypes/TDataset.h
./IOThreader/Volmereader.h
./IOThreader/Volmer2Actor.h
./IOThreader/webtiffreader.h
./dock/datamanagerdock.h
) 


SET( UI_FILES 
./mainwindow.ui
./dock/datamanagerdock.ui
) 


#find pakages
find_package(VTK REQUIRED)
find_package(Qt5Core )
find_package(Qt5Widgets )
find_package(Qt5Gui)
find_package(Qt5NetWork)
 
#set configs
set(CMAKE_AUTOUIC ON)

#includes
include_directories(
    ./src
    ${VTK_INCLUDE_DIRS}
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5Gui_INCLUDE_DIRS}
    ${QT_INCLUDE_DIR}
	${Qt5NetWork_INCLUDE_DIR}
    )
	
include(${VTK_USE_FILE} ${QT_USE_FILE})


SET(QT_UIC_EXECUTABLE ${VTK_QT_UIC_EXECUTABLE} CACHE FILEPATH "")

QT5_WRAP_UI(UISrcs ${UI_FILES})
QT5_WRAP_CPP(MOCSrcs ${HEAD_FILES} )
SOURCE_GROUP("Generated" FILES
  ${MOCSrcs}
   ${UISrcs}
 )
source_group("Ui" FILES ${UI_FILES} ${UISrcs} )
ADD_EXECUTABLE(${PROJECT_NAME} ${SRC_FILES} ${HEAD_FILES} ${UI_FILES}  ${VTK_USE_FILE} ${MOCSrcs} ${UISrcs})


#linking
        target_link_libraries(${PROJECT_NAME}
            Qt5::Core
            Qt5::Widgets
			Qt5::Gui
			Qt5::Network
 ${TIFF_LIBRARIES} 

           ${VTK_LIBRARIES} ${QT_LIBRARIES} ${OpenCV_LIBS}
         
            )


