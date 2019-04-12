#pragma once

#include <QFile>
#include <QDir>

#include "T_NtcElect.hpp"
#include "T_TronDBBundle.hpp"

class T_TerRaTronNewInterfaceObject: public QObject
{
	Q_OBJECT

public:
	T_TerRaTronNewInterfaceObject();
	~T_TerRaTronNewInterfaceObject();
	
public slots:
	void validate(const QString &);

signals:
	void resultReady(const T_NtcElect&);

private:
	QString m_fileName{};
	QFile m_fileHandler;
	QDir m_dir;
	APT_TronDBBundle m_apDBBundle;
};
