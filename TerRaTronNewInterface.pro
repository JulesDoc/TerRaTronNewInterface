
include(../terrasys.pri)

DEFINES += 
QT += widgets network sql xml
 
SOURCES += \
	main.cpp \
	T_TerRaTronNewInterfaceMainWindow.cpp \
	T_TerRaTronNewInterfaceWidget.cpp \
	T_TerRaTronNewInterfaceObject.cpp \
	T_TerRaTronNewInterfaceNoticeTreeView.cpp	
	
 
HEADERS += \  
	T_TerRaTronNewInterfaceMainWindow.hpp \
	T_TerRaTronNewInterfaceWidget.hpp \
	T_TerRaTronNewInterfaceObject.hpp \
	T_TerRaTronNewInterfaceNoticeTreeView.hpp \
	

FORMS += \
	forms/TerRaTronNewInterfaceMainWindow.ui \
	forms/TerRaTronNewInterfaceWidget.ui
	
RESOURCES += resources/resources.qrc