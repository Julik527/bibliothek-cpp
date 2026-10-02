// Aufgabenblatt 52: Bibliotheksverwaltung in C++17
// KI-gestuetzte Musterloesung zum Nachvollziehen und Anpassen.
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

enum class MediumTyp { Buch, DVD, Zeitschrift };

std::string typAlsText(MediumTyp typ) {
    switch (typ) {
        case MediumTyp::Buch: return "Buch";
        case MediumTyp::DVD: return "DVD";
        case MediumTyp::Zeitschrift: return "Zeitschrift";
    }
    throw std::invalid_argument("Ungueltiger Medientyp");
}

class Medium {
private:
    int id;
    std::string titel;
    std::string urheber;
    int jahr;
    MediumTyp typ;
    bool verfuegbar;

public:
    Medium(int id, const std::string& titel,
           const std::string& urheber, int jahr, MediumTyp typ)
        : id(id), titel(titel), urheber(urheber), jahr(jahr),
          typ(typ), verfuegbar(true) {
        if (id <= 0 || jahr <= 0 ||
            titel.find_first_not_of(" \t\r\n") == std::string::npos ||
            urheber.find_first_not_of(" \t\r\n") == std::string::npos) {
            throw std::invalid_argument("Ungueltige Mediendaten");
        }
        typAlsText(typ); // Prueft auch einen ungueltigen Enum-Wert.
    }

    int getId() const { return id; }
    const std::string& getTitel() const { return titel; }
    const std::string& getUrheber() const { return urheber; }
    int getJahr() const { return jahr; }
    MediumTyp getTyp() const { return typ; }
    bool istVerfuegbar() const { return verfuegbar; }

    bool ausleihen() {
        if (!verfuegbar) return false;
        verfuegbar = false;
        return true;
    }

    bool zurueckgeben() {
        if (verfuegbar) return false;
        verfuegbar = true;
        return true;
    }

    std::string beschreibung() const {
        std::string verbindung;
        switch (typ) {
            case MediumTyp::Buch: verbindung = " von "; break;
            case MediumTyp::DVD: verbindung = ", Regie: "; break;
            case MediumTyp::Zeitschrift:
                verbindung = ", Verlag: "; break;
        }
        return typAlsText(typ) + ": " + titel + verbindung + urheber
             + " (" + std::to_string(jahr) + ")";
    }
};

std::ostream& operator<<(std::ostream& os, const Medium& m) {
    return os << m.beschreibung();
}

bool operator==(const Medium& links, const Medium& rechts) {
    return links.getId() == rechts.getId();
}

class Mitglied {
private:
    std::string name;
    int mitgliedsNr;
    // IDs bleiben bei einer Reallokation des Medien-Vektors erhalten.
    // Gespeicherte Zeiger oder Referenzen koennten ungueltig werden.
    std::vector<int> ausgelieheneIds;

public:
    Mitglied(const std::string& name, int mitgliedsNr)
        : name(name), mitgliedsNr(mitgliedsNr) {
        if (mitgliedsNr <= 0 ||
            name.find_first_not_of(" \t\r\n") == std::string::npos) {
            throw std::invalid_argument("Ungueltige Mitgliedsdaten");
        }
    }

    const std::string& getName() const { return name; }
    int getMitgliedsNr() const { return mitgliedsNr; }

    bool hatAusgeliehen(int mediumId) const {
        return std::find(ausgelieheneIds.begin(), ausgelieheneIds.end(),
                         mediumId) != ausgelieheneIds.end();
    }

    bool ausleihen(Medium& m) {
        if (ausgelieheneIds.size() >= 3 || hatAusgeliehen(m.getId())) {
            return false;
        }
        // Speicher reservieren, bevor der Medienstatus geaendert wird.
        ausgelieheneIds.reserve(3);
        if (!m.ausleihen()) return false;
        ausgelieheneIds.push_back(m.getId());
        return true;
    }

    bool zurueckgeben(Medium& m) {
        auto it = std::find(ausgelieheneIds.begin(),
                            ausgelieheneIds.end(), m.getId());
        if (it == ausgelieheneIds.end() || !m.zurueckgeben()) {
            return false;
        }
        ausgelieheneIds.erase(it);
        return true;
    }

    void zeigeAusleihen() const {
        std::cout << name << " (" << mitgliedsNr << "): ";
        if (ausgelieheneIds.empty()) std::cout << "keine Ausleihen";
        for (int id : ausgelieheneIds) std::cout << id << ' ';
        std::cout << '\n';
    }
};

class Bibliothek {
private:
    std::vector<Medium> medien;
    std::vector<Mitglied> mitglieder;

public:
    void mediumHinzufuegen(Medium m) {
        if (findeMedium(m.getId()) != nullptr) {
            throw std::invalid_argument("Medien-ID bereits vorhanden");
        }
        medien.push_back(m);
    }

    void mitgliedHinzufuegen(Mitglied m) {
        if (findeMitglied(m.getMitgliedsNr()) != nullptr) {
            throw std::invalid_argument("Mitgliedsnummer vorhanden");
        }
        mitglieder.push_back(m);
    }

    // Zeiger nur kurzfristig nutzen: Hinzufuegen kann sie entwerten.
    Medium* findeMedium(int id) {
        auto it = std::find_if(medien.begin(), medien.end(),
            [id](const Medium& m) { return m.getId() == id; });
        return it == medien.end() ? nullptr : &*it;
    }

    Mitglied* findeMitglied(int mitgliedsNr) {
        auto it = std::find_if(mitglieder.begin(), mitglieder.end(),
            [mitgliedsNr](const Mitglied& m) {
                return m.getMitgliedsNr() == mitgliedsNr;
            });
        return it == mitglieder.end() ? nullptr : &*it;
    }

    bool ausleihen(int mitgliedsNr, int mediumId) {
        Mitglied* mitglied = findeMitglied(mitgliedsNr);
        Medium* medium = findeMedium(mediumId);
        if (mitglied == nullptr || medium == nullptr) return false;
        return mitglied->ausleihen(*medium);
    }

    bool zurueckgeben(int mitgliedsNr, int mediumId) {
        Mitglied* mitglied = findeMitglied(mitgliedsNr);
        Medium* medium = findeMedium(mediumId);
        if (mitglied == nullptr || medium == nullptr) return false;
        return mitglied->zurueckgeben(*medium);
    }

    void zeigeBestand() const {
        for (const Medium& m : medien) {
            std::cout << m.getId() << " | " << m << " | "
                      << (m.istVerfuegbar() ? "frei" : "verliehen")
                      << '\n';
        }
    }

    // Gross-/Kleinschreibung beachten; leerer Suchtext findet alles.
    // Auch diese Ergebniszeiger nur bis zur naechsten Aenderung nutzen.
    std::vector<const Medium*> suche(const std::string& suchtext) const {
        std::vector<const Medium*> treffer;
        for (const Medium& m : medien) {
            if (m.getTitel().find(suchtext) != std::string::npos ||
                m.getUrheber().find(suchtext) != std::string::npos) {
                treffer.push_back(&m);
            }
        }
        return treffer;
    }
};

// Eigene Testfunktion: wird auch mit -DNDEBUG ausgefuehrt.
void pruefe(bool bedingung, const std::string& text, int& anzahl) {
    if (!bedingung) throw std::runtime_error("Test fehlgeschlagen: " + text);
    ++anzahl;
    std::cout << "[OK] " << text << '\n';
}

int main() {
    try {
        Bibliothek b;
        b.mediumHinzufuegen({1, "C++ Grundlagen", "Anna Weber", 2024,
                            MediumTyp::Buch});
        b.mediumHinzufuegen({2, "Netzwerke", "Ben Wolf", 2023,
                            MediumTyp::Buch});
        b.mediumHinzufuegen({3, "IT im Alltag", "Clara Berg", 2022,
                            MediumTyp::DVD});
        b.mediumHinzufuegen({4, "Technik heute", "Tech Verlag", 2025,
                            MediumTyp::Zeitschrift});
        b.mediumHinzufuegen({5, "Linux Praxis", "Anna Weber", 2024,
                            MediumTyp::Buch});
        b.mitgliedHinzufuegen({"Mia", 101});
        b.mitgliedHinzufuegen({"Noah", 102});

        std::cout << "BIBLIOTHEK | Bestand vor den Tests\n";
        b.zeigeBestand();
        std::cout << "\nAUTOMATISCHE PRUEFUNGEN\n";
        int tests = 0;
        pruefe(b.findeMedium(1)->istVerfuegbar(),
               "Medium anfangs frei", tests);
        pruefe(b.ausleihen(101, 1), "Erste Ausleihe erfolgreich", tests);
        pruefe(!b.findeMedium(1)->istVerfuegbar(), "Status verliehen", tests);
        pruefe(b.findeMitglied(101)->hatAusgeliehen(1),
               "ID gespeichert", tests);
        pruefe(!b.ausleihen(102, 1), "Verliehenes Medium abgelehnt", tests);
        pruefe(b.ausleihen(101, 2), "Zweite Ausleihe erfolgreich", tests);
        pruefe(b.ausleihen(101, 3), "Dritte Ausleihe erfolgreich", tests);
        pruefe(!b.ausleihen(101, 4), "Viertes Medium abgelehnt", tests);
        pruefe(b.findeMedium(4)->istVerfuegbar(),
               "Abgelehntes Medium frei", tests);
        pruefe(!b.zurueckgeben(102, 1), "Fremde Rueckgabe abgelehnt", tests);
        pruefe(b.zurueckgeben(101, 1), "Eigene Rueckgabe erfolgreich", tests);
        pruefe(!b.findeMitglied(101)->hatAusgeliehen(1),
               "ID entfernt", tests);
        pruefe(!b.zurueckgeben(101, 1),
               "Doppelte Rueckgabe abgelehnt", tests);
        pruefe(b.ausleihen(102, 1), "Anderes Mitglied leiht erneut", tests);
        pruefe(b.ausleihen(101, 4), "Nach Rueckgabe wieder Platz", tests);
        pruefe(!b.ausleihen(999, 5), "Unbekanntes Mitglied abgelehnt", tests);
        pruefe(!b.ausleihen(101, 999), "Unbekanntes Medium abgelehnt", tests);
        pruefe(!b.zurueckgeben(999, 1),
               "Rueckgabe mit unbekanntem Mitglied", tests);
        pruefe(!b.zurueckgeben(101, 999),
               "Rueckgabe mit unbekanntem Medium", tests);
        pruefe(b.findeMedium(999) == nullptr,
               "Mediensuche liefert nullptr", tests);
        pruefe(b.findeMitglied(999) == nullptr,
               "Mitgliedssuche liefert nullptr", tests);
        pruefe(b.suche("Linux").size() == 1, "Titelsuche", tests);
        pruefe(b.suche("Anna Weber").size() == 2, "Urhebersuche", tests);
        pruefe(b.suche("xyz").empty(), "Suche ohne Treffer", tests);
        pruefe(b.suche("").size() == 5,
               "Leere Suche findet alle Medien", tests);
        Medium vergleich(1, "Anderer Titel", "Andere Person", 2020,
                         MediumTyp::DVD);
        pruefe(*b.findeMedium(1) == vergleich, "Gleichheit ueber ID", tests);

        bool fehler = false;
        try { Medium ungueltig(6, "   ", "Autor", 2024, MediumTyp::Buch); }
        catch (const std::invalid_argument&) { fehler = true; }
        pruefe(fehler, "Leerer Titel wirft invalid_argument", tests);
        fehler = false;
        try { b.mediumHinzufuegen(vergleich); }
        catch (const std::invalid_argument&) { fehler = true; }
        pruefe(fehler, "Doppelte Medien-ID wirft Fehler", tests);
        fehler = false;
        try { b.mitgliedHinzufuegen({"Andere Person", 101}); }
        catch (const std::invalid_argument&) { fehler = true; }
        pruefe(fehler, "Doppelte Mitgliedsnummer wirft Fehler", tests);

        std::cout << '\n' << tests << " Pruefungen bestanden.\n\n";
        std::cout << "AUSLEIHEN\n";
        b.findeMitglied(101)->zeigeAusleihen();
        b.findeMitglied(102)->zeigeAusleihen();
        std::cout << "\nSUCHE nach Anna Weber\n";
        for (const Medium* m : b.suche("Anna Weber")) std::cout << *m << '\n';
        std::cout << "\nBESTAND nach den Tests\n";
        b.zeigeBestand();
    } catch (const std::exception& e) {
        std::cerr << "Fehler: " << e.what() << '\n';
        return 1;
    }
    return 0;
}
