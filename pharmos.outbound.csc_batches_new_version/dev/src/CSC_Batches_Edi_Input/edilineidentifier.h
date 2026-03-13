//-------------------------------------------------------------------------------------
/*! \file  edilineidentifier.h
*  \brief  class for identification of a Edifact-flatfile-line.
*  \brief  Cannot be instantiated, only static methods.
*  \author Jia Li, P.Kessling
*  \date   02.01.2013
*  history: 
*			05.04.13 pk redo
*           28.10.14 pk store Countrycode as private member and do identification with respect to country
*/
//-------------------------------------------------------------------------------------
#ifndef GUARD_DOMMOD_LINE_LINEIDENTIFIER_H
#define GUARD_DOMMOD_LINE_LINEIDENTIFIER_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//

#include <map>


#include "ediline_definitions.h"

#include "editransferordermapper_definitions.h"

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
		class EdiLineIdentifier 
		{
        //--------------------------------------------------------------------------------------------------//
        // static declaration section
        //--------------------------------------------------------------------------------------------------//
			private:
				// Map consists of 2-tupels (key="NAD" , value="enNAD")
				static std::map <std::string, LineTypeEnum> s_mapLineInfo;

        //--------------------------------------------------------------------------------------------------//
        // object declaration section
        //--------------------------------------------------------------------------------------------------//
			public:
				EdiLineIdentifier(const domMod::orderMapper::InputtypeEnum Inputtype);
				~EdiLineIdentifier();

				LineTypeEnum	findEnumByStr	( const std::string& lineTypeStr ) const;
				std::string		findStrByEnum	( const LineTypeEnum lineTypeEnum ) const;
				LineTypeEnum	identifyEdiLine	( const std::string& lineTypeStr ) const;

			private:

				/////////////////////////////////////////////////////////////////////////
				// private member methods
				/////////////////////////////////////////////////////////////////////////
				// alle abschalten
				//EdiLineIdentifier( const EdiLineIdentifier& );				
				//EdiLineIdentifier& operator=( const EdiLineIdentifier& );	

				void initLineTypeMap();

				/////////////////////////////////////////////////////////////////////////
				// private member variables
				/////////////////////////////////////////////////////////////////////////
				domMod::orderMapper::InputtypeEnum m_Inputtype;
		};
	} // namespace line
} // namespace domMod

#endif // GUARD_DOMMOD_ORDERMAPPER_LINEIDENTIFIER_H
