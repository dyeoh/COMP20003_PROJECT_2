# COMP20003_PROJECT_2
A 2048 solver using Djikstra's algorithm to find the shortest paths between nodes in a graph.

## Getting Started
 The program needs to be compiled before it can be used. Running the included makefile will result
 in an executable named 2048 being created in the root directory of the project.
 

### How to use
There are two modes that you can run the program in, which are listed below.

1. Playing the game with a keyboard

If you want to play the game with a keyboard, no arguments are needed.
Just run the compiled program without any input.

Example:
```
>>./2048
```

This will bring up the game which will require manual keyboard input to move the tiles around.
The directional arrows on the keyboard is used to move the tiles around while the 'q' key is used
to quit the game.

2. Running the solver on the game.

To run the game with the solver you will need to enter parameters

./2048 ai <max/avg> <depth> 
 
 <max/avg> is either max or avg which will make the program look for the maximum possible path or the best average path and <depth> is an integer number indicating the depth of your search.
 
 Example:
```
>>./2048 ai avg 6 
```

Will run the solver with average updates up to depth 6, and if you append slow at the end, it will slow down the ai so that you can
see it playing.

 Example:
```
>>./2048 ai avg 6 slow
```


