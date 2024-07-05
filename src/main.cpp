/*
Building process:
    //1. creating an blank canvas & game loop
    //2. creating the food
    //3. creating the snake
    //4. moving the snake
    //5. making the snake eat the food
    //6. making the snake grow longer
    //7. checking for collisions with edges and tail
    //8. adding title & frame
    //9. keeping the score
    //10. adding sounds  
*/

#include <iostream>
#include <raylib.h>
#include <deque>
#include <raymath.h>

Color green = {173,204,96,255};
Color dark_green = {43,51,24,255};
Color blue = {230,230,250,255};

int cellsize = 30;
int cellcount = 25;
int offset = 75;

double lastUpdateTime = 0;

bool ElementinDeque(Vector2 element, std::deque<Vector2> deque_in ){
    for (unsigned int i = 0; i < deque_in.size(); i++){
        if (Vector2Equals(deque_in[i], element)){
            return true;
        }
    }
    return false;
}

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
    bool addsegment = false;

    void Draw(){
        for (unsigned int i = 0; i < body.size(); i++){
            float x = body[i].x;
            float y = body[i].y;
            Rectangle segment = Rectangle{offset + x*cellsize,offset + y*cellsize,(float)cellsize,(float)cellsize};
            DrawRectangleRounded(segment,0.5,6,dark_green);
        }
    }

    void Update(){
        body.push_front(Vector2Add(body[0],direction));

        if (addsegment == true){
            addsegment = false;
        }
        else{   
            body.pop_back();
        }

    }
    void reset(){
        body = {Vector2{6,9}, Vector2{5,9}, Vector2{4,9}};
        direction = {1,0};
    }
};
class Food{
public:
    Vector2 position;

    Texture2D texture;

    Food(std::deque<Vector2> snakebody){
        Image image = LoadImage("graphics/food.png");
        texture = LoadTextureFromImage(image);
        UnloadImage(image);
        position = GenerateRandomPos(snakebody);

    }

    ~Food(){
        UnloadTexture(texture);
    }

    void Draw(){
        DrawTexture(texture,offset + position.x * cellsize,offset + position.y * cellsize, BLACK);
    }

    Vector2 generaterandomcell(){
        float x = GetRandomValue(0, cellcount - 1);
        float y = GetRandomValue(0, cellcount - 1);
        return Vector2{x,y};
    }

    Vector2 GenerateRandomPos(std::deque<Vector2> snakebody){
        Vector2 position = generaterandomcell();
        while(ElementinDeque(position,snakebody)){
            position = generaterandomcell();
        }
        return position;
    }
};

class Game{
public:
    Snake snake = Snake();
    Food food = Food(snake.body);
    bool running = true;
    int score = 0;

    Sound eatsound;
    Sound wallsound;
    Game(){
        InitAudioDevice();
        eatsound = LoadSound("sounds/eat.mp3");
        wallsound = LoadSound("sounds/wall.mp3");
    }
    ~Game(){
        UnloadSound(eatsound);
        UnloadSound(wallsound);
        CloseAudioDevice();
    }

    void Draw(){
        food.Draw();
        snake.Draw();
    }

    void Update(){
        if (running){
            snake.Update();
            checkcollisionwithfood();
            checkcollisionwithedges();
            checkcollisionwithtail();
        }
    }

    void checkcollisionwithfood(){
        if (Vector2Equals(snake.body[0], food.position)){
            food.position = food.GenerateRandomPos(snake.body);
            snake.addsegment = true;
            score++;
            PlaySound(eatsound);
        }
    }
    void checkcollisionwithedges(){
        if (snake.body[0].x == cellcount || snake.body[0].x == -1){
            GameOver();
        }
        if (snake.body[0].y == cellcount || snake.body[0].y == -1){
            GameOver();
        }
    }

    void GameOver(){
        snake.reset();
        food.position = food.GenerateRandomPos(snake.body);
        running = false;
        score = 0;
        PlaySound(wallsound);
    }
    void checkcollisionwithtail(){
        std::deque<Vector2> headlessbody = snake.body;
        headlessbody.pop_front();
        if (ElementinDeque(snake.body[0],headlessbody)){
            GameOver();
        }
    }
};

int main(){
    std::cout << "Starting the game...." << std::endl;

    InitWindow((cellsize * cellcount) + (2 * offset) ,(cellsize * cellcount)  + (2 * offset),"Retro Snake");
    SetTargetFPS(60);

    Game game = Game();

    while(WindowShouldClose() == false){
        BeginDrawing();

        if (eventTriggered(0.2)){
            game.Update();
        }

        if (IsKeyPressed(KEY_UP) && game.snake.direction.y != 1){
            game.snake.direction = {0,-1};
            game.running = true;
        }
        if (IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1){
            game.snake.direction = {0,1};
            game.running = true;

        }
        if (IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1){
            game.snake.direction = {-1,0};
            game.running = true;

        }
        if (IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1){
            game.snake.direction = {1,0};
            game.running = true;
        }


        //Drawing
        ClearBackground(blue);
        DrawRectangleLinesEx(Rectangle{(float)offset - 5,(float) offset -5,(float)(cellsize * cellcount + 10),(float)(cellsize * cellcount + 10)}, 5, dark_green);
        
        DrawText("Retro Snake",offset -5, 20 ,40 ,dark_green);
        DrawText(TextFormat("%i",game.score),offset -5, offset+ cellsize*cellcount +10 ,40 ,dark_green);

        game.Draw();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}