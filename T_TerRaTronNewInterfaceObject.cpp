#include <QBuffer>

#include "T_TerRaTronNewInterfaceObject.hpp"
#include "T_TronParser.hpp"
#include "T_TronValidator.hpp"
#include "T_TronContext.hpp"
#include "T_DBTarget.hpp"

T_TerRaTronNewInterfaceObject::T_TerRaTronNewInterfaceObject()
{
	qRegisterMetaType<T_TronMessageContainer>("T_TronMessageContainer");
	qRegisterMetaType<T_String>("T_String");
}

T_TerRaTronNewInterfaceObject::~T_TerRaTronNewInterfaceObject()
{
}

void T_TerRaTronNewInterfaceObject::validate(const QString &rcFileContent) 
{
	qDebug() << Q_FUNC_INFO;
	
	if (m_apDBBundle.get() == 0)
	{
		m_apDBBundle = APT_TronDBBundle(new T_TronDBBundle(T_DBTarget::DEVL, T_DBName::TRS_DB, T_TronContext::ONLINE_VALIDATION));
	}
	T_TronParser parser(*m_apDBBundle);
	T_TronValidator validator(*m_apDBBundle);

	QByteArray fileContentQByte = rcFileContent.toUtf8();
	QBuffer buf(&fileContentQByte);

	bool ok = buf.open(QIODevice::ReadOnly | QIODevice::Text);
	if (ok)
	{
		T_NtcElect rNtcElect;
		//TODO: Change QBuffer to String. Overloaded constructor of parse
		parser.parse(&buf, rNtcElect, "UTF-8");
		ok = validator.validate(rNtcElect);
	
		Q_EMIT resultReady(rNtcElect);
	}
	
}