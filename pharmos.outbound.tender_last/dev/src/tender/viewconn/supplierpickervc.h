#ifndef GUARD_TENDER_VIEWCONN_SUPPLIERPICKERVC_H
#define GUARD_TENDER_VIEWCONN_SUPPLIERPICKERVC_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//

#pragma warning (push)
#pragma warning(disable: 4127 4231 4244 4251 4311 4312 4481 4511 4512 4800)
#include "ui_supplierpickervc.h"
#pragma warning (pop)

#include <libutil/flag.h>
#include <libutil/viewconn.h>

#pragma warning (push)
#pragma warning(disable: 4127 4231 4244 4251 4311 4312 4481 4511 4512 4800)
#include <QObject>
#pragma warning (pop)


//-------------------------------------------------------------------------------------------------//
// forward declaration section
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
// class declaration setcion
//-------------------------------------------------------------------------------------------------//
namespace tender
{
    namespace viewConn
    {
        //-------------------------------------------------------------------------------------------------//
        // class declaration
        //-------------------------------------------------------------------------------------------------//
        class SupplierPickerVC : public libutil::viewConn::BaseVC
        {
            Q_OBJECT;

            public:
                SupplierPickerVC( QWidget* parent = 0 );
                ~SupplierPickerVC();

                virtual basar::gui::tie::WidgetReturnEnum show();

                void matchFromSupplierCollection( basar::db::aspect::AccessorPropertyTableRef        propTab   );
                void matchToSearchCriteria     ( basar::db::aspect::AccessorPropertyTable_YIterator yitSearch );

                basar::db::aspect::AccessorPropertyTable_YIterator getPickedSupplier();
                void setSearchCriteria( basar::db::aspect::AccessorPropertyTable_YIterator yitSearchCriteria );

                void displayNoSupplierFound();

            private:
                void wireEvents();
                void setupSupplierTable();
                void setupSearchCriteria();
                void initSupplierHeaderLabels();
                basar::Int32 initMatcher( basar::db::aspect::AccessorPropertyTableRef propTab );
                void clear();

            private slots:
                void on_DialogClose_Requested();
                void on_Search_Requested();
                void on_TakeOver_Requested();

            protected:
                virtual void focusInEvent( QFocusEvent* event );
                virtual void reject();

            private:
                Ui::SupplierPickerVC                                ui;

                libutil::misc::Flag                                m_EventsWiring;
                QStringList                                        m_LstLabels;
                std::list<basar::VarString>                        m_LstOriginalAttributes;
                basar::gui::tie::MatcherRef                        m_SupplierMatcher;
                basar::db::aspect::AccessorPropertyTable_YIterator m_PickedSupplier;
        };
    } // end namespace viewConn
} // end namespace tender

#endif // end GUARD_TENDER_VIEWCONN_SUPPLIERPICKERVC_H
