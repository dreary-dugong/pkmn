#include <math.h> //floor
#include <stdbool.h> //boolean type
#include <stdlib.h> //rand

//project headers
#include "calc.h"

//calculate mathematically damage given raw numbers
int dmg_formula(int atk_lvl, int eff_pwr, int atk, int def, bool crit, bool stab, float effective){

  //https://bulbapedia.bulbagarden.net/wiki/Damage
  int dmg = floor((((2*atk_lvl)/5.0+2)*eff_pwr*((float)atk/def))/50.0+2); //basic damage
  dmg = floor(dmg+(dmg*0.5*crit)); //crit
  dmg = floor(dmg+(dmg*0.5*stab)); //same type attack bonus; 
  dmg = floor(dmg*effective);
  float rand_scalar = ( (rand() % 16) + 85) / 100.0;
  dmg = floor(dmg * rand_scalar);
  return dmg;
}

//calculate the "true" stat of a battler to use in damage calculation
int get_true_stat(int stat_b, int stat_iv, int stat_ev, int lvl, float nature_boost){
  int stat = floor(((2*stat_b + stat_iv + floor(stat_ev / 4.0))*lvl)/100.0) + 5;
  stat = floor(stat*nature_boost);
  return stat;
}

//calculate the "true" hp of a battler to use in battle
int get_true_hp(int hp_base, int hp_iv, int hp_ev, int lvl){
  int hp = floor(((2.0*hp_base+hp_iv+floor(hp_ev/4.0))+lvl)/100.0) + lvl + 10;
  return hp;
}
