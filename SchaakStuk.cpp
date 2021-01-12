//  Student: Deputter Pablo
//  Rolnummer: s0205440
//  Opmerkingen: (bvb aanpassingen van de opgave)
//

#include "SchaakStuk.h"
#include <game.h>
// #include <vector> #TODO staat al in .h file!
using namespace std;

// Berekent geldige zetten van een schaakstuk
vector<pair<int, int>> SchaakStuk::geldige_zetten(const Game &game) {
    vector<pair<int, int>> possibleMoves;
    return possibleMoves; // Returnt lege vector
}

// Checkt of de mogelijke zet niet buiten de dimensies van het chessBoard valt
bool SchaakStuk::checkDimensions(const pair<int, int> & i) {
    if ( (0 <= i.first) && ( i.first <= 7) &&  (0 <= i.second) && ( i.second <= 10) ) {
        return true;
    }
    return false;
}

// Geeft een bool terug of een Pion op de originele startpositie staat of niet
bool Pion::startPosition(const Game & game) const {
    int r = this->position.first;
    int k = this->position.second;

    if ( wit == this->getKleur() ) {
        for ( int i = 0; i < 8; i++ ) {
            if ( this == game.getPiece(0, i) ) {
                return true;
            }
        }
    }
    for ( int i = 0; i < 8; i++ ) {
        if (this == game.getPiece(6, i)) {
            return true;
        }
    }
    return false;
}

// Berekent geldige zetten van een Pion
vector<pair<int, int>> Pion::geldige_zetten(const Game &game) {
    vector<pair<int, int>> possibleMoves;

    // Wordt er naar boven of naar beneden verplaatst?
    int fact = 1;
    if ( getKleur() == zwart ) {
        fact = -1;
    }

    int r = this->position.first;
    int k = this->position.second;

    // Als de pion op de startpositie staat kan deze onmiddelijk 2 stappen zetten
    if ( 0 <= (r + 2)*fact << 7 && startPosition(game) ) { // Mag dimensies van chessBoard niet overschrijven
        possibleMoves.emplace_back((r + 2)*fact, k);
    }
    // Als de pion NIET op de startpositie staat kan deze, maar 1 stap zetten
    if ( 0 <= (r + 2)*fact ) {
        possibleMoves.emplace_back((r + 1)*fact, k);
    }

    // Mogelijke stappen verwijderen waar al een SchaakStuk staat
    for ( const pair<int, int> &i : possibleMoves ) {
        SchaakStuk * moveTo = game.getPiece(i.first, i.second);
        if ( moveTo != nullptr ) {
            // Verwijder stap die niet mogelijk is
            possibleMoves.erase(remove(possibleMoves.begin(), possibleMoves.end(), i), possibleMoves.end());
        }
    }

    // Pion kan ook rechts aanvallen (=SchaakStuk mag niet van dezelfde kleur zijn)
    SchaakStuk * attackRight = game.getPiece((r + 1)*fact, (k + 1)); // Dimensies moeten hier niet gecheckt worden
    if ( attackRight != nullptr && attackRight->getKleur() != getKleur() ) {
        possibleMoves.emplace_back((r + 1)*fact, (k + 1));
    }

    // Pion kan ook links aanvallen (=SchaakStuk mag niet van dezelfde kleur zijn)
    SchaakStuk * attackLeft = game.getPiece((r + 1)*fact, (k - 1));
    if ( attackRight != nullptr && attackRight->getKleur() != getKleur() ) {
        possibleMoves.emplace_back((r + 1)*fact, (k - 1));
    }

    return possibleMoves;
}

// Berekent geldige zetten van een Toren
vector<pair<int, int>> Toren::geldige_zetten(const Game &game) {
    vector<pair<int, int>> possibleMoves;

    // verkrijg de rij en kolom van de Toren
    int r = this->position.first;
    int k = this->position.second;
    zw color = getKleur(); // color is type van zw enum

    while ( r != 7 ) {
        r++;
        SchaakStuk * moveTo = game.getPiece(r, k);

        // Als moveTo een nullPtr is OF niet dezelfde kleur is het een geldige move
        if ( moveTo == nullptr || moveTo->getKleur() != color ) {
            possibleMoves.emplace_back(r, k);
        }
        else {
            continue;
        }

    }

    while ( r != 0 ) {
        r--; // decrease
        SchaakStuk * moveTo = game.getPiece(r, k);


        // Als moveTo een nullPtr is OF niet dezelfde kleur is het een geldige move
        if ( moveTo == nullptr || moveTo->getKleur() != color ) {
            possibleMoves.emplace_back(r, k);
        }
        else {
            continue;
        }

    }

    while ( k != 0 ) {
        k++; // decrease
        SchaakStuk * moveTo = game.getPiece(r, k);

        // Als moveTo een nullPtr is OF niet dezelfde kleur is het een geldige move
        if ( moveTo == nullptr || moveTo->getKleur() != color ) {
            possibleMoves.emplace_back(r, k);
        }
        else {
            continue;
        }

    }

    while ( k != 0 ) {
        k--; // decrease
        SchaakStuk * moveTo = game.getPiece(r, k);

        // Als moveTo een nullPtr is OF niet dezelfde kleur is het een geldige move
        if ( moveTo == nullptr || moveTo->getKleur() != color ) {
            possibleMoves.emplace_back(r, k);
        }
        else {
            continue;
        }

    }

    return possibleMoves;
}

vector<pair<int, int>> Paard::geldige_zetten(const Game &game) {
    vector<pair<int, int>> possibleMoves;
    // verkrijg de rij en kolom van het Paard
    int r = this->position.first;
    int k = this->position.second;
    zw color = getKleur();

    pair <int, int> moveOne;
    moveOne.first = (r - 1); moveOne.second = (k + 2);

    pair <int, int> moveTwo;
    moveTwo.first = (r - 2); moveTwo.second = (k - 1);

    pair <int, int> moveThree;
    moveThree.first = (r - 1); moveThree.second = (k - 2);

    pair <int, int> moveFour;
    moveFour.first = (r + 1); moveFour.second = (k - 2);

    pair <int, int> moveFive;
    moveFive.first = (r + 2); moveFive.second = (k + 1);


    return possibleMoves;
}


vector<pair<int, int>> Loper::geldige_zetten(const Game &game) {
    vector<pair<int, int>> possibleMoves;
    return possibleMoves;
}

vector<pair<int, int>> Koning::geldige_zetten(const Game &game) {
    vector<pair<int, int>> possibleMoves;
    return possibleMoves;
}

vector<pair<int, int>> Koningin::geldige_zetten(const Game &game) {
    vector<pair<int, int>> possibleMoves;
    return possibleMoves;
}