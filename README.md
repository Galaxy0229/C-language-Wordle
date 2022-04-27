
Wordle rules for our version of the game
Your program will actually apply these rules in reverse order, but it is helpful to understand the rules of the game when someone is trying to guess the solution.

Each guess must be a valid WORDLEN-letter word.  After each guess, the colour of the tiles will change to show which, if any, letters are in the solution word.   (WORDLEN is defined as 5 in common.h, but you should not use hard-coded numbers in your code.)

A green tile means that the letter is in the solution in the same position. 
A yellow tile means that the letter is in the solution in a different position. 
A grey tile means that the letter is not in the solution.
For our purposes, we are going to create an extra-hard mode: 

If a letter was marked as grey, it cannot be used again.
If a letter was marked as yellow, that letter must be used in the next guess in a different position that the previous guess.
If a letter was marked as green, that letter must be used in the same position in the next guess.

Step 1: Read the dictionary file into a linked list

The dictionary file contains a list of 5-letter words, one per line.  Read the file and construct a linked list of struct nodes.  This is a task in the extra lab, so you can just copy your code from that lab.

Step 2 Read the wordle input and initialize the wordle struct. This function is given to you.

Step 3 Initialize the root node of the tree.  The constraints struct for the root node initially is empty.

Step 4 Build the solver tree

The solver tree is a general tree where a node can have an arbitrary number of children.  A parent node has a pointer to a linked list of child nodes.  The linked list of child nodes is connected through a next_sibling pointer.  See the solver tree  Download solver treediagram.

Step 4 a) Construct the constraints that will be applied to generate the list of children for this node.  Use the functions set_green, set_yellow, and add_to_cannot_be that you wrote in the extra lab.

Step 4 b) Iterate over the words in the dictionary, calling match_constraints on each word to see if it fits.  If a word matches,  create a new struct solver_node, add it to the list of children of this node, and call solve_subtree on it.  The base case of the recursion is when we reach the end of the rows in the wordle input.

match_constraints() algorithm
There are several checks to make to see if a word matches the constraints in the struct constraints.

For each index in word, check if the word[index] is in the must_be[index] string.  If it isn't, then this word does not match the constraints.
If  must_be[index] is the empty string and word[index] is in the cannot_be set, this word does not match the constraints.
If there are multiple yellow tiles in this row, ensure that we are not matching on words that contain duplicates of letters in the solution.  For example if a row had two yellow tiles and both of them could be either 'u' or 't', then we should only match on words where both yellow are 'u' or both yellow are 't' if there are two 'u's or two 't's in the solution.
