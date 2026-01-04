// You can edit anything you want in this file.
// However, you need to implement all public Huntech functions, which are provided below as a template.

#include "Huntech26a2.h"

Huntech::Huntech(): squadsByIdTree(), squadsByAuraTree(), huntersHash() {}

Huntech::~Huntech() {}

StatusType Huntech::add_squad(int squadId) {
    try{
        if(squadId <= 0){
            return StatusType::INVALID_INPUT;
        }
        if(squadsByIdTree.find(squadId) != nullptr){
            return StatusType::FAILURE;
        }
        std::shared_ptr<Squad> squad = std::make_shared<Squad>(Squad{squadId, 0, NenAbility::zero(), 0, 0, nullptr});
        squadsByIdTree.insert(squadId, squad);
        squadsByAuraTree.insert(squad->getRank(), squad);
        return StatusType::SUCCESS;
    }
    catch(std::bad_alloc& e){
        return StatusType::ALLOCATION_ERROR;
    }
}

StatusType Huntech::remove_squad(int squadId) {
    try{
        if(squadId <= 0){
            return StatusType::INVALID_INPUT;
        }
        std::shared_ptr<Squad> *ptrToSquadShared = squadsByIdTree.find(squadId);
        if(ptrToSquadShared == nullptr){
            return StatusType::FAILURE;
        }
        squadsByIdTree.remove(squadId);
        squadsByAuraTree.remove((*ptrToSquadShared)->getRank());
        return StatusType::SUCCESS;
    }
    catch(std::bad_alloc& e){
        return StatusType::ALLOCATION_ERROR;
    }
}

StatusType Huntech::add_hunter(int hunterId,
                               int squadId,
                               const NenAbility &nenType,
                               int aura,
                               int fightsHad)
{
    try{
        if(hunterId <= 0 || squadId <= 0 || !nenType.isValid() || aura < 0 || fightsHad < 0){
            return StatusType::INVALID_INPUT;
        }
        std::shared_ptr<Squad> *ptrToSquadShared = squadsByIdTree.find(squadId);
        if(ptrToSquadShared == nullptr || huntersHash.find(hunterId) != nullptr){
            return StatusType::FAILURE;
        }
        std::shared_ptr<Squad> squad = *ptrToSquadShared;
        Squad::Rank oldRank = squad->getRank();
        std::shared_ptr<Hunter> hunter = std::make_shared<Hunter>(Hunter{hunterId, nenType, aura, fightsHad, 0, NenAbility::zero(), nullptr, nullptr});
        if(squad->squadSize == 0){
            squad->squadRoot = hunter.get();
            hunter->squad = squad.get();
        }else{
            hunter->fightsHadExtra = -squad->squadRoot->fightsHadExtra;
            hunter->nenAbilityExtra = squad->squadNenAbility;
            hunter->parent = squad->squadRoot;
        }
        squad->squadSize++;
        squad->squadNenAbility += nenType;
        squad->squadAura += aura;
        huntersHash.insert(hunterId ,hunter);
        squadsByAuraTree.remove(oldRank);
        squadsByAuraTree.insert(squad->getRank(), squad);
        return StatusType::SUCCESS;
    }
    catch(std::bad_alloc& e){
        return StatusType::ALLOCATION_ERROR;
    }
}

output_t<int> Huntech::squad_duel(int squadId1, int squadId2) {
    try{
        if(squadId1 <= 0 || squadId2 <= 0 || squadId1 == squadId2){
            return StatusType::INVALID_INPUT;
        }
        std::shared_ptr<Squad> *ptrToSquadShared1 = squadsByIdTree.find(squadId1);
        std::shared_ptr<Squad> *ptrToSquadShared2 = squadsByIdTree.find(squadId2);
        if(ptrToSquadShared1 == nullptr || ptrToSquadShared2 == nullptr){
            return output_t<int>(StatusType::FAILURE);
        }
        std::shared_ptr<Squad> squad1 = *ptrToSquadShared1;
        std::shared_ptr<Squad> squad2 = *ptrToSquadShared2;
        if(squad1->squadSize == 0 || squad2->squadSize == 0){
            return output_t<int>(StatusType::FAILURE);
        }
        int squadEffAura1 = squad1->squadExp + squad1->squadAura;
        int squadEffAura2 = squad2->squadExp + squad2->squadAura;
        squad1->squadRoot->fightsHadExtra++;
        squad2->squadRoot->fightsHadExtra++;
        if(squadEffAura1 > squadEffAura2){
            squad1->squadExp += 3;
            return 1;
        }else if(squadEffAura1 < squadEffAura2){
            squad2->squadExp += 3;
            return 3;
        }else if(squad1->squadNenAbility > squad2->squadNenAbility){
            squad1->squadExp += 3;
            return 2;
        }else if(squad1->squadNenAbility < squad2->squadNenAbility){
            squad2->squadExp += 3;
            return 4;
        }else{
            squad1->squadExp += 1;
            squad2->squadExp += 1;
            return 0;
        }
    }
    catch(std::bad_alloc& e){
        return StatusType::ALLOCATION_ERROR;
    }
}

output_t<int> Huntech::get_hunter_fights_number(int hunterId) {
    try{
        if(hunterId <= 0){
            return StatusType::INVALID_INPUT;
        }
        std::shared_ptr<Hunter> *ptrToHunterShared = huntersHash.find(hunterId);
        if(ptrToHunterShared == nullptr){
            return output_t<int>(StatusType::FAILURE);
        }
        std::shared_ptr<Hunter> hunter = *ptrToHunterShared;
        compressPath(hunter.get());
        return hunter->fightsHad + hunter->fightsHadExtra + ((hunter->parent != nullptr) ? hunter->parent->fightsHadExtra : 0);
    }
    catch(std::bad_alloc& e){
        return StatusType::ALLOCATION_ERROR;
    }
}

output_t<int> Huntech::get_squad_experience(int squadId) {
    try{
        if(squadId <= 0){
            return StatusType::INVALID_INPUT;
        }
        std::shared_ptr<Squad> *ptrToSquadShared = squadsByIdTree.find(squadId);
        if(ptrToSquadShared == nullptr){
            return StatusType::FAILURE;
        }
        return (*ptrToSquadShared)->squadExp;
    }
    catch(std::bad_alloc& e){
        return StatusType::ALLOCATION_ERROR;
    }
}

output_t<int> Huntech::get_ith_collective_aura_squad(int i) {
    try{
        std::shared_ptr<Squad> *ptrToSquadShared = squadsByAuraTree.select(i);
        if(ptrToSquadShared == nullptr){
            return StatusType::FAILURE;
        }
        return (*ptrToSquadShared)->squadId;
    }
    catch(std::bad_alloc& e){
        return StatusType::ALLOCATION_ERROR;
    }
}

output_t<NenAbility> Huntech::get_partial_nen_ability(int hunterId) {
    try{
        if(hunterId <= 0){
            return StatusType::INVALID_INPUT;
        }
        std::shared_ptr<Hunter> *ptrToHunterShared = huntersHash.find(hunterId);
        if(ptrToHunterShared == nullptr){
            return output_t<NenAbility>(StatusType::FAILURE);
        }
        std::shared_ptr<Hunter> hunter = *ptrToHunterShared;
        compressPath(hunter.get());
        return hunter->nenAbility + hunter->nenAbilityExtra + ((hunter->parent != nullptr) ? hunter->parent->nenAbilityExtra : NenAbility::zero());
    }
    catch(std::bad_alloc& e){
        return StatusType::ALLOCATION_ERROR;
    }
}

StatusType Huntech::force_join(int forcingSquadId, int forcedSquadId) {
    try{
        if(forcingSquadId <= 0 || forcedSquadId <= 0 || forcingSquadId == forcedSquadId){
            return StatusType::INVALID_INPUT;
        }
        std::shared_ptr<Squad> *ptrToForcingSquadShared = squadsByIdTree.find(forcingSquadId);
        std::shared_ptr<Squad> *ptrToForcedSquadShared = squadsByIdTree.find(forcedSquadId);
        if(ptrToForcingSquadShared == nullptr || ptrToForcedSquadShared == nullptr){
            return StatusType::FAILURE;
        }
        std::shared_ptr<Squad> forcingSquad = *ptrToForcingSquadShared;
        std::shared_ptr<Squad> forcedSquad = *ptrToForcedSquadShared;
        if(forcingSquad->squadSize == 0 ||
            forcingSquad->squadExp + forcingSquad->squadAura + (forcingSquad->squadNenAbility.getEffectiveNenAbility()) <=
            forcedSquad->squadExp + forcedSquad->squadAura + (forcedSquad->squadNenAbility.getEffectiveNenAbility()))
        {
            return StatusType::FAILURE;
        }
        if(forcedSquad->squadSize == 0){
            return StatusType::SUCCESS;
        }
        squadsByIdTree.remove(forcedSquad->squadId);
        squadsByAuraTree.remove(forcingSquad->getRank());
        squadsByAuraTree.remove(forcedSquad->getRank());
        forcedSquad->squadRoot->nenAbilityExtra += forcingSquad->squadNenAbility;
        if(forcingSquad->squadSize >= forcedSquad->squadSize){
            forcedSquad->squadRoot->parent = forcingSquad->squadRoot;
            forcedSquad->squadRoot->squad = nullptr;
            forcedSquad->squadRoot->fightsHadExtra -= forcingSquad->squadRoot->fightsHadExtra;
            forcedSquad->squadRoot->nenAbilityExtra -= forcingSquad->squadRoot->nenAbilityExtra;
        }else{
            forcingSquad->squadRoot->parent = forcedSquad->squadRoot;
            forcingSquad->squadRoot->squad = nullptr;
            forcingSquad->squadRoot->fightsHadExtra -= forcedSquad->squadRoot->fightsHadExtra;
            forcingSquad->squadRoot->nenAbilityExtra -= forcedSquad->squadRoot->nenAbilityExtra;
            forcingSquad->squadRoot = forcedSquad->squadRoot;
        }
        forcingSquad->squadAura += forcedSquad->squadAura;
        forcingSquad->squadNenAbility += forcedSquad->squadNenAbility;
        forcingSquad->squadExp += forcedSquad->squadExp;
        forcingSquad->squadSize += forcedSquad->squadSize;
        squadsByAuraTree.insert(forcingSquad->getRank(), forcingSquad);
        return StatusType::SUCCESS;
    }
    catch(std::bad_alloc& e){
        return StatusType::ALLOCATION_ERROR;
    }
}

void Huntech::compressPath(Hunter* hunter) {
    if(hunter->parent != nullptr && hunter->parent->parent != nullptr){
        compressPath(hunter->parent);
        hunter->fightsHadExtra += hunter->parent->fightsHadExtra;
        hunter->nenAbilityExtra += hunter->parent->nenAbilityExtra;
        hunter->parent = hunter->parent->parent;
    }
}