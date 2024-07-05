/*
Building process:
    1. creating an blank canvas & game loop
    2. creating the food
    3. creating the snake
    4. moving the snake
    5. making the snake eat the food
    6. making the snake grow longer
    7. checking for collisions with edges and tail
    8. adding title & frame
    9. keeping the score
    10. adding sounds  
*/

#include <iostream>
#include <raylib.h>

Color green = {173,204,96,255};
Color dark_green = {43,51,24,255};


int main(){
    std::cout << "Starting the game...." << std::endl;

    InitWindow(750,750,"Retro Snake");
    SetTargetFPS(60);

    while(WindowShouldClose() == false){
        BeginDrawing();

        //Drawing
        ClearBackground(green);
        
        EndDrawing();
    }

    CloseWindow();
    return 0;
}