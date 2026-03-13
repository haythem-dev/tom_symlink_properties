SELECT 
	op.branchno, az.hersteller_nr, op.articleno, az.artikel_name, az.einheit,
	op.orderproposalqty, ap.preisekgrosso, ap.preisekgrosso*op.orderproposalqty as Gesamtwert, op.orderproposaltime, 'FALSE' as BESTELLT_FLAG, op.purchaseorderno
FROM orderproposal o
	INNER JOIN artikelzentral az ON op.articleno=az.artikel_nr 
	INNER JOIN artikelpreis ap ON op.articleno=ap.artikel_nr 
WHERE DATE(op.orderproposaltime)>=TO_DATE('20120101', '%Y%m%d') 
	AND 
	DATE(op.orderproposaltime)<=TO_DATE('20131231', '%Y%m%d') 
ORDER BY op.branchno ASC, az.hersteller_nr ASC, op.articleno ASC, op.orderproposaltime ASC
INTO TEMP temp2
; 

UPDATE orderproposal
SET articleno=4935414 
WHERE orderbatch=2;


update orderproposal set articleno=5479674 where orderproposaltime > '2013-07-26 15:45:00.0';

update orderproposal set purchaseorderno=1090 where orderproposaltime > '2013-07-26 15:45:00.0';

UPDATE temp1
SET BESTELLT_FLAG='TRUE'
WHERE purchaseorderno>0;

update orderproposal set transfertimebi='9999-12-31 23:59:59.0';


csc_batches_biauswertung.bin -country ch -db zpos3ch -vz 12 -hnr 4 -pzn all -startdate 20130615 -enddate 20130731
csc_batches_biauswertung.bin -country ch -db zpos3ch -vz 12 -hnr all -pzn all -startdate 20130615 -enddate 20130731
csc_batches_biauswertung.bin -country ch -db zpos3ch -vz all -hnr all -pzn all -startdate 20130615 -enddate 20130731
csc_batches_biauswertung.bin -country ch -db zpos3ch -vz all -hnr all -pzn 4935414 -startdate 20130615 -enddate 20130731


csc_batches_biauswertung.bin -country ch -db och21 -vz 12 -hnr 4 -pzn all -startdate 20130615 -enddate 20130731
csc_batches_biauswertung.bin -country ch -db och21 -vz 12 -hnr all -pzn all -startdate 20130615 -enddate 20130731
csc_batches_biauswertung.bin -country ch -db och21 -vz all -hnr all -pzn all -startdate 20130615 -enddate 20130731
csc_batches_biauswertung.bin -country ch -db och21 -vz all -hnr all -pzn 4935414 -startdate 20130615 -enddate 20130731
csc_batches_biauswertung.bin -country ch -db och21 -vz 12 -hnr 4 -pzn 4935414 -startdate 20130615 -enddate 20130731

update orderproposal set orderproposalqty=15 where articleno=4935414;
update orderproposal set orderproposalqty=8 where articleno=5479674;


csc_batches_history.sh -start -branchno 13 -ltorderdate 20100315 -minagefororderproposal 30
csc_batches_history.sh -start -branchno 13 -ltorderdate 20130717 -minagefororderproposal 30

//test orderproposal for bi-export
//erstelle neue Datensätze
insert into orderproposal
(branchno, articleno, orderproposalqty, ordernobatch, posnobatch, orderproposaltime, orderproposalno)
values
(89, 2221811, 3, 101, 102, (CURRENT YEAR TO MINUTE -7 UNITS DAY),1003 );

insert into orderproposal
(branchno, articleno, orderproposalqty, ordernobatch, posnobatch, orderproposaltime, orderproposalno)
values
(89, 2221811, 4, 111, 112, (CURRENT YEAR TO MINUTE -7 UNITS DAY),1013 );

insert into orderproposal
(branchno, articleno, orderproposalqty, ordernobatch, posnobatch, orderproposaltime, orderproposalno)
values
(89, 2221811, 5, 121, 122, (CURRENT YEAR TO MINUTE -7 UNITS DAY),1023 );

insert into orderproposal
(branchno, articleno, orderproposalqty, ordernobatch, posnobatch, orderproposaltime, orderproposalno)
values
(89, 2221811, 6, 131, 132, (CURRENT YEAR TO MINUTE -7 UNITS DAY),1033 );

insert into orderproposal
(branchno, articleno, orderproposalqty, ordernobatch, posnobatch, orderproposaltime, orderproposalno)
values
(89, 2221811, 0, 141, 142, (CURRENT YEAR TO MINUTE -7 UNITS DAY),1043 );

insert into orderproposal
(branchno, articleno, orderproposalqty, ordernobatch, posnobatch, orderproposaltime, orderproposalno)
values
(89, 2221811, 0, 151, 152, (CURRENT YEAR TO MINUTE -7 UNITS DAY),1053 );

insert into orderproposal
(branchno, articleno, orderproposalqty, ordernobatch, posnobatch, orderproposaltime, orderproposalno)
values
(89, 2221811, 0, 161, 162, (CURRENT YEAR TO MINUTE -7 UNITS DAY),1063 );