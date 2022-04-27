#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wordle.h"
#include "constraints.h"

/* Read the wordle grid and solution from fp. 
 * Return a pointer to a wordle struct.
 * See sample files for the format. Assume the input file has the correct
 * format.  In other words, the word on each is the correct length, the 
 * words are lower-case letters, and the line ending is either '\n' (Linux,
 * Mac, WSL) or '\r\n' (Windows)
 */
struct wordle *create_wordle(FILE *fp) {
    struct wordle *w = malloc(sizeof(struct wordle));
    char line[MAXLINE]; 
    w->num_rows = 0;

    while(fgets(line, MAXLINE, fp ) != NULL) {
        
        // remove the newline character(s) 
        char *ptr;
        if(((ptr = strchr(line, '\r')) != NULL) ||
           ((ptr = strchr(line, '\n')) != NULL)) {
            *ptr = '\0';
        }
        
        strncpy(w->grid[w->num_rows], line, SIZE);
        w->grid[w->num_rows][SIZE - 1] = '\0';
        w->num_rows++;
    }
    return w;
}


/* Create a solver_node and return it.
 * If con is not NULL, copy con into dynamically allocated space in the struct
 * If con is NULL set the new solver_node con field to NULL.
 * Tip: struct assignment makes copying con a one-line statements
 */
struct solver_node *create_solver_node(struct constraints *con, char *word) {
    // malloc the memory for the new solver node
    struct solver_node * solver_node = malloc(sizeof(struct solver_node));
    
    if (con != NULL) {
        // set the new solver node to NULL
        solver_node->con = init_constraints();
        memcpy(solver_node->con, con, sizeof(struct constraints));
        for (int i = 0; i < WORDLEN; i++){
            strncpy(solver_node->con->must_be[i], con->must_be[i], strlen(con->must_be[i]));
        }
        for (int j = 0; j < ALPHABET_SIZE; j++){
            solver_node->con->cannot_be[j] = con->cannot_be[j];
        }
    }    
    else {
        solver_node->con = NULL;
    }

    // update the word
    strncpy(solver_node->word, word, strlen(word));
    solver_node->word[strlen(word)] = '\0';

    // update the child list and the sibling
    solver_node->next_sibling = NULL;
    solver_node->child_list = NULL;

    return solver_node;
}

/* Return 1 if "word" matches the constraints in "con" for the wordle "w".
 * Return 0 if it does not match
 */
int match_constraints(char *word, struct constraints *con, 
struct wordle *w, int row) {
    // for each index in word
    for (int i = 0; i < WORDLEN; i++) {
        // the letter is not the same as the word in the yellow index
        if (w->grid[0][i] == word[i] && w->grid[row][i] == 'y' && con->must_be[i][0] != '\0') {
            return 0;
        }
        // check if the word[index] is in the must_be[index] string
        if (strchr(con->must_be[i], word[i]) == NULL && con->must_be[i][0] != '\0') {
            // printf("0_1\n");
            return 0;
        }
        // if must_be[index] is the empty string and word[index] is in the cannot_be
        if (con->must_be[i][0] == '\0' && con->cannot_be[word[i] - 'a'] == 1) {
            // printf("0_2\n");
            return 0;
        }            
    }
    // printf("return 1\n");
    return 1;
}

/* remove "letter" from "word"
 * "word" remains the same if "letter" is not in "word"
 */
void remove_char(char *word, char letter) {
    char *ptr = strchr(word, letter);
    if(ptr != NULL) {
        *ptr = word[strlen(word) - 1];
        word[strlen(word) - 1] = '\0';
    }
}

/* Build a tree starting at "row" in the wordle "w". 
 * Use the "parent" constraints to set up the constraints for this node
 * of the tree
 * For each word in "dict", 
 *    - if a word matches the constraints, then 
 *        - create a copy of the constraints for the child node and update
 *          the constraints with the new information.
 *        - add the word to the child_list of the current solver node
 *        - call solve_subtree on newly created subtree
 */

void solve_subtree(int row, struct wordle *w,  struct node *dict, 
                   struct solver_node *parent) {

    if(verbose) {
        printf("Running solve_subtree: %d, %s\n", row, parent->word);
    }

    // base case
    if (w->num_rows == row) {
        return;
    }

    //detector that updates the constraints
    for (int i = 0; i < WORDLEN; i++){
        // green cases
        if(w->grid[row][i] ==  'g'){
            set_green(parent->word[i], i, parent->con);
            // printf("green\n");    
        }
        // yellow cases
        else if(w->grid[row][i] ==  'y'){
            // case "ggggg" word, first row is the solution
            if (row == 1){
                set_yellow(i, w->grid[row], "ggggg", parent->word, parent->con);
                // printf("yellow_1\n");
            }
            // other yellow cases
            else {
                set_yellow(i, w->grid[row], w->grid[row-1], parent->word, parent->con);
                // printf("yellow_2\n");
            }   
        }
    }   
    // other cases, add cannot_be
    add_to_cannot_be(parent->word, parent->con);
    // printf("cannot\n");
        
    
    

    // printf("node success\n");
    struct node *temp = dict; 
    while (temp != NULL) {
        // printf("check done\n");
        if(match_constraints(temp->word, parent->con, w, row) == 1){
            
            // create the child list solver node
            struct solver_node * child_node = create_solver_node(parent->con, temp->word);
 
            // connect parent to the child 
            if (parent->child_list == NULL){
                parent->child_list = child_node;
                // printf("child connected\n");    
            }
            
            else {
                // if the solver_node meet the constraints more than one time
                // connect the child to the sibling
                // printf("problem?\n");
                // struct solver_node *curr = parent->child_list->next_sibling;
                // while (curr->next_sibling != NULL){
                //     curr->next_sibling = child_node;
                //     curr = curr->next_sibling;
                // }
                struct solver_node *curr = parent->child_list->next_sibling;
                parent->child_list->next_sibling = child_node;
                parent->child_list->next_sibling->next_sibling = curr;
                // printf("sibling connected\n");   
            }
            // recursions
            solve_subtree(row+1, w, dict, child_node);    
        }
        // else if(match_constraints(temp->word, parent->con, w, row) == 0){
        temp = temp->next;
        // }
    }    
    // debugging suggestion, but you can choose how to use the verbose option
    /*if(verbose) {
        print_constraints(c);
    } */

    // TODO
}

/* Print to standard output all paths that are num_rows in length.
 * - node is the current node for processing
 * - path is used to hold the words on the path while traversing the tree.
 * - level is the current length of the path so far.
 * - num_rows is the full length of the paths to print
 */

void print_paths(struct solver_node *node, char **path, 
                 int level, int num_rows) {
    // TODO
    if(level >= num_rows){
        for(int i = 0; i < num_rows; i++){ 
            printf("%s ", path[i]);
        }
        printf("\n");
    }
    else {
        // node->next_sibling != NULL || node->child_list != NULL
        while (node != NULL) {
        path[level-1] = node->word;
        print_paths(node->child_list, path, level+1, num_rows);
        node = node->next_sibling;
        }
    }
}

/* Free all dynamically allocated memory pointed to from w.
 */ 
void free_wordle(struct wordle *w){
    // TODO
    free(w);
}

/* Free all dynamically allocated pointed to from node
 */
void free_tree(struct solver_node *node){
    // TODO
    struct solver_node *temp = node;
    while(temp->child_list != NULL || temp->next_sibling != NULL){
        free(temp->con);
        free_tree(temp->child_list);
        free_tree(temp->next_sibling);
    }
    free(node);   
}
