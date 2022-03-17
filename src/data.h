#ifndef DATA_H_
#define DATA_H_

/* 
 * =======================
 *enumerated types 
 * =======================
 */

//element/type of a species or move
typedef enum element{
  normal,
  fighting,
  poison,
  fairy,
  fire,
  water,
  grass,
  bug,
  dark,
  ghost,
  flying,
  dragon,
  steel,
  rock,
  ice,
  psychic,
  ground,
  electric,
  mystery //aka ???
} elem_t;

//a pokemon stat. This is used for natures. 
typedef enum stat{
  hp, //hit points
  atk, //attack
  def, //defense
  spatk, //special attack
  spdef, //special defense 
  spd //speed
} stat_t;

//the category for a move, used in damage calculation
typedef enum move_category{
  phys, //physical, uses attack/defense
  spec //special, uses sp attack/sp defense
} movecat_t;



/*
 * =====================================
 * structs
 * =====================================
 */

//a damaging move
typedef struct move{
  int pwr; //power
  int acc; //accuracy

  elem_t elem; //element/type 
  movecat_t cat; //category

  char* name;
} move_t;

//the permanent nature of a pkmn, used in stat calculation
typedef struct nature{
  stat_t up; //which stat is increased by 10%
  stat_t down; //which stat is decreased by 10%

  char* name;
} nature_t;

//a species of pokemon
typedef struct species{

  //"base" stats inherent to a species, unchangeable
  int hp_b;
  int atk_b;
  int def_b;
  int spatk_b;
  int spdef_b;
  int spd_b;

  elem_t elem_prim; //primary element/type, must be defined
  elem_t elem_sec; //secondary element/type, may be null

  int dexno; //pokedex number, unique to each species mostly
  char* name;
} species_t;

//an individual pokemon
typedef struct pkmn{

  //"individual values", mostly unchangeable
  //range from 0 to 31
  int hp_iv;
  int atk_iv;
  int def_iv;
  int spatk_iv;
  int spdef_iv;
  int spd_iv;

  //"effort values", mostly changeable
  //total of 512, max 255 per stat
  int hp_ev;
  int atk_ev;
  int def_ev;
  int spatk_ev;
  int spdef_ev;
  int spd_ev;

  int lvl; //level

  nature_t* nature;

  move_t* mv1;
  move_t* mv2;
  move_t* mv3;
  move_t* mv4;

  species_t* species;
} pkmn_t;

//an individual pokemon in battle
typedef struct battler{

  //stat stages
  //these may change in a battle due to abilities and moves
  int atk_stg;
  int def_stg;
  int spatk_stg;
  int spdef_stg;
  int spd_stg;

  //accuracy and evasivenss stages. 
  //These stats only appear in battle
  int acc_stg;
  int eva_stg;

  int hp_curr; //current hp, ranges from 0 to this battler's max hp

  pkmn_t* pkmn; 
} battler_t;

//a team of (up to) 6 pokemon
typedef struct pkmn_team{
  pkmn_t* team[6];
} pkmn_tm_t;

//constructors
move_t* make_move(movecat_t cat, elem_t elem, int pwr, int acc, char* name);
species_t* make_species(char* name, elem_t elem_prim, elem_t elem_sec, 
    int hp_b, int atk_b, int def_b, int spatk_b, int spdef_b, int spd_b, 
    int dexno);
pkmn_t* make_pkmn(species_t* spec, nature_t* nature, int lvl, move_t* mv1, 
    move_t* mv2, move_t* mv3, move_t* mv4, int hp_iv, int atk_iv, int def_iv, 
    int spatk_iv, int spdef_iv, int spd_iv, int hp_ev, int atk_ev, int def_ev, 
    int spatk_ev, int spdef_ev, int spd_ev);
battler_t* make_battler(pkmn_t* p);
nature_t* make_nature(char* name, stat_t up, stat_t down);
pkmn_tm_t* make_team(pkmn_t* p1, pkmn_t* p2, pkmn_t* p3, pkmn_t* p4, 
    pkmn_t* p5, pkmn_t* p6);

//destructors
void free_move(move_t* m);
void free_species(species_t* spec);
void free_pkmn(pkmn_t* p);
void free_battler(battler_t* b);
void free_nature(nature_t* n);
void free_team(pkmn_tm_t* tm);

//debug string functions
char* get_elem_name(elem_t elem);
void print_move(move_t* m);
void print_species(species_t* spec);
void print_pkmn_short(pkmn_t* p);
void print_team(pkmn_tm_t* tm);

//helper conversion methods
elem_t str_to_elem(char* s);
stat_t str_to_stat(char* s);
movecat_t str_to_movecat(char* s);

#endif
