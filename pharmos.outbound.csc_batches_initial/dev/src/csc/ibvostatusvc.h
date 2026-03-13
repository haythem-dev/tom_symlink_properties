#ifndef GUARD_CSCBATCHES_VIEWCONN_IBVOSTATUSVC_H
#define GUARD_CSCBATCHES_VIEWCONN_IBVOSTATUSVC_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include <libbasarguitie_definitions.h>
#include <libbasarguitie_msgboxes.h.>

//-------------------------------------------------------------------------------------------------//
// class declaration section
//-------------------------------------------------------------------------------------------------//
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

//-------------------------------------------------------------------------------------------------//
// class declaration section
//-------------------------------------------------------------------------------------------------//
namespace CSC_Batches
{
namespace viewConn
{
    class IBVOStatusVC
    {
    public:
        virtual ~ IBVOStatusVC() {}

        virtual void                                init( basar::db::aspect::AccessorPropertyTableRef  )                        = 0;
		virtual void								init( basar::db::aspect::AccessorPropertyTableRef, basar::Int32 )           = 0;
        virtual basar::gui::tie::WidgetReturnEnum	show()                                                                      = 0;
        virtual void								hide()                                                                      = 0;
		virtual void                                waitForEvents()                                                             = 0;
        virtual bool                                shutdown()                                                                  = 0;

        virtual void                                matchToSearchCriteria( basar::db::aspect::AccessorPropertyTable_YIterator ) = 0;
        virtual void                                matchFromProposal()                                                         = 0;
		virtual basar::db::aspect::AccessorPropertyTable_YIterator getSelectedOrderProposal()                                   = 0;

		virtual void                                info( const basar::I18nString& text )                                       = 0;
        virtual void                                error( const basar::I18nString& text )                                      = 0;
        virtual const basar::gui::tie::MessageBoxButtonEnum question( const basar::I18nString& text )                           = 0;

		virtual QWidget*							getWidget() = 0;
    };
} // end namespace viewConn
} // end namespace CSC_Batches

#endif // GUARD_CSCBATCHES_VIEWCONN_IBVOSTATUSVC_H
