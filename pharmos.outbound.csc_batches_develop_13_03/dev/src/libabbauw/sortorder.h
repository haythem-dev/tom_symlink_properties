#ifndef GUARD_LIBABBAUW_UTIL_SORTORDER_H
#define GUARD_LIBABBAUW_UTIL_SORTORDER_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include <libbasar_definitions.h>
#include <libbasarproperty_propertydescription.h>
#include <list>

//-------------------------------------------------------------------------------------------------//
// namespace section
//-------------------------------------------------------------------------------------------------//
namespace util
{
	class SortOrder
	{
	public:
		enum SORT
		{
			NONE,
			ASCENDING,
			DESCENDING
		};

		SortOrder();
		explicit SortOrder( const basar::VarString& fromStr );
		~SortOrder();

		typedef enum SORT                           Order;

		Order                                       getOrder(        const basar::property::PropertyDescription& property ) const;
		basar::Int32                                getPriority(     const basar::property::PropertyDescription& property ) const;
		void                                        setOrder(        const basar::property::PropertyDescription& property, Order order );
		const basar::property::PropertyDescription& propertyByIndex( const std::size_t ) const;

		void                                        clear();
		bool                                        empty() const;
		std::size_t                                 size()  const;
		void                                        swapProperties( const basar::property::PropertyDescription&, 
			                                                        const basar::property::PropertyDescription& );

		// serialize / deserialize
		basar::VarString                            toString() const;
		void                                        fromString( const basar::VarString& );

	private:
		typedef std::pair< basar::property::PropertyDescription, Order >      PropertySortOrder;
		typedef std::list< PropertySortOrder >                                PropertySortOrders;
		typedef std::pair< PropertySortOrders::iterator, basar::Int32 >       IteratorIndexPair;
		typedef std::pair< PropertySortOrders::const_iterator, basar::Int32 > ConstIteratorIndexPair;

		IteratorIndexPair                           find( const basar::property::PropertyDescription& property );
		ConstIteratorIndexPair                      find( const basar::property::PropertyDescription& property ) const;
		basar::VarString                            sortOrderToString( Order order )                 const;
		Order                                       stringToSortOrder( const basar::VarString& str ) const;

	private:
		PropertySortOrders                          m_PropertySortOrders;
	};

}	// end namespace util

#endif //end GUARD_LIBABBAUW_UTIL_SORTORDER_H
