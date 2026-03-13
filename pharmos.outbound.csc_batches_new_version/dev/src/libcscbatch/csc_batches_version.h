#ifndef GUARD_CSC_BATCHES_VERSION_H
#define GUARD_CSC_BATCHES_VERSION_H

#define CSC_BATCHES_VERSION "02.04.01.13"

/*******************************************************************************************/
/*!
\mainpage CSC-Batches
________________________________________________________________________________________
\version	02.04.01.13
\date		10.09.2025
\internal
 CPR0002057: add new UADM login
\endinternal
________________________________________________________________________________________
\version	02.04.01.11
\date		15.08.2025
\internal
 CPR0002688: remove discount column from mainview
\endinternal
________________________________________________________________________________________
\version	02.04.01.10
\date		05.08.2025
\internal
 CPR0002688: add GM03 discount
\endinternal
________________________________________________________________________________________
\version	02.04.01.08
\date		17.12.2024
\internal
 I-24107933: avosano logo for TransferOrder.ps (CH, branch 12, 13 + 16)
\endinternal
________________________________________________________________________________________
\version	02.04.01.07
\date		09.09.2024
\internal
 I-24057273: GM03 handling added for automated closing process in CSC
\endinternal
________________________________________________________________________________________
\version	02.04.01.06
\date		05.09.2024
\internal
 P-17000287: Automated EDI Input File Error Handling
\endinternal
________________________________________________________________________________________
\version	02.04.01.05
\date		13.08.2024
\internal
 SR-24100195: Sorting in ABBA-UW App
\endinternal
________________________________________________________________________________________
\version	02.04.01.04
\date		03.06.2024
\internal
 CPR-220424: Add Header Text field to be used when creating new order
\endinternal
________________________________________________________________________________________
\version	02.04.01.03
\date		28.04.2023
\internal
 CPR-220092: AMICO fix maintenance of orders and impact to other countries
\endinternal
________________________________________________________________________________________
\version	02.04.01.02
\date		29.03.2023
\internal
 CPR-220092: AMICO added maintenance of discount flags and values in batches frontend
\endinternal
________________________________________________________________________________________
\version	02.04.01.01
\date		10.02.2023
\internal
 fix for different shell and cfg scripts for cscbatcheshistory
\endinternal
________________________________________________________________________________________
\version	02.04.01.00
\date		09.02.2023
\internal
 CPR-220092: AMICO - discount flags added for cscbatcheshistory
\endinternal
________________________________________________________________________________________
\version	02.04.00.00
\date		02.05.2022
\internal
 CPR-210587: CMI21 - Added features for processing CMI transfer orders
\endinternal

________________________________________________________________________________________
\version	02.03.01.02
\date		29.10.2021
\internal
 Just a test to see if everything is working in the new TFS
\endinternal

________________________________________________________________________________________
\version	02.03.01.01
\date		25.03.2021
\internal
 Adjust cscbatchauto.sh and .option for deployment in PFR
\endinternal

________________________________________________________________________________________
\version	02.03.00.00
\date		28.01.2021
\internal
merge 64 bit branch cscbatches64 into develop
\endinternal

________________________________________________________________________________________
\version	02.02.00.16
\date		16.11.2020
\internal
 merge 64 bit branch cscbatches64 into develop
\endinternal
________________________________________________________________________________________

\version	02.02.00.15
\date		04.09.2020
\internal
- EDI 2.0: allow UEWAUF files with region specifier dc (deutschland comarch)
\endinternal

________________________________________________________________________________________
\version	02.02.00.14
\date		04.09.2020
\internal
- Bugfix: I-20047229 moving files from transfer to input directory
\endinternal
________________________________________________________________________________________
\version	02.02.01.02
\date		23.04.2020
\internal
- I-20017840: Doppelte Überweiser - Fix vorherige version - copy und delete statt rename, da rename nicht funktioniert zwischen 2 Partitionen
\endinternal

________________________________________________________________________________________
\version	02.02.01.01
\date		16.04.2020
\internal
- I-20017840: Doppelte Überweiser - csc_batches_edi_input: uewauf-files will be moved now, not copied. New error dir under transfer dir for not mappable files / files that are already in the input dir.
________________________________________________________________________________________
\version	02.02.00.12
\date		25.07.2019
\internal
- Bugfix: KL Filterung
\endinternal

_______________________________________________________________________________________
\version	02.02.00.11
\date		25.07.2019
\internal
- CPR-170643: added ordertypes AK and HZ, SQL adjusted for searching all orders
\endinternal

________________________________________________________________________________________
\version	02.02.00.10
\date		23.05.2019
\internal
- I-19017012: added transaction->rollback where it was missing
- CPR-170643: added ordertypes AK and HZ, SQL adjusted for searching all orders
- Changes for the upcoming informix upgrade in cscbatchhistory and csc_batches_orderproposal (10.04.2019)
\endinternal

________________________________________________________________________________________
\version	02.02.00.09
\date		15.02.2019
\internal
- P-19000012: cscbatchhistory deletes aorderhead/pos/proposal data which is older than 3 years
\endinternal
________________________________________________________________________________________
\version	02.02.00.08
\date		04.02.2019
\internal
- Project Cons2EdiSys: Allow "PHOENIXQAS" in segmentfield UNB-EMPFBEZ. It ist the address of the PHOENIX-QAS mailbox.
\endinternal
________________________________________________________________________________________
\version	02.02.00.07
\date		14.09.2018
\internal
- Bugfix: csc_batches_edi_input fileMappedCorrectly was always true
\endinternal
________________________________________________________________________________________
\version	02.02.00.06
\date		14.09.2018
\internal
- CPR Cons2EdiSys adjusted edi_input e.g. save uewauf files into sub dirs (UWAR directories)
\endinternal
________________________________________________________________________________________
\version	02.02.00.05
\date		11.09.2018
\internal
- Anpassung damit die Sprache und die LanguageFiles richtig geladen werden!
\endinternal
________________________________________________________________________________________
\version	02.02.00.04
\date		22.06.2018
\internal
- Abfrage für OMG Aufträge ob bereits freigegeben!
- Performance Verbesserung
\endinternal
________________________________________________________________________________________
\version	02.02.00.03
\date		18.05.2018
\internal
- OMG_Duration UADM Parameter
\endinternal
________________________________________________________________________________________
\version	02.02.00.02
\date		12.04.2018
\internal
- Merge: CPR_171166
\endinternal

________________________________________________________________________________________
\version	02.02.00.01
\date		12.04.2018
\internal
- Merge: OMG 2.0 in CH, CR16D130, purchaseordernofix, SR_17095178
\endinternal

________________________________________________________________________________________
\version	02.02.00.00
\date		09.01.2018
\internal
- OMG 2.0 in CH
\endinternal

________________________________________________________________________________________
\version	02.01.07.23
\date		12.03.2018
\internal
- CPR-171166: Manuell erfasste Zeilen
\endinternal

________________________________________________________________________________________
\version	02.01.07.22
\date		23.01.2018
\internal
- P_17000393: purchaseordernofix
\endinternal

________________________________________________________________________________________
\version	02.01.07.21
\date		06.06.2017
\internal
- Neuer Pharmos-Startbildschirm
\endinternal

________________________________________________________________________________________
\version	02.01.07.20
\date		06.06.2017
\internal
- CR17D019 - "Hausrabatt H" für Österrreich parametrierbar durch Parameter Tabelle
- Neuer EventType 9 (Auftrag storniert) beim Protokollieren
\endinternal

________________________________________________________________________________________
\version	02.01.07.19
\date		26.04.2017
\internal
- Anpassungen für den CSV Export
\endinternal

________________________________________________________________________________________
\version	02.01.07.18
\date		21.04.2017
\internal
- Bug Fix: Parametereinstellungen.
\endinternal

________________________________________________________________________________________
\version	02.01.07.17
\date		11.04.2017
\internal
- Bug Fix: beim hinzugüfen eines Artikels, welcher schon hinzugefügt worden ist, wurde BR von BR für alle Positionen nicht übernommen.
\endinternal

________________________________________________________________________________________
\version	02.01.07.16
\date		11.04.2017
\internal
- Anpassungen für den CSV-Export
\endinternal

________________________________________________________________________________________
\version	02.01.07.15
\date		22.03.2017
\internal
- Implementierung Hausrabatt für UW in Österreich AT
\endinternal

________________________________________________________________________________________
\version	02.01.07.14
\date		22.03.2017
\internal
- Implementierung .CSV-Export für BWStatus
\endinternal

________________________________________________________________________________________
\version	02.01.07.13
\date		16.03.2017
\internal
- CR-16D132: Not sales effective reserve qty gesplittet -> release und order view
\endinternal

________________________________________________________________________________________
\version	02.01.07.12
\date		10.03.2017
\internal
- Neues Feld "BR alle Positionen" in der Maske für das manuelle Erfassen
\endinternal

________________________________________________________________________________________
\version	02.01.07.11
\date		27.01.2017
\internal
- SR-16096571: Korrekturen nach Test am neuen Auftragsprotokoll.
\endinternal

________________________________________________________________________________________
\version	02.01.07.10
\date		02.12.2016
\internal
- P-16000325: Bindestriche konnte im Feld "Matchcode" in der CustomerFinder Maske nicht eingegeben werden.
\endinternal

________________________________________________________________________________________
\version	02.01.07.09
\date		10.11.2016
\internal
- Bugfix:     Beim Splitten wurde der article_code und die ordernomf nicht kopiert!
- CR-Intern:  Protokollierung implementiert. Vorerst ohne GUI. In csc_batches, cscbatchhistory und csc_batches_edi_input.
- CR-16D110:  Bestellwunsch senden nur noch in der Bestellsicht aktiviert. Außnahme: Reservemengen sind in beiden Views gleich.
- P-16000293: Leere Überweiser können nicht mehr erzeugt werden!
\endinternal

________________________________________________________________________________________
\version	02.01.07.07
\date		16.09.2016
\internal
- CR-16D119: Neuer Filter "PZN" in der Auftragsliste
- CR-16D119: Für alle Aufträge die Splitbar sind, wird in der Spalte "Fehlpos" nun vor der Fehlmenge SPLIT: angezeigt.
- CSCConnection wurde angepasst. Connection wird beim erstellen eines Auftrags erstellt und danach wieder geschlossen.
\endinternal

________________________________________________________________________________________
\version	02.01.07.06
\date		08.09.2016
\internal
- Die manuelle Erfassung hat in Filiale 21, Hageda-Stumpf, nicht richtig funktioniert.
Der gespeicherte Auftrag wurde nach der Erstellung nicht aus der Tabelle gelöscht und wurde beim erneuten aufrufen des Dialogs wieder angezeigt.
\endinternal

________________________________________________________________________________________
\version	02.01.07.05
\date		07.09.2016
\internal
- P-160000164: Auftragsart war standardmäßig nicht immer auf UW00 bei der manuellen Erfassung
\endinternal

________________________________________________________________________________________
\version	02.01.07.04
\date		06.09.2016
\internal
- P-160000164: ChangeOrdersVC Delete-Button in Datum ein/aus umbenannt & VC Überschrift hinzugefügt
\endinternal

________________________________________________________________________________________
\version	02.01.07.03
\date		05.09.2016
\internal
- CR14D110: Freigabe-Schnittstelle cscorderdm.cpp: Abfrage der Ordtertype für Überweiser (UW,UN,MD,OM) zusammengefasst in eine Methode
\endinternal

________________________________________________________________________________________
\version	02.01.07.02
\date		05.09.2016
\internal
- P-16000164: Freigabe-Schnittstelle cscorderdm.cpp erweitert, so dass bei Überweisern Valutadaten <= heute nicht an KSC-Client übergeben werden und dann auch nicht gedruckt werden.
\endinternal

________________________________________________________________________________________
\version	02.01.07.01
\date		05.09.2016
\internal
- CR14D110 HAGEDA: Freigabe-Schnittstelle cscorderdm.cpp für MD-Aufträge (=spezielle Ueberweiser) erweitert
\endinternal

________________________________________________________________________________________
\version	02.01.07.00
\date		27.07.2016
\internal
- P-16000164: 2 neue Lösch Button im ChangeOrdersVC für das löschen der Datumsangaben
\endinternal

________________________________________________________________________________________
\version	02.01.06.00
\date		13.07.2016
\internal
- CR-16D087: Neue Spalte "Lieferinfo" in transferorderVC
- CR-14D110: Änderungen für HAGEDA-Stumpf (Bugfixes)
\endinternal

________________________________________________________________________________________
\version	02.01.05.00
\date		UNRELEASED
\internal
- CR-14D110 Änderungen für HAGEDA-Stumpf
\endinternal

________________________________________________________________________________________
\version	02.01.04.01
\date		07.06.2016
\internal
- I-16022529: Keine Eingabe schriftlicher Aufträge möglich
\endinternal

________________________________________________________________________________________
\version	02.01.04.00
\date		31.05.2016
\internal
- SR-16036979: Doppelte Clearing-Bestellungen
- P-16000106:  Doppelte Aufträge ohne Positionen nach Split
\endinternal

________________________________________________________________________________________
\version	02.01.03.00
\date		26.04.2016
\internal
- SR-16034054: Verbesserung der Geschwindigkeit beim Laden von Bestellwünschen in der Bestellwunsch-Maske.
- SR-16033927: Löschen von Positionen mit Bestellwunsch ist auch dann möglich, wenn der Bestellwunsch schon
			   verarbeitet wurde.
\endinternal

________________________________________________________________________________________
\version	02.01.02.00
\date		20.04.2016
\internal
- I-16015055 behoben: Auftragssplit Schaltfläche immer ausgegraut
\endinternal

________________________________________________________________________________________
\version	02.01.01.00
\date		14.04.2016
\internal
- neu EK-23: Bestellwünsche zurückziehen
- Filterfeld für den Apothekenort in der Überweiser-Aufträge-Maske wird nun – wie alle anderen
  Filter auch - durch den "Reset"-Button geleert
- CR-15D005: Unnötige Tabellenspalten entfernt. Es können maximal 200 Aufträge auf einmal freigegeben werden.
\endinternal

________________________________________________________________________________________
\version	02.01.00.00
\date		16.02.2016
\internal
- neu KSC-55: Kettartikel werden in Positionenmaske markiert
- neu EK-34:  Der Blick in die Zukunft
- Korrektur: Reservemengenfaktor für UW03-Aufträge funktioniert jetzt
- Korrektur: #Ticket SR-16005131# ABBA-UW: Überweiser-Editor löscht Feld
- neu EK-35: Schnelle Bestellwunscherstellung für Auftragsmengen
- CR-15D005: Erweiterung der Selektionskriterien CSC Batch und Summenbildung
\endinternal

________________________________________________________________________________________
\version	02.00.03.08
\date		08.12.2015
\internal
- (MM) SR-15066583: Artikel ohne Lagerfach können im Editor hinzugefügt werden
- (MM) Bugfix: Die BVO-Status-Maske zeigt beim Klick auf "Aktualisieren" nur den aktuellen Auftrag
			   an, falls sie aus der Auftragsmaske gestartet wird.
\endinternal

________________________________________________________________________________________
\version	02.00.03.07
\date		07.12.2015
\internal
- (MM) neue Spalte Kundenort in Hauptmaske
- (MM) EK-32: Wiederholung verfallener Bestellwünsche möglich
\endinternal

________________________________________________________________________________________
\version	02.00.03.06
\date		17.09.2015
\internal
- (MM): Performance-Optimierungen
\endinternal

________________________________________________________________________________________
\version	02.00.03.05
\date		01.09.2015
\internal
- (MM): neuer Auftragstyp "UN"
\endinternal

________________________________________________________________________________________
\version	02.00.03.03 + 04
\date		24.08.2015 + 01.09.2015
\internal
- (MM): Kosmetische Änderungen
\endinternal

________________________________________________________________________________________
\version	02.00.03.02
\date		20.08.2015
\internal
- (MM): Bugfix Sortierdreiecke wurden nach dem Scrollen nicht mehr angezeigt.
\endinternal

________________________________________________________________________________________
\version	02.00.03.01
\date		19.08.2015
\internal
- (MM): Verbesserungen an Multiuser-Koordination um ungültige Locks zu erkennen
\endinternal

________________________________________________________________________________________
\version	02.00.03.00
\date		13.07.2015
\internal
- (MM): Mit Multiuser-Koordination
\endinternal

________________________________________________________________________________________
\version	02.00.02.41
\date		06.07.2015
\internal
- (MM): Sammel-Bestellwunsch innerhalb eines Auftrags implementiert
\endinternal

________________________________________________________________________________________
\version	02.00.02.40
\date		19.06.2015
\internal
- (MM): Splittfunktion auch im Wechsel mit Bestellwunsch möglich. Geht auch mit gleichartigen Artikeln im selben Auftrag.
- (MM): Zusätzlicher Faktor für Reservemengen bei Buchungsart 3 in Einstellmaske
\endinternal

________________________________________________________________________________________
\version	02.00.02.39
\date		10.06.2015
\internal
- (MM): Reservemengenbestimmung liest Prognose korrekt vom letzten Werktag
\endinternal

________________________________________________________________________________________
\version	02.00.02.38
\date		20.05.2015
\internal
- (MM): GUI in trunk zurückgemerged. Auftragssplitt auch für Positionen mit Bestellwunsch möglich.
\endinternal

________________________________________________________________________________________
\version	02.00.02.37
\date		12.05.2015
\internal
- (MM): Bugfix: Absturz nach Druck in CH wenn der Kunde nicht bekannt ist (falsche Filiale), Lieferant-Anzeige im Editor
\endinternal

________________________________________________________________________________________
\version	02.00.02.35,36
\date		28.04.2015
\internal
- (MM): Bugfix: Splitt-Division durch 0 bei Null-AMGE vermeiden. UW-Editor bietet Artikel aus Hersteller ODER Kieferant des ersten Artikels an.
\endinternal
\~german
\~english
\~

________________________________________________________________________________________
\version	02.00.02.34
\date		22.04.2015
\internal
- (MM): Bugfix: Positionsbezogenen Splitt-Text auch für CH ermöglichen.
\endinternal
\~german
\~english
\~

________________________________________________________________________________________
\version	02.00.02.33
\date		21.04.2015
\internal
- (MM): Bugfix: Aus UW-Editor wurde de articlecode nicht übergeben. Gelöst durch article-Code - Join in batchorderposacc.cpp, line 57 und 76
\endinternal
\~german
\~english
\~

________________________________________________________________________________________
\version	02.00.02.32
\date		20.04.2015
\internal
- (MM): Editor-Knopf "Position ändern" wieder dazu, Datumsauswahl in Auftragsänderung korrigiert, Freigabe
gemäß Spezifikation: Direktfreigabe in DE wirkt nur für grüne Aufträge (ER). Rot und gelb verzögert freigeben (PostponeOrder()->ZU).
in CH: Alle Farben direkt freigeben mit CloseOrder()->ER.
\endinternal
\~german
\~english
\~

________________________________________________________________________________________
\version	02.00.02.31
\date		17.04.2015
\internal
- (MM): Bugfix: Absturz wenn Parameter STORAGESLOT_TRANSIT nicht gesetzt wurde behoben
\endinternal
\~german
\~english
\~

________________________________________________________________________________________
\version	02.00.02.30
\date		16.04.2015
\internal
- (MM): Bugfixes, UW-Editor: NR-Position nach negativ bestätigter Warnung verwerfen
\endinternal
\~german
\~english
\~

________________________________________________________________________________________
\version	02.00.02.29
\date		13.04.2015
\internal
- (MM): Warning-Message wegen NR-Feld im Editor und neuer Parameter EMIT_WARN_ON_ND , diverse Bugfixes,
								 Header-Dateien aufgeräumt, CL/SH-Maske nicht mehr automatisch gestartet
\endinternal
\~german
\~english
\~

________________________________________________________________________________________
\version	02.00.02.28
\date		09.04.2015
\internal
- (MM): Positionsmaske brachte core: Buttons nach BVO nicht demarkiert
\endinternal
\~german
\~english
\~

________________________________________________________________________________________
\version	02.00.02.27
\date		09.04.2015
\internal
- (MM): Bugfix Datenfelder bei Split, Freigabe abgesplitteter KL-Positionen wieder abschalten
\endinternal
\~german
\~english
\~

________________________________________________________________________________________
\version	02.00.02.26
\date		31.03.2015
\internal
- (MM): Germany-GUI-version
- (MM): Bugfix im Filter, Freigabe abgesplitteter KL-Positionen ermöglichen
\endinternal
\~german
\~english
\~

________________________________________________________________________________________
\version	02.00.01.25
\date		19.03.2015
\internal
- (MM): Bugfix Refresh nach Auftragsänderung
- (MM): use libutil 0.4
\endinternal
\~german
\~english
\~

________________________________________________________________________________________
\version	02.00.01.24
\date		04.03.2015
\internal
- (MM): Bugfix Speicherverbrauch
\endinternal
\~german
\~english
\~

________________________________________________________________________________________
\version	02.00.02.23 (DE)
\date		31.03.2015
\internal
- (MM): Mit  Editorversion und verbreiterter Ampelmaske für DE Stufe 1 zum Kunden-Test
\endinternal
\~german
\~english
\~

________________________________________________________________________________________
\version	02.00.01.23
\date		03.03.2015
\internal
- (MM): Ausrollversion trunk, Erstmalig mit Editor ohne DE-Erweiterung
\endinternal

________________________________________________________________________________________
\version	02.00.01.07 - 02.00.01.22
\date		05.01.2015 - 26.02.2015
\internal
- (MM): Mit erster Editorversion und diversen Testversionen
\endinternal

________________________________________________________________________________________
\version	02.00.01.06
\date		xx.xx.xxxx
\internal
- (MM): Headertext mit Kennung ORDER SPLIT beim gesplitteten Auftrag
\endinternal

________________________________________________________________________________________
\version	02.00.01.05
\date		xx.xx.xxxx
\internal
- (PK): Jetzt mit Buildserver gebaut
\endinternal

________________________________________________________________________________________
\version	02.00.01.04
\date		xx.xx.xxxx
\internal
- (PK): Notwendige Files aus veralteter Libutil 0.0.3.xx in Solution einkopiert: libabbauw/Utilities/libutil3
\endinternal

________________________________________________________________________________________
\version	02.00.01.03
\date		xx.xx.xxxx
\internal
- (MM): Bugfix "ABBA-Maske mehrfach öffnen->Exception"
\endinternal

________________________________________________________________________________________
\version	02.00.01.02
\date		xx.xx.xxxx
\internal
- (MM): Eingemerged in trunk und gegenseitiger Ausschluss in alten csc_Batches Masken eingebaut
\endinternal

________________________________________________________________________________________
\version	02.00.01.01
\date		xx.xx.xxxx
\internal
- (MM): Migration auf VS2010
\endinternal

________________________________________________________________________________________
\version	02.00.00.14
\date		xx.xx.xxxx
\internal
- (MM): new userright "UW_CHANGE_ACCESS" for changing operations
- (MM): some new shortcuts in GUI
- (MM): print pharmacynoEAN when idfnr cannot be calculated
\endinternal

________________________________________________________________________________________
\version	02.00.00.13
\date		xx.xx.xxxx
\internal
- (MM): Purple color for OMG-position with stocklocation 77777777 activated
\endinternal

________________________________________________________________________________________
\version	02.00.00.12
\date		xx.xx.xxxx
\internal
- (PK): AddOrderCodeRab initializes discountPCT, FreeRebate with zero to avoid "NL" in item display
\endinternal

________________________________________________________________________________________
\version	02.00.00.11
\date		xx.xx.xxxx
\internal
- (MM): Refresh after Order release: To avoid double releases
\endinternal

________________________________________________________________________________________
\version	02.00.00.9-10
\date		xx.xx.xxxx
\internal
- (MM): Screen for UW-orders requires access right UW_ACCESS now
\endinternal

________________________________________________________________________________________
\version	02.00.00.08
\date		xx.xx.xxxx
\internal
- (PK): Corrected release method: Do not set discountpct and kindofdiscount
\endinternal

________________________________________________________________________________________
\version	02.00.00.07
\date		xx.xx.xxxx
\internal
- (PK): Corrected release method: new method setBatchValueDate
\endinternal

________________________________________________________________________________________
\version	02.00.00.06
\date		xx.xx.xxxx
\internal
- (PK): Corrected release method: rebateinkind
\endinternal

________________________________________________________________________________________
\version	02.00.00.05
\date		xx.xx.xxxx
\internal
- (MM): Added Field NBGrund in BVO-status.Added security popup for button "Release green orders"
\endinternal

________________________________________________________________________________________
\version	02.00.00.04
\date		xx.xx.xxxx
\internal
- (MM): Added Printmethod for an order. Added button "Release green orders"
\endinternal

________________________________________________________________________________________
\version	02.00.00.03
\date		xx.xx.xxxx
\internal
- (MM): Added AART OM00, ABBA-Parameterwindow with new Accessright
\endinternal

________________________________________________________________________________________
\version	02.00.00.02
\date		xx.xx.xxxx
\internal
- (MM): With ABBA-UW, improved order release function
\endinternal

________________________________________________________________________________________
\version	02.00.00.01
\date		xx.xx.xxxx
\internal
- (PK): With ABBA-UW, second Version, for customer-ozek Test
- (MM): Upgrading ssuk-Version  to 1.9.0.2
\endinternal

________________________________________________________________________________________
\version	02.00.00.01
\date		xx.xx.xxxx
\internal
- (MM): Adjustments for unix foundation
- (MM): Upgrading ssuk-Version from 1.7.0.0 to 1.8.0.2
\endinternal

________________________________________________________________________________________
\version	01.09.00.00
\date		xx.xx.xxxx
\internal
- Da es jetzt mehrere Preisdatensätze pro Artikel geben kann, muss der Join eingeschränkt werden (auf Preistyp 0 = Freiverkauf)
\endinternal

________________________________________________________________________________________
\version	01.08.00.00
\date		xx.xx.xxxx
\internal
- Erweiterung um Artikelcode (ERNUM) in Positionsansicht
\endinternal

________________________________________________________________________________________
\version	01.07.00.00
\date		xx.xx.xxxx
\internal
- gebaut mit neuem kscserver nach Buffererweiterung
\endinternal

________________________________________________________________________________________
\version	01.06.00.00
\date		xx.xx.xxxx
\internal
- Sperre eingebaut: Ein Auftrag kann nicht mehrfach freigegeben werden,
auch nicht wenn gleichzeitig 2 Benutzer zugreifen oder der PC abstürzt und das Programm
auf der Citrix weiterläuft.
\endinternal
________________________________________________________________________________________
\version	01.05.00.00
\date		xx.xx.xxxx
\internal
- Scopeänderung: bei Freigabe von mehr als  50 Stück, Nachfrage, ob das beabsichtigt ist
- Statusänderung auf "storniert" bzw. "zurückgesetllt" jetzt auch für mehrere Aufträge möglich! (1)
- Keine Möglichkeit mehr, Funktion auszuführen, wenn Selektion hinter Selektionsbuchstabe leer (3)
- Felder "Auftragsnummer von" und "... bis" werden gelöscht, wenn Benutzer in Tabelle klickt (4)
- Man muss jetzt entweder eine Funktion oder eine neue IDF-Nr, Auftragsart oder Tour eingeben (5)
- Sperren der Mehrfachauswahl in Tabellen (7)
- Fehlermeldung, wenn bei PZN-Umbuchung Artikel in dieser Filiale nicht existiert (8)
- Felder löschen nach Ausführung: rejected. Wird in Pharmos auch nicht so gehandhabt, außer Feld "Funktion" (9)
\endinternal

________________________________________________________________________________________
\version	01.05.00.01
\date		xx.xx.xxxx
\internal
- Bugfix: yitTable in releaseOrders geclont
\endinternal

________________________________________________________________________________________
\version	01.05.00.01
\date		xx.xx.xxxx
\internal
- Vers. für UW
- Text in Positionsanzeige in Datenbank abspeichern (6) (DKBAP im Feld BAP-PTEXT)
		- einen Positionstext (=>kdauftragpos.bemerkungen)
		- oder handelt es sich um eine eigene Position (z.B. Bestellung von 3 Säcken Kartoffeln =>kdauftragpos.bemerkungen mit artikelnr=0, nur Bestellmenge und Text gefüllt)
\endinternal
*/

#endif // GUARD_CSC_BATCHES_VERSION_H
