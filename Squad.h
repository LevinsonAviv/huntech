#pragma once

#include "wet2util.h"

class Hunter;

struct Squad{
    int squadId;
    int squadSize;
    NenAbility squadNenAbility;
    int squadAura;
    int squadExp;
    Hunter *squadRoot;

    struct Rank{
        int aura;
        int id;

        bool operator==(const Rank& other) const{
            return aura == other.aura && id == other.id;
        }

        bool operator<(const Rank& other) const{
            return aura < other.aura || (aura == other.aura && id < other.id);
        }

        bool operator>(const Rank& other) const{
            return aura > other.aura || (aura == other.aura && id > other.id);
        }
    };

    Rank getRank(){
        return Rank{squadAura, squadId};
    }
};