#include "Combat.h"

#include <random>

#define MAX_TURNS 99

// Crude early implementation
EncounterLog Combat::simulateEncounter(const Contender *contender1,
    const Contender *contender2)
{
    EncounterLog log;
    log.contender1 = contender1;
    log.contender2 = contender2;

    int currentHPContender1 = contender1->hp;
    int currentHPContender2 = contender2->hp;

    // Simulate encounter turn by turn
    while (true)
    {
        if (log.turn >= MAX_TURNS)
        {
            log.outcome = Draw;
            break;
        }
        
        log.attacks.push_back(calculateDamage(contender1, contender2));
        currentHPContender2 -= log.attacks.back().dmg;
        if (currentHPContender2 <= 0)
        {
            log.outcome = PlayerWon;
            break;
        }

        log.attacks.push_back(calculateDamage(contender2, contender1));
        currentHPContender1 -= log.attacks.back().dmg;
        if (currentHPContender1 <= 0)
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
