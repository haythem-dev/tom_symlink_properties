#ifndef GUARD_LIBABBAUW_DOMMOD_CSCORDER_CSCORDERDM_H
#define GUARD_LIBABBAUW_DOMMOD_CSCORDER_CSCORDERDM_H

//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include "icscorderreleaser.h"
#include "cscorderinitparams.h"

#include "iordergetterptr.h"
#include "icscordercreatorptr.h"

#ifdef _MSC_VER
#pragma warning (push)
#pragma warning(disable: 4512 )
#endif
#include <QtCore/QCoreApplication>
#ifdef _MSC_VER
#pragma warning (pop)
#endif

#include <libutil/misc/componentinitialization.hpp>

//--------------------------------------------------------------------------------------------------//
// forward declaration
//--------------------------------------------------------------------------------------------------//
namespace basar
{
	namespace db
	{
		namespace aspect
		{
			class AccessorPropertyTable_YIterator;
			class AccessorPropertyTableRef;
		}
	}
}

namespace libutil
{
	class ClassInfo;
}

namespace libabbauw
{
namespace domMod
{
namespace cscorder
{

    class CSCOrderDM : public libutil::misc::ComponentInitialization< CSCOrderInitParams, CSCOrderDM >, 
                       public ICSCOrderReleaser
    {
        Q_DECLARE_TR_FUNCTIONS( CSCOrderDM )

    public:
	    CSCOrderDM  ( basar::db::aspect::ConnectionRef );
	    ~CSCOrderDM ();

        void                                                   injectOrderGetter( IOrderGetterPtr );
        void                                                   injectCSCOrderCreator( ICSCOrderCreatorPtr );

        // defined in csc order interface
        void                                                   release( bool postpone );

    public:
        static const libutil::misc::ClassInfo&                 getClassInfo();

    protected:
        // from ComponentInitialization
        void                                                   doInit( const CSCOrderInitParams& );
        void                                                   doShutdown();

    private:
        void                                                   checkForCorrectOrderStatus( const basar::db::aspect::AccessorPropertyTable_YIterator ) const;

        void                                                   releaseOrder( basar::db::aspect::AccessorPropertyTable_YIterator, bool postpone );

    private:
        CSCOrderDM ( const CSCOrderDM& );
        CSCOrderDM operator= ( const CSCOrderDM& );

        const log4cplus::Logger&                               getLogger() const;

    private:
        const log4cplus::Logger&                               m_Logger;
        CSCOrderInitParams                                     m_InitParams;
        ICSCOrderCreatorPtr                                    m_CSCOrderCreator;
        IOrderGetterPtr                                        m_OrderGetter;

        bool                                                   m_isCreatorInitialized;
    };

} // end namespace cscorder
} // end namespace domMod
} // end namespace libabbauw

#endif // GUARD_LIBABBAUW_DOMMOD_CSCORDER_CSCORDERDM_H
