#ifndef BATTLE_H_
#define BATTLE_H_
#include "data.h"

int get_max_hp(battler_t* b);
int get_true_atk(battler_t* b);
int get_true_def(battler_t* b);
int get_true_spatk(battler_t* b);
int get_true_spdef(battler_t* b);
int get_true_spd(battler_t* b);
float get_eff_scalar_single(elem_t atk_elem, elem_t def_elem);
float get_eff_scalar_double(elem_t atk_elem, elem_t def_elem1, 
    elem_t def_elem2);
float get_eff(battler_t* b, move_t* m);
int calc_dmg(battler_t* a, battler_t* d, move_t* mv, bool crit);

#endif
