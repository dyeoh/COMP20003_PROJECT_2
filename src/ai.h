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

#ifndef __AI__
#define __AI__

#include <stdint.h>
#include <unistd.h>
#include "node.h"
#include "priority_queue.h"
#define MAX_MOVES 4
#define OUTPUT_FILE "output.txt"
#define TIME_CONVERT 1000000.0

/*Initializes the heap and start time for the ai*/
void initialize_ai();

/*Gets the best next move based on the specified max depth and propagation*/
move_t get_next_move(uint8_t board[SIZE][SIZE], int max_depth, propagation_t propagation);

/*Variant of dijkstra's used in finding the best action*/
void dijkstra(node_t *node, int max_depth, propagation_t propagation);

/*Returns a properly initialized node*/
node_t* init_node(uint8_t board[SIZE][SIZE]);

/*Copies a board to the specified target*/
void set_board(uint8_t target[SIZE][SIZE], uint8_t board[SIZE][SIZE]);

/*Compares two boards and returns 0 if they are equal and 1 if they are not*/
int board_cmp(uint8_t board1[SIZE][SIZE], uint8_t board2[SIZE][SIZE]);

/*Returns the resultant node state after a move was applied*/
node_t* applyAction(node_t *node, int move);

/*Propagates the maximum value back to the root*/
void prop_max(node_t *first_choice, uint32_t priority);

/*Returns the best choice based on the root*/
int get_best(node_t *first_choice, uint8_t [SIZE][SIZE]);

/*Propagates the average value back to the root*/
void prop_avg(node_t*first_choice, int total_score);

/*Ends the ai and prints to output and frees the heap*/
void finish_ai(int max_depth, uint8_t board[SIZE][SIZE], uint32_t score);

/*Returns the maximum tile on a specified board board*/
uint32_t get_maxtile(uint8_t board[SIZE][SIZE]);

#endif
