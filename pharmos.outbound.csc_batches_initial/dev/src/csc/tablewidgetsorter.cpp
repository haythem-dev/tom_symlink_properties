#include "tablewidgetsorter.h"

#include <QtWidgets/QHeaderView>
#pragma warning (push)
#pragma warning(disable: 4251 4800)
#include <QtGui/QPainter>
#pragma warning (pop)

namespace util
{
    TableWidgetSorter::TableWidgetSorter() : m_TableWidget( 0 ), m_EventsWiring( "TableWidgetSorter::EventsWiring" )
    {
    }

    TableWidgetSorter::~TableWidgetSorter()
    {
    }

    // static
    QIcon TableWidgetSorter::drawSortIcon( util::SortOrder::Order order, int prio )
    {
        static QWidget widget;

	    QPixmap arrow = QPixmap( 10, 16 );
	    arrow.fill( Qt::transparent );

	    QPainter painter( &arrow );
	    painter.setPen( Qt::red );
	
	    QFont font = painter.font();
	    font.setBold( true);
	    font.setPointSize( 6 );
	    painter.setFont( font );
	
	    QStyleOptionHeader opt;
	    opt.rect = QRect( 0, 5, 12, 12 );

	    if( order == util::SortOrder::DESCENDING )
	    {
		    opt.sortIndicator = QStyleOptionHeader::SortUp;
            widget.style()->drawPrimitive( QStyle::PE_IndicatorArrowDown, &opt, &painter );
            if( prio > 0 )
            {
		        painter.drawText( QRect( 0, 0, 12, 8 ), Qt::AlignCenter, QString::number(prio) );
            }
	    }
	    else if( order == util::SortOrder::ASCENDING )
	    {
		    opt.sortIndicator = QStyleOptionHeader::SortDown;
		    widget.style()->drawPrimitive( QStyle::PE_IndicatorArrowUp, &opt, &painter );
            if( prio > 0 )
            {
		        painter.drawText( QRect( 0, 0, 12, 8 ), Qt::AlignCenter, QString::number(prio) );
            }
	    }
	    else // order == libabbauw::util::SortOrder::NONE
        {
		    widget.style()->drawPrimitive( QStyle::PE_IndicatorArrowRight, &opt, &painter );
        }

	    return QIcon( arrow );
    }

    // private
    void TableWidgetSorter::wireEvents()
    {
        if( ! m_EventsWiring.isSet() )
        {
            connect( m_TableWidget->horizontalHeader(), SIGNAL( sectionClicked( int )                                 ),
		             this,                              SLOT  ( onHorizontalHeaderClicked( int ) )                    );
            
            connect( m_TableWidget,                     SIGNAL( dataSelectionChanged()                                ),
	                 this,                              SLOT  ( onTableDataSelectionChanged() )                       );

            m_EventsWiring.set();
        }
    }

    basar::property::PropertyDescription TableWidgetSorter::tableIndexToProperty( const int columnIndex ) const
    {
        basar::property::PropertyDescription prop;
        TableIndexToPropertyDesc::const_iterator it = m_TableIndexToPropertyDesc.find( columnIndex );
        if( it != m_TableIndexToPropertyDesc.end() )
        {
            prop = it->second;
        }

        return prop;
    }

    void TableWidgetSorter::updateListHeader()
    {
        for( int column = 0; column < m_TableWidget->columnCount(); column++ )
	    {
		    basar::property::PropertyDescription prop = tableIndexToProperty( column );
		    if( ! prop.getName().empty() )
		    {
                QIcon sortIcon = drawSortIcon( m_SortOrder.getOrder( prop ), m_SortOrder.getPriority( prop ) );
                m_TableWidget->horizontalHeaderItem( column )->setIcon( sortIcon );
		    }
	    }
    }

    // slots
    void TableWidgetSorter::onHorizontalHeaderClicked( int columnIndex )
    {
        const basar::property::PropertyDescription propertyDesc = tableIndexToProperty( columnIndex );

        if( propertyDesc.getType().getType() != basar::UNKNOWN )
        {
            // switch to the next sorting order on each click:
	        // From NONE order to ascending, from ascending to descending and from descending back to NONE
	        util::SortOrder::Order order = m_SortOrder.getOrder( propertyDesc );
	        if( order == util::SortOrder::NONE )
		        order = util::SortOrder::ASCENDING;
	        else if( order == util::SortOrder::ASCENDING )
		        order = util::SortOrder::DESCENDING;
	        else
		        order = util::SortOrder::NONE;

	        m_SortOrder.setOrder( propertyDesc, order );
	
	        updateListHeader();
        }
    }

    void TableWidgetSorter::onTableDataSelectionChanged()
    {
        updateListHeader();
    }

    // public
    void TableWidgetSorter::addSortableColumn( const basar::property::PropertyDescription& propDesc )
    {
        int i = 0;
        for( ListAttributeCollection::const_iterator it = m_ListAttributeCollection.begin(); it != m_ListAttributeCollection.end(); ++it, ++i )
        {
            const basar::VarString& desc = *it;
            if( desc == propDesc.getName() )
            {
                m_TableIndexToPropertyDesc[i] = propDesc;
                break;
            }
        }
    }

    void TableWidgetSorter::setSortOrder( const util::SortOrder& sortOrder )
    {
        m_SortOrder = sortOrder;
        updateListHeader();
    }

    const util::SortOrder& TableWidgetSorter::getSortOrder() const
    {
        return m_SortOrder;
    }

    void TableWidgetSorter::setListAttributes( const ListAttributeCollection&  attrCollection )
    {
        m_ListAttributeCollection = attrCollection;
    }

    void TableWidgetSorter::setTableWidget( BasarTableWidget* tableWidget )
    {
        m_TableWidget = tableWidget;
        wireEvents();
    }

} // end namespace util
