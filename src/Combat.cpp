#include "Combat.h"

#include <random>

#define MAX_TURNS 99

// Crude early implementation
EncounterLog Combat::simulateEncounter(
    const Player *player, const Monster *monster)
{
    EncounterLog log{.player = player, .monster = monster};

    int currentHPPlayer = player->hp;
    int currentHPMonster = monster->hp;

    // Simulate encounter turn by turn
    while (true)
    {
        if (log.turn >= MAX_TURNS)
        {
            log.outcome = Draw;
            break;
        }
        
        log.attacks.push_back(calculateDamage(player, monster));
        currentHPMonster -= log.attacks.back().dmg;
        if (currentHPMonster <= 0)
        {
            log.outcome = PlayerWon;
            break;
        }

        log.attacks.push_back(calculateDamage(monster, player));
        currentHPPlayer -= log.attacks.back().dmg;
        if (currentHPPlayer <= 0)
        {
            log.outcome = PlayerLost;
            break;
        }

        log.turn++;
    }

    return log;
}

Attack Combat::calculateDamage(const Contender *source, const Contender *target)
{
    // Prepare random
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(1, source->agi + target->agi);

    // Check if attack missed
    if (distrib(gen) <= target->agi)
	    return Attack{.dmg = 0, .flag = Miss};

    // Caclulating damage
    int baseDamage = source->str + source->getDamage();
    int skillDamage = 0;
    // source.applyAtkSkills();
    // target.applyDefSkills();

    // Return result
    if (baseDamage + skillDamage <= 0)
	    return Attack{.dmg = 0, .flag = DefenceTooHigh};
    return Attack{.dmg = baseDamage + skillDamage, .flag = Normal};
}
