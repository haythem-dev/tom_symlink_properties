#ifndef GUARD_LIBTENDER_DOMMOD_TENDER_TENDERPOSITIONCOLLECTION_H
#define GUARD_LIBTENDER_DOMMOD_TENDER_TENDERPOSITIONCOLLECTION_H

#include "itenderpositioncollection.h"
#include <libutil/accessor.h>
#include <libutil/classinfo.h>

namespace basar
{
namespace db
{
namespace aspect
{
	class AccessorInstanceRef;
	class AccessorPropertyTable_YIterator;
	class AccessorPropertyTableRef;
}
}
}

namespace libtender
{
namespace domMod
{
namespace tender
{
class TenderPositionCollection : public ITenderPositionCollection
{
public:
	static const libutil::misc::ClassInfo& getClassInfo();

public:
	TenderPositionCollection();
	~TenderPositionCollection();

	void injectTenderPositionCollectionAccessor( libutil::infrastructure::accessor::IAccessorPtr );

	virtual void findByKeyFR(const basar::db::aspect::AccessorPropertyTable_YIterator);
	virtual void findByKeyRS(const basar::db::aspect::AccessorPropertyTable_YIterator);
	virtual void findByKeyBG(const basar::db::aspect::AccessorPropertyTable_YIterator);
	virtual void findByKeyCH(const basar::db::aspect::AccessorPropertyTable_YIterator);

	virtual basar::db::aspect::AccessorPropertyTable_YIterator addEmptyPosition();
	virtual void deletePosition( basar::db::aspect::AccessorPropertyTable_YIterator yit );

	virtual basar::db::aspect::AccessorPropertyTableRef get();
	virtual const basar::db::aspect::AccessorPropertyTableRef get() const;
	virtual basar::db::aspect::AccessorPropertyTable_YIterator getByArticleCode( const basar::VarString articleCode );

	virtual void setForeignKey( const basar::Int32 );
	virtual void setState(const TenderStateEnum tenderState);
	virtual void setNameOfUser( const std::string );
	virtual void save();
	virtual bool isEmpty() const;
	virtual void validate() const;
	virtual bool isValid() const;


private:
	TenderPositionCollection( const TenderPositionCollection& r );
	TenderPositionCollection& operator = ( const TenderPositionCollection& r );
	libutil::infrastructure::accessor::IAccessorPtr getAccessor() const;

	void saveDeletePositions();
	void saveExistingPositions();

	void completeForeignKey( basar::db::aspect::AccessorPropertyTable_YIterator );
	bool isForeignKeySet ( const basar::db::aspect::AccessorPropertyTable_YIterator yitPos ) const;

	bool isSavable() const;
	void checkNumberPositions() const;
	bool hasSavableStates( const basar::db::aspect::AccessorPropertyTable_YIterator yitPos ) const;
	bool hasDeleteStates( const basar::db::aspect::AccessorPropertyTable_YIterator yitPos ) const;

	const log4cplus::Logger & getLogger() const;
	const log4cplus::Logger 							m_Logger;
	libutil::infrastructure::accessor::IAccessorPtr		m_Accessor;
};

} // end namespace tender
} // end namespace domMod
} // end namespace libtender

#endif // GUARD_LIBTENDER_DOMMOD_TENDER_TENDERPOSITIONCOLLECTION_H
