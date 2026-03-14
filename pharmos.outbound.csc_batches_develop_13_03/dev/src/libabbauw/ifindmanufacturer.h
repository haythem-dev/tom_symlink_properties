//----------------------------------------------------------------------------//
/*! \file	
 *  \brief  
 *  \author Hendrik Polzin 
 *  \date 15.08.2013
 */
//----------------------------------------------------------------------------//
#ifndef GUARD_LIBABBAUW_DOMMOD_MANUFACTURERDM_IFINDMANUFACTURER_H
#define GUARD_LIBABBAUW_DOMMOD_MANUFACTURERDM_IFINDMANUFACTURER_H

//----------------------------------------------------------------------------//
// forward declaration section
//----------------------------------------------------------------------------//
namespace basar
{
    namespace db
    {
        namespace aspect
        {
            class AccessorPropertyTableRef;
			class AccessorPropertyTable_YIterator;
        }
    }
}

//----------------------------------------------------------------------------//
// class declaration section
//----------------------------------------------------------------------------//
namespace libabbauw
{
namespace domMod
{
namespace manufacturerdm
{
    class IFindManufacturer
    {
        public:	
            virtual ~IFindManufacturer(){}

			virtual basar::db::aspect::AccessorPropertyTableRef			getPropTab()                                                           = 0;
			virtual void			                                    findManufacturer( basar::db::aspect::AccessorPropertyTable_YIterator ) = 0;
			virtual void                                                resetSearchYit()                                                       = 0;
            virtual basar::db::aspect::AccessorPropertyTable_YIterator	getSearchYit()                                                         = 0;
            virtual void                                                clear()                                                                = 0;
    };
} 
} 
}
//----------------------------------------------------------------------------
#endif // GUARD_LIBABBAUW_DOMMOD_MANUFACTURERDM_IFINDMANUFACTURER_H
