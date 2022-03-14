#include <stdio.h> //printf
#include <stdlib.h> //NULL, rand, srand
#include <time.h> //time
#include <stdbool.h>

//project headers
#include "battle.h"
#include "data.h"

//TODO: change types to minimum memory amount
//TODO: features to add include: nicknames, genders, items, abilities, forms, parsing teams, database lookup, weather, terrain, hazards, statuses,
//move edge cases, stat effect moves, trick room/wonder room, battle turn simulation, stretch goal: graphics, seconary move effects, alternative forms,
//probably more

//currently: make 2 garchomps and have one deal damage to the other
//used for debugging structs and damage calculation
int main(){
  srand((unsigned)time(NULL));
  species_t* gar = make_species("garchomp",dragon,ground,108,130,95,80,85,102, 445);

  move_t* outrage = make_move(phys, dragon, 120, 100, "outrage");
  print_move(outrage);
  printf("\n");

  nature_t* adamant = make_nature("adamant", atk, spatk);
  
  pkmn_t* a_pkmn = make_pkmn(gar, adamant, 100, outrage, outrage, outrage, outrage, 
      31,31,31,31,31,31, 10,252,0,0,0,252);
  pkmn_t* d_pkmn = make_pkmn(gar, adamant, 100, outrage, outrage, outrage, outrage, 
      31,31,31,31,31,31, 10,252,0,0,0,252);

  print_pkmn_short(a_pkmn);
  printf("\n");
  print_pkmn_short(d_pkmn);
  printf("\n");

  battler_t* a_bat = make_battler(a_pkmn);
  battler_t* d_bat = make_battler(d_pkmn);

  printf("\nDamage: %d\n", calc_dmg(a_bat, d_bat, outrage, false));


  free_battler(a_bat);
  free_battler(d_bat);
  free_pkmn(a_pkmn);
  free_pkmn(d_pkmn);
  free_nature(adamant);
  free_move(outrage);
  free_species(gar);
}




