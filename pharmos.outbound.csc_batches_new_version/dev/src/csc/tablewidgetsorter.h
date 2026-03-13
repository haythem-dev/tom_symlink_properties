#ifndef GUARD_UTIL_TABLEWIDGETSORTER_H
#define GUARD_UTIL_TABLEWIDGETSORTER_H

#include "libabbauw/sortorder.h"
#include <libbasarproperty_propertydescription.h>
#include <libutil/flag.h>
#include <list>
#include <map>

#pragma warning (push)
#pragma warning(disable: 4127 4481 4512)
#include <libbasarqtwidget_tablewidget.h>
#pragma warning (pop)

namespace util
{
    class TableWidgetSorter : public QObject
    {
        Q_OBJECT

    public:
        TableWidgetSorter();
        ~TableWidgetSorter();

        static QIcon                         drawSortIcon( util::SortOrder::Order, int prio = -1 );

        void                                 addSortableColumn( const basar::property::PropertyDescription& );
        
        void                                 setSortOrder( const util::SortOrder& );
        const util::SortOrder&               getSortOrder() const;

        typedef std::list< basar::VarString > ListAttributeCollection;
        void                                 setListAttributes( const ListAttributeCollection& );

        void                                 setTableWidget( BasarTableWidget* );

    private:
        TableWidgetSorter( const TableWidgetSorter& );
		TableWidgetSorter& operator=( const TableWidgetSorter& );

        void                                 wireEvents();
        void                                 updateListHeader();
        basar::property::PropertyDescription tableIndexToProperty( const int colIndex ) const;

    private slots:
        void                                 onHorizontalHeaderClicked( int );
        void                                 onTableDataSelectionChanged();

    private:
        BasarTableWidget*                    m_TableWidget;
        util::SortOrder                      m_SortOrder;

        typedef std::map< int, basar::property::PropertyDescription > TableIndexToPropertyDesc;
        TableIndexToPropertyDesc             m_TableIndexToPropertyDesc;
        ListAttributeCollection              m_ListAttributeCollection;
        libutil::misc::Flag                  m_EventsWiring;
    };

} // end namespace util

#endif // GUARD_UTIL_TABLEWIDGETSORTER_H
