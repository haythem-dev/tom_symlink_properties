#ifndef GUARD_LIBTENDER_INFRASTRUCTURE_ACCESSOR_ARTICLECOLLECTIONACC_H
#define GUARD_LIBTENDER_INFRASTRUCTURE_ACCESSOR_ARTICLECOLLECTIONACC_H

namespace libtender {
namespace infrastructure {
namespace accessor {
namespace article {

	BEGIN_QUERY_BUILDER_DECLARATION( SelectArticleByPattern )
	END_BUILDER_DECLARATION

    BEGIN_QUERY_BUILDER_DECLARATION( SelectArticleByPatternRS )
	END_BUILDER_DECLARATION


	BEGIN_QUERY_BUILDER_DECLARATION( SelectArticleByArticleCode )
	END_BUILDER_DECLARATION

    BEGIN_QUERY_BUILDER_DECLARATION( SelectArticleByArticleCodeRS )
	END_BUILDER_DECLARATION
/*
class SelectArticleBase: public basar::db::aspect::SQLQueryStringBuilder
{
public:
	~SelectArticleBase() {}
	virtual const boost::shared_ptr<basar::db::aspect::SQLStringBuilder> create() const = 0;

protected:
	virtual bool isExecutable() const;
	virtual void buildSQLString();

	virtual bool doIsExecutable() const = 0;
	virtual void doBuildSQLString() = 0;
};

class SelectArticleByPattern : public SelectArticleBase
{
public:
	~SelectArticleByPattern() {}
	virtual const boost::shared_ptr<basar::db::aspect::SQLStringBuilder> create() const;

protected:
	virtual bool doIsExecutable() const;
	virtual void doBuildSQLString();
};

class SelectArticleByPatternRS : public SelectArticleBase
{
public:
	~SelectArticleByPatternRS() {}
	virtual const boost::shared_ptr<basar::db::aspect::SQLStringBuilder> create() const;

protected:
	virtual bool doIsExecutable() const;
	virtual void doBuildSQLString();
};

class SelectArticleByArticleCode : public SelectArticleBase
{
public:
	~SelectArticleByArticleCode() {}
	virtual const boost::shared_ptr<basar::db::aspect::SQLStringBuilder> create() const;

protected:
	virtual bool doIsExecutable() const;
	virtual void doBuildSQLString();
};

class SelectArticleByArticleCodeRS : public SelectArticleBase
{
public:
	~SelectArticleByArticleCodeRS() {}
	virtual const boost::shared_ptr<basar::db::aspect::SQLStringBuilder> create() const;

protected:
	virtual bool doIsExecutable() const;
	virtual void doBuildSQLString();
};
*/


} // end namespace article
} // end namespace accessor
} // end namespace infrastructure
} // end namespace libtender

#endif // end GUARD_LIBTENDER_INFRASTRUCTURE_ACCESSOR_ARTICLECOLLECTIONACC_H
