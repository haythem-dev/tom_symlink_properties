#ifndef GUARD_TENDER_VIEWCONN_TENDERARTICLEINFOSWITZERLAND_H
#define GUARD_TENDER_VIEWCONN_TENDERARTICLEINFOSWITZERLAND_H

#pragma warning (push)
#pragma warning(disable: 4127 4231 4244 4251 4311 4312 4481 4511 4512 4800)
#include "ui_tenderarticleinfoswitzerland.h"
#include <QObject>
#pragma warning (pop)

#include "itenderarticleinfo.h"

#include <libtender/domainmodule/tender/tendertypecollectionptr.h>
#include <libtender/domainmodule/tender/tenderstatecollectionptr.h>

#include <libutil/flag.h>
#include <libutil/viewconn.h>
#include "inputvalidator.h"


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

namespace log4cplus
{
    class Logger;
}

namespace tender
{
namespace viewConn
{
class TenderArticleInfoSwitzerland : public ITenderArticleInfo
{
Q_OBJECT

public:
	TenderArticleInfoSwitzerland( QWidget* parent = 0 );
	virtual ~TenderArticleInfoSwitzerland();

	void setFocus();
	void resetArticle();
	void matchToTenderPosition( basar::db::aspect::AccessorPropertyTable_YIterator yitTender );
	void matchFromTenderPosition( basar::db::aspect::AccessorPropertyTable_YIterator yitTender );
	basar::VarString getArticleCode();
	void matchFromArticle( basar::db::aspect::AccessorPropertyTable_YIterator article );
	void resetArticleName();
	void resetTenderPosition();
	void disableArticleCode();
	void enableArticleCode();
	void setFocusArticleCode();
	void selectArticlecode();
	void setFocusQuantity() ;
	void selectQuantity() ;
	void enableInputValidator( bool ip );
	bool hasFocus();
	void addArtValidator();

signals:
	void inputCodeQtyValid();
	void inputCodeQtyInvalid();

private slots:
	void onStartSearch();
	void onInputValid();
	void onCheckArticle();

protected:
	virtual void focusInEvent( QFocusEvent* event);
	virtual void focusOutEvent( QFocusEvent* event);

private:
	void wireEvents();
	void setupArticleInfo();
	const basar::Decimal getDecimal( const QString & str ) const;
	basar::Int32 getInt( const QString & str ) const;

	Ui::tenderArticleInfoSwitzerland                    ui;
	const log4cplus::Logger                             m_Logger;
	libutil::misc::Flag                                 m_EventsWiring;
	libtender::domMod::tender::TenderStateCollectionPtr m_TenderStateCollection;
	libtender::domMod::tender::TenderTypeCollectionPtr  m_TenderTypeCollection;
	InputValidator                                      m_ArticleInputValidator;
	InputValidator                                      m_ArticleDiscountInputValidator;
};
}
}

#endif //end GUARD_TENDER_VIEWCONN_TENDERARTICLEINFOSWITZERLAND_H
