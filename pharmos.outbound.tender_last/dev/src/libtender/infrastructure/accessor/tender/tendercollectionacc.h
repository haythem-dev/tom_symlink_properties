#ifndef GUARD_LIBTENDER_INFRASTRUCTURE_ACCESSOR_TENDERCOLLECTIONACC_H
#define GUARD_LIBTENDER_INFRASTRUCTURE_ACCESSOR_TENDERCOLLECTIONACC_H

//-------------------------------------------------------------------------------------------------//
// sqlstring builder declaration section
//-------------------------------------------------------------------------------------------------//
namespace libtender
{
namespace infrastructure
{
namespace accessor
{
namespace tender
{

    BEGIN_QUERY_BUILDER_DECLARATION( SelectSuccessorTender )
    END_BUILDER_DECLARATION

    BEGIN_QUERY_BUILDER_DECLARATION( SelectTenderByEndDate )
    END_BUILDER_DECLARATION

	BEGIN_QUERY_BUILDER_DECLARATION( SelectExpiredTenders )
    END_BUILDER_DECLARATION

    BEGIN_QUERY_BUILDER_DECLARATION( SelectValidTenders )
    END_BUILDER_DECLARATION

class SelectTenderByPatternBase: public basar::db::aspect::SQLQueryStringBuilder
{
public:
	~SelectTenderByPatternBase() {}
	virtual const boost::shared_ptr<basar::db::aspect::SQLStringBuilder> create() const = 0;

protected:
	virtual bool isExecutable() const;
	virtual void buildSQLString();

	virtual bool doIsExecutable() const = 0;
	virtual void doBuildSQLString() = 0;
};

class SelectTenderByPattern : public SelectTenderByPatternBase
{
public:
	~SelectTenderByPattern() {}
	virtual const boost::shared_ptr<basar::db::aspect::SQLStringBuilder> create() const;

protected:
	virtual bool doIsExecutable() const;
	virtual void doBuildSQLString();
};

class SelectTenderByPatternCH : public SelectTenderByPatternBase
{
public:
	~SelectTenderByPatternCH() {}
	virtual const boost::shared_ptr<basar::db::aspect::SQLStringBuilder> create() const;

protected:
	virtual bool doIsExecutable() const;
	virtual void doBuildSQLString();
};

}
}
}
}

#endif //end GUARD_LIBTENDER_INFRASTRUCTURE_ACCESSOR_TENDERCOLLECTIONACC_H
