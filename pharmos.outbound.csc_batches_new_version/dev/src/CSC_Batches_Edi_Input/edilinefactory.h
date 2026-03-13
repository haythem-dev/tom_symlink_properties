//----------------------------------------------------------------------------//
/*! \file	edilinefactory.h
 *  \brief  Creates Edi-Line-Objects of correkt subtype, e.g. nadLine
*  \author Jia Li ,P. Kessling
*  \date   02.01.2013, 
*  history 05.04.13  pk redesign
*/
//----------------------------------------------------------------------------//
#ifndef GUARD_DOMMOD_LINE_EDILINEFACTORY_H
#define GUARD_DOMMOD_LINE_EDILINEFACTORY_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "editransferordermapper_definitions.h"

#include "edifactline.h"
#include "ediline_definitions.h"

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
		class EdiLineFactory
		{  
		public:
        //--------------------------------------------------------------------------------------------------//
        // static declaration section
        //--------------------------------------------------------------------------------------------------//

			static EDIFACTLineBasePtr create( const LineTypeEnum lineType, const domMod::orderMapper::InputtypeEnum inputtype );

        //--------------------------------------------------------------------------------------------------//
        // object declaration section
        //--------------------------------------------------------------------------------------------------//
		private:
			/////////////////////////////////////////////////////////////////////////
			// private member methods
			/////////////////////////////////////////////////////////////////////////
			EdiLineFactory();
			//EdiLineFactory ( const EDIFACTLineFactory & r );
			//EdiLineFactory operator = ( const EDIFACTLineFactory & r );

			
			/////////////////////////////////////////////////////////////////////////
			// private member variables
			/////////////////////////////////////////////////////////////////////////
		};
	}
}

#endif // GUARD_DOMMOD_LINE_EDILINEFACTORY_H
