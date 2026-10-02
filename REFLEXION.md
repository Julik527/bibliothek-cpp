# Reflexion zu Aufgabenblatt 52

Diese Musterantworten dienen als Lernvorlage. Passe sie an deinen tatsächlichen
Arbeitsprozess an; besonders die Aussagen zur KI-Nutzung müssen für dich stimmen.

## 1. Private Attribute und Getter

Die Attribute von `Medium` sind `private`, damit anderer Code sie nicht beliebig
verändern kann. Getter geben gezielt lesenden Zugriff, während Methoden wie
`ausleihen()` die erlaubten Zustandsänderungen kontrollieren. So kann beispielsweise
der Titel nicht nachträglich unbemerkt auf einen leeren Text gesetzt werden.

## 2. Const bei Methoden

Eine `const`-Methode darf die normalen Attribute des aufgerufenen Objekts nicht
verändern. Deshalb sind die Getter, `beschreibung()`, `hatAusgeliehen()`,
`zeigeAusleihen()`, `zeigeBestand()` und `suche()` als `const` markiert. Eine Ausgabe
auf der Konsole ist trotzdem möglich, weil sie den Zustand des Objekts nicht ändert.

## 3. Zeiger und Reallokation

`findeMedium()` liefert einen Zeiger auf ein Element im Medien-Vektor. Wenn
`mediumHinzufuegen()` dessen Kapazität überschreitet, kann der Vektor seine Elemente
in einen neuen Speicherbereich verschieben; der alte Zeiger ist dann ungültig.
Deshalb speichert ein Mitglied nur IDs und ein Medium wird bei Bedarf erneut gesucht.

## 4. Enum und Switch gegenüber mehreren Klassen

Mit `enum class` ist die Lösung übersichtlich, solange alle Medientypen dieselben
Grunddaten besitzen. Bei einem neuen Typ müssen die betreffenden `switch`-Anweisungen
ergänzt werden, und besondere Daten wie die Laufzeit einer DVD passen nicht gut in
die gemeinsame Klasse. Eigene Klassen können solche Unterschiede gezielter abbilden,
machen das Modell aber aufwendiger.

## 5. Wachsende Projekte und mehrere Dateien

Für diese Übung ist eine einzige Datei vorgeschrieben und ausreichend. In größeren
Projekten enthalten Header üblicherweise die Klassendeklarationen und Source-Dateien
die Methodendefinitionen. Das trennt Schnittstelle und Umsetzung, erleichtert die
Zusammenarbeit und ermöglicht getrennte Übersetzung.

## 6. KI-Nutzung und eigene Prüfung

Formulierungsvorschlag, nur nach entsprechender eigener Prüfung übernehmen:
Ich habe eine KI-gestützte Musterlösung für Code, Erklärungen und Testfälle verwendet.
Anschließend habe ich selbst geprüft, wie Ausleihlimit, Rückgabe und die Suche über
IDs funktionieren. Einen konkreten Widerspruch oder eine Korrektur beschreibe ich
nur dann, wenn dies tatsächlich vorgekommen ist; andernfalls halte ich das offen fest.
