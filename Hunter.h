#pragma once

#include "wet2util.h"
#include "Squad.h"

struct Hunter{
    int hunterId;
    NenAbility nenAbility;
    int aura;
    int fightsHad;
    
    int fightsHadExtra;
    NenAbility nenAbilityExtra;

    Hunter *parent;
    Squad *squad;
};