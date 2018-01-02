/*
 * COMP20003 Algorithms and Data Structures
 * Semester 2 2016
 *
 * University Username: dyeoh
 * Name: Darren Yeoh Cheang Leng
 * Student ID: 715863
 *
 * This module provides all the functionality related to the ai
 * used in the automatically solving the 2048 game.
 *
 *
 *
*/

#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include "ai.h"
#include "utils.h"
#include "priority_queue.h"

/*Global variables*/
struct heap h;
struct timeval start, end;
int total_generated = 0;
int expanded_nodes = 0;

void initialize_ai(){
	heap_init(&h);
	gettimeofday(&start, NULL);
}

/**
 * Find best action by building all possible paths up to depth max_depth
 * and back propagate using either max or avg
 */
move_t get_next_move( uint8_t board[SIZE][SIZE], int max_depth, propagation_t propagation ){
	int i;
	
	/*Initialize first choices(node) and add to heap*/
	node_t *first_choice = init_node(board);
	first_choice = (node_t*)realloc(first_choice, sizeof(node_t)* MAX_MOVES);
	for( i=0; i<MAX_MOVES; i++ ){
		expanded_nodes ++;
		first_choice[i].priority= 0;
		first_choice[i].num_childs = 0;
		first_choice[i].depth = 0;
		first_choice[i].move = i;
		set_board( first_choice[i].board, board );
		dijkstra( &first_choice[i], max_depth, propagation );
	}
	
	move_t best_action = get_best(first_choice, board);
	
	free(first_choice);
	return best_action;	

}

/**
 * Variant of Dijkstra's algorithm implemented from the pseudocode provided
 * in the assignment.
 */
void dijkstra( node_t *first_choice, int max_depth, propagation_t propagation ){
	int i;
	int expanded_count = 0;
	int total_score= 0;
	
	/*Initialize explored array*/
	node_t *explored = (node_t*)malloc(sizeof(node_t));
	
	/*Initializes a first move*/
	node_t *start = applyAction(first_choice, first_choice->move);
	heap_push(&h, start);
	if(max_depth >= 1){
		total_generated++;
	}
	first_choice->num_childs ++;

	/*Explores all paths starting from the first move*/
	while(h.count!=0){
		node_t *node = heap_delete(&h);
		expanded_count ++;
		explored = (node_t*)realloc(explored, sizeof(node_t)* expanded_count);
		explored[expanded_count-1] = *node;
		if(node->depth==1 && max_depth == 1){
			if(propagation == 0){
				prop_max(first_choice, node->priority);
			}
			else if(propagation == 1){
				total_score += node->priority;
			}
		}
		/*explores all paths up to a certain depth*/
		if(node->depth<max_depth){
			for(i=0;i<MAX_MOVES;i++){
				node_t *child = applyAction(node, i);
				total_generated ++;
				if(board_cmp(child->board, node->board) == 1){
					heap_push(&h, child);
					first_choice->num_childs ++;
					/*propagates back to the root node*/
					if(propagation == 0){
						prop_max(first_choice, child->priority);
					}
					else if(propagation == 1){
						total_score += child->priority;
					}
				}
				else{
					free(child);
				}
			}
		}
	}
	/*Used in finding the average propagation*/
	if(propagation == 1){
		prop_avg(first_choice, total_score);
	}

	emptyPQ(&h);
	free(explored);
	expanded_nodes += expanded_count;
	free(start);
}


/*Returns the best first choice move*/
int get_best(node_t *first_choice, uint8_t board[SIZE][SIZE]){
	int i;
	int highest = 0;
	int current = 0;
	
	/*Finds the highest value*/
	for(i=0;i<MAX_MOVES;i++){
		if(first_choice[i].priority > highest){
			highest = first_choice[i].priority;
		}
	}
	
	/*Makes a list of those tied with the highest value*/
	for(i=0;i<MAX_MOVES;i++){
		if(first_choice[i].priority == highest){
			/*Picks first valid tied highest move*/
			node_t *temp= init_node(board);
			node_t *nextBoard = applyAction(temp, i);
			if(board_cmp(nextBoard->board, board)==1){
				current = i;
			}
			free(temp);
			free(nextBoard);
		}
	}

	return current;
	
}

/*Propagates max score to the root node*/
void prop_max(node_t *first_choice ,uint32_t priority){
	
	if(priority>first_choice->priority){
		first_choice->priority = priority;
	}
}

/*Propagate average score to the root node*/
void prop_avg(node_t* first_choice, int total_score){
	
	first_choice->priority = total_score / first_choice->num_childs;

}

/*Initializes an empty node*/
node_t* init_node( uint8_t board[SIZE][SIZE] ) {
	node_t *temp = NULL;
	temp = (node_t*)malloc(sizeof(node_t));
	temp->depth = 0;
	temp->num_childs = 0;
	temp->move = 0;
	temp->parent = NULL;
	set_board(temp->board, board);
	
	return temp;
}


/*Copies a board to the target*/
void set_board( uint8_t target[SIZE][SIZE], uint8_t board[SIZE][SIZE] ) {
	int i, j;

	for( i = 0; i < SIZE; i++ ) {
		for( j= 0; j< SIZE; j++ ){
			target[i][j] = board[i][j];
			}
		}

}

/*Compares two boards*/
int board_cmp( uint8_t board1[SIZE][SIZE], uint8_t board2[SIZE][SIZE] ) {
	int i, j;
	
	for( i = 0; i < SIZE; i++ ) {
		for( j= 0; j< SIZE; j++ ) {
			if( board1[i][j] != board2[i][j] ) {
				return 1;
			}
		}
	}
	return 0;
}

/*Returns the resultant node state after a move was applied*/
node_t* applyAction( node_t *node, int move ) {
	
	/*Initialization*/
	node_t *temp = init_node(node->board);
	temp->priority = node-> priority;
	temp->depth = node->depth + 1;
	temp->parent = node;
	set_board(temp->board, node->board);
	
	/*updates the priority and board*/
	execute_move_t(temp->board, &temp->priority, move);
	
	return temp;
}

/*Get max tile value*/
uint32_t get_maxtile( uint8_t board[SIZE][SIZE] ){
	int i,j;
	uint32_t max = 0;
	
	for( i = 0; i < SIZE; i++ ) {
		for( j= 0; j< SIZE; j++ ) {
			if( getTile(board, i, j) > max ) {
				max = getTile(board, i, j);
			}
		}
	}
	
	return max;
}

/*Frees the heap and prints results to output file*/
void finish_ai( int max_depth, uint8_t board[SIZE][SIZE], uint32_t score) {
	FILE *fw;
	double elapsed;
	uint32_t max_tile;
	
	gettimeofday(&end, NULL);
	elapsed = (end.tv_sec - start.tv_sec) + 
              ((end.tv_usec - start.tv_usec)/TIME_CONVERT);
	max_tile = get_maxtile(board);
	
	fw = fopen(OUTPUT_FILE, "w");
	fprintf(fw, "MaxDepth = %d\n", max_depth);
	fprintf(fw, "Generated = %d\n", total_generated);
	fprintf(fw, "Expanded = %d\n", expanded_nodes);
	fprintf(fw, "Time = %lfs\n", elapsed);
	fprintf(fw, "Expanded/second = %d\n", (int)(expanded_nodes/elapsed));
	fprintf(fw, "max_tile = %d\n", max_tile);
	fprintf(fw, "Score = %d\n", score);
	
	free(h.heaparr);
	fclose(fw);
	}
	
	
	