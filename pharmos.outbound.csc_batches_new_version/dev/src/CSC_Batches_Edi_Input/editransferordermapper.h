//-------------------------------------------------------------------------------------
/*! \file  editransferordermapper.h
*  \brief  Maps the Phoenix-Inhouse-Format (=UEWAUF)-Files) of a given directory into TransferOrders
*  \author Jia Li, P. Kessling
*  \date   02.01.2013
*  history: 03.04.13 pk rework
*/
//-------------------------------------------------------------------------------------
#ifndef GUARD_DOMMOD_ORDERMAPPER_EDITRANSFERORDERMAPPER_H
#define GUARD_DOMMOD_ORDERMAPPER_EDITRANSFERORDERMAPPER_H

#include <boost/filesystem.hpp>	// includes all needed Boost.Filesystem declarations for directories
#include <boost/filesystem/fstream.hpp> // fuer filestream
#include <libbasardbaspect_connectionref.h>

#include "ediline_definitions.h"
#include "editransferordermapper_definitions.h"
#include "transferordermapperbase.h"

//Protocol
#include "orderheadprotocoldm.h"
#include "orderposcollectionprotocoldm.h"

#include "editransferorder.h"

#include <string>

#include "infrastructure/mail/mailalertptr.h"


namespace domMod {
namespace orderMapper {

	class EdiTransferOrderMapper : public TransferOrderMapperBase
	{
	public:
		//domMod::orderMapper::EdiTransferOrderMapper etom(strInputDir, m_AppSession->m_DBConnection->getCurrentConnection());	// +connection

		EdiTransferOrderMapper(basar::db::aspect::ConnectionRef connectionRef,
								const basar::I18nString& Countrycode,
								const basar::I18nString& TransferDir,
								const basar::I18nString& InputDir,
								const basar::I18nString& ForBranchnos)
			:  TransferOrderMapperBase(connectionRef, Countrycode, TransferDir, InputDir, ForBranchnos)
			{
				// Check and set CountryCode parameter
				if ("CH" == Countrycode)
				{
					m_Inputtype = enInputtype_inhCH;
				}
				else // "DE1" or "DE2" == Countrycode
				{
					m_Inputtype = enInputtype_inhDE;
				}

	            m_OrderHeadProtocol.init(connectionRef);
	            m_OrderPosCollectionProtocol.init(connectionRef);

				//  Check and set ForBranchnos parameter	(e.g. "06,17,57")
				//  Hint: Could be solved using BString::tokenizeAndTrim() also
				std::string				strForbranchnos(ForBranchnos);
				std::string::size_type	anf, end;
				basar::I18nString		token;
				basar::Int16			branchno;

				// first begin
				anf = strForbranchnos.find_first_not_of(PARAM_FORBRANCHNOS_SEPARATORS);

				// as long as tokenbegin is found
				while ( anf != std::string::npos)
				{
					// find end for last anf
					end = strForbranchnos.find_first_of(PARAM_FORBRANCHNOS_SEPARATORS, anf);		// look for separator beginning from anf

					// maybe we reached the end of string
					if (end == std::string::npos)
					{
						end = strForbranchnos.length();
					}

					// store branchno in vector
					token = strForbranchnos.substr(anf, end-anf);		// get end-anf chars beginning from anf
					if ( token.isDigit() )								// Non-numeric values are ignored
					{
						std::istringstream is (token);
						is >> branchno;

						//branchno = token.stoi();
						m_Branchnos.push_back(branchno);
					}

					// find next begin
					anf = strForbranchnos.find_first_not_of(PARAM_FORBRANCHNOS_SEPARATORS, end);
				}

			} ;	// Param is transferred to constructor of base class (N.J. S. 271)

		~EdiTransferOrderMapper() {};

		bool doMoveFromTransferDirToInputDir();	// Moves uewauf-flatfiles from transferdir to inputdir
		bool doMapInputDir();					// Mapping for a complete directory full of UEWAUF-Files
		bool doCleanInputDir();					// Deleting old and correctly read files of the input-directory with UEWAUF-Files

		void injectMailAlert(abbaUW::infrastructure::mail::MailAlertPtr mailAlert);

	private:
		EdiTransferOrderMapper( const EdiTransferOrderMapper& );			// abschalten
		EdiTransferOrderMapper& operator=( const EdiTransferOrderMapper& );	// abschalten

		bool isFileAlreadyExisting(const boost::filesystem::path& filePath, const basar::I18nString& errorMessage);

		bool isUWARDirectoryDeletable(const boost::filesystem::path& filePath);

		bool doCopyFlatfile(const boost::filesystem::path& pathTransferDir,
							const boost::filesystem::path& pathInputDir);					// Moved a single UEWAUF file from transfer to input directory
		bool doMapFlatfile(const boost::filesystem::directory_entry& dirEntry, basar::I18nString& errorList);	// Mapping for a single UEWAUF file
		//bool doDeleteFlatfile(const boost::filesystem::directory_entry& dirEntryFlatfile);	// Delete a single UEWAUF file

		bool isStartOfAnOrder(const domMod::line::LineTypeEnum lineType);					// Does this line contain the start of a new order?
		bool isEndOfAnOrder(const domMod::line::LineTypeEnum lineType);						// Does this line contain the end of an order?
		bool isMappableFlatfile(const boost::filesystem::directory_entry& dirEntry);		// Ist es ein einzulesendes Flatfile?
		bool isFilenameOfaMappableFlatfileCH(const basar::I18nString& fileName);			// Is the filname correct for Switzerland?
		bool isFilenameOfaMappableFlatfileCHPint(const basar::I18nString& fileName);		// Is the filname correct for Switzerland processed by PINT?
		bool isFilenameOfaMappableFlatfileDE(const basar::I18nString& fileName);			// Is the filname correct for Germany?

		//bool isFlatfileToBeCleaned(const boost::filesystem::directory_entry& dirEntry);	// Ist es ein aufzuraeumendes Flatfile?
		//bool isFilenameOfaCleanableFlatfileCH(const basar::I18nString& fileName);			// Is the filname correct for Switzerland?
		//bool isFilenameOfaCleanableFlatfileDE(const basar::I18nString& fileName);			// Is the filname correct for Germany?

		basar::I18nString getTodaysUWARDirectoryName();
		basar::I18nString getERRORDirectoryName();

		void tickOffFlatfile(const boost::filesystem::directory_entry& dirEntryFlatfile);	// Append file extention ".ok"
																							// Change orderstartline: UNB->XNB, BON added

		void tickOffOrderInFlatfileCH(boost::filesystem::fstream& fileStream, const basar::Int32 ordernoBatch,
									  const std::ostream::pos_type unbPositionInFile); // Change orderstartline: 00UNOB->X0UNOB, BON added

		void tickOffOrderInFlatfileDE(boost::filesystem::fstream& fileStream, const basar::Int32 ordernoBatch,
									  const std::ostream::pos_type unbPositionInFile, const bool isToBeIgnored);

		void saveOrderHeadProtocol( const domMod::transferOrderDM::EdiTransferOrder& );
	    void saveOrderPosCollectionProtocol( const domMod::transferOrderDM::EdiTransferOrder& );

		void sendErrorMail(const basar::I18nString& errorFileName, const basar::I18nString& errorList);


		InputtypeEnum				m_Inputtype;	// Format of inputfile with transferorders to be read
													// Remark: Would be a member of parent-object better, but where would we have to define the InputType-Type then?
		std::vector<basar::Int16>	m_Branchnos;	// Vector for branchnos, whose orders are accepted

	    domMod::libAbbauw::orderProtocol::OrderHeadProtocolDM			m_OrderHeadProtocol;
	    domMod::libAbbauw::orderProtocol::OrderPosCollectionProtocolDM  m_OrderPosCollectionProtocol;

		abbaUW::infrastructure::mail::MailAlertPtr						m_MailAlert;
	};

} // namespace orderMapper
} // namespace domMod

#endif // GUARD_DOMMOD_ORDERMAPPER_EDITRANSFERORDERMAPPER_H
