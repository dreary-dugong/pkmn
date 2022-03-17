#include <stdbool.h> //boolean type

//project headers
#include "battle.h"
#include "calc.h"
#include "data.h"

//element/type effects
#define SUPER_EFFECTIVE 2.0
#define NOT_EFFECTIVE 0.0
#define NOTVERY_EFFECTIVE 0.5
#define EFFECTIVE 1.0

//determine the max hp of a given battler based on its properties
int get_max_hp(battler_t* b){
  pkmn_t*  pkmn = b->pkmn;
  int ev = pkmn->hp_ev;
  int iv = pkmn->hp_iv;
  int lvl = pkmn->lvl;

  int base = pkmn->species->hp_b;

  int hp  = get_true_hp(base, iv, ev, lvl);
  return hp;
}

//calculate the effective attack stat of a battler for use in damage calc
int get_true_atk(battler_t* b){

  pkmn_t* pkmn = b->pkmn;
  int ev = pkmn->atk_ev;
  int iv = pkmn->atk_iv;
  int lvl = pkmn->lvl;

  int base = pkmn->species->atk_b;

  nature_t* nature = pkmn->nature;
  float nature_boost = 1;
  if(nature->up==atk){
    nature_boost = 1.1;
  } else if(nature->down==atk){
    nature_boost = 0.9;
  }

  int atk = get_true_stat(base, iv, ev, lvl, nature_boost);
  return atk;
}

//calculate the effective defense stat of a battler for use in damage calc
int get_true_def(battler_t* b){

  pkmn_t*  pkmn = b->pkmn;
  int ev = pkmn->def_ev;
  int iv = pkmn->def_iv;
  int lvl = pkmn->lvl;

  int base = pkmn->species->def_b;

  nature_t* nature = pkmn->nature;
  float nature_boost = 1;
  if(nature->up==def){
    nature_boost = 1.1;
  } else if(nature->down==def){
    nature_boost = 0.9;
  }

  int def = get_true_stat(base, iv, ev, lvl, nature_boost);
  return def;
}

//calculate the effective special attack  stat of a battler for use in damage calc
int get_true_spatk(battler_t* b){

  pkmn_t*  pkmn = b->pkmn;
  int ev = pkmn->spatk_ev;
  int iv = pkmn->spatk_iv;
  int lvl = pkmn->lvl;

  int base = pkmn->species->spatk_b;

  nature_t* nature = pkmn->nature;
  float nature_boost = 1;
  if(nature->up==spatk){
    nature_boost = 1.1;
  } else if(nature->down==spatk){
    nature_boost = 0.9;
  }

  int spatk = get_true_stat(base, iv, ev, lvl, nature_boost);
  return spatk;
}

//calculate the effective special defense stat of a battler for use in damage calc
int get_true_spdef(battler_t* b){

  pkmn_t*  pkmn = b->pkmn;
  int ev = pkmn->spdef_ev;
  int iv = pkmn->spdef_iv;
  int lvl = pkmn->lvl;

  int base = pkmn->species->spdef_b;

  nature_t* nature = pkmn->nature;
  float nature_boost = 1;
  if(nature->up==spdef){
    nature_boost = 1.1;
  } else if(nature->down==spdef){
    nature_boost = 0.9;
  }

  int spdef = get_true_stat(base, iv, ev, lvl, nature_boost);
  return spdef;
}

//calculate the effective speed stat of a battler for use in move order 
int get_true_spd(battler_t* b){

  pkmn_t*  pkmn = b->pkmn;
  int ev = pkmn->spd_ev;
  int iv = pkmn->spd_iv;
  int lvl = pkmn->lvl;

  int base = pkmn->species->spd_b;

  nature_t* nature = pkmn->nature;
  float nature_boost = 1;
  if(nature->up==spd){
    nature_boost = 1.1;
  } else if(nature->down==spd){
    nature_boost = 0.9;
  }

  int spd = get_true_stat(base, iv, ev, lvl, nature_boost);
  return spd;
}

//return half of the damage scalar given the type of an attacking move
//and one of the types of a defending pokemon
//this is common knowledge but we reffered to https://pokemondb.net/type
float get_eff_scalar_single(elem_t atk_elem, elem_t def_elem){
  //do we need the break statements if we just return after every match?
  //should we keep them anyway just for convention?

  switch(atk_elem){

    case(normal):
      switch(def_elem){
        //immune
        case(ghost): return NOT_EFFECTIVE; break;
        //resist
        case(rock): return NOTVERY_EFFECTIVE; break;
        case(steel): return NOTVERY_EFFECTIVE; break;
        //effective
        default: return EFFECTIVE; break;
      }
      break;
    
    case(fire):
      switch(def_elem){
        //resist
        case(fire): return NOTVERY_EFFECTIVE; break;
        case(water): return NOTVERY_EFFECTIVE; break;
        case(rock): return NOTVERY_EFFECTIVE; break;
        case(dragon): return NOTVERY_EFFECTIVE; break;
        //weak
        case(grass): return SUPER_EFFECTIVE; break;
        case(bug): return SUPER_EFFECTIVE; break;
        case(ice): return SUPER_EFFECTIVE; break;
        //effective
        default: return EFFECTIVE; break;
      }
      break;

    case(water):
      switch(def_elem){
        //resist
        case(water): return NOTVERY_EFFECTIVE; break;
        case(grass): return NOTVERY_EFFECTIVE; break;
        case(dragon): return NOTVERY_EFFECTIVE; break;
        //weak
        case(fire): return SUPER_EFFECTIVE; break;
        case(ground): return SUPER_EFFECTIVE; break;
        case(rock): return SUPER_EFFECTIVE; break;
        //effective
        default: return EFFECTIVE; break;
      }
      break;
    
    case(electric):
      switch(def_elem){
        //immune
        case(ground): return NOT_EFFECTIVE; break;
        //resist
        case(electric): return NOTVERY_EFFECTIVE; break;
        case(grass): return NOTVERY_EFFECTIVE; break;
        case(dragon): return NOTVERY_EFFECTIVE; break;
        //weak
        case(water): return SUPER_EFFECTIVE; break;
        case(flying): return SUPER_EFFECTIVE; break;
        //effective
        default: return EFFECTIVE; break;
      }
      break;
    
    case(grass):
      switch(def_elem){
        //resist
        case(fire): return NOTVERY_EFFECTIVE; break;
        case(grass): return NOTVERY_EFFECTIVE; break;
        case(poison): return NOTVERY_EFFECTIVE; break;
        case(flying): return NOTVERY_EFFECTIVE; break;
        case(bug): return NOTVERY_EFFECTIVE; break;
        case(dragon): return NOTVERY_EFFECTIVE; break;
        case(steel): return NOTVERY_EFFECTIVE; break;
        //weak
        case(rock): return SUPER_EFFECTIVE; break;
        case(water): return SUPER_EFFECTIVE; break;
        case(ground): return SUPER_EFFECTIVE; break;
        //effective
        default: return EFFECTIVE; break;
      }
      break;

    case(ice):
      switch(def_elem){
        //resist
        case(fire): return NOTVERY_EFFECTIVE; break;
        case(water): return NOTVERY_EFFECTIVE; break;
        case(ice): return NOTVERY_EFFECTIVE; break;
        case(steel): return NOTVERY_EFFECTIVE; break;
        //weak
        case(grass): return SUPER_EFFECTIVE; break;
        case(ground): return SUPER_EFFECTIVE; break;
        case(flying): return SUPER_EFFECTIVE; break;
        case(dragon): return SUPER_EFFECTIVE; break;
        //effective
        default: return EFFECTIVE; break;
      }
      break;

    case(fighting):
      switch(def_elem){
        //resist
        case(poison): return NOTVERY_EFFECTIVE; break;
        case(flying): return NOTVERY_EFFECTIVE; break;
        case(psychic): return NOTVERY_EFFECTIVE; break;
        case(bug): return NOTVERY_EFFECTIVE; break;
        case(fairy): return NOTVERY_EFFECTIVE; break;
        //weak
        case(normal): return SUPER_EFFECTIVE; break;
        case(ice): return SUPER_EFFECTIVE; break;
        case(rock): return SUPER_EFFECTIVE; break;
        case(dark): return SUPER_EFFECTIVE; break;
        case(steel): return SUPER_EFFECTIVE; break;
        //effective
        default: return EFFECTIVE; break;
      }
      break;

    case(poison):
      switch(def_elem){
        //immune
        case(steel): return NOT_EFFECTIVE; break;
        //resist
        case(poison): return NOTVERY_EFFECTIVE; break;
        case(ground): return NOTVERY_EFFECTIVE; break;
        case(rock): return NOTVERY_EFFECTIVE; break;
        case(ghost): return NOTVERY_EFFECTIVE; break;
        //weak
        case(grass): return SUPER_EFFECTIVE; break;
        case(fairy): return SUPER_EFFECTIVE; break;
        //effective
        default: return EFFECTIVE; break;
      }
      break;

    case(ground):
      switch(def_elem){
        //immune
        case(flying): return NOT_EFFECTIVE; break;
        //resist
        case(bug): return NOTVERY_EFFECTIVE; break;
        case(grass): return NOTVERY_EFFECTIVE; break;
        //weak
        case(fire): return SUPER_EFFECTIVE; break;
        case(electric): return SUPER_EFFECTIVE; break;
        case(rock): return SUPER_EFFECTIVE; break;
        case(poison): return SUPER_EFFECTIVE; break;
        case(steel): return SUPER_EFFECTIVE; break;
        //effective
        default: return EFFECTIVE; break;
      }
      break;

    case(flying):
      switch(def_elem){
        //resist
        case(electric): return NOTVERY_EFFECTIVE; break;
        case(rock): return NOTVERY_EFFECTIVE; break;
        case(steel): return NOTVERY_EFFECTIVE; break;
        //weak
        case(grass): return SUPER_EFFECTIVE; break;
        case(fighting): return SUPER_EFFECTIVE; break;
        case(bug): return SUPER_EFFECTIVE; break;
        //effective
        default: return EFFECTIVE; break;
      }
      break;

    case(psychic):
      switch(def_elem){
        //immune
        case(dark): return NOT_EFFECTIVE; break;
        //resist
        case(psychic): return NOTVERY_EFFECTIVE; break;
        case(steel): return NOTVERY_EFFECTIVE; break;
        //weak
        case(fighting): return SUPER_EFFECTIVE; break;
        case(poison): return SUPER_EFFECTIVE; break;
        //effective
        default: return EFFECTIVE; break;
      }
      break;

    case(bug):
      switch(def_elem){
        //resist
        case(fire): return NOTVERY_EFFECTIVE; break;
        case(fighting): return NOTVERY_EFFECTIVE; break;
        case(poison): return NOTVERY_EFFECTIVE; break;
        case(flying): return NOTVERY_EFFECTIVE; break;
        case(ghost): return NOTVERY_EFFECTIVE; break;
        case(steel): return NOTVERY_EFFECTIVE; break;
        case(fairy): return NOTVERY_EFFECTIVE; break;
        //weak
        case(grass): return SUPER_EFFECTIVE; break;
        case(psychic): return SUPER_EFFECTIVE; break;
        case(dark): return SUPER_EFFECTIVE; break;
        //effective
        default: return EFFECTIVE; break;
      }
      break;

    case(rock):
      switch(def_elem){
        //resist
        case(fighting): return NOTVERY_EFFECTIVE; break;
        case(ground): return NOTVERY_EFFECTIVE; break;
        case(steel): return NOTVERY_EFFECTIVE; break;
        //weak
        case(fire): return SUPER_EFFECTIVE; break;
        case(ice): return SUPER_EFFECTIVE; break;
        case(flying): return SUPER_EFFECTIVE; break;
        case(bug): return SUPER_EFFECTIVE; break;
        //effective
        default: return EFFECTIVE; break;
      }
      break;

    case(ghost):
      switch(def_elem){
        //immune
        case(normal): return NOT_EFFECTIVE; break;
        //resist
        case(dark): return NOTVERY_EFFECTIVE; break;
        //weak
        case(psychic): return SUPER_EFFECTIVE; break;
        case(ghost): return SUPER_EFFECTIVE; break;
        //effective
        default: return EFFECTIVE; break;
      }
      break;

    case(dragon):
      switch(def_elem){
        //immune
        case(fairy): return NOT_EFFECTIVE; break;
        //resist
        case(steel): return NOTVERY_EFFECTIVE; break;
        //weak
        case(dragon): return SUPER_EFFECTIVE; break;
        //effective
        default: return EFFECTIVE; break;
      }
      break;

    case(dark):
      switch(def_elem){
        //resist
        case(fighting): return NOTVERY_EFFECTIVE; break;
        case(dark): return NOTVERY_EFFECTIVE; break;
        case(fairy): return NOTVERY_EFFECTIVE; break;
        //weak
        case(psychic): return SUPER_EFFECTIVE; break;
        case(ghost): return SUPER_EFFECTIVE; break;
        //effective
        default: return EFFECTIVE; break;
      }
      break;

    case(steel):
      switch(def_elem){
        //resist
        case(fire): return NOTVERY_EFFECTIVE; break;
        case(water): return NOTVERY_EFFECTIVE; break;
        case(electric): return NOTVERY_EFFECTIVE; break;
        case(steel): return NOTVERY_EFFECTIVE; break;
        //weak
        case(ice): return SUPER_EFFECTIVE; break;
        case(fairy): return SUPER_EFFECTIVE; break;
        case(rock): return SUPER_EFFECTIVE; break;
        //effective
        default: return EFFECTIVE; break;
      }
      break;

    case(fairy):
      switch(def_elem){
        //resist
        case(fire): return NOTVERY_EFFECTIVE; break;
        case(poison): return NOTVERY_EFFECTIVE; break;
        case(steel): return NOTVERY_EFFECTIVE; break;
        //weak
        case(fighting): return SUPER_EFFECTIVE; break;
        case(dark): return SUPER_EFFECTIVE; break;
        case(dragon): return SUPER_EFFECTIVE; break;
        //effective
        default: return EFFECTIVE; break;
      }
      break;

    //any other type
    //this is undefined behavior
    //note that this covers mystery, for which there is no damaging move
    default: return EFFECTIVE; break;
  }
}

//get type effectiveness scalar for two defending types given the attacking type
float get_eff_scalar_double(elem_t atk_elem, elem_t def_elem1, elem_t def_elem2){
  return get_eff_scalar_single(atk_elem, def_elem1) *
    get_eff_scalar_single(atk_elem, def_elem2);
}

//get the type effectivenss of a move on a battler
float get_eff(battler_t* b, move_t* m){
  elem_t atk_elem = m->elem;
  elem_t def_elem1 = b->pkmn->species->elem_prim;
  elem_t def_elem2 = b->pkmn->species->elem_sec;

  if(def_elem2){
    return get_eff_scalar_double(atk_elem, def_elem1, def_elem2);
  } else{
    return get_eff_scalar_single(atk_elem, def_elem1);
  }
}

//calculate damage done from one battler to another
int calc_dmg(battler_t* a, battler_t* d, move_t* mv, bool crit){
  int pwr = mv->pwr; 
  int atk_lvl = a->pkmn->lvl;

  //determine whether to use physical or special attack
  movecat_t cat = mv->cat;
  int atk_stat=0;
  int def_stat=0;
  if(cat==phys){
    atk_stat = get_true_atk(a);
    def_stat = get_true_def(d);
  } else if(cat==spec){
    atk_stat = get_true_spatk(a);
    def_stat = get_true_spdef(d);
  }

  //determine if the same-type attack bonus applies
  bool stab = false;
  elem_t a_elem_prim = a->pkmn->species->elem_prim;
  elem_t a_elem_sec = a->pkmn->species->elem_sec;
  elem_t mv_elem = mv->elem;
  if(a_elem_prim==mv_elem || a_elem_sec==mv_elem){
    stab=true;
  }

  float effective = get_eff(a, mv); //determine element/type effecitveness
  return dmg_formula(atk_lvl, pwr, atk_stat, def_stat, crit, stab, effective);
}
