char possible_characters[SIZE];

    // check the possible characters for this index
    for (int i = 0; i < strlen(next_tiles); i++){
        if (next_tiles[i] == 'g') {
            strncpy(possible_characters, next_tiles[i], strlen(next_tiles[i]));
        }
    }

    // check the character cannot be in the index
    char result_characters[SIZE];

    if (strchr(cur_tiles, 'g') != NULL) {
        // there are green tiles in the current tiles
        if (cur_tiles[index] == 'g') {  
            for (int j = 0; j < strlen(possible_characters); j++ ){
                if (possible_characters[j] == cur_tiles[index]){
                    memmove(&possible_characters[j], &possible_characters[j+1], strlen(possible_characters) - 1);
                }
            }
        }
        strncpy(con->must_be[index], possible_characters, strlen(possible_characters));
    }
    else if (strchr(cur_tiles, 'y') != NULL){        
        // there are green tiles in the current tiles
        if (next_tiles[index] == 'g') {  
            for (int k = 0; k < strlen(possible_characters); k++ ){ 
                if (possible_characters[k] == next_tiles[index]){
                    memmove(&possible_characters[k], &possible_characters[k+1], strlen(possible_characters) - 1);
                }
            }
        }
        strncpy(con->must_be[index], possible_characters, strlen(possible_characters));
    }
    else{
        if (strchr(next_tiles, 'y') != NULL){
            for (int m = 0; m < index; m++){
                if (next_tiles[m] == 'y'){
                   strncpy(possible_characters, next_tiles[m], strlen(next_tiles[m]));
                }
            }
            for (int m = index+1; m < strlen(next_tiles); m++ ){
                if (next_tiles[m] == 'y'){
                   strncpy(possible_characters, next_tiles[m], strlen(next_tiles[m]));
                }
            }   
        }
        strncpy(con->must_be[index], possible_characters, strlen(possible_characters));
    }








        char possible_characters[WORDLEN];

    for (int i = 0; i < strlen(next_tiles); i++){
        if (next_tiles[i] == 'g' && strchr(possible_characters, word[i]) == NULL) {
            strncpy(possible_characters, &word[i], strlen(&word[i]));
        }
    }

    if ((cur_tiles[index] == 'y') && (next_tiles[index] = '-')){
        for (int i = 0; i < strlen(cur_tiles), i++;){
            if (cur_tiles[i] == 'g'){
                removeChar(possible_characters, word[i]);
            }
            else{
                for (int j = 0; j < index; j++) {
                    if (next_tiles[j] == 'y'){
                        strcat(possible_characters, &next_tiles[j]);
                    }
                }
                for (int j = index+1; j < strlen(next_tiles); j++) {
                    if (next_tiles[j] == 'y'){
                        strcat(possible_characters, &next_tiles[j]);
                    }
                }
            }
        }
        strcat(con->must_be[index], possible_characters);
    }

    if ((cur_tiles[index] == 'y') && (next_tiles[index] = 'g')){
        for (int i = 0; i < strlen(cur_tiles), i++;){
            if (strchr(cur_tiles, 'g') == NULL) {
                removeChar(possible_characters, word[index]);
            }
            else{
                continue;
            }
        }
        strcat(con->must_be[index], possible_characters);
    }



    void removeChar(char *str, char garbage) {

    char *src, *dst;
    for (src = dst = str; *src != '\0'; src++) {
        *dst = *src;
        if (*dst != garbage) dst++;
    }
    *dst = '\0';
}


if (i != index && (next_tiles[i] == 'g') && (cur_tiles[i] != 'g')){
            con->must_be[index][strlen(con->must_be[index])] = word[i];
            con->must_be[index][strlen(con->must_be[index])] = '\0';
        } 
        if (i != index && (next_tiles[i] == 'y') && (cur_tiles[i] != 'y')){
            con->must_be[index][strlen(con->must_be[index])] = word[i];
            con->must_be[index][strlen(con->must_be[index])] = '\0';
        }