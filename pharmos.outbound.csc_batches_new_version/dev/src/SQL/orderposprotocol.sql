CREATE TABLE orderposprotocol
(
                protocolid            serial  NOT NULL,
                branchno              smallint   DEFAULT 0 NOT NULL,
                orderno               integer   DEFAULT 0 NOT NULL,
                posno                 integer   DEFAULT 0 NOT NULL,
                protocoldate          datetime year to second   DEFAULT CURRENT NOT NULL,
                activitytype          char(1)   DEFAULT "" NOT NULL,
                eventtype             smallint   DEFAULT 0 NOT NULL,
                processedby           smallint   DEFAULT 0 NOT NULL,
                articleno             integer   DEFAULT 0 NOT NULL,
                orderedqty            integer   DEFAULT 0 NOT NULL,
                nonchargedqty         integer   DEFAULT 0 NOT NULL,
                discountpct           decimal(5,2)   DEFAULT 0 NOT NULL,
                splitfrom             integer   DEFAULT 0 NOT NULL,
                view                  char(1)   DEFAULT "" NOT NULL,
                orderrequestqty       integer   DEFAULT 0 NOT NULL,
                itemtext              varchar(255,0)   DEFAULT "" NOT NULL,
                extratext             varchar(255,0)   DEFAULT "" NOT NULL
)
EXTENT SIZE 40000
NEXT SIZE 15000
LOCK MODE ROW
;

CREATE UNIQUE INDEX XPKorderposprotocol ON orderposprotocol
(
                protocolid            ASC
)
;

CREATE INDEX XIE1orderposprotocol ON orderposprotocol
(
                orderno               ASC,
                posno                 ASC
)
;
