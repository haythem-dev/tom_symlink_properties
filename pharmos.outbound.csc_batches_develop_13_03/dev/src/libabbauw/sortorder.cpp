#include "sortorder.h"

#include <libbasarcmnutil_bstring.h>

namespace util
{

SortOrder::SortOrder()
{
}

SortOrder::SortOrder( const basar::VarString& fromStr )
{
	fromString( fromStr );
}

SortOrder::~SortOrder()
{
}

SortOrder::Order SortOrder::getOrder( const basar::property::PropertyDescription& property ) const
{
	Order order = NONE;
	ConstIteratorIndexPair iteratorIndex = find( property );
	if( iteratorIndex.first != m_PropertySortOrders.end() )
		order = iteratorIndex.first->second;

	return order;
}

basar::Int32 SortOrder::getPriority( const basar::property::PropertyDescription& property ) const
{
	basar::Int32 prio = -1;
	ConstIteratorIndexPair iteratorIndex = find( property );
	if( iteratorIndex.first != m_PropertySortOrders.end() )
		prio = iteratorIndex.second;

	return prio;
}

void SortOrder::setOrder( const basar::property::PropertyDescription& property, Order order )
{
	IteratorIndexPair iteratorIndex = find( property );
	if( iteratorIndex.first != m_PropertySortOrders.end() )
		if( order != NONE )
			iteratorIndex.first->second = order;
		else
			m_PropertySortOrders.erase( iteratorIndex.first );
	else
		if( order != NONE )
			m_PropertySortOrders.push_back( PropertySortOrder( property, order ) );
}

const basar::property::PropertyDescription& SortOrder::propertyByIndex( const std::size_t index ) const
{
	std::size_t i = 0;
	PropertySortOrders::const_iterator it;

	for( it = m_PropertySortOrders.begin(); it != m_PropertySortOrders.end() && i < index; ++it, ++i );

	return it->first;
}

void SortOrder::swapProperties( const basar::property::PropertyDescription& p1, 
			                            const basar::property::PropertyDescription& p2 )
{
	IteratorIndexPair iteratorIndex1 = find( p1 );
	IteratorIndexPair iteratorIndex2 = find( p2 );

	if( iteratorIndex1.first != m_PropertySortOrders.end() && iteratorIndex2.first != m_PropertySortOrders.end() )
		std::iter_swap( iteratorIndex1.first, iteratorIndex2.first );	
}

void SortOrder::clear()
{
	m_PropertySortOrders.clear();
}

std::size_t SortOrder::size() const
{
	return m_PropertySortOrders.size();
}

bool SortOrder::empty() const
{
	return m_PropertySortOrders.empty();
}

basar::VarString SortOrder::toString() const
{
	basar::VarString result;
	for( PropertySortOrders::const_iterator it = m_PropertySortOrders.begin(); it != m_PropertySortOrders.end(); ++it )
		result += it->first.getName() + " " + sortOrderToString( it->second ) + ", ";

	// delete last comma + blank
	if( result.length() > 2 )
		result = result.substr(0, result.length() - 2 );

	return result;
}

void SortOrder::fromString( const basar::VarString& fromStr )
{
	basar::cmnutil::CollBString splitStrs = fromStr.tokenize( ", ", true );
	for (basar::cmnutil::CollBString::const_iterator it = splitStrs.begin(); it != splitStrs.end(); ++it )
	{
		PropertySortOrder propSortOrder = PropertySortOrder( basar::property::PropertyDescription( it->left(' ').c_str() ), 
			                                                 stringToSortOrder( it->right(' ') ) );
		m_PropertySortOrders.push_back( propSortOrder );
	}
}

SortOrder::IteratorIndexPair SortOrder::find( const basar::property::PropertyDescription& property )
{
	basar::Int32 index = 1;
	PropertySortOrders::iterator it;

	for( it = m_PropertySortOrders.begin(); it != m_PropertySortOrders.end(); ++it, ++index )
		if( it->first.getName() == property.getName() )
			break;

	return IteratorIndexPair( it, index );
}

SortOrder::ConstIteratorIndexPair SortOrder::find( const basar::property::PropertyDescription& property ) const
{
	basar::Int32 index = 1;
	PropertySortOrders::const_iterator it;

	for( it = m_PropertySortOrders.begin(); it != m_PropertySortOrders.end(); ++it, ++index )
		if( it->first.getName() == property.getName() )
			break;

	return ConstIteratorIndexPair( it, index );
}

basar::VarString SortOrder::sortOrderToString( Order order ) const
{
	basar::VarString str;

	switch( order )
	{
	case NONE:
		str = "NONE";
		break;
	case ASCENDING:
		str = "ASC";
		break;
	case DESCENDING:
		str = "DESC";
		break;
	}

	return str;
}

SortOrder::Order SortOrder::stringToSortOrder( const basar::VarString& str ) const
{
	Order order = NONE;

	if( str == "ASC" )
		order = ASCENDING;
	else if( str == "DESC" )
		order = DESCENDING;

	return order;
}

} // end namespace util
