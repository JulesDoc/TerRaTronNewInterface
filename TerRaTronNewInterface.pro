
include(../terrasys.pri)

DEFINES += 
QT += widgets network sql xml testlib concurrent
 
SOURCES += \
	main.cpp \
	T_TerRaTronNewInterfaceMainWindow.cpp \
	T_TerRaTronNewInterfaceWidget.cpp \
	T_TerRaTronNewInterfaceObject.cpp \
	T_TerRaTronNewInterfaceNoticeTreeView.cpp\
	T_Configuration.cpp
	
 
HEADERS += \  
	T_TerRaTronNewInterfaceMainWindow.hpp \
	T_TerRaTronNewInterfaceWidget.hpp \
	T_TerRaTronNewInterfaceObject.hpp \
	T_TerRaTronNewInterfaceNoticeTreeView.hpp \
	T_Configuration.hpp
	

FORMS += \
	forms/TerRaTronNewInterfaceMainWindow.ui \
	forms/TerRaTronNewInterfaceWidget.ui \
	forms/TerRaTronNewInterfaceNoticeTreeView.ui
	
RESOURCES += resources/resources.qrc