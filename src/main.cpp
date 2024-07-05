/*
Building process:
    //1. creating an blank canvas & game loop
    //2. creating the food
    //3. creating the snake
    //4. moving the snake
    5. making the snake eat the food
    6. making the snake grow longer
    7. checking for collisions with edges and tail
    8. adding title & frame
    9. keeping the score
    10. adding sounds  
*/

#include <iostream>
#include <raylib.h>
#include <deque>
#include <raymath.h>

Color green = {173,204,96,255};
Color dark_green = {43,51,24,255};

int cellsize = 30;
int cellcount = 25;

double lastUpdateTime = 0;

bool eventTriggered(double interval){
    double currentTime = GetTime();
    if (currentTime - lastUpdateTime >= interval){
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

class Snake{
public:
    std::deque<Vector2> body = {Vector2{6,9}, Vector2{5,9}, Vector2{4,9}};
    Vector2 direction = {1,0};

    void Draw(){
        for (unsigned int i = 0; i < body.size(); i++){
            float x = body[i].x;
            float y = body[i].y;
            Rectangle segment = Rectangle{x*cellsize,y*cellsize,(float)cellsize,(float)cellsize};
            DrawRectangleRounded(segment,0.5,6,dark_green);
        }
    }

    void Update(){
        body.pop_back();
        body.push_front(Vector2Add(body[0], direction));
    }   
};
class Food{
public:
    Vector2 position;

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

class Game{
public:
    Snake snake = Snake();
    Food food = Food();

    void Draw(){
        food.Draw();
        snake.Draw();
    }

    void Update(){
        // food.Update();
        snake.Update();
    }
};

int main(){
    std::cout << "Starting the game...." << std::endl;

    InitWindow(cellsize*cellcount,cellsize*cellcount,"Retro Snake");
    SetTargetFPS(60);

    Game game = Game();

    while(WindowShouldClose() == false){
        BeginDrawing();

        if (eventTriggered(0.2)){
            game.Update();
        }

        if (IsKeyPressed(KEY_UP) && game.snake.direction.y != 1){
            game.snake.direction = {0,-1};
        }
        if (IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1){
            game.snake.direction = {0,1};
        }
        if (IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1){
            game.snake.direction = {-1,0};
        }
        if (IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1){
            game.snake.direction = {1,0};
        }


        //Drawing
        ClearBackground(green);
        game.Draw();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}