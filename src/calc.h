#ifndef CALC_H_
#define CALC_H_

int dmg_formula(int atk_lvl, int eff_pwr, int atk, int def, bool crit,
    bool stab, float effective);

int get_true_stat(int stat_b, int stat_iv, int stat_ev, 
    int lvl, float nature_boost);

int get_true_hp(int hp_base, int hp_iv, int hp_ev, int lvl);

#endif
