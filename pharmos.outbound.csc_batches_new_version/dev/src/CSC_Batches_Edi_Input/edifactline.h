//-------------------------------------------------------------------------------------
/*! \file  edifactline.h
*  \brief  superclass for all the specific EDIFACTLine-classes. 
*  \author Jia Li
*  \date   02.01.2013
*/
//-------------------------------------------------------------------------------------
#ifndef GUARD_DOMMOD_LINE_EDIFACTLINEBASE_H
#define GUARD_DOMMOD_LINE_EDIFACTLINEBASE_H

#include "ediline_definitions.h"
#include <boost/shared_ptr.hpp>

namespace domMod
{
	namespace line
	{
		class EDIFACTLineBase;
		typedef boost::shared_ptr< domMod::line::EDIFACTLineBase > EDIFACTLineBasePtr;
		
		class EDIFACTLineBase 
		{
			public:
				
				virtual ~EDIFACTLineBase(){};

				virtual LineTypeEnum		getLineTypeEnum() const				= 0;

				virtual EDIFACTLineBasePtr	clone() const						= 0;
				virtual void				process(const std::string& lineStr) = 0;
				virtual bool				isValidLine()const					= 0;

				// The lines cannot mount themselves to editransferorder, because 
				// this would lead to a circular include of unbline.h etc. and editransferorder.h
				// Moreover the logic of mounting really belongs to transferOrder.
			
			protected:

				EDIFACTLineBase(){};  // Not public, this means its an abstract class
				
			
			private:
				//virtual EDIFACTLineBase( const EDIFACTLineBase& )				=0;	// Copy-Konstruktor 
				//virtual EDIFACTLineBase& operator=( const EDIFACTLineBase& )	=0;	// Assignment operator
		};
		
	} // namespace line
} // namespace domMod

#endif // GUARD_DOMMOD_LINE_EDIFACTLINEBASE_H
