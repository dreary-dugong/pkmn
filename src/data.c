//external libraries
#include <stdio.h> //printf
#include <stdlib.h> //NULL, rand, srand
#include <stdbool.h> //boolean type
#include <math.h> //floor
#include <string.h> //strlen
#include <bsd/string.h> //strlcpy
#include <time.h> //time

//project headers
#include "data.h"
#include "battle.h"
#include "calc.h"


/*
 * ===================================
 * constructors
 * ===================================
 */

//make a move struct
//TODO:other kinds of moves
move_t* make_move(movecat_t cat, elem_t elem, int pwr, int acc, char* name){
  char* name_ptr = malloc(strlen(name)+1);
  strlcpy(name_ptr, name, sizeof(name_ptr)+1);

  move_t* m = malloc(sizeof(move_t));
  m->cat=cat;
  m->pwr=pwr;
  m->acc=acc;
  m->elem=elem;
  m->name=name_ptr;

  return m;
}

//make a species struct
species_t* make_species(char* name, elem_t elem_prim, elem_t elem_sec, int hp_b, int atk_b, int def_b, int spatk_b, int spdef_b, int spd_b, int dexno){
    //maybe change these arguments to an array
  
  char* name_ptr = malloc(strlen(name)+1);
  strlcpy(name_ptr, name, sizeof(name_ptr)+1);

  species_t* spec = malloc(sizeof(species_t));
  spec->name = name_ptr;
  spec->dexno = dexno;

  spec->elem_prim = elem_prim;
  spec->elem_sec = elem_sec;

  spec->hp_b = hp_b;
  spec->atk_b = atk_b;
  spec->def_b = def_b;
  spec->spatk_b = spatk_b;
  spec->spdef_b = spdef_b;
  spec->spd_b = spd_b;

  return spec;
}

//make a pkmn struct
//TODO: ability, nickname, item, gender
pkmn_t* make_pkmn(species_t* spec, nature_t* nature, int lvl, move_t* mv1, move_t* mv2, move_t* mv3, move_t* mv4,
    int hp_iv, int atk_iv, int def_iv, int spatk_iv, int spdef_iv, int spd_iv,
    int hp_ev, int atk_ev, int def_ev, int spatk_ev, int spdef_ev, int spd_ev){
  //maybe I should use arrays for IVs and EVs? These argumetns are unwieldy

  pkmn_t* p = malloc(sizeof(pkmn_t));
  p->species = spec;

  p->nature=nature;
  p->lvl=lvl;

  p->mv1 = mv1;
  p->mv2 = mv2;
  p->mv3 = mv3;
  p->mv4 = mv4;

  p->hp_iv=hp_iv;
  p->atk_iv=atk_iv;
  p->spatk_iv=spatk_iv;
  p->def_iv=def_iv;
  p->spdef_iv=spdef_iv;
  p->spd_iv=spd_iv;

  p->hp_ev=hp_ev;
  p->atk_ev=atk_ev;
  p->spatk_ev=spatk_ev;
  p->def_ev=def_ev;
  p->spdef_ev=spdef_ev;
  p->spd_ev=spd_ev;

  return p;
}

//make a battler struct
battler_t* make_battler(pkmn_t* p){
  battler_t* b = malloc(sizeof(battler_t));

  b->atk_stg = 0;
  b->def_stg = 0;
  b->spatk_stg = 0;
  b->spdef_stg = 0;
  b->spd_stg = 0;

  b->acc_stg = 0;
  b->eva_stg = 0;
  
  b->pkmn = p;
  
  b->hp_curr = get_max_hp(b); //this MUST come after pkmn

  return b;
}

//make a nature struct
nature_t* make_nature(char* name, stat_t up, stat_t down){
  nature_t* n = malloc(sizeof(nature_t));

  char* naturename = malloc(strlen(name)+1);
  strlcpy(naturename, name, sizeof(naturename));
  n->name = naturename;

  n->up = up;
  n->down = down;

  return n;
}

//create a pkmn team
pkmn_tm_t* make_team(pkmn_t* p1, pkmn_t* p2, pkmn_t* p3, pkmn_t* p4, pkmn_t* p5, pkmn_t* p6){
  pkmn_tm_t* team = malloc(sizeof(pkmn_tm_t));
  memcpy(team->team, (pkmn_t*[6]){p1,p2,p3,p4,p5,p6}, sizeof(pkmn_t*)*6); //clang wants this to be safer. Can it be?
  return team;
}

/*
 * ================================
 * destructors
 * ================================
 */

//free a move struct
//TODO: other kinds of moves?
void free_move(move_t* m){
  free(m->name);
  free(m);
}

//free a species struct
void free_species(species_t* spec){
  free(spec->name);
  free(spec);
}

//free a pkmn struct
//TODO: nickname
void free_pkmn(pkmn_t* p){
  free(p);
}

//free a battler struct
void free_battler(battler_t* b){
  free(b);
}

//free a nature struct
void free_nature(nature_t* n){
  free(n->name);
  free(n);
}

//free a pkmn team
void free_team(pkmn_tm_t* tm){
  free(tm);
}

/*
 * ================================
 * debug strings
 * ================================
 */

//return the string name of an element/type for use in printing
char* get_elem_name(elem_t elem){
  static char output[9];  //needs to be static because we can't return an array
                          //this can be a problem if we multithread ever
                        
  switch(elem){
    case(normal): strlcpy(output,"normal",sizeof(output)); break;
    case(fire): strlcpy(output,"fire",sizeof(output)); break;
    case(grass): strlcpy(output,"grass",sizeof(output)); break;
    case(water): strlcpy(output,"water",sizeof(output)); break;
    case(electric): strlcpy(output,"electric",sizeof(output)); break;
    case(rock): strlcpy(output,"rock",sizeof(output)); break;
    case(ground): strlcpy(output,"ground",sizeof(output)); break;
    case(steel): strlcpy(output,"steel",sizeof(output)); break;
    case(fairy): strlcpy(output,"fairy",sizeof(output)); break;
    case(psychic): strlcpy(output,"psychic",sizeof(output)); break;
    case(ghost): strlcpy(output,"ghost",sizeof(output)); break;
    case(dark): strlcpy(output,"dark",sizeof(output)); break;
    case(fighting): strlcpy(output,"fighting",sizeof(output)); break;
    case(ice): strlcpy(output,"ice",sizeof(output)); break;
    case(dragon): strlcpy(output,"dragon",sizeof(output)); break;
    case(bug): strlcpy(output,"bug",sizeof(output)); break;
    case(poison): strlcpy(output,"poison",sizeof(output)); break;
    case(flying): strlcpy(output,"flying",sizeof(output)); break;
    case(mystery): strlcpy(output,"???",sizeof(output)); break;
    default: strlcpy(output, "UNDEFINED", sizeof(output)); break;
  }

  return output;
}

//print a move struct for debugging
void print_move(move_t* m){
  printf("Name: %s\n", m->name);
  printf("Type: %s\n", get_elem_name(m->elem));
  if(m->cat==phys){
    printf("Category: physical\n");
  } else if(m->cat==spec){
    printf("Category: special\n");
  }
  printf("Power: %d\n", m->pwr);
  printf("Accuracy: %d\n", m->acc);
}

//print information about a species struct for debugging
void print_species(species_t* spec){
  printf("Species Name: %s\n", spec->name);
  printf("Pokedex Number: %d\n", spec->dexno);
  printf("\tBase hp: %d\n", spec->hp_b);
  printf("\tBase attack: %d\n", spec->atk_b);
  printf("\tBase defense: %d\n", spec->def_b);
  printf("\tBase sp. attack: %d\n", spec->spatk_b);
  printf("\tBase sp. defense: %d\n", spec->spdef_b);
  printf("\tBase speed: %d\n", spec->spd_b);
}

//print debug info about a pkmn struct in Pokemon Showdown team builder format
//TODO: ability, nickname, item, gender
void print_pkmn_short(pkmn_t* p){
  char* spec_name = p->species->name;
  printf("%s\n", spec_name);

  int lvl = p->lvl;
  printf("Level: %d\n", lvl);

  printf("%s Nature\n", p->nature->name);

  //evs
  printf("EVs: ");
  bool evsWritten = false;
  if(p->hp_ev > 0){
    printf("%d HP", p->hp_ev);
    evsWritten = true;
  }
  if(p->atk_ev > 0){
    if(evsWritten) printf(" / ");
    printf("%d Atk", p->atk_ev);
    evsWritten = true;
  }
  if(p->def_ev > 0){
    if(evsWritten) printf(" / ");
    printf("%d Def", p->def_ev);
    evsWritten = true;
  }
  if(p->spatk_ev > 0){
    if(evsWritten) printf(" / ");
    printf("%d SpA", p->spatk_ev);
    evsWritten = true;
  }
  if(p->spdef_ev > 0){
    if(evsWritten) printf(" / ");
    printf("%d SpD", p->spdef_ev);
    evsWritten = true;
  }
  if(p->spd_ev > 0){
    if(evsWritten) printf(" / ");
    printf("%d Spd", p->spd_ev);
  }
  printf("\n");

  //moves
  if(p->mv1 != NULL){
    printf("- %s\n", p->mv1->name);
  }
  if(p->mv2 != NULL){
    printf("- %s\n", p->mv2->name);
  }
  if(p->mv3 != NULL){
    printf("- %s\n", p->mv3->name);
  }
  if(p->mv4 != NULL){
    printf("- %s\n", p->mv4->name);
  }

}

//print a team of pokemon for debugging
void print_team(pkmn_tm_t* tm){
  printf("Team:\n");
  for(int i=0;i<6;i++){
    printf("%s\n", tm->team[i]->species->name);
  }
}

