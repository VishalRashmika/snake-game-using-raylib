/*
Building process:
    //1. creating an blank canvas & game loop
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

int cellsize = 30;
int cellcount = 25;

class Food{
public:
    Texture2D texture;

    Food(){
        Image image = LoadImage("graphics/food.png");
        texture = LoadTextureFromImage(image);
        UnloadImage(image);
        position = GenerateRandomPos();

    }

    ~Food(){
        UnloadTexture(texture);
    }

    void Draw(){
        DrawTexture(texture, position.x * cellsize,position.y * cellsize, BLACK);
    }

    Vector2 GenerateRandomPos(){
        float x = GetRandomValue(0, cellcount - 1);
        float y = GetRandomValue(0, cellcount - 1);
        return Vector2{x,y};
    }
};

int main(){
    std::cout << "Starting the game...." << std::endl;

    InitWindow(cellsize*cellcount,cellsize*cellcount,"Retro Snake");
    SetTargetFPS(60);

    Food food = Food();

    while(WindowShouldClose() == false){
        BeginDrawing();

        //Drawing
        ClearBackground(green);
        food.Draw();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}