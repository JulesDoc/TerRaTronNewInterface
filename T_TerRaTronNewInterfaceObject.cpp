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

void T_TerRaTronNewInterfaceObject::validate(const QString &fileContent) 
{
	qDebug() << Q_FUNC_INFO;

	T_TronDBBundle aDBBundle(T_DBTarget::DEVL, T_DBName::TRS_DB, T_TronContext::ONLINE_VALIDATION);
	T_TronParser parser(aDBBundle);
	T_TronValidator validator(aDBBundle);

	QByteArray fileContentQByte = fileContent.toUtf8();
	QBuffer buf(&fileContentQByte);

	bool ok = buf.open(QIODevice::ReadOnly | QIODevice::Text);
	if (ok)
	{
		T_NtcElect rNtcElect;
		T_String messages;
		T_TronMessageContainer messageContainer;

		parser.parse(&buf, rNtcElect);
		ok = validator.validate(rNtcElect);
		
		rNtcElect.getMessagesAsString(messages);
		rNtcElect.getMessageContainer(messageContainer);

		Q_EMIT resultReady(messageContainer, messages);
	}
	
}