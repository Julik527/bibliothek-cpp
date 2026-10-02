# Bibliotheksverwaltung in C++17

KI-gestützte Musterlösung zu Aufgabenblatt 52 von Dr. Michael Litera.
Die Bibliothek verwaltet Bücher, DVDs und Zeitschriften sowie Mitglieder mit
höchstens drei gleichzeitigen Ausleihen. Die Klassen `Medium`, `Mitglied` und
`Bibliothek` liegen zusammen mit `main()` in `src/main.cpp`.

## Dokumentation

- [Musterlösung als PDF](docs/Aufgabenblatt_52_Musterloesung_Julian_Krauss.pdf)
- [Bearbeitbare Word-Datei](docs/Aufgabenblatt_52_Musterloesung_Julian_Krauss.docx)

Die 16-seitige Dokumentation enthält Erläuterungen, den vollständigen Quellcode,
Testergebnisse und die Reflexionsvorlage im Corporate Design von Julian Krauß.
Die Git-Hinweise in den Dokumenten zeigen den Stand vor der Veröffentlichung.
Für den GitHub-Stand gilt [GIT_ABGABE.md](GIT_ABGABE.md).

## Dateien

- `src/main.cpp`: vollständiges Programm mit 29 automatischen Prüfungen.
- `README.md`: Projektbeschreibung, Startanleitung und echte Beispielausgabe.
- `REFLEXION.md`: sechs Reflexionsantworten als persönlich anzupassende Vorlage.
- `.gitignore`: schließt Build-Ausgaben und IDE-Dateien aus.
- `GIT_ABGABE.md`: Veröffentlichungsstand und Anleitung für die eigene Abgabe.
- `docs/`: gestaltete Musterlösung als PDF und Word-Datei.

## Voraussetzungen

Ein C++17-fähiger g++-Compiler muss installiert und im Suchpfad verfügbar sein.
Alle folgenden Befehle werden im Projektordner `bibliothek-cpp` ausgeführt.

Repository zunächst herunterladen:

```bash
git clone https://github.com/Julik527/bibliothek-cpp.git
cd bibliothek-cpp
```

### Linux und macOS mit g++

```bash
g++ -std=c++17 -Wall -Wextra src/main.cpp -o bibliothek
./bibliothek
```

### Windows mit g++ und PowerShell

```powershell
g++ -std=c++17 -Wall -Wextra src/main.cpp -o bibliothek.exe
.\bibliothek.exe
```

`-std=c++17` wählt den Sprachstandard; `-Wall -Wextra` aktiviert zusätzliche
Compilerwarnungen. Unter Windows ist eine passende g++-Installation nötig.
Die resultierende ausführbare Datei bleibt lokal und wird nicht eingecheckt.

## Verhalten

Das Programm benötigt keine Eingabe. `main()` legt fünf Beispielmedien und zwei
Mitglieder an, führt 29 Prüfungen durch und zeigt anschließend Ausleihen,
Suchergebnisse und den veränderten Bestand an. Die Medientitel und Personen sind
frei gewählte Beispieldaten.

Die Suche beachtet Groß- und Kleinschreibung, sucht Teiltexte in Titel und Urheber
und liefert bei leerem Suchtext alle Medien. Ungültige Stammdaten und doppelte IDs
lösen `std::invalid_argument` aus. Nicht mögliche Ausleihen und Rückgaben liefern
`false`; eine Suche nach einer unbekannten ID liefert `nullptr`.

Zeiger aus `findeMedium()`, `findeMitglied()` und `suche()` werden nur kurzfristig
verwendet. Eine Reallokation des betreffenden Vektors kann sie ungültig machen.
Ausleihen und Rückgaben laufen im Hauptprogramm über `Bibliothek`, damit der
Medienstatus und die beim Mitglied gespeicherten IDs zusammenpassen.

## Tatsächliche Beispielausgabe

Der folgende Lauf wurde am 02.10.2026 mit g++ 13.3.0 unter Linux geprüft.

```text
BIBLIOTHEK | Bestand vor den Tests
1 | Buch: C++ Grundlagen von Anna Weber (2024) | frei
2 | Buch: Netzwerke von Ben Wolf (2023) | frei
3 | DVD: IT im Alltag, Regie: Clara Berg (2022) | frei
4 | Zeitschrift: Technik heute, Verlag: Tech Verlag (2025) | frei
5 | Buch: Linux Praxis von Anna Weber (2024) | frei

AUTOMATISCHE PRUEFUNGEN
[OK] Medium anfangs frei
[OK] Erste Ausleihe erfolgreich
[OK] Status verliehen
[OK] ID gespeichert
[OK] Verliehenes Medium abgelehnt
[OK] Zweite Ausleihe erfolgreich
[OK] Dritte Ausleihe erfolgreich
[OK] Viertes Medium abgelehnt
[OK] Abgelehntes Medium frei
[OK] Fremde Rueckgabe abgelehnt
[OK] Eigene Rueckgabe erfolgreich
[OK] ID entfernt
[OK] Doppelte Rueckgabe abgelehnt
[OK] Anderes Mitglied leiht erneut
[OK] Nach Rueckgabe wieder Platz
[OK] Unbekanntes Mitglied abgelehnt
[OK] Unbekanntes Medium abgelehnt
[OK] Rueckgabe mit unbekanntem Mitglied
[OK] Rueckgabe mit unbekanntem Medium
[OK] Mediensuche liefert nullptr
[OK] Mitgliedssuche liefert nullptr
[OK] Titelsuche
[OK] Urhebersuche
[OK] Suche ohne Treffer
[OK] Leere Suche findet alle Medien
[OK] Gleichheit ueber ID
[OK] Leerer Titel wirft invalid_argument
[OK] Doppelte Medien-ID wirft Fehler
[OK] Doppelte Mitgliedsnummer wirft Fehler

29 Pruefungen bestanden.

AUSLEIHEN
Mia (101): 2 3 4 
Noah (102): 1 

SUCHE nach Anna Weber
Buch: C++ Grundlagen von Anna Weber (2024)
Buch: Linux Praxis von Anna Weber (2024)

BESTAND nach den Tests
1 | Buch: C++ Grundlagen von Anna Weber (2024) | verliehen
2 | Buch: Netzwerke von Ben Wolf (2023) | verliehen
3 | DVD: IT im Alltag, Regie: Clara Berg (2022) | verliehen
4 | Zeitschrift: Technik heute, Verlag: Tech Verlag (2025) | verliehen
5 | Buch: Linux Praxis von Anna Weber (2024) | frei
```

Die Prüfungen laufen auch mit `-DNDEBUG`, weil eine eigene Prüffunktion verwendet
wird. Bei einer fehlgeschlagenen Prüfung gibt das Programm eine Fehlermeldung aus
und endet mit Rückgabecode 1; bei Erfolg endet es mit 0.

## Was ich gelernt habe

Lernpunkte der Musterlösung, vor der persönlichen Abgabe selbst nachvollziehen:

- Private Attribute schützen den Objektzustand; Getter erlauben kontrolliertes Lesen.
- Konstruktoren initialisieren und prüfen ein Objekt direkt beim Anlegen.
- `const` kennzeichnet Methoden, die den Zustand des Objekts nicht verändern.
- IDs bleiben als Kennungen erhalten, wenn ein Vektor seinen Speicher neu belegt.
- Tests prüfen erfolgreiche Vorgänge, Ablehnungen und unveränderte Zustände.

## Git und Abgabe

Die Musterlösung wird über den Branch `feature/aufgabenblatt-52` und einen Pull
Request nach `main` veröffentlicht. Die optionale Aufteilung in mehrere Dateien
ist als [Issue #1](https://github.com/Julik527/bibliothek-cpp/issues/1) erfasst.

Der Import ersetzt nicht die geforderten mindestens sieben eigenen Arbeitsschritte.
Die persönliche Reflexion und der Tag `v1.0` sind vor der endgültigen Abgabe zu
ergänzen. Weitere Schritte stehen in [GIT_ABGABE.md](GIT_ABGABE.md).

## Quellen und KI-Nutzung

Aufgabenquelle: Dr. Michael Litera, `Aufgabenblatt52.pdf`, Seiten 1 bis 5.
Code, Erklärungen und Testvorschläge dieser Musterlösung wurden mit KI erstellt.
Die persönliche Reflexion und der eigene Arbeitsverlauf müssen wahrheitsgemäß
ergänzt werden. Die optionale Erweiterung auf mehrere Dateien ist hier bewusst
nicht umgesetzt, da die Pflichtaufgabe eine einzige C++-Datei verlangt.

Dokumentgestaltung © 2026 Julian Krauß. Designbasis: *Corporate Identity & Design
System*, JK-CD-DE-2026-003, PDF-Revision 3.2, Release 4.6.3.
