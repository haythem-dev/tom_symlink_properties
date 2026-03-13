CREATE TABLE orderheadprotocol
(
                protocolid            serial  NOT NULL,
                branchno              smallint   DEFAULT 0 NOT NULL,
                orderno               integer   DEFAULT 0 NOT NULL,
                protocoldate          datetime year to second   DEFAULT CURRENT NOT NULL,
                activitytype          char(1)   DEFAULT "" NOT NULL,
                eventtype             smallint   DEFAULT 0 NOT NULL,
                processedby           smallint   DEFAULT 0 NOT NULL,
                changedattribute      varchar(30,6)   DEFAULT "" NOT NULL,
                oldvalue              varchar(30,1)   DEFAULT "" NOT NULL,
                newvalue              varchar(30,1)   DEFAULT "" NOT NULL,
                splitfrom             integer   DEFAULT 0 NOT NULL,
                splitto               integer   DEFAULT 0 NOT NULL,
                headertext            varchar(255,30)   DEFAULT "" NOT NULL,
                ordernocsc            integer   DEFAULT 0 NOT NULL,
                extratext             varchar(255,0)   DEFAULT "" NOT NULL
)
EXTENT SIZE 15000
NEXT SIZE 5000
LOCK MODE ROW
;

CREATE UNIQUE INDEX XPKorderheadprotocol ON orderheadprotocol
(
                protocolid            ASC
)
;

CREATE INDEX XIE1orderheadprotocol ON orderheadprotocol
(
                orderno               ASC
)
;
