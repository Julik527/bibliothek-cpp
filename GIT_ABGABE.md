# Git und Abgabe

Dieses Repository veröffentlicht die KI-gestützte Musterlösung zu Aufgabenblatt 52.
Der vollständige Code und die Dokumentation werden als zusammenhängender Import
über `feature/aufgabenblatt-52` und einen Pull Request nach `main` übernommen.
Die optionale Erweiterung ist in [Issue #1](https://github.com/Julik527/bibliothek-cpp/issues/1)
erfasst. Der Import bildet keine schrittweise eigene Entwicklung ab.

Vor der persönlichen Abgabe bleiben die Reflexion, die geforderten mindestens
sieben sinnvollen Commits aus der tatsächlichen Arbeit und der Tag `v1.0` zu
ergänzen. Die folgenden Abschnitte helfen bei diesen eigenen Arbeitsschritten.
Die PDF- und Word-Dateien in `docs/` dokumentieren den Stand vor dem GitHub-Upload;
für den Veröffentlichungsstand ist diese Datei maßgeblich.

## 0. Repository klonen

Das öffentliche Repository ist bereits unter `Julik527/bibliothek-cpp` angelegt:

```bash
git clone https://github.com/Julik527/bibliothek-cpp.git
cd bibliothek-cpp
```

Die Dateien der Musterlösung schrittweise zum Lernen und Vergleichen einsetzen.
Nicht nachträglich eine angeblich selbst entwickelte Commit-Historie erzeugen.

## 1. Mindestens sieben echte Arbeitsschritte festhalten

Die folgende Planung zeigt mögliche Entwicklungsschritte. Sie beschreibt keine
bereits vorhandenen Commits. Da die Musterlösung fertig importiert wurde, dürfen
diese Nachrichten nicht nachträglich als angeblicher Entwicklungsverlauf verwendet
werden. Weitere Commits sollen tatsächliche eigene Änderungen dokumentieren.

1. `Projektstruktur und Build-Anleitung angelegt`
2. `Medium mit Enum und Validierung implementiert`
3. `Mitglied mit Ausleihlimit implementiert`
4. `Bibliothek mit ID-Suche und Ausleihe implementiert`
5. `Mediensuche und Vergleichsoperatoren ergaenzt`
6. `Hauptprogramm und automatische Pruefungen ergaenzt`
7. `Reflexion und Programmausgabe dokumentiert`

Nach einem tatsächlich abgeschlossenen Schritt die betroffenen Dateien gezielt
vormerken und mit einer passenden Nachricht committen. Beispiel für Teil 1:

```bash
git status
git diff
git add src/main.cpp
git commit -m "Medium mit Enum und Validierung implementiert"
```

## 2. Feature-Branch und Pull Request

Für den Import dieser Musterlösung wird `feature/aufgabenblatt-52` verwendet.
Das folgende Beispiel zeigt den Ablauf für einen eigenen weiteren Arbeitsschritt.

Vor der Arbeit an Teil 2:

```bash
git switch -c feature/mitglied
```

Nach der eigenen Umsetzung und Prüfung:

```bash
git add src/main.cpp
git commit -m "Mitglied mit Ausleihlimit implementiert"
git push -u origin feature/mitglied
```

Auf GitHub einen Pull Request von `feature/mitglied` nach `main` öffnen, prüfen
und mit **Create a merge commit** zusammenführen. So bleiben die einzelnen
Arbeitsschritte sichtbar. Danach lokal synchronisieren:

```bash
git switch main
git pull --ff-only
```

## 3. Offene Zusatzaufgabe als Issue

Bereits angelegt: [Issue #1](https://github.com/Julik527/bibliothek-cpp/issues/1).
Kein zweites Issue für dieselbe Aufgabe erstellen.

Titel: `Klassen auf Header- und Source-Dateien aufteilen`

Beschreibung: `Medium`, `Mitglied` und `Bibliothek` künftig auf je eine Header-
und Source-Datei verteilen. Include Guards oder `#pragma once` verwenden und
eine `CMakeLists.txt` ergänzen. Anschließend müssen alle Tests weiterhin bestehen.
Diese Erweiterung erst nach Abschluss der vorgeschriebenen Ein-Datei-Lösung umsetzen.

## 4. Sauberen Klon prüfen

Alle fertigen Änderungen zuerst committen und `main` pushen. In einem anderen,
noch nicht vorhandenen Ordner frisch klonen:

```bash
git push origin main
git clone https://github.com/Julik527/bibliothek-cpp.git bibliothek-cpp-pruefung
cd bibliothek-cpp-pruefung
g++ -std=c++17 -Wall -Wextra src/main.cpp -o bibliothek
./bibliothek
git status --short
git ls-files
```

Unter Windows mit g++ stattdessen `-o bibliothek.exe` und anschließend
`.\bibliothek.exe` verwenden. Das Programm muss mit 29 bestandenen Prüfungen enden.
`git status --short` soll leer bleiben; `git ls-files` darf keine Build- oder
IDE-Dateien auflisten.

## 5. Version markieren und abgeben

Im geprüften Repository:

```bash
git tag v1.0
git push origin v1.0
```

Einen bestehenden Tag nicht überschreiben. Anschließend den Repository-Link
abgeben und bei einem privaten Repository die Lehrkraft als Collaborator einladen.

## Pflichtkontrolle

- [x] `src/main.cpp`, `README.md`, `REFLEXION.md` und `.gitignore` vorhanden.
- [x] README enthält Build-Befehl, echte Beispielausgabe und 3 bis 5 Lernpunkte.
- [ ] Mindestens sieben sinnvolle Commits aus dem tatsächlichen Verlauf.
- [ ] Feature-Branch verwendet und nach `main` gemergt.
- [x] Ein offenes Issue für eine Zusatzaufgabe vorhanden.
- [x] Keine kompilierten Dateien und keine IDE-Dateien eingecheckt.
- [ ] Frisch geklontes Repository baut und alle Tests bestehen.
- [ ] Tag `v1.0` gepusht und Zugriff der Lehrkraft geklärt.

Ein GitHub-Actions-Workflow ist laut Aufgabenblatt optional.
