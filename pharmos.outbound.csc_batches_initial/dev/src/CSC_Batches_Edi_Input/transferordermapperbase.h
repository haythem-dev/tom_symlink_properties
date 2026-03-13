//-------------------------------------------------------------------------------------
/*! \file  transferordermapperbase.h
*  \brief  superclass for all the specific mapper-classes. 
*  \author Jia Li, Petra Kessling
*  \date   02.01.2013
*  \date   03.04.13 pk rework
*/
//-------------------------------------------------------------------------------------
#ifndef GUARD_DOMMOD_ORDERMAPPER_TRANSFERORDERMAPPERBASE_H
#define GUARD_DOMMOD_ORDERMAPPER_TRANSFERORDERMAPPERBASE_H

#include <libbasardbaspect.h>
#include <libbasarcmnutil_i18nstring.h> 

namespace domMod {
namespace orderMapper {

	class TransferOrderMapperBase 
	{
		public:
			TransferOrderMapperBase(basar::db::aspect::ConnectionRef connectionRef, const basar::I18nString& Countrycode, const basar::I18nString& TransferDir, const basar::I18nString& InputDir, const basar::I18nString& ForBranchnos )
										: m_ConnectionRef(connectionRef), m_Countrycode(Countrycode), m_TransferDir(TransferDir), m_InputDir(InputDir), m_ForBranchnos(ForBranchnos)  {};
			virtual ~TransferOrderMapperBase(){}

			virtual bool doMapInputDir() = 0;			// Complete Mapping for a complete directory full of UEWAUF-Files
	
		protected:
			const basar::db::aspect::ConnectionRef	m_ConnectionRef;	// current database connection
			const basar::I18nString					m_Countrycode;		// "CH" oder "DE"
			const basar::I18nString					m_TransferDir;		// Path and name of the readonly system-directory, 
																		// that contains files with transfer orders
																		//(e.g. /transfer/ksc/data/ch/edi/uw )
			const basar::I18nString					m_InputDir;			// Path and name of the writable directory, 
																		// that contains files with transfer orders
																		// (e.g. /software/wss/ch/ksc/batch/data/edi_input)
			const basar::I18nString					m_ForBranchnos;		// Comma separated list of branchnos, whose orders are accepted by edi_input
																		// (e.g. "06,07,25,57")

		private:
			TransferOrderMapperBase( const TransferOrderMapperBase& );
			TransferOrderMapperBase& operator=( const TransferOrderMapperBase& );
	};

	typedef boost::shared_ptr<TransferOrderMapperBase> TransferOrderMapperBasePtr;

} // namespace orderMapper
} // namespace domMod

#endif // GUARD_DOMMOD_ORDERMAPPER_TRANSFERORDERMAPPERBASE_H
