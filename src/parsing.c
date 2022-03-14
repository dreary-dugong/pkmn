#define PCRE2_CODE_UNIT_WIDTH 8 //we're working in ascii (for now), so characters are 8 bytes 
#include <pcre2.h> //perl-compatible regular expressions

//create a pokemon from a string
//TODO: use static variables so we don't recompile the patterns on every call
//TODO: error handling
pkmn_t* read_pkmn(char* s){
  //pokemon showdown format. There's no official spec but the model implementation is here: 
  //https://github.com/smogon/pokemon-showdown-client/blob/master/js/storage.js#L1095 (as of 2/9/2022)
  //regex library documentation at https://www.pcre.org/current/doc/html/pcre2.html

  //our patterns
  PCRE2_SPTR long_pat;
  pcre2_code* long_re;
  PCRE2_SPTR ability_pat;
  pcre2_code* ability_re;
  PCRE2_SPTR shiny_pat; 
  pcre2_code* shiny_re;
  PCRE2_SPTR lvl_pat; 
  pcre2_code* lvl_re;
  PCRE2_SPTR happy_pat;
  pcre2_code* happy_re;
  PCRE2_SPTR ball_pat;
  pcre2_code* ball_re;
  PCRE2_SPTR hpower_pat;
  pcre2_code* hpower_re;
  PCRE2_SPTR gmax_pat;
  pcre2_code* gmax_re;
  PCRE2_SPTR ev_pat;
  pcre2_code* ev_re;
  PCRE2_SPTR nature_pat;
  pcre2_code* nature_re;
  PCRE2_SPTR iv_pat;
  pcre2_code* iv_re;
  PCRE2_SPTR mv_pat;
  pcre2_code* mv_re;

  //note that these include C escape sequences so they may not work if they're copy/pasted directly for testing
  long_pat = (PCRE2_SPTR) "^(([^@\\(]+) \\(([^@\\)]+)\\))|(^([^@\\(])+))( \\(([MF])\\))\?(( @ )(.+))\?$";
  ability_pat = (PCRE2_SPTR)"^(Ability|Trait): (.+)$";
  shiny_pat = (PCRE2_SPTR) "^Shiny: (Yes|No)$";
  lvl_pat = (PCRE2_SPTR) "^Level: (\\d+)$";
  happy_pat = (PCRE2_SPTR) "^Happiness: (\\d+)$";
  ball_pat = (PCRE2_SPTR) "^Pokeball: (.+)$";
  hpower_pat = (PCRE2_SPTR) "^Hidden Power: (.+)$";
  gmax_pat = (PCRE2_SPTR) "^Gigantamax: (Yes|No)$";
  ev_pat=(PCRE2_SPTR)"^EVs: ((\\d+) HP)\?( \\/ )\?((\\d+) Atk)\?( \\/ )\?((\\d+) Def)\?( \\/ )\?((\\d+) SpA)\?( \\/ )\?((\\d+) SpD)\?( \\/ )\?((\\d+) Spe)\?$";
  nature_pat = (PCRE2_SPTR) "^(.+) Nature$";
  iv_pat=(PCRE2_SPTR)"^IVs: ((\\d+) HP)\?( \\/ )\?((\\d+) Atk)\?( \\/ )\?((\\d+) Def)\?( \\/ )\?((\\d+) SpA)\?( \\/ )\?((\\d+) SpD)\?( \\/ )\?((\\d+) Spe)\?$";
  mv_pat = (PCRE2_SPTR) "^- (.+)$"; 

  //compile patterns
  int errno; //we don't expect compilation errors so we ignore these
  PCRE2_SIZE erroffset;

  long_re = pcre2_compile(long_pat,PCRE2_ZERO_TERMINATED,0,&errno,&erroffset,NULL);
  ability_re = pcre2_compile(ability_pat,PCRE2_ZERO_TERMINATED,0,&errno,&erroffset,NULL);
  shiny_re = pcre2_compile(shiny_pat,PCRE2_ZERO_TERMINATED,0,&errno,&erroffset,NULL);
  lvl_re = pcre2_compile(lvl_pat,PCRE2_ZERO_TERMINATED,0,&errno,&erroffset,NULL);
  happy_re = pcre2_compile(happy_pat,PCRE2_ZERO_TERMINATED,0,&errno,&erroffset,NULL);
  ball_re = pcre2_compile(ball_pat,PCRE2_ZERO_TERMINATED,0,&errno,&erroffset,NULL);
  hpower_re = pcre2_compile(hpower_pat,PCRE2_ZERO_TERMINATED,0,&errno,&erroffset,NULL);
  gmax_re = pcre2_compile(gmax_pat,PCRE2_ZERO_TERMINATED,0,&errno,&erroffset,NULL);
  ev_re = pcre2_compile(ev_pat,PCRE2_ZERO_TERMINATED,0,&errno,&erroffset,NULL);
  nature_re = pcre2_compile(nature_pat,PCRE2_ZERO_TERMINATED,0,&errno,&erroffset,NULL);
  iv_re = pcre2_compile(iv_pat,PCRE2_ZERO_TERMINATED,0,&errno,&erroffset,NULL);
  mv_re = pcre2_compile(mv_pat,PCRE2_ZERO_TERMINATED,0,&errno,&erroffset,NULL);

  //attributes of our pokemon that we can pull from patterns
  //TODO: do these need to be malloced since we pass them to other functions?
  //TODO: set first character to \0 in the optional ones
  char nickname[16]; //maximum legal nickname size is 12 characters
  char species[16]; //maximum legal species name should be 11 characters
  char item[32]; //maximum legal item name should be 16 but this is dubious https://bulbapedia.bulbagarden.net/wiki/List_of_items_by_name
  char gender; //M, F, or NULL
  char ability[32];  //maximum legal ability name should be 16 but this is dubious https://pokemondb.net/ability
  bool shiny; //true or false
  char lvl[4]; //maximum legal level is 100, or 255 technically? It's not clear
  char happiness[4]; //maximum legal happiness is 255
  char ball[16]; //maximum legal ball size is 12 https://nintendo.fandom.com/wiki/Pok%C3%A9_Ball
  char hpower[16]; //maximum legal hidden power type is 8 (electric)
  bool gmax; //true or false
  char hp_ev[4], atk_ev[4], def_ev[4], spatk_ev[4], spdef_ev[4], spd_ev[4]; //max ev is 255 per stat
  char nature[8]; //maximum nature length is 7 (adamant)
  char hp_iv[3], atk_iv[3], def_iv[3], spatk_iv[3], spdef_iv[3], spd_iv[3]; //max iv is 31 per stat
  char mv1[32], mv2[32], mv3[32], mv4[32]; //longest legal "normal" move is 16 (25 for z-move but we don't worry about that)


  //tokenize by line and match lines with regex
  int match_c; //integer match code that lets us know if it succeeded
  pcre2_match_data *match_data; //stores data from a match
  PCRE2_SIZE *ovector; //output vector stores substring offsets after a match
  PCRE2_SPTR curr_sub; //current subject to match, derived from currline
  int sub_len; //length of the current subject

  char* currline = strtok(s, "\n");
  do{

    curr_sub = (PCRE2_SPTR) currline;
    sub_len = strlen(currline);

    //long pattern match
    match_data= pcre2_match_data_create_from_pattern(long_re, NULL);
    match_c = pcre2_match(long_re, curr_sub, sub_len, 0, 0, match_data, NULL);

    //we have a match, so decode our capture groups
    if(match_c >= 0){
      ovector = pcre2_get_ovector_pointer(match_data);

      //if there's a nickname (if capture group 3 is nonempty)
      if(ovector[3*2]){
        int nn_start = ovector[3*2]; 
        int nn_len = ovector[3*2+1] - nn_start;
        memcpy(nickname, currline+nn_start, nn_len);
        *(nickname + nn_len) = '\0';

        int species_start = ovector[4*2];
        int species_len = ovector[4*2+1] - species_start;
        memcpy(species, currline+species_start, species_len);
        *(species + species_len) = '\0';

      } else{  //otherwise, we have species in group 5 and no nickname
        nickname[0] = '\0';

        int species_start = ovector[5*2];
        int species_len = ovector[5*2+1] - species_start;
        memcpy(species, currline+species_start, species_len);
        *(species + species_len) = '\0';
      }

      //if gender is given
      if(ovector[8*2]){
        int gender_index = ovector[8*2];
        gender = currline[gender_index];
      } else {
        gender = '\0';
      }

      //if item is given
      if(ovector[11*2]){
        int item_start = ovector[11*2];
        int item_len = ovector[11*2+1] - item_start;
        memcpy(item, currline+item_start, item_len);
        *(item + item_len) = '\0';
      }
      
      break; //pattern matched, move to next line

    }

    //ability pattern match
    match_data= pcre2_match_data_create_from_pattern(ability_re, NULL);
    match_c = pcre2_match(ability_re, curr_sub, sub_len, 0, 0, match_data, NULL);

    if(match_c >= 0){
      ovector = pcre2_get_ovector_pointer(match_data);

      //we got a match, so retrieve ability from the capture group
      int ability_start = ovector[1*2];
      int ability_len = ovector[1*2+1] - ability_start;
      memcpy(ability, currline+ability_start, ability_len);
      *(ability + ability_len) = '\0';

      break; //pattern matched, move to next line
    }

    //shiny pattern match
    match_data= pcre2_match_data_create_from_pattern(shiny_re, NULL);
    match_c = pcre2_match(shiny_re, curr_sub, sub_len, 0, 0, match_data, NULL);
    if(match_c >= 0){
      ovector = pcre2_get_ovector_pointer(match_data);

      //pull shiny response from capture group
      int shiny_start = ovector[1*2];
      int shiny_len = ovector[1*2+1] - shiny_start;
      char shiny_str[4];
      memcpy(shiny_str, currline+shiny_start, shiny_len);
      *(shiny_str+shiny_len) = '\0';

      shiny = (strcmp(shiny_str, "Yes")==0);

      break; //pattern matched, move to next line
    }

    //level pattern match
    match_data= pcre2_match_data_create_from_pattern(lvl_re, NULL);
    match_c = pcre2_match(lvl_re, curr_sub, sub_len, 0, 0, match_data, NULL);

    if(match_c >= 0){
      ovector = pcre2_get_ovector_pointer(match_data);

      //we got a match, so retrieve level from the capture group
      int lvl_start = ovector[1*2];
      int lvl_len = ovector[1*2+1] - lvl_start;
      memcpy(lvl, currline+lvl_start, lvl_len);
      *(lvl + lvl_len) = '\0';

      break; //pattern matched, move to next line
    }

    //happiness pattern match
    match_data= pcre2_match_data_create_from_pattern(happy_re, NULL);
    match_c = pcre2_match(happy_re, curr_sub, sub_len, 0, 0, match_data, NULL);

    if(match_c >= 0){
      ovector = pcre2_get_ovector_pointer(match_data);

      //we got a match, so retrieve happiness from the capture group
      int happiness_start = ovector[1*2];
      int happiness_len = ovector[1*2+1] - happiness_start;
      memcpy(happiness, currline+happiness_start, happiness_len);
      *(happiness + happiness_len) = '\0';

      break; //pattern matched, move to next line
    }

    //ball pattern match
    match_data= pcre2_match_data_create_from_pattern(ball_re, NULL);
    match_c = pcre2_match(ball_re, curr_sub, sub_len, 0, 0, match_data, NULL);

    if(match_c >= 0){
      ovector = pcre2_get_ovector_pointer(match_data);

      //we got a match, so retrieve ball from the capture group
      int ball_start = ovector[1*2];
      int ball_len = ovector[1*2+1] - ball_start;
      memcpy(ball, currline+ball_start, ball_len);
      *(ball + ball_len) = '\0';

      break; //pattern matched, move to next line
    }

    //hidden power pattern match
    match_data= pcre2_match_data_create_from_pattern(hpower_re, NULL);
    match_c = pcre2_match(hpower_re, curr_sub, sub_len, 0, 0, match_data, NULL);

    if(match_c >= 0){
      ovector = pcre2_get_ovector_pointer(match_data);

      //we got a match, so retrieve hidden power from the capture group
      int hpower_start = ovector[1*2];
      int hpower_len = ovector[1*2+1] - hpower_start;
      memcpy(hpower, currline+hpower_start, hpower_len);
      *(hpower + hpower_len) = '\0';

      break; //pattern matched, move to next line
    }

    //gigantamax pattern match
    match_data= pcre2_match_data_create_from_pattern(gmax_re, NULL);
    match_c = pcre2_match(gmax_re, curr_sub, sub_len, 0, 0, match_data, NULL);
    if(match_c >= 0){
      ovector = pcre2_get_ovector_pointer(match_data);

      //pull gmax response from capture group
      int gmax_start = ovector[1*2];
      int gmax_len = ovector[1*2+1] - gmax_start;
      char gmax_str[4];
      memcpy(gmax_str, currline+gmax_start, gmax_len);
      *(gmax_str+gmax_len) = '\0';

      gmax = (strcmp(gmax_str, "Yes")==0);

      break; //pattern matched, move to next line
    }

    //nature pattern match
    match_data= pcre2_match_data_create_from_pattern(nature_re, NULL);
    match_c = pcre2_match(nature_re, curr_sub, sub_len, 0, 0, match_data, NULL);

    if(match_c >= 0){
      ovector = pcre2_get_ovector_pointer(match_data);

      //we got a match, so retrieve nature from the capture group
      int nature_start = ovector[1*2];
      int nature_len = ovector[1*2+1] - nature_start;
      memcpy(nature, currline+nature_start, nature_len);
      *(nature + nature_len) = '\0';

      break; //pattern matched, move to next line
    }

    //ev pattern match
    //note we assume evs are in order
    match_data= pcre2_match_data_create_from_pattern(ev_re, NULL);
    match_c = pcre2_match(ev_re, curr_sub, sub_len, 0, 0, match_data, NULL);

    if(match_c >= 0){
      ovector = pcre2_get_ovector_pointer(match_data);

      //retrieve each stat if it exists
      //hit points
      if(ovector[2*2]){
        int hp_start = ovector[2*2];
        int hp_len = ovector[2*2+1] - hp_start;
        memcpy(hp_ev, currline+hp_start, hp_len);
        *(hp_ev + hp_len) = '\0';
      }
      //attack
      if(ovector[5*2]){
        int atk_start = ovector[5*2];
        int atk_len = ovector[5*2+1] - atk_start;
        memcpy(atk_ev, currline+atk_start, atk_len);
        *(atk_ev + atk_len) = '\0';
      }
      //defense 
      if(ovector[8*2]){
        int def_start = ovector[8*2];
        int def_len = ovector[8*2+1] - def_start;
        memcpy(def_ev, currline+def_start, def_len);
        *(def_ev + def_len) = '\0';
      }
      //special attack
      if(ovector[11*2]){
        int spatk_start = ovector[11*2];
        int spatk_len = ovector[11*2+1] - spatk_start;
        memcpy(spatk_ev, currline+spatk_start, spatk_len);
        *(spatk_ev + spatk_len) = '\0';
      }
      //special defense
      if(ovector[14*2]){
        int spdef_start = ovector[14*2];
        int spdef_len = ovector[14*2+1] - spdef_start;
        memcpy(spdef_ev, currline+spdef_start, spdef_len);
        *(spdef_ev + spdef_len) = '\0';
      }
      //speed
      if(ovector[17*2]){
        int spd_start = ovector[17*2];
        int spd_len = ovector[17*2+1] - spd_start;
        memcpy(spd_ev, currline+spd_start, spd_len);
        *(spd_ev + spd_len) = '\0';
      }

      break; //pattern matched, move to next line
    }





    currline = strtok(NULL, "\n");
  }while(currline);

  //free our patterns
  pcre2_code_free(long_re);
  pcre2_code_free(ability_re);
  pcre2_code_free(shiny_re);
  pcre2_code_free(lvl_re);
  pcre2_code_free(happy_re);
  pcre2_code_free(ball_re);
  pcre2_code_free(hpower_re);
  pcre2_code_free(gmax_re);
  pcre2_code_free(ev_re);
  pcre2_code_free(nature_re);
  pcre2_code_free(iv_re);
  pcre2_code_free(mv_re);

  return NULL; //temporary, just so it compiles
}

//create a pkmn team from a file
//TODO: so much error handling
pkmn_tm_t* read_team(char* f){
  const int BUFF_SIZE = 2048; //this should be plenty of space for an ascii team
                              //
  //read in the whole file
  char buff[BUFF_SIZE];
  FILE* fd = fopen(f, "r");
  fgets(buff, BUFF_SIZE, fd);
  fclose(fd);

  //do this so I can loop. This feels wrong. Is there a better way?
  pkmn_t *p1,*p2,*p3,*p4,*p5,*p6;
  pkmn_t** argads[] = {&p1,&p2,&p3,&p4,&p5,&p6};

  //tokenize pokemon delimited by \n\n
  int pkmn_count = 0;
  char* pkmn_str;
  pkmn_str = strtok(buff, "\n\n");
  do{
    *(argads[pkmn_count]) = read_pkmn(pkmn_str);
    pkmn_str = strtok(NULL, "\n\n");
  } while(pkmn_str && pkmn_count++<6);

  //if there weren't 6, set the remaining to null
  if(pkmn_count < 6){
    for(int i=pkmn_count;i<6;i++){
      *(argads[i]) = NULL;
    }
  }

  return make_team(p1,p2,p3,p4,p5,p6);

  return NULL;
}
