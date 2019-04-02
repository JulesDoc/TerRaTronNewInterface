#pragma once

#include <QFile>
#include <QDir>
#include <QBuffer>
#include <QMainWindow>

#include "T_TronMessageContainer.hpp"

class T_TerRaTronNewInterfaceObject: public QObject
{
	Q_OBJECT

public:
	T_TerRaTronNewInterfaceObject();
	~T_TerRaTronNewInterfaceObject();
	
public slots:
	void validate(const QString &);

signals:
	void resultReady(T_TronMessageContainer, T_String);

private:
	QString m_fileName{};
	QFile m_fileHandler;
	QDir m_dir;
};
