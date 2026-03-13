#ifndef GUARD_TENDER_VIEWCONN_TENDERARTICLEINFO_H
#define GUARD_TENDER_VIEWCONN_TENDERARTICLEINFO_H

#pragma warning (push)
#pragma warning(disable: 4127 4231 4244 4251 4311 4312 4481 4511 4512 4800)
#include "ui_tenderarticleinfo.h"
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
        class TenderArticleInfo : public ITenderArticleInfo
        {
            Q_OBJECT

            public:
                TenderArticleInfo( QWidget* parent , const basar::Int16 area );

                ~TenderArticleInfo();

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
				void onCheckArticle();

            protected:
                virtual void focusInEvent( QFocusEvent* event);
                virtual void focusOutEvent( QFocusEvent* event);

            private:
                void wireEvents();
                void setupArticleInfo();

                Ui::tenderArticleInfo                    ui;
                const log4cplus::Logger                             m_Logger;
                libutil::misc::Flag                                 m_EventsWiring;
                libtender::domMod::tender::TenderStateCollectionPtr m_TenderStateCollection;
                libtender::domMod::tender::TenderTypeCollectionPtr  m_TenderTypeCollection;
                //QAction *                                           m_StartSearchAction;

                InputValidator                                      m_ArticleInputValidator;
                InputValidator                                      m_ArticlePriceDiscountInputValidator;
                basar::UInt16                                       m_Area;
        };
    }
}

#endif //end GUARD_TENDER_VIEWCONN_TENDERARTICLEINFO_H
