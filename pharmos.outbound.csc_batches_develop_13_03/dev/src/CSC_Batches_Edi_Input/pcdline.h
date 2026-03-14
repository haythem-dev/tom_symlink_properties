//-------------------------------------------------------------------------------------
/*! \file  pcdline.h
*  \brief  subclass of EDIFACTLineBase for PCD-Segment. 
*  \brief  Main purpose: Extract an PCD-Line from a flatfile-line
*  \brief                into an internal data structure and represent it 
*  \author P. Kessling
*  \date   27.03.2013, 
*  history 27.03.13  pk new
*/
//-------------------------------------------------------------------------------------
#ifndef GUARD_DOMMOD_LINE_PCDLINE_H
#define GUARD_DOMMOD_LINE_PCDLINE_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include <libbasarcmnutil_decimal.h>		// for datatype decimal
#include "edifactline.h"

//-------------------------------------------------------------------------------------------------//
// namespace section
//-------------------------------------------------------------------------------------------------//
namespace domMod
{
	namespace line
	{

    //-------------------------------------------------------------------------------------------------//
    // class declaration section
    //-------------------------------------------------------------------------------------------------//
		class PcdLine : public EDIFACTLineBase 
		{
        //--------------------------------------------------------------------------------------------------//
        // static declaration section
        //--------------------------------------------------------------------------------------------------//
		private:
				// static members
				static const basar::I18nString	s_LineIndicator;	// Line-ID (3 chars long), here "PCD" 
				static const basar::I18nString	s_LineQualifier;    // Data line qualifier, eg 12 for PCD12
				static const basar::UInt32		s_MinLength;		// Minimum length of netto data
        //--------------------------------------------------------------------------------------------------//
        // object declaration section
        //--------------------------------------------------------------------------------------------------//
		public:
				PcdLine();								// private -> public : used in Linetypecollection.h 
														// it is OK with the clone() according to prototype pattern 	
				~PcdLine();

				// enums
				// Enumeration for base of PCD lines (percentValue causes price increase (Zuschlag) or price decrease (Abschlag))
				// Usage e.g.: domMod::line::PcdLine::PcdBaseEnum
				enum PcdBaseEnum 
				{
					enUNKNOWN,			// base is not known yet
					enPriceDecrease,	// base="1  " Preisabschlag
					enPriceIncrease,	// base="2  " Preiszuschlag
					enOTHER				// base is something else, but not processable by ABBA-UW
				};


				// Getter 
				virtual LineTypeEnum				getLineTypeEnum()		const;
				const	basar::cmnutil::Decimal		getPercentValue()		const;
						PcdBaseEnum					getBase()				const;

						bool						isPriceDecrease()		const;

				void		process(const std::string& lineStr);		// extracts lineStr to members
				bool		isValidLine()		const;

				virtual EDIFACTLineBasePtr	clone() const;				

		private:
				/////////////////////////////////////////////////////////////////////////
				// private member methods
				/////////////////////////////////////////////////////////////////////////
				// private member functions
				bool					isQualifierOK()	const;
				bool					isBaseOK()		const;
				void					resetMembers();

				// special member functions
				PcdLine( const EDIFACTLineBase& ) ;				// Copy-Konstruktor : abschalten
				PcdLine& operator=( const EDIFACTLineBase& );	// Assignment operator : abschalten 
		
				/////////////////////////////////////////////////////////////////////////
				// private member variables
				/////////////////////////////////////////////////////////////////////////
				bool						m_QualifierDiscount;	// mandatory: field PROZQUAL, must be "12 ", nothing else 
				basar::cmnutil::Decimal		m_PercentValue;			// mandatory: percent value 
				PcdBaseEnum					m_Base;					// mandatory(??): price reduction or increase


		};
		
		typedef boost::shared_ptr< PcdLine > PcdLinePtr;

	} // namespace line
} // namespace domMod

#endif // GUARD_DOMMOD_LINE_PCDLINE_H