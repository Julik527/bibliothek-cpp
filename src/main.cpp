// Aufgabenblatt 52: Bibliotheksverwaltung in C++17
// KI-gestuetzte Musterloesung zum Nachvollziehen und Anpassen.
// Das Programm benoetigt keine Eingaben: Es legt Beispieldaten an,
// fuehrt 29 automatische Pruefungen aus und zeigt den Endzustand an.
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

// Die Medientypen teilen dieselben Grunddaten. Das Enum bestimmt,
// wie der Urheber in der Beschreibung eines Mediums bezeichnet wird.
enum class MediumTyp { Buch, DVD, Zeitschrift };

// Uebersetzt einen Medientyp fuer die Ausgabe und prueft seine Gueltigkeit.
std::string typAlsText(MediumTyp typ) {
    switch (typ) {
        case MediumTyp::Buch: return "Buch";
        case MediumTyp::DVD: return "DVD";
        case MediumTyp::Zeitschrift: return "Zeitschrift";
    }
    // Auch ein enum class kann durch eine explizite Umwandlung einen Wert
    // erhalten, der keinem der oben aufgefuehrten Eintraege entspricht.
    throw std::invalid_argument("Ungueltiger Medientyp");
}

/**
 * Repraesentiert ein einzelnes Medium mit Stammdaten und Ausleihstatus.
 * Das Medium kennt seinen Entleiher nicht. Dessen ausgeliehene IDs werden
 * in Mitglied gespeichert; Bibliothek verbindet beide Objekte ueber ihre IDs.
 */
class Medium {
private:
    int id;
    std::string titel;
    std::string urheber;
    int jahr;
    MediumTyp typ;
    bool verfuegbar;

public:
    // Die Initialisierungsliste setzt die Attribute direkt beim Erzeugen.
    // Ein neues Medium ist frei; ungueltige Stammdaten brechen den Bau ab.
    Medium(int id, const std::string& titel,
           const std::string& urheber, int jahr, MediumTyp typ)
        : id(id), titel(titel), urheber(urheber), jahr(jahr),
          typ(typ), verfuegbar(true) {
        // IDs und Jahreszahlen muessen positiv sein. find_first_not_of
        // verwirft auch Titel und Urheber, die nur Leerzeichen, Tabs oder
        // Zeilenumbrueche enthalten.
        if (id <= 0 || jahr <= 0 ||
            titel.find_first_not_of(" \t\r\n") == std::string::npos ||
            urheber.find_first_not_of(" \t\r\n") == std::string::npos) {
            throw std::invalid_argument("Ungueltige Mediendaten");
        }
        typAlsText(typ); // Prueft auch einen ungueltigen Enum-Wert.
    }

    // const-Getter lesen den Zustand, ohne ihn zu veraendern.
    // Die String-Referenzen vermeiden Kopien und erlauben nur lesenden Zugriff.
    int getId() const { return id; }
    const std::string& getTitel() const { return titel; }
    const std::string& getUrheber() const { return urheber; }
    int getJahr() const { return jahr; }
    MediumTyp getTyp() const { return typ; }
    bool istVerfuegbar() const { return verfuegbar; }

    // Nur ein freies Medium kann verliehen werden. Bei Ablehnung bleibt
    // sein Zustand erhalten; true meldet eine erfolgreiche Zustandsaenderung.
    bool ausleihen() {
        if (!verfuegbar) return false;
        verfuegbar = false;
        return true;
    }

    // Ein bereits freies Medium kann nicht erneut zurueckgegeben werden.
    bool zurueckgeben() {
        if (verfuegbar) return false;
        verfuegbar = true;
        return true;
    }

    // Derselbe Urheber wird je nach Medientyp als Autor, Regie oder Verlag
    // ausgegeben. Die Methode liefert Text und veraendert das Medium nicht.
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

// Der zurueckgegebene Stream erlaubt verkettete Ausgaben wie cout << m << '\n'.
std::ostream& operator<<(std::ostream& os, const Medium& m) {
    return os << m.beschreibung();
}

// Die fachliche Identitaet ergibt sich allein aus der ID, nicht aus dem Titel
// oder den uebrigen Stammdaten. Bibliothek verhindert doppelte Medien-IDs.
bool operator==(const Medium& links, const Medium& rechts) {
    return links.getId() == rechts.getId();
}

/**
 * Verwaltet die Zuordnung zwischen einem Mitglied und seinen Ausleihen.
 * Pro Mitglied sind hoechstens drei Medien gleichzeitig erlaubt.
 * Die Methoden halten die ID-Liste und den Status des uebergebenen Mediums
 * bei erfolgreichen Ausleihen und Rueckgaben gemeinsam auf dem neuen Stand.
 */
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

    // std::find liefert end(), wenn die ID nicht in der Ausleihliste steht.
    bool hatAusgeliehen(int mediumId) const {
        return std::find(ausgelieheneIds.begin(), ausgelieheneIds.end(),
                         mediumId) != ausgelieheneIds.end();
    }

    bool ausleihen(Medium& m) {
        // Limit und doppelte Zuordnung pruefen, bevor das Medium geaendert wird.
        if (ausgelieheneIds.size() >= 3 || hatAusgeliehen(m.getId())) {
            return false;
        }
        // Speicher fuer alle drei IDs vorab reservieren. Falls reserve wirft,
        // ist das Medium noch unveraendert. Danach benoetigt push_back fuer
        // diese int-ID keinen weiteren Speicher und kann die Zuordnung sichern.
        ausgelieheneIds.reserve(3);
        if (!m.ausleihen()) return false;
        ausgelieheneIds.push_back(m.getId());
        return true;
    }

    bool zurueckgeben(Medium& m) {
        auto it = std::find(ausgelieheneIds.begin(),
                            ausgelieheneIds.end(), m.getId());
        // || wertet den rechten Ausdruck nur aus, wenn die ID gefunden wurde.
        // Eine fremde Rueckgabe veraendert deshalb den Medienstatus nicht.
        if (it == ausgelieheneIds.end() || !m.zurueckgeben()) {
            return false;
        }
        // Die Zuordnung erst nach erfolgreicher Rueckgabe des Mediums loeschen.
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

/**
 * Besitzt die Medien und Mitglieder als Werte in zwei Vektoren.
 * Sie verhindert doppelte Kennungen, sucht Objekte und ist im Hauptprogramm
 * der zentrale Zugang fuer Ausleihen und Rueckgaben anhand von IDs.
 */
class Bibliothek {
private:
    std::vector<Medium> medien;
    std::vector<Mitglied> mitglieder;

public:
    // Eindeutige Kennungen verhindern, dass eine Suche mehrere Datensaetze
    // derselben ID uneindeutig behandeln muesste. Abgelehnte Eintraege werden
    // nicht in den Bestand aufgenommen; fuer Mitglieder gilt dieselbe Regel.
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

    // nullptr kennzeichnet eine unbekannte ID. Der Ergebniszeiger gehoert
    // weiterhin zum Vektor und darf nicht mit delete freigegeben werden.
    // Eine Reallokation beim Hinzufuegen von Medien kann ihn ungueltig machen.
    Medium* findeMedium(int id) {
        // Die Lambda-Funktion uebernimmt die gesuchte ID als Kopie und
        // entscheidet fuer jedes Medium, ob es der gesuchte Treffer ist.
        auto it = std::find_if(medien.begin(), medien.end(),
            [id](const Medium& m) { return m.getId() == id; });
        // &*it liefert die Adresse des gefundenen Elements. end() wird durch
        // die vorherige Bedingung niemals dereferenziert.
        return it == medien.end() ? nullptr : &*it;
    }

    // Auch dieser Zeiger ist nur gueltig, solange das Mitglied existiert
    // und der Mitglieder-Vektor seine Elemente nicht durch Reallokation bewegt.
    Mitglied* findeMitglied(int mitgliedsNr) {
        auto it = std::find_if(mitglieder.begin(), mitglieder.end(),
            [mitgliedsNr](const Mitglied& m) {
                return m.getMitgliedsNr() == mitgliedsNr;
            });
        return it == mitglieder.end() ? nullptr : &*it;
    }

    // Beide IDs zuerst aufloesen. Nur vorhandene Objekte werden dereferenziert;
    // Mitglied prueft danach das Limit und aktualisiert beide Ausleihzustaende.
    bool ausleihen(int mitgliedsNr, int mediumId) {
        Mitglied* mitglied = findeMitglied(mitgliedsNr);
        Medium* medium = findeMedium(mediumId);
        if (mitglied == nullptr || medium == nullptr) return false;
        return mitglied->ausleihen(*medium);
    }

    // Die Pruefung, ob dieses Mitglied das Medium ausgeliehen hat, liegt in
    // Mitglied::zurueckgeben. Bibliothek vermittelt die passenden Objekte.
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

    // Teiltextsuche in Titel ODER Urheber mit Gross-/Kleinschreibung.
    // Ein leerer Suchtext findet alle Medien; npos bedeutet keinen Treffer.
    // Die Ergebnisliste enthaelt nur lesende Zeiger auf vorhandene Medien,
    // keine Kopien. Nach einer Reallokation des Medien-Vektors oder dem Ende
    // der Bibliothek duerfen diese Zeiger nicht mehr verwendet werden.
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

// Die eigene Testfunktion bleibt im Gegensatz zu assert auch mit -DNDEBUG
// aktiv. Nur erfolgreiche Pruefungen erhoehen den Zaehler; ein Fehler wirft
// eine Ausnahme, die main abfaengt und mit Rueckgabecode 1 meldet.
void pruefe(bool bedingung, const std::string& text, int& anzahl) {
    if (!bedingung) throw std::runtime_error("Test fehlgeschlagen: " + text);
    ++anzahl;
    std::cout << "[OK] " << text << '\n';
}

int main() {
    try {
        // 1. Einen kleinen Bestand mit allen drei Medientypen und zwei
        // Mitgliedern anlegen. So lassen sich auch fremde Rueckgaben pruefen.
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

        // 2. Ausleihe, Medienstatus und gespeicherte IDs gemeinsam pruefen.
        // Anschliessend muessen eine zweite Ausleihe desselben Mediums und
        // die vierte gleichzeitige Ausleihe desselben Mitglieds scheitern.
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

        // 3. Fremde und doppelte Rueckgaben ablehnen. Eine eigene Rueckgabe
        // entfernt die ID, gibt das Medium frei und schafft wieder Kapazitaet.
        pruefe(!b.zurueckgeben(102, 1), "Fremde Rueckgabe abgelehnt", tests);
        pruefe(b.zurueckgeben(101, 1), "Eigene Rueckgabe erfolgreich", tests);
        pruefe(!b.findeMitglied(101)->hatAusgeliehen(1),
               "ID entfernt", tests);
        pruefe(!b.zurueckgeben(101, 1),
               "Doppelte Rueckgabe abgelehnt", tests);
        pruefe(b.ausleihen(102, 1), "Anderes Mitglied leiht erneut", tests);
        pruefe(b.ausleihen(101, 4), "Nach Rueckgabe wieder Platz", tests);

        // 4. Unbekannte Kennungen muessen kontrolliert false beziehungsweise
        // nullptr liefern, statt einen ungueltigen Zeiger zu dereferenzieren.
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

        // 5. Treffer in beiden Suchfeldern sowie leere Ergebnismengen pruefen.
        // Das Vergleichsobjekt hat absichtlich andere Daten bei gleicher ID.
        pruefe(b.suche("Linux").size() == 1, "Titelsuche", tests);
        pruefe(b.suche("Anna Weber").size() == 2, "Urhebersuche", tests);
        pruefe(b.suche("xyz").empty(), "Suche ohne Treffer", tests);
        pruefe(b.suche("").size() == 5,
               "Leere Suche findet alle Medien", tests);
        Medium vergleich(1, "Anderer Titel", "Andere Person", 2020,
                         MediumTyp::DVD);
        pruefe(*b.findeMedium(1) == vergleich, "Gleichheit ueber ID", tests);

        // 6. Erwartete Validierungsfehler lokal abfangen, damit die weiteren
        // Tests laufen. Das Flag vor jedem Fall neu setzen, damit ein frueherer
        // Fehler keinen spaeteren, fehlenden Fehler verdecken kann.
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

        // 7. Die Tests veraendern absichtlich den Bestand. Die abschliessende
        // Ausgabe macht die verbleibenden Ausleihen und Suchtreffer sichtbar.
        std::cout << '\n' << tests << " Pruefungen bestanden.\n\n";
        std::cout << "AUSLEIHEN\n";
        b.findeMitglied(101)->zeigeAusleihen();
        b.findeMitglied(102)->zeigeAusleihen();
        std::cout << "\nSUCHE nach Anna Weber\n";
        for (const Medium* m : b.suche("Anna Weber")) std::cout << *m << '\n';
        std::cout << "\nBESTAND nach den Tests\n";
        b.zeigeBestand();
    } catch (const std::exception& e) {
        // Unerwartete Fehler und fehlgeschlagene Pruefungen beenden den Lauf.
        // stderr trennt Fehlermeldungen von der normalen Programmausgabe.
        std::cerr << "Fehler: " << e.what() << '\n';
        return 1;
    }
    return 0;
}
