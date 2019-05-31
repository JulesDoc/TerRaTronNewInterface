
#include "T_TerRaTronNewInterfaceObject.hpp"
#include "T_TronParser.hpp"
#include "T_TronValidator.hpp"
#include "T_TronContext.hpp"
#include "T_DBTarget.hpp"

void T_TerRaTronNewInterfaceObject::validate(const QString &rcFileContent) 
{
	PRECONDITION(!rcFileContent.isEmpty());

	if (m_apDBBundle.get() == 0)
	{
		m_apDBBundle = APT_TronDBBundle(new T_TronDBBundle(T_DBTarget::DEVL, T_DBName::TRS_DB, T_TronContext::ONLINE_VALIDATION));
	}
	T_TronParser parser(*m_apDBBundle);
	T_TronValidator validator(*m_apDBBundle);
	T_NtcElect rNtcElect;
		
	parser.parse(rcFileContent, rNtcElect);
	validator.validate(rNtcElect);
	
	POSTCONDITION(!rNtcElect.isUnknown());
	//Signal emitted to Widget to start showing results
	Q_EMIT resultReady(rNtcElect);
}