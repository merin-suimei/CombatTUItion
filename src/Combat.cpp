#include "Combat.h"

#include <random>

#define MAX_TURNS 99

// Crude early implementation
EncounterLog Combat::simulateEncounter(
    const Player *player, const Monster *monster)
{
    EncounterLog log{.player = player, .monster = monster, .initiative =
        player->agi >= monster->agi ? PlayerFirst :
                                      MonsterFirst};

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

        if (log.initiative == PlayerFirst)
        {
            log.attacks.push_back(calculateDamage(player, monster, log.turn));
            currentHPMonster -= log.attacks.back().dmg;
            if (currentHPMonster <= 0)
            {
                log.outcome = PlayerWon;
                break;
            }
        }

        log.attacks.push_back(calculateDamage(monster, player, log.turn));
        currentHPPlayer -= log.attacks.back().dmg;
        if (currentHPPlayer <= 0)
        {
            log.outcome = PlayerLost;
            break;
        }

        if (log.initiative == MonsterFirst)
        {
            log.attacks.push_back(calculateDamage(player, monster, log.turn));
            currentHPMonster -= log.attacks.back().dmg;
            if (currentHPMonster <= 0)
            {
                log.outcome = PlayerWon;
                break;
            }
        }

        log.turn++;
    }

    return log;
}

Attack Combat::calculateDamage(
    const Contender *source, const Contender *target, int turn)
{
    Attack attack = Attack{.dmg = 0, .flag = Normal};

    // Prepare random
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(1, source->agi + target->agi);

    // Check if attack missed
    if (distrib(gen) <= target->agi)
	    return Attack{.dmg = 0, .flag = Miss};

    // Caclulating damage
    attack.dmg = source->str + source->getDamage();
    source->applyAttackSkills(&attack, target, turn);
    target->applyDefenceSkills(&attack, source, turn);

    // Return result
    if (attack.dmg <= 0)
        attack.dmg = 0;

    return attack;
}
