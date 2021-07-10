#pragma once
#include "ScoreKeeper.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

class HighScores
{
public:
        
    HighScores() {
        readFile();
    }
    void appendScoresToFile(ScoreKeeper *sc) {
        readFile();
        bestscore = std::max(sc->getScore(), bestscore);
        bestlevel = std::max(sc->getLevel(), bestlevel);
        writeFile();
    }
    unsigned long long int getBestScore() const noexcept { return bestscore; };
    unsigned long long int getBestLevel() const noexcept { return bestlevel; };


private:
    void readFile() {
        file.open("scores.dat", std::ios::in | std::ios::binary | std::ios::ate);
        if (file.is_open()) {
            if (file.tellg() > 0) {
                file.seekg(0, std::ios::beg);
                file.read((char*)&bestscore, sizeof(bestscore));
                file.read((char*)&bestlevel, sizeof(bestlevel));
                file.close();
            }
        }
    }
    void writeFile() {
        file.open("scores.dat", std::ios::out | std::ios::binary);
        if (file.is_open()) {
            file.clear();
            file.write((char*)&bestscore, sizeof(bestscore));
            file.write((char*)&bestlevel, sizeof(bestlevel));
            file.close();
        }
    }

    unsigned long long int bestscore = 0;
    unsigned long int bestlevel = 0;

    std::fstream file;
};