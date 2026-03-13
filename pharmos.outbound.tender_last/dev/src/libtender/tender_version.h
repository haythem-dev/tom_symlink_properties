#ifndef GUARD_TENDER_VERSION_H
#define GUARD_TENDER_VERSION_H

#define TENDER_VERSION "01.02.02.02"


/*******************************************************************************************/
/*!
\mainpage notitle
\internal
\include svntags
\endinternal

____________________________________________________________________________
\version 01.02.02.02
\date 2026-02-18
\internal
- CPR0001965 replace synonyms liefer and herstel with supplier and manufacturer tables
\endinternal
\customer
\de
- CPR0001965 ersetzen der Synonyme liefer und herstel mit supplier und manufacturer tabellen
\en
- CPR0001965 replace synonyms liefer and herstel with supplier and manufacturer tables
\endcustomer
____________________________________________________________________________
\version 01.02.02.01
\date 2025-10-15
\internal
- CPR0002057 update to new login via UADM 
\endinternal
\customer
\de
- CPR0002057 update auf neues Login via UADM
\en
- CPR0002057 update to new login via UADM
\endcustomer
____________________________________________________________________________
\version 01.02.02.00
\date 2024-08-14
\internal
- SR_24044232 bugfix tenderinvoiceuc, omguc, imrproved logging
\endinternal
\customer
\de
- SR_24044232 bugfix tenderinvoiceuc, omguc, imrproved logging
\en
- SR_24044232 bugfix tenderinvoiceuc, omguc, imrproved logging
\endcustomer
____________________________________________________________________________
\version 01.02.01.00
\date 2023-09-13
\internal
- I-24017594: bugfix tenderordercreator not setting quantitydelivered
\endinternal
\customer
\de
- I-24017594: bugfix tenderordercreator not setting quantitydelivered
\en
- I-24017594: bugfix tenderordercreator not setting quantitydelivered
\endcustomer
____________________________________________________________________________
\version 01.02.00.00
\date 2023-09-13
\internal
- SR-23099417: migration from KSC server to CscService
\endinternal
\customer
\de
- SR-23099417: migration from KSC server to CscService
\en
- SR-23099417: migration from KSC server to CscService
\endcustomer
____________________________________________________________________________
\version 01.01.01.15
\date 2023-04-28
\internal
- CPR-220092 AMICO add discount conditions
\endinternal
\customer
\de
- CPR-220092 AMICO Ergänzung der Pflege der Rabattkonditionen für CH, Behebung Auswirkung auf anderen Ländern
\en
- CPR-220092 AMICO Add maintenance of discount conditions for CH, fix impact to other countries
\endcustomer
____________________________________________________________________________
\version 01.01.01.14
\date 2023-03-29
\internal
- CPR-220092 AMICO add discount conditions
\endinternal
\customer
\de
- CPR-220092 AMICO Ergänzung der Pflege der Rabattkonditionen für CH
\en
- CPR-220092 AMICO Add maintenance of discount conditions for CH
\endcustomer
____________________________________________________________________________
\version 01.01.01.12
\date 2021-11-29
\internal
-Update to VisualStudio 2019
\endinternal
\customer
\de
\en
\endcustomer
____________________________________________________________________________
\version 01.01.01.11
\date 2021-10-02
\internal
Testing for new TFS Azure Devops.
\endinternal
\customer
\de
\en
\endcustomer
____________________________________________________________________________
\version 01.01.01.09
\date 2020-11-12
\internal
--Merge from 64 bit into 32 bit develop and master
\endinternal
\customer
\de
\en
\endcustomer
____________________________________________________________________________
\version 01.01.01.05

\date 2020-07-21
\internal
-- This Build is only For 64 Bit 
-- Added a try catch for sending mail in 32 and 64 bit.
-- adding try catch in tenderordercreator mainuc for sendErrorMail

\endinternal
\customer
\de
\en
\endcustomer
________________________________________________________________________________________
\version 01.01.01.04
\date 2020-07-09
\internal
-- For Serbia they want to add in tender all Artikel even if its not available remove the check artikelaktiv = '1' OR al.artikelaktiv = '4'
\endinternal
\customer
\de
\en
-- For Serbia they want to add in tender all Artikel even if its not available remove the check artikelaktiv = '1' OR al.artikelaktiv = '4'
\endcustomer
________________________________________________________________________________________
\version 01.01.01.03
\date 2020-06-23
\internal

- I-20032795,SR-20072781 Made possible for contract number to be 45 characters long.

\endinternal
\customer
\de
\en
________________________________________________________________________________________
\version 01.01.01.02
\date 2018-06-02
\internal
- SR-20053007 Added the possibility to block/unblock an article in a tender.
\endinternal
\customer
\de
\en
- SR-20053007 Added the possibility to block/unblock an article in a tender.
\endcustomer
________________________________________________________________________________________
\version 01.01.01.01
\date 2018-06-14
\internal
- SR-20041434 CloseOrder remarkstext initialized with empty string 
\endinternal
\customer
\de
- SR-20041434 Leerer Bemerkung auf Kopfebene 
\en
- SR-20041434 Remark on head level set to empty string

\endcustomer

________________________________________________________________________________________
\version 01.01.00.10
\date 2018-06-14
\internal
- SR-18071995 Adjusted article search e.g. preferred_flag must be 1 and search article price with article no
\endinternal
\customer
\de
- SR-18071995 Artikelsuche angepasst
\en
- SR-18071995 Adjusted article search 
\endcustomer
________________________________________________________________________________________
\version 01.01.00.09
\date 2018-06-14
\internal
- Fixed Bug: Save new Tender with state activ, won't calculate the enddate.
- Fixed Bug: Save new Tender with state successor, won't calculate the enddate.
- Fixed Bug for CH: Clone tender with DateTo which is in the past
\endinternal
\customer
\de
- Bug fixing: Speichern eines neuen Tenders mit dem Status Aktiv, 
  berechnet nicht automatisch das EndeDatum des Tenders.
- Bug fixing: Speichern eines neuen Tenders mit dem Status Nachfolger, 
  berechnet nicht automatisch das EndeDatum des Tenders.
- Bug fixing für CH: Klonen eines Tenders mit einem Endedatum welches in der Vergangenheit liegt.
\en
- Fixed Bug: Save new Tender with state activ, won't calculate the enddate.
- Fixed Bug: Save new Tender with state successor, won't calculate the enddate.
- Fixed Bug for CH: Clone tender with DateTo which is in the past
\endcustomer
________________________________________________________________________________________
\version 01.01.00.08
\date 2018-05-02
\internal
- OMG Laufzeit nun nicht mehr hart codiert 3 Monate, UADM Parameter hinzugefügt
\endinternal
\customer
\de
- OMG Laufzeit ist nun über einen UADM Paramter steuerbar.
\en
- OMG duration is now adjustable through a UADM parameter.
\endcustomer
________________________________________________________________________________________
\version 01.01.00.07
\date 2018-02-12
\internal
- Display error when deleting tender fixed
\endinternal
\customer
\de
- Display error when deleting tender fixed
\en
- Display error when deleting tender fixed
\endcustomer
________________________________________________________________________________________
\version 01.01.00.06
\date 2018-02-12
\internal
- GUI for Switzerland and language file adjusted
\endinternal
\customer
\de
- GUI for Schweiz und languagefile angepasst.
\en
- GUI for Switzerland and language file adjusted
\endcustomer
________________________________________________________________________________________
\version 01.01.00.05
\date 2018-02-12
\internal
- Bugfix: No merge with other csc orders on creating a new one
\endinternal
\customer
\de
- Bugfix: Beim Erzeugen von Aufträgen im KSC werden neue Aufträge erzeugt, zurückgestellte bleiben unangetastet.
\en
\endcustomer
________________________________________________________________________________________
\version 01.01.00.04
\date 2018-02-05
\internal
- Bug fixing after adding the Bulgarian translation file.
\endinternal
\customer
\de
- Fixed issues after adding the Bulgarian translation file.
\en
- Fixed issues after adding the Bulgarian translation file.
\endcustomer
________________________________________________________________________________________
\version 01.01.00.03
\date 2018-01-25
\internal
- Bug fixing
\endinternal
\customer
\de
- Bug fixing
\en
- Bug fixing
\endcustomer
________________________________________________________________________________________
\version 01.01.00.02
\date 2018-01-25
\internal
- Bug fixing
\endinternal
\customer
\de
- Bug fixing
\en
- Bug fixing
\endcustomer
________________________________________________________________________________________
\version 01.01.00.01
\date 2018-01-25
\internal
- Show artikellangname if available, artikelname if not
- Bug fixing
- SR-18009093, SR-18008264
\endinternal
\customer
\de
- Artikellangname anzeigen wenn vorhanden, wenn nicht artikel name
- Bug fixing
- SR-18009093, SR-18008264
\en
- Show artikellangname if available, artikelname if not
- Bug fixing
- SR-18009093, SR-18008264
\endcustomer

________________________________________________________________________________________
\version 01.01.00.00
\date 2018-01-09
\internal
- OMG 2.0 in CH
\endinternal
\customer
\de
- OMG 2.0 in der Schweiz
\en
- OMG 2.0 in Switzerland
\endcustomer

________________________________________________________________________________________
\version 01.00.02.42
\date 2017-12-21
\internal
- Tender Overview dialoag now shows last 1000 tender instead of first.
- LanguageFileLoader
\endinternal
\customer
\de
- Tender Overview Dialog zeigt nun die 1000 neusten anstatt die 1000 ältesten Tender.
\en
- Tender Overview dialoag now shows last 1000 tender instead of first.
\endcustomer

________________________________________________________________________________________
\version 01.00.02.41
\date 2017-01-05
\internal
- Adjustments for the go live in Serbia.
\endinternal
\customer
\de
- Anpassungen für den go live in Serbien.
\en
- Adjustments for the go live in Serbia.
\endcustomer

________________________________________________________________________________________
\version 01.00.02.40
\date 2017-01-05
\internal
- Adjustments for the go live in Bulgaria.
- Two new fields: CustomerNo and BranchNo in ScheduledDeliveryMaintenance 
\endinternal
\customer
\de
- Anpassungen für den go live in Bulgarien.
- Zwei neue Felder: CustomerNo und BranchNo in ScheduledDeliveryMaintenance
\en
- Adjustments for the go live in Bulgaria.
- Two new fields: CustomerNo und BranchNo in ScheduledDeliveryMaintenance
\endcustomer

________________________________________________________________________________________
\version 01.00.02.39
\date 2016-06-20
\internal
- CR15A032: added an aditional field "customer group ID" and also adjusted the import for Serbia.
\endinternal
\customer
\de
- CR15A032: neues Feld "customer group ID" hinzugefügt & import für Serbien angepasst.
\en
- CR15A032: added an aditional field "customer group ID" and also adjusted the import for Serbia.
\endcustomer

________________________________________________________________________________________
\version 01.00.02.38
\date 2016-06-14
\internal
- Adjustments for the go live in Bulgaria and Serbia.
\endinternal
\customer
\de
- Anpassungen.
\en
- Adjustments.
\endcustomer

________________________________________________________________________________________
\version 01.00.02.37
\date 2016-05-11
\internal
- Undo last changes. + Comment out 3 sub selects in tendercollectionacc.
\endinternal
\customer
\de
- Letzte Anpassung rückgängig gemacht.
\en
- Undo last changes.
\endcustomer

________________________________________________________________________________________
\version 01.00.02.36
\date 2016-05-10
\internal
- Adjustments for a better performance.
\endinternal
\customer
\de
- Anpassungen für eine bessere Performance
\en
- Adjustments for a better performance.
\endcustomer

________________________________________________________________________________________
\version 01.00.02.35
\date 2016-04-11
\internal
- Bugfix: Deselect and select the same article in an existing delivery caused a basar exception.
\endinternal
\customer
\de
- Bugfix: Beim deselektieren und wieder selektieren eines Artikels in einer bereits bestehenden
  delivery erschien eine basar excpetion.
\en
- Bugfix: Deselect and select the same article in an existing delivery caused a basar exception.
\endcustomer

________________________________________________________________________________________
\version 01.00.02.34
\date 2016-03-31
\internal
- Code refactoring (magic numbers etc.)
- Bugfix: No inserting into pubatchorderpos table during clone process.
- Bugfix: No deleting of pubatchorderpos records when deleting a tender.
- Bugfix: No deleting of pubatchorderpos records when deleting a delivery.
- Bugfix: Request if the future delivieries of a tender should also be cloned 
did not pop up, when the current date is the last day of the month.
- An info message will pop up, when the user tries to delete a tender which has at least one delivery 
for which a purchase order proposal has already been created. 
\endinternal
\customer
\de
- Bugfix: Beim Klonen von Tendern wurden die Positionen nicht in die pubatchorderpos Tabelle geschrieben.
- Bugfix: Beim Löschprozess eines Tenders, werden die pubatchorderpos Einträge nicht gelöscht,
die noch keine proposalno besitzen.
- Bugfix: Beim Löschprozess einer delivery, werden die pubatchorderpos Einträge nicht gelöscht,
die noch keine proposalno besitzen.
- Bugfix: Keine Abfrage ob die deliveries die in der Zukunft liegen auch geklont werden sollen,
wenn das heutige Datum der letzte Tag im Monat ist.
- Von jetzt an erscheint eine Info Message, wenn der User versucht einen Tender zu löschen, 
der mindestens eine delivery besitzt, für die eine purchase order proposal erstellt worden ist.
\en
- Bugfix: No inserting into pubatchorderpos table during clone process.
- Bugfix: No deleting of pubatchorderpos records when deleting a tender.
- Bugfix: No deleting of pubatchorderpos records when deleting a delivery.
- Bugfix: Request if the future delivieries of a tender should also be cloned 
did not pop up, when the current date is the last day of the month.
- An info message will pop up, when the user tries to delete a tender which has at least one delivery 
for which a purchase order proposal has already been created. 
\endcustomer

________________________________________________________________________________________
\version 01.00.02.33
\date 2016-03-29
\internal
- Bugfix: Wrong ordernobatch on creating order proposals during tender import
\endinternal
\customer
\de
- Bugfix: Falsche ID wurde bei der Erzeugúng von Bestellvorschlägen angegeben beim Import von Tendern
\en
- Bugfix: Wrong id on creating order proposals during tender import
\endcustomer

________________________________________________________________________________________
\version 01.00.02.32
\date 2016-03-22
\internal
- Import of TenderDeliverySchedulePositionACC enhanced by BranchNoPOP 
\endinternal
\customer
\de
- Import von Lieferpositionen erweitert um Filial-Nr für den Bestellvorschlag
\en
- Import of scheduled delivery positions enhanced by branch number for order proposal 
\endcustomer

________________________________________________________________________________________
\version 01.00.02.31
\date 2016-03-08
\internal
- TenderDeliverySchedulePositionACC using the BranchNoPOP instead of the branchno from the tenderhead
- ScheduledDeliveryCollectionDM findByPattern is now executed with HOLD_RECORDSET.
- New column Order No in TDR-03 Dialog 
\endinternal
\customer
\de
- Bug fix: POP No wird nun richtig angezeigt.
- Bug fix: tenderordercreator batch
- Neue Spalte Order No in TDR-03 Dialog 
\en
- Bug fix: POP No will be displayed correct know.
- Bug fix: tenderordercreator batch
- New column Order No in TDR-03 Dialog 
\endcustomer

________________________________________________________________________________________
\version 01.00.02.30
\date 2016-02-24
\internal
- Tenderstate will now be saved in the tenderpos table.
- Partner No is the manufacturer again (before it was the supplier).
- The contract period was 01.01.nextyear - 31.12.nextyear, now it is the actual year.
- The supplierno field in the pubatchorderpos table will not be filled anymore.
- New column in TDR-03/04 "branch no for pop" 
- Bug fix: changed delivery date will now also be updated in the pubatchorderpos table
  when POP ist checked and no orderproposalno exists.
\endinternal
\customer
\de
- Partner no ist wieder der Hersteller (Davor lieferant).
- Beim erstellen eines neuen Tenders, war die contract period 01.01.dieses jahr - 31.12.nächstes jahr, von jetzt an wird das aktuelle Jahr verwendet.
- Neue Spalte in TDR-04 "Branch no for pop". Nur editierbar wenn Purchase order proposal checkbox mit einem Hacken versehen wurde.
\en
- Partner no is the manufacturer again (before it was the supplier).
- When creating a new Tender, the contract period was 01.01.nextyear - 31.12.nextyear, now it is the actual year.
- New column in TDR-04 "branch no for pop". Only editable when Purchase order proposal checkbox is checked.
\endcustomer

________________________________________________________________________________________
\version 01.00.02.29
\date 2016-02-17
\internal
- After a CSC order has been created, a new search for the tender protocols table will started, so the actual quantities are shown. 
- Partner is only enabled and changeable, when the state of the tender is "Capturing data".
- UADM POP_FUNCTIONALITY - für jedes Land, bestimmt ob purchase order proposals, beim erstellen von deliveries erstellt werden können.
- Tenderordercreator - error mail is only send when lackreason > 0 or ordered quantity > delivered quantity
\endinternal
\customer
\de
- Nachdem ein CSC order erstellt wurde, wird automatisch eine Suche der Tender protocols gestarted, damit die aktuellen Mengen angezeigt werden.
- Partner ist nur dann aktiv und änderbar, wenn der Status des Tenders "Capturing data" ist. 
- Tenderordercreator - Error Mail wird nur gesendet wenn error auftaucht oder die bestelle Menge größer als die gelieferte Menge ist.
\en
- After a CSC order has been created, a new search for the tender protocols table will started, so the actual quantities are shown. 
- Partner is only enabled and changeable, when the state of the tender is "Capturing data".
- Tenderordercreator - error mail is only send when an error occurs or the ordered quantity is bigger than the delivered quantity.
\endcustomer

________________________________________________________________________________________
\version 01.00.02.28
\date 2016-02-12
\internal
- CSC Invoices können nun mit Hilfer des Invoice dialogs erstellt werden.
- Beim erstellen von Invoices wird in die "tenderprotocol" Tabelle geschrieben.
- Protocols nur noch bei Tendern mit dem Typ "State" verfügbar.
- After a CSC order has been created, a new search for the tender protocols table will started, so the actual quantities are shown. 
- Partner is only enabled and changeable, when the state of the tender is "Capturing data".
\endinternal
\customer
\de
- Erstellen von CSC Invoices in Bulgarien nun möglich. (Betrifft nur Bulgarien).
- Änderung: Protocols nur noch bei Tendern mit dem Typ "State" verfügbar. (Betrifft nur Bulgarien).
- Nachdem ein CSC order erstellt wurde, wird automatisch eine Suche der Tender protocols gestarted, damit die aktuellen Mengen angezeigt werden.
- Partner ist nur dann aktiv und änderbar, wenn der Status des Tenders "Capturing data" ist. 
\en
- Creating CSC invoices now possible. (Concerns Bulgaria only).
- Change: Protocols only available for tenders with type "state" (Conecerns Bulgaria only).
- After a CSC order has been created, a new search for the tender protocols table will started, so the actual quantities are shown. 
- Partner is only enabled and changeable, when the state of the tender is "Capturing data".
\endcustomer

________________________________________________________________________________________
\version 01.00.02.27
\date 2016-02-04
\internal
- ArticleAcc SQL Statement angepasst, um Fehler bei der Erstellung von TOs/TIs zu vermeiden.
- TenderProtocol angepasst
- Partner angepasst -> lieferant inner join auf hersteller
\endinternal
\customer
\de
- Artikel die in der artikelzentral und artikellokal stehen bzw. das artikelaktiv flag auf 1 oder 4 sind valide.
\en
- Only articles which are in the artikelzentral and artikellokal table and have the artikelaktiv flag 1 or 4 are valid.
\endcustomer

________________________________________________________________________________________
\version 01.00.02.26
\date 2016-01-28
\internal
- Tender Protocol DM & ACC erstellt
- Tender Invoices - ausgewählte Orders werden im Dialog angezeigt
\endinternal
\customer
\de
- Die Protokolle für einen Tender, werde nun im Protocol Dialog angezeigt.
- Im Tender Invoice Dialog werden nun die ausgewählten Order angezeigt.
\en
- The protocols for the tender are displayed now in the protocol dialog.
- The selected orders for the invoice are displayed in the tender invoice dialog now.
\endcustomer

________________________________________________________________________________________
\version 01.00.02.25
\date 2016-01-14
\internal
- Bugfixing: Prüfung Delivery-Date gegen Start- und End-Date beim Import
\endinternal
\customer
\de
- Bugfixing: Beim Import der Schedules wird nun das Lieferdatum gegen Start- und End-Date des Tenders geprüft.
\en
- Bugfixing: On import of schedules the delivery date is getting checked: It has to be within contract period.
\endcustomer

________________________________________________________________________________________
\version 01.00.02.24
\date 2015-12-31
\internal
- Bugfixing: 
- Wenn die contract period abgelaufen ist, wird dem User nun eine warnung angezeigt wenn er versucht eine delivery anzulegen.
- TenderInvoices VC/UC
- TenderProtocol VC/UC
\endinternal
\customer
\de
- Bugfixing: Hinzufügen, löschen und wieder hinzufügen des selben Kunden in der Customer list führte zu einem Error.
- Wenn die contract period abgelaufen ist, wird dem User nun eine warnung angezeigt wenn er versucht eine delivery anzulegen.
\en
- Bugfixing: Adding, deleting and adding the same customer again in the customer list dialog.
- A warning message will be displayed, when the contract period is out of date and the user tries to add a new delivery.
\endcustomer

________________________________________________________________________________________
\version 01.00.02.23
\date 2015-12-23
\internal
- CustomerList (VC, UC, DM, ACC)
- Bug fixing löschen von Tendern.
- Bug fixing löschen von Tendern die eine Delivery in der Vergangenheit ohne CSC order no besitzen nicht möglich.
- Löschen von Tendern: statt dem deletedflag wird der tender nun komplett gelöscht.
\endinternal
\customer
\de
- CustomerList Dialog für Tender Bulgarien 
- Der Contract Info Dialog für Bulgarien wurde überarbeitet und angepasst.
- Bug fixing: löschen von Tendern
\en
- CustomerList dialog for Tender Bulgaria
- The contract info dialog for Bulgaria was adjusted.
- Bug fixing deleting tender.
\endcustomer

________________________________________________________________________________________
\version 01.00.02.22
\date 2015-12-09
\internal
- Contract Info Serbia VC angepasst
- Feld changedate in tenderhead & tenderpos wird beim inserten/updaten mit dem aktuellen Datum befüllt
- Feld nameofuser in tenderhead & tenderpos wird beim inserten/updaten mit dem aktuell Namen des Benutzers befüllt der im Moment in der Anwendungen angmeldet ist.
- TenderCollectionAcc stat ContractNo =, like
\endinternal
\customer
\de
- Der Contract Info Dialog für Serbien wurde überarbeitet und angepasst.
\en
- The contract info dialog for Serbia was adjusted. 
\endcustomer

________________________________________________________________________________________
\version 01.00.02.21
\date 2015-12-02
\internal
- Neuer VC: Contract Info FR VC (Tender Maintenance) 
- Neuer Picker: Pharmacy Chain VC, DM, UC und ACC
- TenderTypeEnum TT_STANDARD set to 1 from 0
- Im TenderOverview Dialog wird neben der Customer No auch der Customer Name angezeigt.
- In der Tabelle "tenderhead" wird ab jetzt beim speichern das Feld "changedate" befüllt
- TenderCollectionAcc pharmacychainno hinzugefügt
\endinternal
\customer
\de
- Im TenderOverview Dialog wird neben der Customer No auch der Customer Name angezeigt.
- Tender Serbien: Pharmacy Chain Picker ist nun verfügbar.
\en
- In the TenderOverview dialog is a new column named "Customer name"  
- A Pharmacy chain picker is now available for Serbia.
\endcustomer

________________________________________________________________________________________
\version 01.00.02.20
\date 2015-11-17
\internal
- A link to the user documentation can now be found in the help dialog.
\endinternal
\customer
\de
- Ein Link zur Benutzerdokumentation kann ab jetzt im Help Dialog gefunden werden.
\en
- A link to the user documentation can now be found in the help dialog.
\endcustomer

________________________________________________________________________________________
\version 01.00.02.19
\date 2015-11-13
\internal
- Bug fixing: Issue - Cloning a tender and deleting an articles caused a basar exception.
\endinternal
\customer
\de
- Fehlerbehebung: Beim klonen von Tendern wurde eine Fehler angezeigt wenn Artikel gelöscht wurden.
\en
- Bug fixing: Cloning a tender and deleting articles caused an exception.
\endcustomer

________________________________________________________________________________________
\version 01.00.02.18
\date 2015-10-29
\internal
- Bug fixing: Issue - After Deleting a Tender position, adding the same position was not able.
- Small GUI adjustments in dialog TDR-03 - Tender Maintenance 
- Bug fixing: Adding deliveries with a delivery date out of contract period was possible.
- A calendar will pop up on every date field now. 
- TDR-03 - Tender Maintenance: For the discount and price a point can also be used now as a decimal separator.
\endinternal
\customer
\de
\en
\endcustomer

*/

/*
________________________________________________________________________________________
\version XX.XX.XX.XX
\date XXXX-XX-XX
\internal
\endinternal
\customer
\de
\en
\endcustomer

*/

#endif 
