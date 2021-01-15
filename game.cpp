//  Student: Deputter Pablo
//  Rolnummer: s0205440
//  Opmerkingen: Ik heb geprobeerd zo veel mogelijk gebruik te maken van functies
//

#include "game.h"
#include <algorithm> // Voor te zoeken in vectoren
#include <iostream>
using namespace std;


Game::Game() {
    turnToMove = wit; // Wit begint met zetten
    moving = false;
    pieceToMove = nullptr;
}

Game::~Game() {}

// Zet het bord klaar; voeg de stukken op de juiste plaats toe
void Game::setStartBord() {

    SchaakStuk * null = nullptr; // Maak nullptr aan van SchaakStuk object

    // Zet het schaakbord vol met nullptr's die later vervangen zullen worden met schaakstukken
    for ( int i = 0; i < 8; i++ ) {
        for ( int j = 0; j < 8; j++ ) {
            setNullptr(i, j, null );
        }
    }

    // Zet alle pionen klaar op het bord
    int i = 0;
    while ( i != 8 ) {
        setPawns(i);
    }

    // Zet witte schaakstukken klaar op schaakbord
    setPiece(0, 0, new Toren(zwart));
    setPiece(0, 1, new Paard(zwart));
    setPiece(0, 2, new Loper(zwart));
    setPiece(0, 3, new Koningin(zwart));
    setPiece(0, 4, new Koning(zwart));
    setPiece(0, 5, new Loper(zwart));
    setPiece(0, 6, new Paard(zwart));
    setPiece(0, 7, new Toren(zwart));

    // Zet zwarte schaakstukken klaar op schaakbord
    setPiece(7, 0, new Toren(wit));
    setPiece(7, 1, new Paard(wit));
    setPiece(7, 2, new Loper(wit));
    setPiece(7, 3, new Koningin(wit));
    setPiece(7, 4, new Koning(wit));
    setPiece(7, 5, new Loper(wit));
    setPiece(7, 6, new Paard(wit));
    setPiece(7, 7, new Toren(wit));
}

// Zet alle pionen klaar op het bord
void Game::setPawns(int & i) {
    setPiece(1, i, new Pion(zwart));
    setPiece(6, i, new Pion(wit));
    i++; // overloop elke plaats waar een pion kan staan
}


// Geeft terug welk schaakstuk op een bepaalde positie op het spelbord staat
SchaakStuk* Game::getPiece(const int & r, const int & k) const {
    return bord[r][k];
}

// Verplaatst een schaakstuk naar de gegeven positie op het spelbord
void Game::setPiece(const int & r, const int & k, SchaakStuk* s) {
    s->position.first = r;
    s->position.second = k;
    bord[r][k] = s;
}

// Verplaatst een nullptr naar de gegeven positie op het spelbord
void Game::setNullptr(const int & r, const int & k, SchaakStuk* s) {
    bord[r][k] = s;
}

// Verplaats stuk s naar positie (r,k)
// Als deze move niet mogelijk is, wordt false teruggegeven
// en verandert er niets aan het schaakbord.
// Anders wordt de move uitgevoerd en wordt true teruggegeven
bool Game::move(SchaakStuk * s, const pair<int, int> & p) {
    bool found = false;
    vector<pair<int, int>> possibleMoves; // Vector met mogelijke zetten


    // Is het SchaakStuk dat we willen verplaatsen de Koning?
    if ( s->piece().type() == Piece::King ) {
        // Koning mag zich zelf niet schaak zetten
        possibleMoves = checkForKingCheck(s->geldige_zetten(*this, false), s);
    }

    else { // Wanneer koning schaak staat moeten er een omweg gemaakt worden
        possibleMoves = checkForKingCheck(s->geldige_zetten(*this, false), s); // Verkrijg geldige zetten als King schaak staat
    }
    // Kijk of de zet geldig is
    if ( find(possibleMoves.begin(), possibleMoves.end(), p ) != possibleMoves.end() ) {
        found = true; // Wanneer gevonden kan het SchaakStuk verplaatst worden
        setNullptr(s->position.first, s->position.second, nullptr); // Originele plaats wordt vervangen met nullptr
        capturedPiece(p); // Ander SchaakStuk verovert?
        setPiece(p.first, p.second, s); // Verplaats SchaakStuk
    }

    // Men is niet meer bezig met verplaatsen en moet dus opnieuw een SchaakStuk selecteren om te verplaatsen
    resetMovingAndPieceToMove();
    return found; // Wanneer er geen geldige zet wordt gekozen blijft de huidige kleur aan de beurt
}

// Kijkt na of de positie waar het SchaakStuk naar wilt bewegen een
// SchaakStuk is van de tegenstander en verwijdert deze wanneer nodig
void Game::capturedPiece(const pair<int, int> & moveTo) const {
    if ( getPiece(moveTo.first, moveTo.second) == nullptr ) { // Er moet niks gedaan worden bij een nullptr
    }
        // Bij een SchaakStuk wordt deze verwijdert uit de heap
    else if ( getPiece(moveTo.first, moveTo.second)->getKleur() != turnToMove ) {
        delete getPiece(moveTo.first, moveTo.second);
    }
    return;
}

// Geeft true als koning van de speler die aan de beurt is schaak staat
bool Game::schaak(const zw & kleur) {
    bool found = false;
    // Zoek de positie van de koning
    const pair<int, int> & king = findKing(kleur);

    // Itereer over alle vakjes
    for ( int i = 0; i < 8; i++ ) {
        for ( int j = 0; j < 8; j++ ) {

            // Overloop alle SchaakStukken van de tegenstander
            if ( getPiece(i, j) != nullptr && getPiece(i, j)->getKleur() != kleur ) {
                // Is positie van een van de geldige zetten van de tegenstander gelijk aan die van "onze" koning
                const vector<pair<int, int>> & moves = getPiece(i, j)->geldige_zetten(*this, false);
                if ( find(moves.begin(), moves.end(), king) != moves.end() ) {
                    found = true; // Schaak --> Stop onmiddleijk met zoeken
                    break;
                }
            }
        }
    }
    return found;
}

// Zoek de positie van de koning van de kleur die momenteel aan de beurt is
pair<int, int> Game::findKing(const zw & kleur) const {
    pair<int, int> kingPosition;
    for ( int i = 0; i < 8; i++ ) {
        for ( int j = 0; j < 8; j++ ) {
            if ( getPiece(i, j) != nullptr && // Mag geen nullptr zijn anders seg fault
                 getPiece(i, j)->getKleur() == kleur && // Moet van kleur zijn die mom. aan de beurt is
                 getPiece(i, j)->piece().type() == Piece::King ) { // Moet van type King zijn
                kingPosition = make_pair(i, j);
                break;
            }
        }
    }
    return kingPosition;
}

vector<pair<int, int>> Game::checkForKingCheck(const vector<pair<int, int>> & possibleMoves, SchaakStuk * s) {

    vector<pair<int, int>> newMoves; // Vector waar we de geldige zetten in gaan opslaan

    // Originele positie van het SchaakStuk dat we steeds gaan verschuiven
    const pair<int, int> & originalPosition = make_pair(s->position.first, s->position.second);

    // Verkrijg de kleur die NIET aan de beurt is
    zw color = wit;
    if ( getTurnMove() == wit ) {
        color = zwart;
    }
    // Overloop alle mogelijke zetten die het SchaakStuk kan zetten wanneer de koning NIET schaak staat
    for ( const pair<int, int> & i : possibleMoves ) {

        // Verkrijg het SchaakStuk waar we naar gaan verplaatsen
        SchaakStuk * oldPiece = getPiece(i.first, i.second);

        // Verplaats SchaakStuk naar positie
        setPiece(i.first, i.second, s);

        // Zet een nullptr op orignele positie, anders zijn er bv 2 koningen
        setNullptr(originalPosition.first, originalPosition.second, nullptr);

        if ( !schaak(getTurnMove()) ) { // Check of de king nog steeds schaak staat

            newMoves.push_back(i); // Koning staat niet meer schaak en kan dus toegevoegd worden aan nieuwe vector

        }
        capturedPiece(i);

        // Verplaatst SchaakStuk wordt terug naar zijn originele plaatst verzet
        setPiece(originalPosition.first, originalPosition.second, s);
        if ( oldPiece == nullptr ) {
            setNullptr(i.first, i.second, oldPiece);
        }
        else {
            // Verplaats oud SchaakStuk terug naar zijn originele plaats
            setPiece(i.first, i.second, oldPiece );
        }

    }
    return newMoves;
}

// Checkt of de meegegeven integer paar de positie is van de Koning van
// de kleur die momenteel aan de beurt is
bool Game::checkKing(const int & r, const int & k) const {

    if (getPiece(r, k) != nullptr && // Anders krijgen we een segFault
        getPiece(r, k)->getKleur() == getTurnMove() && // Moet van de juiste kleur zijn
        getPiece(r, k)->piece().type() == Piece::King ) { // Moet van type King zijn
        return true;
    }
    return false;
}

// Geeft true als kleur schaakmat staat
bool Game::schaakmat(const zw & kleur) {

    bool checkMate = true;
    // Itereer over alle vakjes
    for ( int i = 0; i < 8; i++ ) {
        for ( int j = 0; j < 8; j++ ) {

            // Check alle mogelijke zetten van SchaakStukken van een andere kleur wanneer
            // de koning SCHAAK staat
            if ( getPiece(i, j) != nullptr && getPiece(i, j)->getKleur() == kleur ) {
                if ( !checkForKingCheck(getPiece(i, j)->geldige_zetten(*this, false), getPiece(i, j)).empty() ) {
                    // Wanneer er een SchaakStuk gevonden wordt waarbij de vector van geldige zetten
                    // bij schaak niet leeg is wordt het zoeken onmmiddelijk gestopt.
                    checkMate = false; // Geen schaak!
                    break;
                }
            }
        }
    }
    return checkMate;
}

// Geeft true als kleur pat staat
// (pat = geen geldige zet mogelijk, maar kleur staat niet schaak;
// dit resulteert in een gelijkspel)
bool Game::pat(const zw & kleur) {
    bool pat = true;
    for ( int i = 0; i < 8; i++ ) {
        for ( int j = 0; j < 8; j++ ) {

            if ( getPiece(i, j) != nullptr ) {

                if (getPiece(i, j)->getKleur() == kleur) {

                    if (!checkForKingCheck(getPiece(i, j)->geldige_zetten(*this, false), getPiece(i, j)).empty()) {

                        pat = false;
                        break;
                    }
                }
            }
            else {

            }
        }
    }
    // Als pat true is en de koning staat niet schaak dan spreken we over pat
    if ( !schaak(kleur) && pat == true ) {
        return true;
    }
    else { // Bij schaak wordt er onmiddelijk false teruggegeven
        return false;
    }
}

vector<pair<int, int>> Game::getThreats(const vector<pair<int, int>> & moves, const zw & color, SchaakStuk * s) {
    vector<pair<int, int>> threats;

    // Originele positie van SchaakStuk dat we steeds gaan verschuiven
    const pair<int, int> & originalPosition = make_pair(s->position.first, s->position.second);

    // Overloop elk vakje van het schaakBord
    for ( int i = 0; i < 8; i++ ) {
        for ( int j = 0; j < 8; j++ ) {

            // Zoeken naar vijanden
            if ( getPiece(i, j) != nullptr && getPiece(i, j)->getKleur() != color ) {

                // Overlopen van de geldige zetten van het SchaakStuk dat geselecteerd is
                for ( const pair<int, int> & k : moves ) {

                    // Verkrijg het SchaakStuk waar we naar gaan verplaatsen
                    SchaakStuk * oldPiece = getPiece(k.first, k.second);

                    // Verplaats SchaakStuk naar positie
                    setPiece(k.first, k.second, s);

                    // Zet een nullptr op orignele positie
                    setNullptr(originalPosition.first, originalPosition.second, nullptr);

                    // Verkrijg de geldige zetten van de vijand, hier staat de bool kills op true
                    // want een Pion kan bv niet rechtdoor aanvallen
                    const vector<pair<int, int>> enemyMoves = getPiece(i, j)->geldige_zetten(*this, true);

                    // Zit k in een van de zetten van de vijand?
                    if ( find( enemyMoves.begin(), enemyMoves.end(), k ) != enemyMoves.end() ) {

                        // Zoja, voeg deze toe aan de vector
                        threats.push_back(k);
                    }

                    // Wordt er een SchaakStuk verovert?
                    capturedPiece(k);
                    // Verplaatst SchaakStuk wordt terug naar zijn originele plaatst verzet
                    setPiece(originalPosition.first, originalPosition.second, s);

                    if ( oldPiece == nullptr ) {
                        setNullptr(k.first, k.second, oldPiece);
                    }
                    else {
                        // Verplaats oud SchaakStuk terug naar zijn originele plaats
                        setPiece(k.first, k.second, oldPiece );
                    }

                }
            }
        }
    }
    return threats;
}


