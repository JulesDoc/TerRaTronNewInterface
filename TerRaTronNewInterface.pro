
include(../terrasys.pri)

DEFINES += 
QT += widgets network sql xml
 
SOURCES += \
	main.cpp \
	T_TerRaTronNewInterfaceMainWindow.cpp \
	T_TerRaTronNewInterfaceWidget.cpp \
	T_TerRaTronNewInterfaceObject.cpp
	
 
HEADERS += \  
	T_TerRaTronNewInterfaceMainWindow.hpp \
	T_TerRaTronNewInterfaceWidget.hpp \
	T_TerRaTronNewInterfaceObject.hpp
	

FORMS += \
	forms/TerRaTronNewInterfaceMainWindow.ui \
	forms/TerRaTronNewInterfaceWidget.ui
	
RESOURCES += resources/resources.qrc