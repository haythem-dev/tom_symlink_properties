CREATE TABLE orderpos
(
    branchno            	smallint   		DEFAULT 0 	NOT NULL,
	orderno             	integer   		DEFAULT 0 	NOT NULL,
	posno 					integer   		DEFAULT 0 	NOT NULL,
	articleno 				integer   		DEFAULT 0 	NOT NULL,
	article_code 			char(25)   		DEFAULT "" 	NOT NULL,
	orderedqty				integer			DEFAULT 0	NOT NULL,
	nonchargedqty			integer			DEFAULT 0	NOT NULL,
	textfield				char(30)		DEFAULT ""	NOT NULL,
	ibtrecordtype			char(2)			DEFAULT ""	NOT NULL,
	selloffriseflag			char(1)			DEFAULT ""	NOT NULL,
	customerorderno			integer			DEFAULT 0	NOT NULL,
	customerposno			integer			DEFAULT 0	NOT NULL,
	subsequentdeliveryqty	integer			DEFAULT 0	NOT NULL,
	bookingflag				char(1)			DEFAULT ""	NOT NULL,
	wholesalerpurchaseprice	decimal(7,2)	DEFAULT 0	NOT NULL,
	subsequentdeliveryflag	char(1)			DEFAULT ""	NOT NULL,
	customerordernoadd		smallint		DEFAULT 0	NOT NULL,
	estorelflashcode		char(1)			DEFAULT ""	NOT NULL,
	lkmailshortageqty		integer			DEFAULT 0	NOT NULL,
	discountpct				decimal(5,2)	DEFAULT 0	NOT NULL,
	discounttype			char(2)			DEFAULT ""	NOT NULL,
	batch					char(25)		DEFAULT ""	NOT NULL,
	expirydate				integer			DEFAULT 0	NOT NULL,
	entrytype				char(1)			DEFAULT ""	NOT NULL,
	fixbatch				smallint		DEFAULT 0	NOT NULL
)
;

CREATE UNIQUE INDEX xpkorderpos ON orderpos
(
    branchno            ASC,
	orderno				ASC,
	posno				ASC
);
