
include(../terrasys.pri)

DEFINES += 
QT += widgets network sql xml
 
SOURCES += \
	main.cpp \
	T_MainWindow.cpp
	
 
HEADERS += \  
	T_MainWindow.hpp
	

FORMS += \
	forms/mainWindow.ui
	
RESOURCES += resources/resources.qrc