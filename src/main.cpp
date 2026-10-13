#include<iostream>
#include<raylib.h>
using namespace std;

int player_score = 0;
int cpu_score = 0;

class Ball{
    public:
        float x , y;
        int speed_x , speed_y;
        int radius;
    void ResetBall(){
        x = GetScreenWidth()/2;
        y = GetScreenHeight()/2;
        
        int speed_choices[2] = {-1 , 1};
        speed_x *= speed_choices[(GetRandomValue(0 , 1))];
        speed_y *= speed_choices[(GetRandomValue(0 , 1))];

    }
    void Draw(){
        DrawCircle(x , y , radius , WHITE);
    }
    void Update(){
        x += speed_x * GetFrameTime();
        y += speed_y * GetFrameTime();

        if (y + radius >= GetScreenHeight() || y - radius <= 0) speed_y *= -1;
        if (x + radius >= GetScreenWidth()) { 
            cpu_score++; 
            ResetBall();
        }
        if (x - radius <= 0){ 
            player_score++;
            ResetBall();
        }
    }
};

class Paddle{
    protected:
        void LimitMovement(){
            if(y <= 0) y = 0;
            if(y + height >= GetScreenHeight()) y = GetScreenHeight() - height;
        }

    public:
        float x , y;
        int speed_x , speed_y;
        int width , height;

    void Draw(){
        DrawRectangle(x , y , width , height , WHITE);
    }
    void Update(){
        if (IsKeyDown(KEY_S)){
            y += speed_y * GetFrameTime();
        }
        if (IsKeyDown(KEY_W)){
            y -= speed_y * GetFrameTime();
        }
        LimitMovement();
    }
};

class CpuPaddle : public Paddle{
    public:
        void Update(float ball_y){
            if (y + height/2 < ball_y) y += speed_y * GetFrameTime();
            if (y + height/2 >= ball_y) y -= speed_y * GetFrameTime();
            LimitMovement();
        }
};

Ball ball;
Paddle paddle1;
CpuPaddle cpu;

int main(void){
    const int screen_width = 1280;
    const int screen_height = 800;
    InitWindow(screen_width , screen_height , "PongGame");
    SetTargetFPS(60);

    // Ball
    ball.x = screen_width / 2;
    ball.y = screen_height / 2;
    ball.speed_x = 500;
    ball.speed_y = 500;
    ball.radius = 20;

    // Paddle trái (W / S)
    paddle1.width = 25;
    paddle1.height = 120;
    paddle1.x = 10;
    paddle1.y = screen_height / 2 - paddle1.height / 2;
    paddle1.speed_x = 300;
    paddle1.speed_y = 300;

    // cpu
    cpu.height = 120;
    cpu.width = 25;
    cpu.x = screen_width - paddle1.width - 10;
    cpu.y = screen_height/2 - paddle1.height / 2;
    cpu.speed_x = 300;
    cpu.speed_y = 300;

    while(!WindowShouldClose()){
        BeginDrawing();
        
        Rectangle rec1 = {paddle1.x , paddle1.y , (float)paddle1.width , (float)paddle1.height};
        Rectangle rec2 = {cpu.x , cpu.y , (float)cpu.width , (float)cpu.height};
        if (CheckCollisionCircleRec(Vector2{ball.x , ball.y} , ball.radius , rec1) && ball.speed_x < 0){
            ball.speed_x *= -1; // neu bong va cham va dang huong vao trong vot -> chi dao huong khi huong cua bong huong vao vot
        }
        if (CheckCollisionCircleRec(Vector2{ball.x , ball.y} , ball.radius , rec2) && ball.speed_x > 0){
            ball.speed_x *= -1; 
        }
        ball.Update();
        paddle1.Update();
        cpu.Update(ball.y);
        ClearBackground(BLACK);
        DrawLine(screen_width/2 , 0 , screen_width/2 , screen_height , WHITE);
        ball.Draw();
        paddle1.Draw();
        cpu.Draw();
        DrawText(TextFormat("%d" , player_score) , screen_width/4 - 20 , 20 , 80 , WHITE);
        DrawText(TextFormat("%d" , cpu_score) , 3 * screen_width/4 - 20 , 20 , 80 , WHITE);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}