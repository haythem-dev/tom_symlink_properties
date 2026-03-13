//-------------------------------------------------------------------------------------------------//
/*! \file	editransferordermapper.cpp
 *  \brief  Maps the Phoenix-Inhouse-Format (=UEWAUF)-Files) of a given directory into TransferOrders
 *  \brief  Concentrates all file and directory handling concerning flatfiles
*  \author Jia Li, P. Kessling
*  \date   02.01.2013
*  history: 03.04.13 pk rework
 */
//-------------------------------------------------------------------------------------------------//

#include <string>
#include <sstream>				// for stringstream
#include <iomanip>				// for stringstream
#include <exception>			// for exceptions
#include <ctime>				// for checking cleaning-time
#include <classinfo_macros.h>	// for BLOG_TRACE_METHOD-Makros
#include "loggerpool.h"			// for LoggerPool
#include "abbauwexception.h"
#include "unprocessableexception.h"	//??? necessary? correct one??
#include "orderprotocolexception.h"

#include <boost/filesystem.hpp>   // includes all needed Boost.Filesystem declarations for directories
#include <boost/filesystem/fstream.hpp> // fuer filestream
#include <libbasarcmnutil_bstring.h>
#include <libbasarcmnutil_i18nstring.h>

#include "abbauworderdm.h"

#include "ediline_definitions.h"
#include "edilinefactory.h"
#include "edilineidentifier.h"
#include "edifactline.h"

#include "editransferorder.h"

#include "flatfileordirexception.h"
#include "editransferordermapper_definitions.h"
#include "editransferordermapper.h"

#include "edi_input_properties_definitions.h"

#include "transactionguard.h"

#include "orderprotocol_definitions.h"

#include <libbasardbaspect.h>

#include "infrastructure/mail/mailalert.h"


namespace domMod {
namespace orderMapper {

	// Moves uewauf-flatfiles from transferdir to inputdir
	// returns: true: everything was ok,  false: error(s) occurred
	bool EdiTransferOrderMapper::doMoveFromTransferDirToInputDir()
	{

		METHODNAME_DEF( EdiTransferOrderMapper, doMoveFromTransferDirToInputDir )
		BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

		basar::Int16 countFlatfilesProcessed	= 0;	// all flatfiles in transferdir, that should be moved
		basar::Int16 countFlatfilesMoved		= 0;	// number of flatfiles, that could be moved to inputdir
		bool fileCopiedCorrectly				= true;
		bool retval								= true;

		try
		{
			// Is the transfer-directory ok?
			boost::filesystem::path transferDir( m_TransferDir.c_str() );

			if ( !exists(transferDir) )
			{
				std::stringstream str;
				str << "Transfer directory does not exist: \" " << m_TransferDir << "\" " << std::endl;
				throw libabbauw::FlatfileOrDirException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}

			if ( !is_directory(transferDir) )
			{
				std::stringstream str;
				str << "Parameter -transferDir does not contain a directory \" " << m_TransferDir << "\" " << std::endl;
				throw libabbauw::FlatfileOrDirException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}

			// Is the transfer error directory ok?

			// If it is not existing, create it
			boost::filesystem::path transferErrorDir( m_TransferDir.c_str() );
			transferErrorDir /= "ERROR";

			if ( !exists(transferErrorDir) )
			{
				boost::filesystem::create_directory(transferErrorDir);
				BLOG_INFO( abbaUW::LoggerPool::LoggerAbbaUW, "Transfer error directory created \"" + transferErrorDir.string() + "\"" );
			}

			// Is the input-directory ok?
			boost::filesystem::path inputDir ( m_InputDir.c_str());

			if ( !exists(inputDir) )
			{
				std::stringstream str;
				str << "Input directory does not exist: \" " << m_InputDir << "\" " << std::endl;
				throw libabbauw::FlatfileOrDirException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}

			if ( !is_directory(inputDir) )
			{
				std::stringstream str;
				str << "Parameter -inputdir does not contain a directory \" " << m_InputDir << "\" " << std::endl;
				throw libabbauw::FlatfileOrDirException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}

			BLOG_INFO( abbaUW::LoggerPool::LoggerAbbaUW, "Moving flatfiles from transfer directory \"" + m_TransferDir + "\"" );
			BLOG_INFO( abbaUW::LoggerPool::LoggerAbbaUW, "                  into input directory    \"" + m_InputDir + "\"" );

			// Open transfer directory and loop for all files in the directory
			boost::filesystem::directory_iterator endDirItr; // default construction yields past-the-end

			for ( boost::filesystem::directory_iterator currDirItr( transferDir ); currDirItr != endDirItr; ++currDirItr )
			{
				if(boost::filesystem::is_directory(*currDirItr))
				{
					continue;
				}

				if ( !isMappableFlatfile(*currDirItr) )
				{
					// It is not a valid flatfile: move to Error directory, don't count
					boost::filesystem::path transferErrorDirFile = transferErrorDir;
					transferErrorDirFile /= currDirItr->path().filename();

					boost::filesystem::rename(*currDirItr, transferErrorDirFile);

					BLOG_INFO( abbaUW::LoggerPool::LoggerAbbaUW, "Flatfile not mappable, moved to Error directory: \"" + transferErrorDirFile.string() + "\"" );
                    continue;
				}

				// we got a flatfile to be copied
				++countFlatfilesProcessed;

				//	now we process it
				fileCopiedCorrectly = doCopyFlatfile(*currDirItr, inputDir);

				if (fileCopiedCorrectly)
				{
					++countFlatfilesMoved;

					//File copied successfully, now we can delete the file from the transfer directory
					boost::filesystem::remove(*currDirItr);
					BLOG_INFO( abbaUW::LoggerPool::LoggerAbbaUW, "Flatfile removed from directory: \"" + currDirItr->path().string() + "\"" );
				}
				else
				{
					//File not copied, means it is already in the input directory or another error - move file to Error directory
					boost::filesystem::path transferErrorDirFile = transferErrorDir;
					transferErrorDirFile /= currDirItr->path().filename();

					boost::filesystem::rename(*currDirItr, transferErrorDirFile);

					BLOG_INFO( abbaUW::LoggerPool::LoggerAbbaUW, "Flatfile not copied, moved to Error directory: \"" + transferErrorDirFile.string() + "\"" );
				}
			}

		}
		catch (basar::Exception& e)
		{
			std::stringstream str;
			str << "Basar-Error during moving flatfiles from transfer to input directory: " << e.what() << std::endl;
			BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());

			retval = false;
		}
		catch (std::exception& e)
		{
			std::stringstream str;
			str << "Basar-Error during moving flatfiles from transfer to input directory: " << e.what() << std::endl;
			BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());

			retval = false;
		}
		catch (...)
		{
			std::stringstream str;
			str << "Basar-Error during moving flatfiles from transfer to input directory: Unknown exception! " << std::endl;
			BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());

			retval = false;
		}

		// Log result summary
		{
			std::stringstream stringStream;

			stringStream << "End moving flatfiles from transfer directory \"" << m_TransferDir << "\"" << std::endl;
			stringStream << "                      to input directory \""	   << m_InputDir << "\"" << std::endl;
			stringStream << "   ===> Summary: Number of flatfiles processed: " << countFlatfilesProcessed << std::endl;
			stringStream << "                                        moved: " << countFlatfilesMoved << std::endl;
			stringStream << "                                    not moved: " << (countFlatfilesProcessed - countFlatfilesMoved) << std::endl;

			BLOG_INFO( abbaUW::LoggerPool::LoggerAbbaUW, stringStream.str().c_str() );
		}

		return retval;
	}

	// Copying a single UEWAUF file from source directory into destination directory
	// All file handling tasks are concentrated in this method.
	// Params:  pathSourceDirFile - complete path of the flatfile to be processed
	// Params:  pathDestDir       - path of the destination directory
	// returns: true: flatfile was successfully moved.  false: not moved. Needs not to be an error.
	// throws:	flatfileOrDirException
	bool EdiTransferOrderMapper::doCopyFlatfile(const boost::filesystem::path& pathSourceDirFile, const boost::filesystem::path& pathDestDir)
	{
		METHODNAME_DEF( EdiTransferOrderMapper, doCopyFlatfile )
		BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

		bool wasCopied = false; // all copy activities have been ok so far

		try
		{
			// complete destination file path
			boost::filesystem::path pathDestDirFile = pathDestDir;
			pathDestDirFile /= pathSourceDirFile.filename();

			// Does the destination file already exist in inputDir?
			if ( exists(pathDestDirFile) )
			{
				std::stringstream str;
				str << "   File exists in Input directory, not copied: \" "
					<< pathSourceDirFile.filename() << "\" " << std::endl;
				BLOG_TRACE( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());

				wasCopied = false;	// this is ok
				return wasCopied;
			}

			// Does the destination file already exist with Extention ".ok" in inputDir?
			boost::filesystem::path pathDestDirFileChecked = pathDestDir;
			pathDestDirFileChecked /= pathSourceDirFile.filename().string() + FLATFILE_WAS_CORRECTLY_READ_EXTENTION;

			if ( exists(pathDestDirFileChecked))
			{
				std::stringstream str;
				str << "   <file>.ok exists in Input directory, not moved: \" "
					<< pathSourceDirFile.filename() << "\" " << std::endl;
				BLOG_TRACE( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());

				wasCopied = false;	// this is ok
				return wasCopied;
			}

			// Check if file is existing in a sub directory
			boost::filesystem::directory_iterator endDirItr; // default construction yields past-the-end

			for ( boost::filesystem::directory_iterator directoryIterator( pathDestDir ); directoryIterator != endDirItr; ++directoryIterator )
			{
				if(!boost::filesystem::is_directory(directoryIterator->path()))
				{
					continue;
				}
				else
				{
					boost::filesystem::path x = directoryIterator->path();
					x /= pathSourceDirFile.filename();

					if(isFileAlreadyExisting(x, "   File exists in Input directory, not moved: \" "))
					{
						wasCopied = false;
						return wasCopied;
					}

					x = x.string() + FLATFILE_WAS_CORRECTLY_READ_EXTENTION;

					if(isFileAlreadyExisting(x, "   <file>.ok exists in Input directory, not moved: \" "))
					{
						wasCopied = false;
						return wasCopied;
					}
				}
			}

			// copy file
			boost::filesystem::copy( pathSourceDirFile, pathDestDirFile );
			wasCopied = true;

			// Log result
			{
				std::stringstream stringStream;
				stringStream << "   File copied: " << pathSourceDirFile.filename() << std::endl;

				BLOG_INFO( abbaUW::LoggerPool::LoggerAbbaUW, stringStream.str().c_str());
			}
		}
		catch (const boost::filesystem::filesystem_error& e)
		{
			// e.g. file cannot be copied, if it already exists
			std::stringstream str;
			str << "Error on copying flatfile " << pathSourceDirFile.filename()
				<< " " <<e.what() << std::endl;
			BLOG_INFO( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());

			// Do not throw: Not necessarily serious!
			wasCopied = false;
		}
		catch (std::exception& e)
		{
			std::stringstream str;
			str << "Error on copying flatfile_ " << pathSourceDirFile.filename()
				<< " " <<e.what() << std::endl;

			// Only serious errors should appear here, because throw ends the whole loop for the copy of transfer-directory
			//throw libabbauw::FlatfileOrDirException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			wasCopied = false;
		}
		catch (...)
		{
			std::stringstream str;
			str << "Error on copying flatfile " << pathSourceDirFile.filename()
				<< ": Unknown exception! " << std::endl;
			BLOG_INFO( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());

			// Only serious errors should appear here, because throw ends the whole loop for the copy of transfer-directory
			//throw libabbauw::FlatfileOrDirException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			wasCopied = false;
		}

		return wasCopied;
	}

	bool EdiTransferOrderMapper::isFileAlreadyExisting(const boost::filesystem::path& filePath, const basar::I18nString& errorMessage)
	{
		METHODNAME_DEF( EdiTransferOrderMapper, isFileAlreadyExisting )
		BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

		if(exists(filePath))
		{
			std::stringstream stringStream;
			stringStream << errorMessage << filePath.string() << "\" " << std::endl;

			BLOG_TRACE( abbaUW::LoggerPool::LoggerAbbaUW, stringStream.str().c_str());

			return true;
		}

		return false;
	}

	// Mapping for a complete directory full of UEWAUF-flatfiles
	// This method handles the the input-Directory and loops the set of files inside
	//
	// returns: true: everything was ok,  false: error(s) occurred
	bool EdiTransferOrderMapper::doMapInputDir()
	{
		METHODNAME_DEF( EdiTransferOrderMapper, doMapInputDir )
		BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

		basar::Int16 countFlatfilesProcessed			= 0;	// all flatfiles, that have been processed
		basar::Int16 countFlatfilesProcessedCorrectly	= 0;	// number of flatfiles, that could be read completely
																// and therefore could be renamed to uewauf.xxxxxx.UNIX.ok
		bool fileMappedCorrectly = false;
		bool retval				 = false;

		try
		{
			// Is the input-directory ok?
			boost::filesystem::path inputDir( m_InputDir.c_str() );
			if ( !exists(inputDir) )
			{
				std::stringstream str;
				str << "Input directory does not exist: \" " << m_InputDir << "\" " << std::endl;
				throw libabbauw::FlatfileOrDirException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}

			if ( !is_directory(inputDir) )
			{
				std::stringstream str;
				str << "Parameter -inputdir does not contain a directory \" " << m_InputDir << "\" " << std::endl;
				throw libabbauw::FlatfileOrDirException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}

			basar::I18nString todaysUWARDirectoryName = getTodaysUWARDirectoryName();
			basar::I18nString errorDirectoryName = getERRORDirectoryName();

			boost::filesystem::path todaysUWARDirectory( inputDir );
			todaysUWARDirectory /= todaysUWARDirectoryName.c_str();

			boost::filesystem::path errorDirectory(inputDir);
			errorDirectory /= errorDirectoryName.c_str();

			if( !exists(todaysUWARDirectory) )
			{
				boost::filesystem::create_directory(todaysUWARDirectory);
				BLOG_INFO( abbaUW::LoggerPool::LoggerAbbaUW, "Create UWAR directory \"" + todaysUWARDirectory.string() + "\"" );
			}

			if (!exists(errorDirectory))
			{
				boost::filesystem::create_directory(errorDirectory);
				BLOG_INFO(abbaUW::LoggerPool::LoggerAbbaUW, "Create ERROR directory \"" + errorDirectory.string() + "\"");
			}



			// Logging
			BLOG_INFO( abbaUW::LoggerPool::LoggerAbbaUW, "Processing flatfile input directory \"" + m_InputDir + "\"" );

			// Open directory and loop for all files in the input directory
			boost::filesystem::directory_iterator endDirItr; // default construction yields past-the-end

			for ( boost::filesystem::directory_iterator currDirItr( inputDir ); currDirItr != endDirItr; ++currDirItr )
			{
				basar::I18nString errorList;

				if ( !isMappableFlatfile(*currDirItr) )
				{
					continue;
				}

				// we got a flatfile to be processed
				++countFlatfilesProcessed;
				fileMappedCorrectly = false;

				// now we process it
				fileMappedCorrectly = doMapFlatfile(*currDirItr, errorList);

				if ( fileMappedCorrectly )
				{
					++countFlatfilesProcessedCorrectly;

					// move mapped file into UWAR sub directory

					boost::filesystem::path source( currDirItr->path().string() +  FLATFILE_WAS_CORRECTLY_READ_EXTENTION.c_str());

					boost::filesystem::path destination( todaysUWARDirectory );
					destination /= (currDirItr->path().filename() += FLATFILE_WAS_CORRECTLY_READ_EXTENTION.c_str());

					boost::filesystem::rename(source, destination);
				}
				else //could not process the file
				{
					// move mapped file into ERROR sub directory

					boost::filesystem::path source(currDirItr->path().string());

					boost::filesystem::path destination(errorDirectory);
					destination /= (currDirItr->path().filename() += FLATFILE_WAS_NOT_CORRECTLY_READ_EXTENTION.c_str());

					boost::filesystem::rename(source, destination);

					sendErrorMail(source.string(), errorList);
				}
			}

			retval = true;
		}
		catch (basar::Exception& e)
		{
			std::stringstream str;
			str << "Basar-Error during handling flatfile input directory: " << e.what() << std::endl;
			BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());

			retval = false;
		}
		catch (std::exception& e)
		{
			std::stringstream str;
			str << "Error during handling flatfile input directory: " << e.what() << std::endl;
			BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());

			retval = false;
		}
		catch (...)
		{
			std::stringstream str;
			str << "Error during handling flatfile input directory: Unknown exception! " << std::endl;
			BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());

			retval = false;
		}

		// Log result summary
		{
			std::stringstream str;
			str << "End processing flatfile input directory \""	<< m_InputDir << "\"" << std::endl;
			str << "   ===> Summary: Number of flatfiles processed: " << countFlatfilesProcessed << std::endl;
			str << "                        processed correctly   : " << countFlatfilesProcessedCorrectly << std::endl;
			str << "                        processed with error  : " << (countFlatfilesProcessed - countFlatfilesProcessedCorrectly) << std::endl;

			BLOG_INFO( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
		}

		return retval;
	}

	void EdiTransferOrderMapper::sendErrorMail(const basar::I18nString& errorFileName, const basar::I18nString& errorList)
	{
		std::stringstream body;
		body << "" 
			<< "Error processing file: " << errorFileName << std::endl
			<< std::endl
			<< "Error list:" << std::endl	
			<< errorList << std::endl;
		m_MailAlert->send("Error CSC Batches Edi Input", body.str());
	}

	basar::I18nString EdiTransferOrderMapper::getTodaysUWARDirectoryName()
	{
		METHODNAME_DEF( EdiTransferOrderMapper, getTodaysUWARDirectoryName )
		BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

		std::stringstream stringStream;
		stringStream <<	basar::Date::getCurrent().getYear()
					 << basar::Date::getCurrent().getMonth()
					 << basar::Date::getCurrent().getDay();

		return "UWAR." + basar::Date::getCurrent().toString("YYYYmmdd");;
	}

	basar::I18nString EdiTransferOrderMapper::getERRORDirectoryName()
	{
		METHODNAME_DEF(EdiTransferOrderMapper, getERRORirectoryName)
		BLOG_TRACE_METHOD(abbaUW::LoggerPool::LoggerAbbaUW, fun);

		return "ERROR";
	}

	// Mapping for a single UEWAUF file
	// All file handling tasks are concentrated in this method.
	// Params:  dirEntryFlatfile - directory entry of the flatfile to be processed
	// returns: true: All transfer orders in the flatfile were successfully processed and the file was renamed  false: else
	// throws:	flatfileOrDirException
	//
	// Remark: This could also be implemented with a small state machine:
	//           States would be InsideOrder / outsideOrder
	//			 Then separate FSM-Logik from work-Logik
	bool EdiTransferOrderMapper::doMapFlatfile(const boost::filesystem::directory_entry& dirEntryFlatfile, basar::I18nString& errorList)
	{
		METHODNAME_DEF( EdiTransferOrderMapper, doMapFlatfile )
		BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

		basar::Int16 countOrders		= 0;	// all orders, that have been successfully processed in this file
		basar::Int32 countPositions		= 0;	// all positions, that have been successfully processed in this file
		basar::Int16 countLines			= 0;	// all lines, that have been read in this file
		bool		 everythingOkInFile	= true;// all lines and orders in this file have been ok so far

		domMod::transferOrderDM::EdiTransferOrder	currEdiTransferOrder;	// This object serves as assembling place for one transfer order at a time.
													// It holds the currently read, assembled, completed and stored transfer order.												// at the endstored to orderhead/orderpos-tables.
													// After delivery to orderhead/pos database, this object is cleaned and the
													// method can assemble the next order in this object.
		domMod::libAbbauw::abbauwOrderDM::AbbauwOrderDM abbauwOrderDM;		// Domain Module, handles db operations for a complete order with positions
		abbauwOrderDM.init(m_ConnectionRef, m_InputDir );

		libabbauw::TransactionGuard guard( m_ConnectionRef );

		try
		{
			// extract filedata
			boost::filesystem::path filePath	= dirEntryFlatfile.path();
			basar::I18nString fileName			= dirEntryFlatfile.path().filename().string();

			// open file  (the file is closed automatically when fstream is destroyed (end of scope))
			boost::filesystem::fstream fileStream(filePath);
			if(!fileStream)
			{
				return false;
			}
			{
				// Loggen
				std::stringstream str;
				str << "Processing file \"" << fileName << "\"";
				BLOG_INFO( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
			}


			// Init state
			bool insideOrder						= false;	// from UNB / UNOB/UNOC to UNZ line
			bool insideOrderWithError				= false;	// some error has occurred in this order. Order has to be skipped.
			std::ostream::pos_type currLineStartPos	= 0;		// Startposition of the line in work

			// Zeilen abarbeiten
			std::string line;

			while( true )
			{
				// get current fileposition before reading the line
				currLineStartPos = fileStream.tellg();

				// Read next line
				if (!std::getline( fileStream, line ) )
				{
					// file was completely read
					break;
				}
				++countLines;

				{
					// Tracen
					std::stringstream str;
					str << "Line at position " << currLineStartPos << ": " << line.c_str();
					BLOG_TRACE( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
				}

				// Identify line
				domMod::line::EdiLineIdentifier ediLineIdentifier(m_Inputtype);
				domMod::line::LineTypeEnum lineTypeEnum = ediLineIdentifier.identifyEdiLine(line);

				// Does an order start?
				if ( isStartOfAnOrder(lineTypeEnum) )
				{
					// Wrong status?
					if ( insideOrder )
					{
						// Log error. Former order will be disrupted and not stored.
						std::stringstream str;
						str << "Error: Unexpected start of a new order! Old order disrupted. Char pos: \""
							<< currEdiTransferOrder.getOrderStartPos() << "\"";
						
						BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());

						errorList += "- " + str.str();

						everythingOkInFile = false;
					}

					// Clean and prepare the assembly place for the next order
					currEdiTransferOrder.empty();
					currEdiTransferOrder.setInputtype(m_Inputtype);


					// In case of a UNB- or UNOB/UNOC-line a new transfer order starts:
					// Keep fileposition in mind for later marking as read (XNB)
					currEdiTransferOrder.setOrderStartPos(currLineStartPos);

					// set status "inside of an order"
					insideOrder = true;
				}

				if ( !insideOrder )
				{
					// dont bother with this line outside of an order
					continue;
				}

				// Create object for line and put it into pointer for baseclass.
				// From now on use polymorphism
				domMod::line::EDIFACTLineBasePtr lineBasePtr(domMod::line::EdiLineFactory::create( lineTypeEnum, m_Inputtype ));

				// Extract line content and check for correctness
				try
				{
					lineBasePtr->process(line);
				}
				catch (libabbauw::UnprocessableException& e)
				{
					std::stringstream str;
					str << "Error: Cannot process a line: " << e.what() << std::endl;
					BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
					errorList += "- " + str.str();

					// An unprocessable line may not end the whole read loop
					insideOrderWithError = true;
					continue;
				}

				if (   !lineBasePtr->isValidLine())
				{
					// There is a wrong line inside this order!
					// Keep in mind, log and continue with next line
					// ToDo kes: Watch out: in case of an invalid UNB-line we nevertheless started an Order! Perhaps we shouldn't.
					std::stringstream str;
					str << "Error: Cannot map line: \"" << line.c_str() << "\"" << std::endl;
					BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
					errorList += "- " + str.str();

					insideOrderWithError = true;
					continue;
				}

				// Mount line into transfer order
				if ( !currEdiTransferOrder.mountLine(lineBasePtr))
				{
					// The line could not be mounted!
					// Keep in mind, log and continue with next line without storing this one
					std::stringstream str;
					str << "Error: Cannot mount line to transfer order: \"" << line.c_str() << "\"" << std::endl;
					BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
					errorList += "- " + str.str();

					insideOrderWithError = true;
					continue;
				}

				// Does the order end here?
				if ( isEndOfAnOrder(lineTypeEnum) )
				{
					// TransferOrder toBeIgnored() - complete() - valid() - store()

					// Complete the information inside transfer order from outbound-database
					if ( !abbauwOrderDM.completeInfoFromEdiTransferOrder(currEdiTransferOrder))
					{
						// Could not complete transfer order!
						// Keep in mind, log and end order
						std::stringstream str;
						str << "Error: Cannot complete information for transfer order on char-position: \""
							<< currEdiTransferOrder.getOrderStartPos() << "\"" << std::endl;
						BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
						errorList += "- " + str.str();

						insideOrderWithError = true;
					}

					if (   currEdiTransferOrder.isToBeIgnored(m_Branchnos))
					{
						// This order has to be ignored: Don't put its data into orderhead, orderpos. But:Tick Off the order with 'Y' instead of 'X'
						// Keep in mind, log and continue with next line
						// ToDo kes: Watch out: in case of an invalid UNB-line we nevertheless started an Order! Perhaps we shouldn't.
						std::stringstream str;
						str << "INFO: Ignored Transfer order (ticked off with Y) starts on char-position: \""
							<< currEdiTransferOrder.getOrderStartPos() << "\""
							<< " BON: " << "\""
							<< currEdiTransferOrder.getOrdernoBatch() << "\"";

						BLOG_INFO( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());

						//insideOrderWithError = true;
					}
					// Check that no syntax issues etc. occurred in this order
					else if ( insideOrderWithError )
					{
						// Current transfer order is not valid!
						// Keep in mind, log and end order
						std::stringstream str;
						str << "Error: Transfer order is not valid. Char-position: \""
							<< currEdiTransferOrder.getOrderStartPos() << "\"" << std::endl;
						BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
						errorList += "- " + str.str();
					}
					// Check that this order has got all its necessary components
					else if ( !currEdiTransferOrder.isValid())
					{
						// Current transfer order is not valid!
						// Keep in mind, log and end order
						std::stringstream str;
						str << "Error: Transfer order misses mandatory lines. Char-position: \""
							<< currEdiTransferOrder.getOrderStartPos() << "\"" << std::endl;
						BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
						errorList += "- " + str.str();

						insideOrderWithError = true;
					}
					else
					{
						// Start the DB transaction for writing one order
					    guard.beginTransaction();

						if ( !abbauwOrderDM.saveInsertAbbauwOrder(currEdiTransferOrder))
						{
							guard.rollbackTransaction();

							// Current transfer order could not be stored!
							// Keep in mind, log and end order
							std::stringstream str;
							str << "Error: Transfer order could not be stored to database. Char-position: \""
								<< currEdiTransferOrder.getOrderStartPos() << "\"" << std::endl;
							BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
							errorList += "- " + str.str();

							insideOrderWithError = true;
						}
						else
						{
	                        guard.commitTransaction();
						}
					}

					// Was everything successful inside this order?
					if (!insideOrderWithError)
					{
						// Mark UNBline of order as read with "XNB" for CH resp. X0UNOB for DE
						// and with ordernoBatch ..
						if (enInputtype_inhCH == m_Inputtype)
						{
							// e.g. CH : change „UNB7601001339958 7601001358034 UEWAUF“
							//           to  	„XNB7601001339958 BON1234567890 UEWAUF“

							tickOffOrderInFlatfileCH (  fileStream
													,currEdiTransferOrder.getOrdernoBatch()
													,currEdiTransferOrder.getOrderStartPos());
						}
						else // "DE1" or "DE2" == Countrycode
						{
							// e.g. DE : change „00UNOB268758258                           PH                76539419                           PH                14070910312665975    ...“
							//           to  	„X0UNOB268758258                           PH                76539419                           PH                14070910312665975    ...“: order was transmitted
							//           or to  "Y0UNOB268758258                           PH                76539419                           PH                14070910312665975    ...“: order was ignored
							tickOffOrderInFlatfileDE (  fileStream
												,currEdiTransferOrder.getOrdernoBatch()
												,currEdiTransferOrder.getOrderStartPos()
												,currEdiTransferOrder.isToBeIgnored(m_Branchnos));
						}

						++countOrders;
						countPositions += currEdiTransferOrder.getCntPositions();

                        //Protocol it!
                        if (0 != currEdiTransferOrder.getOrdernoBatch())
                        {
                            saveOrderHeadProtocol(currEdiTransferOrder);
                            saveOrderPosCollectionProtocol(currEdiTransferOrder);
                        }
					}
					else
					{
						// We found an order that cannot be transformed, completed or that is not valid
						// and therefore will never be delivered!! Serious error! Log it and return error!
						std::stringstream str;
						str << "Error: Cannot map order: File, Startpos: " << fileName
							<<" , "<< currEdiTransferOrder.getOrderStartPos() << std::endl;
						BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
						errorList += "- " + str.str();

						everythingOkInFile = false;
					}

					// Set status "outside of an order"
					// and do the work for marking it as read (XNB)
					insideOrder = false;
					insideOrderWithError = false;
				}
			}

			// Log result summary of this file
			{
				std::stringstream str;
				str << "                                 orders/positions inserted: "
											<< countOrders << " / "	<< countPositions
											<< "  Lines read: "					<< countLines;
				BLOG_INFO( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
			}
			
		}
		catch (std::exception& e)
		{
			std::stringstream str;
			str << "Error during handling of a flatfile: " << e.what() << std::endl;
			errorList += "- " + str.str();

			// The transaction is automatically rolled bach by the destructor

			// Only serious errors should appear here, because throw ends the whole loop for the input-directory
			throw libabbauw::FlatfileOrDirException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
		}
		catch (...)
		{
			std::stringstream str;
			str << "Error reading flatfile: Unknown exception! " << std::endl;
			errorList += "- " + str.str();

			// The transaction is automatically rolled bach by the destructor

			// Only serious errors should land here, because throw ends the whole loop for the input-directory
			throw libabbauw::FlatfileOrDirException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
		}


		// If everything in this file was ok and no exception was caught ...
		// ... mark the file as correctly read by renaming it: New extention ".ok"
		if ( everythingOkInFile )
		{
			tickOffFlatfile(dirEntryFlatfile);
		}

		return everythingOkInFile;
	}

    void EdiTransferOrderMapper::saveOrderHeadProtocol( const domMod::transferOrderDM::EdiTransferOrder& ediTransferOrder )
    {
        METHODNAME_DEF( EdiTransferOrderMapper, saveOrderHeadProtocol )
		BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

        try
        {
            basar::db::aspect::AccessorPropertyTable_YIterator yitHead = m_OrderHeadProtocol.getHead();

            yitHead.setInt16(  libabbauw::properties::BRANCHNO,             ediTransferOrder.getBranchno()                  );
            yitHead.setInt32(  libabbauw::properties::ORDERNO,              ediTransferOrder.getOrdernoBatch()              );
            yitHead.setString( libabbauw::properties::ACTIVITY_TYPE,        libAbbauw::orderProtocol::CREATE                );
            yitHead.setInt16(  libabbauw::properties::EVENT_TYPE,           libAbbauw::orderProtocol::NO_EVENT_TYPE         );
            yitHead.setInt16(  libabbauw::properties::PROCESSED_BY,         libAbbauw::orderProtocol::CSC_BATCHES_EDI_INPUT );
            yitHead.setString( libabbauw::properties::CHANGED_ATTRIBUTE,    "" );
            yitHead.setString( libabbauw::properties::OLD_VALUE,            "" );
            yitHead.setString( libabbauw::properties::NEW_VALUE,            "" );
            yitHead.setInt32(  libabbauw::properties::SPLIT_FROM,           0  );
            yitHead.setInt32(  libabbauw::properties::SPLIT_TO,             0  );
            yitHead.setString( libabbauw::properties::HEADERTEXT,           ediTransferOrder.getHeadertext() );
            yitHead.setInt32(  libabbauw::properties::ORDERNOCSC,           0  );
            yitHead.setString( libabbauw::properties::EXTRATEXT,            "" );

            m_OrderHeadProtocol.protocolCreateOrder(yitHead);
        }
        catch(...)
        {
            std::stringstream str;
			str << "Error during saving order head protocol " << std::endl;
            BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str() );

            throw libabbauw::OrderProtocolException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
        }
    }

    void EdiTransferOrderMapper::saveOrderPosCollectionProtocol( const domMod::transferOrderDM::EdiTransferOrder& ediTransferOrder )
    {
        METHODNAME_DEF( EdiTransferOrderMapper, saveOrderPosCollectionProtocol )
		BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

        try
        {
            basar::db::aspect::AccessorPropertyTable_YIterator yitPos = m_OrderPosCollectionProtocol.getPos();

            yitPos.setInt16(   libabbauw::properties::BRANCHNO,            ediTransferOrder.getBranchno()                  );
            yitPos.setInt32(   libabbauw::properties::ORDERNO,             ediTransferOrder.getOrdernoBatch()              );
            yitPos.setString(  libabbauw::properties::ACTIVITY_TYPE,       libAbbauw::orderProtocol::CREATE                );
            yitPos.setInt16(   libabbauw::properties::EVENT_TYPE,          libAbbauw::orderProtocol::NO_EVENT_TYPE         );
            yitPos.setInt16(   libabbauw::properties::PROCESSED_BY,        libAbbauw::orderProtocol::CSC_BATCHES_EDI_INPUT );
            yitPos.setString(  libabbauw::properties::VIEW,                ""                                              );
            yitPos.setInt32(   libabbauw::properties::ORDERREQUESTQTY,     0                                               );
            yitPos.setString(  libabbauw::properties::ITEMTEXT,            ""                                              );
            yitPos.setString(  libabbauw::properties::EXTRATEXT,           ""                                              );

            unsigned int i = 0;
            if( enInputtype_inhCH == m_Inputtype )
            {
                while( i < ediTransferOrder.getPositionSegments().size() )
                {
                    yitPos.setInt32(   libabbauw::properties::POSNO,           i+1                                                                                          );
                    yitPos.setInt32(   libabbauw::properties::ARTICLENO,       ediTransferOrder.getPositionSegments().at(i)->getArticleno()                                 );

					yitPos.setInt32(   libabbauw::properties::ORDEREDQTY,      ediTransferOrder.getPositionSegments().at(i)->getQtyLine_OrderedQtyPtr()->getQuantity() );	// =QTY_21	(Gesamte Menge: AMGE + NR! wie in orderpos)

                    basar::Int32 nonChargedQty = ediTransferOrder.getPositionSegments().at(i)->getQtyLine_OrderedQtyPtr()->getQuantity() -
                                                 ediTransferOrder.getPositionSegments().at(i)->getQtyLine_ChargedQuantityPtr()->getQuantity();								// =QTY21(AMGE+NR) - QTY47(AMGE) = NR
					yitPos.setInt32( libabbauw::properties::NONCHARGEDQTY,   nonChargedQty );

					yitPos.setDecimal( libabbauw::properties::DISCOUNTPCT,     ediTransferOrder.getPositionSegments().at(i)->getDiscountPct()                               );

                    m_OrderPosCollectionProtocol.protocolCreateOrder(yitPos);
                    ++i;
                }
            }
            else
            {
                while( i < ediTransferOrder.getPositionSegments().size() )
                {
                    yitPos.setInt32(   libabbauw::properties::POSNO,           i+1                                                                                         );
                    yitPos.setInt32(   libabbauw::properties::ARTICLENO,       ediTransferOrder.getPositionSegments().at(i)->getArticleno()                                );

					basar::Int32 nonChargedQty = 0;
                    if( ediTransferOrder.getPositionSegments().at(i)->getQty_10_Line_NonChargedQtyPtr().get() != NULL )
                    {
                        nonChargedQty = ediTransferOrder.getPositionSegments().at(i)->getQty_10_Line_NonChargedQtyPtr()->getQuantity();
                    }
                    yitPos.setInt32( libabbauw::properties::NONCHARGEDQTY,  nonChargedQty);

					// Because field orderedqty in tables orderpos and orderposprotocol contain always AMGE+NR,
					// but German inhouse format has only QTY-10-segmant with "21" contains only AMGE
					// (That is different from Swiss QTY21-segment, that contains AMGE+NR )
					basar::Int32 ChargedQty =     ediTransferOrder.getPositionSegments().at(i)->getQty_10_Line_ChargedQtyPtr()->getQuantity()
												+ nonChargedQty;
					yitPos.setInt32(   libabbauw::properties::ORDEREDQTY,   ChargedQty);


                    yitPos.setDecimal( libabbauw::properties::DISCOUNTPCT,     ediTransferOrder.getPositionSegments().at(i)->getDiscountPct()                              );

                    m_OrderPosCollectionProtocol.protocolCreateOrder(yitPos);
                    ++i;
                }
            }

        }
        catch(...)
        {
            std::stringstream str;
			str << "Error during saving order pos protocol " << std::endl;
            BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str() );

            throw libabbauw::OrderProtocolException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
        }
    }

	bool EdiTransferOrderMapper::isStartOfAnOrder(const domMod::line::LineTypeEnum lineType)
	{
		METHODNAME_DEF( EdiTransferOrderMapper, isStartOfAnOrder )
		BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

		if ( enInputtype_inhCH	== m_Inputtype && domMod::line::enUNB == lineType )
		{
			return true;
		}
		else if ( enInputtype_inhDE	== m_Inputtype && domMod::line::en00 == lineType )
		{
			return true;
		}

		return false;
	}

	bool EdiTransferOrderMapper::isEndOfAnOrder(const domMod::line::LineTypeEnum lineType)
	{
		METHODNAME_DEF( EdiTransferOrderMapper, isEndOfAnOrder )
		BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

		if ( enInputtype_inhCH	== m_Inputtype &&  domMod::line::enUNZ == lineType ) // for CH
		{
			return true;
		}
		else if ( enInputtype_inhDE	== m_Inputtype &&  domMod::line::en97 == lineType ) // for DE
		{
			return true;
		}

		return false;
	}

	// Check, if path contains a correct flatfile, that can be mapped to transfer orders
	// returns true: can be mapped  false: skip it
	// throws:	flatfileOrDirException
	bool EdiTransferOrderMapper::isMappableFlatfile(const boost::filesystem::directory_entry& dirEntry)
	{
		METHODNAME_DEF( EdiTransferOrderMapper, isMappableFlatfile )
		BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

		try
		{
			basar::I18nString fileName	= dirEntry.path().filename().string();

			// Cannot map a subdirectory
			if ( is_directory(dirEntry.status()) )
			{
				std::stringstream str;
				str << "Skip File (is directory) \"" << fileName << "\"" ;				
				BLOG_TRACE( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());

				return false;
			}

			bool ret = false;

			if ( enInputtype_inhCH	== m_Inputtype)
			{
				ret = isFilenameOfaMappableFlatfileCH(fileName) || isFilenameOfaMappableFlatfileCHPint(fileName); //TODO when migration to PINT successful first check obsolete
			}
			else if ( enInputtype_inhDE	== m_Inputtype)
			{
				ret = isFilenameOfaMappableFlatfileDE(fileName);
			}

			return ret;
		}
		catch (std::exception& e)
		{
			std::stringstream str;
			str << "Error during analysation of input file name: " << e.what() << std::endl;
			throw libabbauw::FlatfileOrDirException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
		}
	}

	// In CH the Filename of an uewauf-File has to be like "uewauf.123456.UNIX"
	bool EdiTransferOrderMapper::isFilenameOfaMappableFlatfileCH(const basar::I18nString& fileName)
	{
		METHODNAME_DEF( EdiTransferOrderMapper, IsFilenameOfaMappableFlatfileCH )
		BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

		// Length of Filename is ok? (e.g. "uewauf.123456.UNIX")
		if (	(FLATFILE_NAME_PREFIX.size()
		 	+	 FLATFILE_NAME_COUNTER_LENGTH_CH
			+	 FLATFILE_NAME_POSTFIX.size()) != fileName.size() )
		{
			std::stringstream str;
			str << "Skip File (wrong length) \"" << fileName << "\"" ;
			BLOG_TRACE( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());

			return false;
		}

		// Filename has correct prefix "uewauf."?
		if( FLATFILE_NAME_PREFIX != fileName.substr(0, FLATFILE_NAME_PREFIX.size()) )
		{
			std::stringstream str;
			str << "Skip File (wrong prefix) \"" << fileName << "\"" ;
			BLOG_TRACE( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());

			return false;
		}

		// Filename-counter is numeric?
		if( !(fileName.substr(FLATFILE_NAME_PREFIX.size(), FLATFILE_NAME_COUNTER_LENGTH_CH).isDigit()) )
		{
			std::stringstream str;
			str << "Skip File (counter is not numeric) \"" << fileName << "\"" ;
			BLOG_TRACE( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());

			return false;
		}

		// Filename has correct postfix ".UNIX"?
		if( FLATFILE_NAME_POSTFIX != fileName.substr(FLATFILE_NAME_PREFIX.size()+ FLATFILE_NAME_COUNTER_LENGTH_CH, FLATFILE_NAME_POSTFIX.size()) )
		{
			std::stringstream str;
			str << "Skip File (wrong Postfix) \"" << fileName << "\"" ;
			BLOG_TRACE( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());

			return false;
		}

		return true;
	}

	// In CH the Filename of an uewauf-File has to be like "uewauf.21012709162179.UNIX"
	bool EdiTransferOrderMapper::isFilenameOfaMappableFlatfileCHPint(const basar::I18nString& fileName)
	{
		METHODNAME_DEF(EdiTransferOrderMapper, IsFilenameOfaMappableFlatfileCH)
			BLOG_TRACE_METHOD(abbaUW::LoggerPool::LoggerAbbaUW, fun);

		// Length of Filename is ok? (e.g. "uewauf.123456.UNIX")
		if ((FLATFILE_NAME_PREFIX.size()
			+ FLATFILE_NAME_COUNTER_LENGTH_CH_PINT
			+ FLATFILE_NAME_POSTFIX.size()) != fileName.size())
		{
			std::stringstream str;
			str << "Skip File (wrong length) \"" << fileName << "\"";
			BLOG_TRACE(abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());

			return false;
		}

		// Filename has correct prefix "uewauf."?
		if (FLATFILE_NAME_PREFIX != fileName.substr(0, FLATFILE_NAME_PREFIX.size()))
		{
			std::stringstream str;
			str << "Skip File (wrong prefix) \"" << fileName << "\"";
			BLOG_TRACE(abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());

			return false;
		}

		// Filename-counter is numeric?
		if (!(fileName.substr(FLATFILE_NAME_PREFIX.size(), FLATFILE_NAME_COUNTER_LENGTH_CH_PINT).isDigit()))
		{
			std::stringstream str;
			str << "Skip File (counter is not numeric) \"" << fileName << "\"";
			BLOG_TRACE(abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());

			return false;
		}

		// Filename has correct postfix ".UNIX"?
		if (FLATFILE_NAME_POSTFIX != fileName.substr(FLATFILE_NAME_PREFIX.size() + FLATFILE_NAME_COUNTER_LENGTH_CH_PINT, FLATFILE_NAME_POSTFIX.size()))
		{
			std::stringstream str;
			str << "Skip File (wrong Postfix) \"" << fileName << "\"";
			BLOG_TRACE(abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());

			return false;
		}

		return true;
	}

	// In DE the filename of an uewauf-File has to be like "uewauf.nd.201512301157.UNIX" (Format counter: YYYYMMDDhhmm)
	bool EdiTransferOrderMapper::isFilenameOfaMappableFlatfileDE(const basar::I18nString& fileName)
	{
		METHODNAME_DEF( EdiTransferOrderMapper, IsFilenameOfaMappableFlatfileDE )
		BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

		// Length of Filename is ok? (e.g. "uewauf.nd.123456789012.UNIX")
		if (	(FLATFILE_NAME_PREFIX.size()
			+	 FLATFILE_NAME_REGION_LENGTH_DE
		 	+	 FLATFILE_NAME_COUNTER_LENGTH_DE
			+	 FLATFILE_NAME_POSTFIX.size()) != fileName.size() )
		{
			std::stringstream str;
			str << "Skip File (wrong length) \"" << fileName << "\"" ;
			BLOG_TRACE( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());

			return false;
		}

		// Filename has correct prefix "uewauf."?
		if( FLATFILE_NAME_PREFIX != fileName.substr(0, FLATFILE_NAME_PREFIX.size()) )
		{
			std::stringstream str;
			str << "Skip File (wrong prefix) \"" << fileName << "\"" ;
			BLOG_TRACE( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());

			return false;
		}

		// Filename has correct region specifier ".nd" or ".sd"?
		// We do not check for correlation region / countrycode, because orders for branches that are not specified in forbranches-parameter are ignored later on.
		basar::I18nString regionSpecifier = fileName.substr(FLATFILE_NAME_PREFIX.size(), FLATFILE_NAME_REGION_LENGTH_DE);

		if(		FLATFILE_NAME_REGION_ND != regionSpecifier
			&&	FLATFILE_NAME_REGION_DC != regionSpecifier )
		{
			std::stringstream str;
			str << "Skip File (wrong region specifier) \"" << fileName << "\"" ;
			BLOG_TRACE( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());

			return false;
		}

		// Filename-counter is numeric?
		basar::I18nString filenameCounter = fileName.substr(FLATFILE_NAME_PREFIX.size() + FLATFILE_NAME_REGION_LENGTH_DE, FLATFILE_NAME_COUNTER_LENGTH_DE);

		if( !(filenameCounter.isDigit()) )
		{
			std::stringstream str;
			str << "Skip File (timestamp is not numeric) \"" << fileName << "\"" ;
			BLOG_TRACE( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());

			return false;
		}

		// Filename has correct postfix ".UNIX"?
		basar::I18nString postfix = fileName.substr(FLATFILE_NAME_PREFIX.size() + FLATFILE_NAME_REGION_LENGTH_DE + FLATFILE_NAME_COUNTER_LENGTH_DE, FLATFILE_NAME_POSTFIX.size());

		if( FLATFILE_NAME_POSTFIX != postfix )
		{
			std::stringstream str;
			str << "Skip File (wrong Postfix) \"" << fileName << "\"" ;
			BLOG_TRACE( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());

			return false;
		}

		return true;
	}

	// Marks the flatfile as correctly read by renaming it: Append new file extention ".ok"
	// Same for CH and DE
	void EdiTransferOrderMapper::tickOffFlatfile(const boost::filesystem::directory_entry& dirEntryFlatfile)
	{
		METHODNAME_DEF( EdiTransferOrderMapper, tickOffFlatfile )
		BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

		try
		{
			// ... mark the file as correctly read by renaming it: New extention ".ok"
			boost::filesystem::path dstPath(dirEntryFlatfile.path().string() + FLATFILE_WAS_CORRECTLY_READ_EXTENTION);
			boost::filesystem::rename(dirEntryFlatfile.path(), dstPath);
		}
		catch (std::exception& e)
		{
			// Here we throw an exception, that causes the end of the complete processing of the
			// input-Directory, because there is a risk of processing the same file several times.
			std::stringstream str;
			str << "Error renaming flatfile: " << dirEntryFlatfile.path()<< " -- " << e.what() << std::endl;
			throw libabbauw::FlatfileOrDirException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
		}
		catch (...)
		{
			// Here we throw an exception, that causes the end of the complete processing of the
			// input-Directory, because there is a risk of processing the same file several times.
			std::stringstream str;
			str << "Error renaming flatfile: Unknown exception! " << std::endl;
			throw libabbauw::FlatfileOrDirException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
		}
	}

	// for CH:
	// Marks the order inside of a flatfile as correctly read by renaming it:
	// Change "UNB" to "XNB" and add "BON <orderno>"
	// Parameter:
	// fileStream: opened flatfile, position will be the same afterwards
	// ordernoBatch: internal orderno, that was assigned to this order
	// unbPositionInFile: characterposition of begin of tag "UNB" of the current order
	//
	// throws: libabbauw::FlatfileOrDirException
	void EdiTransferOrderMapper::tickOffOrderInFlatfileCH(boost::filesystem::fstream& fileStream, const basar::Int32 ordernoBatch, const std::ostream::pos_type unbPositionInFile)
	{
		METHODNAME_DEF( EdiTransferOrderMapper, tickOffOrderInFlatfileCH )
		BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

		try
		{
			// Mark UNBline of order with "XNB" and ordernoBatch ..
			// e.g.: change „UNB7601001339958 7601001358034 UEWAUF“
			//       to  	„XNB7601001339958 BON1234567890 UEWAUF“

			// .. keep in mind current position in file
			std::ostream::pos_type afterLinePos	= fileStream.tellg();
			if ( fileStream.eof() )
			{
				// we reached the end of the file already: clear state
				fileStream.clear();
			}

			// .. write XNB at the beginning of the line
			fileStream.seekp( unbPositionInFile);
			fileStream << domMod::line::EDILINE_IDF_ORDER_WAS_READ_CH;		// write XNB

			// .. write "ANR" and orderno instead of the receiving (second) EAN
			fileStream.seekp( domMod::line::EDILINE_UNB_OFFSET_WHOLESALER_EAN  + unbPositionInFile);
			fileStream	<< domMod::line::EDILINE_BON_ORDER_WAS_READ
				<< std::setw(domMod::line::EDILINE_LENGTH_EAN - domMod::line::EDILINE_BON_ORDER_WAS_READ.size())		// set fieldlen for orderno
				<< std::left																			// orderno will be bound to the left side in the field
				<< ordernoBatch;
			fileStream	<< std::flush;

			// Get back to former pos
			fileStream.seekp( afterLinePos );
			fileStream.seekg( afterLinePos );

		}
		catch (std::exception& e)
		{
			// Here we throw an exception, that causes the end of the complete processing of the
			// input-Directory, because there is a risk of processing the same order several times.
			std::stringstream str;
			str << "Error ticking off order at position  " << fileStream.tellg() << ": " << e.what() << std::endl;
			throw libabbauw::FlatfileOrDirException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
		}
		catch (...)
		{
			// Here we throw an exception, that causes the end of the complete processing of the
			// input-Directory, because there is a risk of processing the same order several times.
			std::stringstream str;
			str << "Error ticking off order at position  " << fileStream.tellg() << ": Unknown exception!" << std::endl;
			throw libabbauw::FlatfileOrDirException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
		}


		return;
	}

	// for DE:
	// Marks the order inside of a flatfile as correctly read by renaming it:
	// Change "00UNOB" to "X0UNOB" and add "BON <orderno>": For orders that are transferred correctly
	// Change "00UNOB" to "Y0UNOB" and add "BON <orderno>": For orders that had to be ignored
	// Parameter:
	// fileStream: opened flatfile, position will be the same afterwards
	// ordernoBatch: internal orderno, that was assigned to this order
	// unbPositionInFile: characterposition of begin of tag "UNB" of the current order
	//
	// throws: libabbauw::FlatfileOrDirException
	void EdiTransferOrderMapper::tickOffOrderInFlatfileDE(boost::filesystem::fstream& fileStream, const basar::Int32 ordernoBatch, const std::ostream::pos_type unbPositionInFile, const bool orderIsToBeIgnored)
	{
		METHODNAME_DEF( EdiTransferOrderMapper, tickOffOrderInFlatfileDE )
		BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

		try
		{
			// Mark UNB_00_line ("00UNOB" resp. "00UNOC") of order with "X0UNOB" resp. "X0UNOC" and ordernoBatch ..
			// e.g.: change „00UNOB268758258                           PH                76539419                           PH                14070910312665975                                       EANCOM“
			//       to  	„00UNOB268758258  BON1234567890            PH                76539419                           PH                14070910312665975                                       EANCOM“
			////OLD  to  	„XNB7601001339958 BON1234567890 UEWAUF“

			// .. keep in mind current position in file
			std::ostream::pos_type afterLinePos	= fileStream.tellg();
			if ( fileStream.eof() )
			{
				// we reached the end of the file already: clear state
				fileStream.clear();
			}

			// .. write XNB at the beginning of the line
			fileStream.seekp( unbPositionInFile);
			if ( !orderIsToBeIgnored)
			{
				fileStream << domMod::line::EDILINE_IDF_ORDER_WAS_READ_DE;		// write X
			}
			else
			{
				fileStream << domMod::line::EDILINE_IDF_ORDER_WAS_IGNORED_DE;		// write Y
			}

			// .. write "BON" and orderno into the first UNB-FILLLER C(45) at same position as in CH
			fileStream.seekp( domMod::line::EDILINE_UNB_00_OFFSET_UNB_FILLER1  + unbPositionInFile);
			fileStream	<< domMod::line::EDILINE_BON_ORDER_WAS_READ
				<< std::setw(domMod::line::EDILINE_LENGTH_UNB_FILLER1 - domMod::line::EDILINE_BON_ORDER_WAS_READ.size())		// set fieldlen for orderno
				<< std::left																			// orderno will be bound to the left side in the field
				<< ordernoBatch;
			fileStream	<< std::flush;

			// Get back to former pos
			fileStream.seekp( afterLinePos );
			fileStream.seekg( afterLinePos );

		}
		catch (std::exception& e)
		{
			// Here we throw an exception, that causes the end of the complete processing of the
			// input-Directory, because there is a risk of processing the same order several times.
			std::stringstream str;
			str << "Error ticking off order at position  " << fileStream.tellg() << ": " << e.what() << std::endl;
			throw libabbauw::FlatfileOrDirException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
		}
		catch (...)
		{
			// Here we throw an exception, that causes the end of the complete processing of the
			// input-Directory, because there is a risk of processing the same order several times.
			std::stringstream str;
			str << "Error ticking off order at position  " << fileStream.tellg() << ": Unknown exception!" << std::endl;
			throw libabbauw::FlatfileOrDirException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
		}


		return;
	}

	// =============  Methods for Cleaning ==========================

	bool EdiTransferOrderMapper::doCleanInputDir()
	{
		METHODNAME_DEF( EdiTransferOrderMapper, doCleanInputDir )
		BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

		bool retval = false;

		int directoriesDeleted = 0;

		try
		{
			// Is the input-directory ok?
			boost::filesystem::path inputDir( m_InputDir.c_str() );

			if ( !exists(inputDir) )
			{
				std::stringstream str;
				str << "Input directory does not exist: \" " << m_InputDir << "\" " << std::endl;
				throw libabbauw::FlatfileOrDirException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}

			if ( !is_directory(inputDir) )
			{
				std::stringstream str;
				str << "Parameter -inputdir does not contain a directory \" " << m_InputDir << "\" " << std::endl;
				throw libabbauw::FlatfileOrDirException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
			}

			// Logging
			BLOG_INFO( abbaUW::LoggerPool::LoggerAbbaUW, "Cleaning flatfile input directory ...\"" + m_InputDir + "\"" );

			// Open directory and loop for all files in the input directory
			boost::filesystem::directory_iterator endDirItr; // default construction yields past-the-end

			for ( boost::filesystem::directory_iterator currDirItr( inputDir ); currDirItr != endDirItr; ++currDirItr )
			{
				if( !is_directory(*currDirItr) )
				{
					continue;
				}

				if( isUWARDirectoryDeletable(*currDirItr) )
				{
					boost::filesystem::remove_all( *currDirItr );

					directoriesDeleted++;

					BLOG_INFO( abbaUW::LoggerPool::LoggerAbbaUW, "Deleted directory ...\"" + currDirItr->path().string() + "\"" );
				}
			}

			retval = true;
		}
		catch ( basar::Exception& e )
		{
			std::stringstream str;
			str << "Basar-Error during cleaning flatfile input directory: " << e.what() << std::endl;
			BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());

			retval = false;
		}
		catch ( std::exception& e )
		{
			std::stringstream str;
			str << "Error during cleaning flatfile input directory: " << e.what() << std::endl;
			BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());

			retval = false;
		}
		catch (...)
		{
			std::stringstream str;
			str << "Error during cleaning flatfile input directory: Unknown exception! " << std::endl;
			BLOG_ERROR( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());

			retval = false;
		}

		// Log result summary
		{
			std::stringstream stringStream;
			stringStream << "End cleaning UWAR directories \"" << m_InputDir << "\"" << std::endl;
			stringStream << "   ===> Summary: Number of direcotries deleted: " << directoriesDeleted << std::endl;

			BLOG_INFO( abbaUW::LoggerPool::LoggerAbbaUW, stringStream.str().c_str() );
		}

		return retval;
	}

	bool EdiTransferOrderMapper::isUWARDirectoryDeletable(const boost::filesystem::path& directoryPath)
	{
		METHODNAME_DEF( EdiTransferOrderMapper, isUWARDirectoryDeletable )
		BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );

		int cmpresult = directoryPath.filename().string().compare(0, 5, "UWAR.");
		if (cmpresult)
		{
			// Not an UWAR-directory
			return false;
		}

		std::time_t lastWriteTime =	boost::filesystem::last_write_time(directoryPath);
		std::time_t diffTime = (time(0) - lastWriteTime); // calculating in seconds since 01.01.1970

		if ( diffTime >= FLATFILE_TO_BE_CLEANED_AFTER_DAYS * SECONDS_OF_A_DAY )
		{
			return true;
		}

		return false;
	}

	void EdiTransferOrderMapper::injectMailAlert(abbaUW::infrastructure::mail::MailAlertPtr mailAlert)
	{
		METHODNAME_DEF(EdiTransferOrderMapper, injectMailAlert)
		BLOG_TRACE_METHOD(abbaUW::LoggerPool::LoggerAbbaUW, fun);

		m_MailAlert = mailAlert;
	}

	// Delete a single UEWAUF file
	// All file handling tasks are concentrated in this method.
	// Params:  dirEntryFlatfile - directory entry of the flatfile to be deleted
	// returns: true: File was deleted  false: else
	// throws:	flatfileOrDirException
	//
	//bool EdiTransferOrderMapper::doDeleteFlatfile(const boost::filesystem::directory_entry& dirEntryFlatfile)
	//{
	//	bool		 everythingOkInFile	= true;// all lines and orders in this file have been ok so far
	//
	//	METHODNAME_DEF( EdiTransferOrderMapper, doDeleteFlatfile )
	//	BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );
	//
	//	try
	//	{
	//		// extract filedata
	//		boost::filesystem::path filePath	= dirEntryFlatfile.path();
	//		basar::I18nString       fileName	= dirEntryFlatfile.path().filename().string();
	//
	//		//std::stringstream temp;
	//		//temp << filePath.filename();
	//		//basar::I18nString	fileName( temp.str());
	//		//orig: basar::I18nString	fileName		= dirEntryFlatfile.filename();
	//
	//		// delete file
	//		if( !boost::filesystem::remove( filePath) )
	//		{
	//			everythingOkInFile = false;
	//		}
	//
	//		{
	//			// Loggen
	//			std::stringstream	str;
	//
	//			str << "File deleted: \"" << fileName << "\"";
	//			BLOG_INFO( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
	//		}
	//
	//
	//		//  Watch out: Potentially problem with removing files during an iteration in boost library?
	//
	//	}
	//	catch (std::exception& e)
	//	{
	//		std::stringstream str;
	//		str << "Error during deletion of a flatfile: " << e.what() << std::endl;
	//		throw libabbauw::FlatfileOrDirException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
	//	}
	//	catch (...)
	//	{
	//		std::stringstream str;
	//		str << "Error deleting flatfile: Unknown exception! " << std::endl;
	//		throw libabbauw::FlatfileOrDirException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
	//	}
	//
	//	return everythingOkInFile;
	//}

	// Check, if path contains a flatfile, that has to be deleted,
	// because it is already correctly read and it is older than x days
	// returns true: has to be deleted  false: do not delete it
	// throws:	flatfileOrDirException
	//bool EdiTransferOrderMapper::isFlatfileToBeCleaned(const boost::filesystem::directory_entry& dirEntry)
	//{
	//	METHODNAME_DEF( EdiTransferOrderMapper, isFlatfileToBeCleaned )
	//	BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );
	//
	//	try
	//	{
	//		basar::I18nString fileName	= dirEntry.path().filename().string();
	//
	//		// Cannot delete a subdirectory
	//		if ( is_directory(dirEntry.status()) )
	//		{
	//			std::stringstream str;
	//			str << "Don't delete File (is directory) \"" << fileName << "\"" ;
	//			BLOG_TRACE( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
	//
	//			return false;
	//		}
	//
	//		bool ret = false;
	//
	//		if ( enInputtype_inhCH	== m_Inputtype )
	//		{
	//			ret = isFilenameOfaCleanableFlatfileCH(fileName);
	//		}
	//		else if ( enInputtype_inhDE	== m_Inputtype )
	//		{
	//			ret = isFilenameOfaCleanableFlatfileDE(fileName);
	//		}
	//
	//		if ( !ret )
	//		{
	//			return false;
	//		}
	//
	//		// File is older than FLATFILE_TO_BE_CLEANED_AFTER_DAYS days?
	//		std::time_t lastWriteTime =	boost::filesystem::last_write_time(dirEntry.path());
	//		std::time_t nowTime	= time(0);
	//		std::time_t diffTime = nowTime - lastWriteTime;		// calculating in seconds since 01.01.1970
	//
	//		if ( diffTime <= FLATFILE_TO_BE_CLEANED_AFTER_DAYS * SECONDS_OF_A_DAY )
	//		{
	//			std::stringstream str;
	//			str << "Don't delete File (last write time didn't expire) \"" << fileName << "\"" ;
	//			BLOG_TRACE( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
	//
	//			return false;
	//		}
	//
	//		// yes, file is to be cleaned (deleted)
	//		return true;
	//	}
	//	catch (std::exception& e)
	//	{
	//		std::stringstream str;
	//		str << "Error during analyzation of input file name for cleaning: " << e.what() << std::endl;
	//		throw libabbauw::FlatfileOrDirException( basar::ExceptInfo( fun, str.str().c_str(), __FILE__, __LINE__ ) );
	//	}
	//}

	// In CH the Filename of a successfully read uewauf-File has to be like "uewauf.123456.UNIX.ok"
	//bool EdiTransferOrderMapper::isFilenameOfaCleanableFlatfileCH(const basar::I18nString& fileName)
	//{
	//	METHODNAME_DEF( EdiTransferOrderMapper, isFilenameOfaCleanableFlatfileCH )
	//	BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );
	//
	//	// Length of Filename is ok?
	//	if (	(  FLATFILE_NAME_PREFIX.size()
	//				+ FLATFILE_NAME_COUNTER_LENGTH_CH
	//				+ FLATFILE_NAME_POSTFIX.size()
	//				+ FLATFILE_WAS_CORRECTLY_READ_EXTENTION.size()) != fileName.size() )
	//	{
	//		std::stringstream str;
	//		str << "Don't delete File (wrong length) \"" << fileName << "\"" ;
	//		BLOG_TRACE( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
	//
	//		return false;
	//	}
	//
	//	// Filename has correct prefix "uewauf."?
	//	if( FLATFILE_NAME_PREFIX != fileName.substr(0, FLATFILE_NAME_PREFIX.size()) )
	//	{
	//		std::stringstream str;
	//		str << "Don't delete File (wrong prefix) \"" << fileName << "\"" ;
	//		BLOG_TRACE( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
	//
	//		return false;
	//	}
	//
	//		// Filename-counter is numeric?
	//		if( !(fileName.substr(FLATFILE_NAME_PREFIX.size(), FLATFILE_NAME_COUNTER_LENGTH_CH).isDigit()) )
	//		{
	//			std::stringstream str;
	//			str << "Don't delete File (first counter is not numeric) \"" << fileName << "\"" ;
	//			BLOG_TRACE( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
	//
	//			return false;
	//		}
	//
	//		// Filename has correct postfix ".UNIX"?
	//		if( FLATFILE_NAME_POSTFIX != fileName.substr(FLATFILE_NAME_PREFIX.size()+ FLATFILE_NAME_COUNTER_LENGTH_CH, FLATFILE_NAME_POSTFIX.size()) )
	//		{
	//			std::stringstream str;
	//			str << "Don't delete File (wrong .UNIX-postfix) \"" << fileName << "\"" ;
	//			BLOG_TRACE( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
	//
	//			return false;
	//		}
	//
	//		// Filename has correct postfix ".ok"?
	//		if( FLATFILE_WAS_CORRECTLY_READ_EXTENTION
	//			   != fileName.substr(FLATFILE_NAME_PREFIX.size() + FLATFILE_NAME_COUNTER_LENGTH_CH + FLATFILE_NAME_POSTFIX.size()
	//									, FLATFILE_WAS_CORRECTLY_READ_EXTENTION.size()) )
	//		{
	//			std::stringstream str;
	//			str << "Don't delete File (wrong .ok-postfix) \"" << fileName << "\"" ;
	//			BLOG_TRACE( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
	//
	//			return false;
	//		}
	//
	//	return true;
	//}

	// In DE the Filename of a successfully read uewauf-File has to be like "uewauf.nd.201512301157.UNIX.ok" (Format counter: YYYYMMDDhhmm)
	//bool EdiTransferOrderMapper::isFilenameOfaCleanableFlatfileDE(const basar::I18nString& fileName)
	//{
	//	METHODNAME_DEF( EdiTransferOrderMapper, isFilenameOfaCleanableFlatfileDE )
	//	BLOG_TRACE_METHOD( abbaUW::LoggerPool::LoggerAbbaUW, fun );
	//
	//	// Length of Filename is ok? (e.g. "uewauf.nd.123456789012.UNIX")
	//	if (	   (  FLATFILE_NAME_PREFIX.size()
	//				+ FLATFILE_NAME_REGION_LENGTH_DE
	//				+ FLATFILE_NAME_COUNTER_LENGTH_DE
	//				+ FLATFILE_NAME_POSTFIX.size()
	//				+ FLATFILE_WAS_CORRECTLY_READ_EXTENTION.size()) != fileName.size() )
	//	{
	//		std::stringstream str;
	//		str << "Don't delete File (wrong length) \"" << fileName << "\"" ;
	//		BLOG_TRACE( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
	//
	//		return false;
	//	}
	//
	//	// Filename has correct prefix "uewauf."?
	//	if( FLATFILE_NAME_PREFIX != fileName.substr(0, FLATFILE_NAME_PREFIX.size()) )
	//	{
	//		std::stringstream str;
	//		str << "Don't delete File (wrong prefix) \"" << fileName << "\"" ;
	//		BLOG_TRACE( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
	//
	//		return false;
	//	}
	//
	//	// Filename has correct region specifier ".nd" or ".sd"?
	//	// We do not check for correlation region / countrycode, because orders for branches that are not specified in forbranches-parameter are ignored later on.
	//	basar::I18nString regionSpecifier = fileName.substr(FLATFILE_NAME_PREFIX.size(), FLATFILE_NAME_REGION_LENGTH_DE);
	//
	//	if(		FLATFILE_NAME_REGION_ND != regionSpecifier
	//		&&	FLATFILE_NAME_REGION_SD != regionSpecifier )
	//	{
	//		std::stringstream str;
	//		str << "Don't delete File (wrong region specifier) \"" << fileName << "\"" ;
	//		BLOG_TRACE( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
	//
	//		return false;
	//	}
	//
	//
	//	// Filename-counter is numeric?
	//	basar::I18nString filenameCounter = fileName.substr(FLATFILE_NAME_PREFIX.size() + FLATFILE_NAME_REGION_LENGTH_DE, FLATFILE_NAME_COUNTER_LENGTH_DE);
	//
	//	if( !(filenameCounter.isDigit()) )
	//	{
	//		std::stringstream str;
	//		str << "Don't delete File (timestamp is not numeric) \"" << fileName << "\"" ;
	//		BLOG_TRACE( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
	//
	//		return false;
	//	}
	//
	//
	//	// Filename has correct postfix ".UNIX"?
	//	basar::I18nString postfix = fileName.substr(FLATFILE_NAME_PREFIX.size() + FLATFILE_NAME_REGION_LENGTH_DE + FLATFILE_NAME_COUNTER_LENGTH_DE
	//													, FLATFILE_NAME_POSTFIX.size());
	//
	//	if( FLATFILE_NAME_POSTFIX != postfix )
	//	{
	//		std::stringstream str;
	//		str << "Don't delete File (wrong .UNIX-postfix) \"" << fileName << "\"" ;
	//		BLOG_TRACE( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
	//
	//		return false;
	//	}
	//
	//
	//	// Filename has correct postfix ".ok"?
	//	basar::I18nString postfixOK = fileName.substr(FLATFILE_NAME_PREFIX.size() + FLATFILE_NAME_REGION_LENGTH_DE + FLATFILE_NAME_COUNTER_LENGTH_DE + FLATFILE_NAME_POSTFIX.size()
	//													, FLATFILE_WAS_CORRECTLY_READ_EXTENTION.size());
	//
	//	if( FLATFILE_WAS_CORRECTLY_READ_EXTENTION != postfixOK )
	//	{
	//		std::stringstream str;
	//		str << "Don't delete File (wrong .ok-postfix) \"" << fileName << "\"" ;
	//		BLOG_TRACE( abbaUW::LoggerPool::LoggerAbbaUW, str.str().c_str());
	//
	//		return false;
	//	}
	//
	//	return true;
	//}

}  // namespace orderMapper
}  // namespace domMod
