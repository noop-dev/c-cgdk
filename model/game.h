#ifndef _GAME_H_
#define _GAME_H_

struct ct_game {
    int move_count;
    int last_player_elimination_score;
    int player_elimination_score;
    int trooper_elimination_score;
    double trooper_damage_score_factor;
    int stance_change_cost;
    int standing_move_cost;
    int kneeling_move_cost;
    int prone_move_cost;
    int commander_aura_bonus_action_points;
    double commander_aura_range;
    int commander_request_enemy_disposition_cost;
    int commander_request_enemy_disposition_max_offset;
    int field_medic_heal_cost;
    int field_medic_heal_bonus_hitpoints;
    int field_medic_heal_self_bonus_hitpoints;
    double sniper_standing_stealth_bonus;
    double sniper_kneeling_stealth_bonus;
    double sniper_prone_stealth_bonus;
    double sniper_standing_shooting_range_bonus;
    double sniper_kneeling_shooting_range_bonus;
    double sniper_prone_shooting_range_bonus;
    double scout_stealth_bonus_negation;
    int grenade_throw_cost;
    double grenade_throw_range;
    int grenade_direct_damage;
    int grenade_collateral_damage;
    int medikit_use_cost;
    int medikit_bonus_hitpoints;
    int medikit_heal_self_bonus_hitpoints;
    int field_ration_eat_cost;
    int field_ration_bonus_action_points;
    /*    public:
    Game(int moveCount,
    int lastPlayerEliminationScore, int playerEliminationScore,
    int trooperEliminationScore, double trooperDamageScoreFactor,
    int stanceChangeCost, int standingMoveCost, int kneelingMoveCost, int proneMoveCost,
    int commanderAuraBonusActionPoints, double commanderAuraRange,
    int commanderRequestEnemyDispositionCost, int commanderRequestEnemyDispositionMaxOffset,
    int fieldMedicHealCost, int fieldMedicHealBonusHitpoints, int fieldMedicHealSelfBonusHitpoints,
    double sniperStandingStealthBonus, double sniperKneelingStealthBonus, double sniperProneStealthBonus,
    double sniperStandingShootingRangeBonus, double sniperKneelingShootingRangeBonus,
    double sniperProneShootingRangeBonus, double scoutStealthBonusNegation,
    int grenadeThrowCost, double grenadeThrowRange, int grenadeDirectDamage, int grenadeCollateralDamage,
    int medikitUseCost, int medikitBonusHitpoints, int medikitHealSelfBonusHitpoints,
    int fieldRationEatCost, int fieldRationBonusActionPoints);*/
};

#endif
