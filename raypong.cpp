#include <raylib.h>
#include <iostream>

/*
1. Create a blank screen and game loop
2. Draw paddles and the ball
3. Move the ball around
4. Check for colision with all edges
5. Move the player's paddle
6. Move the CPU paddle with Artifical Intelligence
7. Check for a collision with the paddes
8. Add scoring*/

/* Evey game has two sections, definitions which define the variables needed and creating
game objects and the game oo which updates the ositions of the game objects and checks fo collisions*/
int player_score = 0;
int cpu_score = 0;

class Ball{
    public:
        float x,y;
        int speed_x, speed_y;
        int radius;
        bool reset = 0;

        void Draw(){
            DrawCircle(x,y,radius,GREEN);
        }

        void Update(){
            x += speed_x;
            y += speed_y;
            if(y + radius >= GetScreenHeight() || y - radius <= 0){
                speed_y *= -1;
            }
            if(x + radius >= GetScreenWidth()){
                cpu_score++;
                ResetBall();
                }
            
            if( x - radius <= 0){
                player_score++;
                ResetBall();
            }

            
        }

        void ResetBall(){
            x = GetScreenWidth()/2;
            y = GetScreenHeight()/2;

            int speed_choices[2] = {-1,1};
            speed_y *= speed_choices[GetRandomValue(0,1)];
            speed_x *= speed_choices[GetRandomValue(0,1)]; 
        }
};

class Paddle{
    public:
        float x,y;
        float width, height;
        int speed;

        void Draw(){
            DrawRectangle(x,y,width,height, WHITE);
        }

        void Update(){
            if(IsKeyDown(KEY_UP)){
                y -= speed;
            }
            if(IsKeyDown(KEY_DOWN)){
                y += speed;
            }
            LimitMovement();
        }
        protected:
            void LimitMovement(){
                if(y <= 0){
                y = 0;
            }
            if(y + height >= GetScreenHeight()){
                y = GetScreenHeight() -height;
            }
        }

};

class CPU_Paddle: public Paddle{
    public:
        void Update(int ball_y){
            if(y + height/2 > ball_y){
                y -= speed;
            }
            if( y + height/2 <= ball_y){
                y += speed;
            }
            LimitMovement();
        }
};
Paddle player;
Ball ball;
CPU_Paddle cpu_player;

int main(){
    std::cout << "Starting the game" << std::endl;
    //definitions
    const int screen_width = 1280;
    const int screen_height = 800;
    
    InitWindow(screen_width,screen_height, "Pong");
    SetTargetFPS(60);

    ball.radius = 20;
    ball.x = screen_width/2;
    ball.y = screen_height/2;
    ball.speed_x = 7;
    ball.speed_y = 7;

    player.width = 25;
    player.height = 120;
    player.x = screen_width - player.width;
    player.y = screen_height/2 - player.height/2;
    player.speed = 6;

    cpu_player.height = 120;
    cpu_player.width = 25;
    cpu_player.x = 10;
    cpu_player.y = screen_height/2 - cpu_player.height/2;
    cpu_player.speed = 6; 

    while(WindowShouldClose()==false){
        //Game loop. Begin drawing creates canvas. End drawing removes it

        BeginDrawing();

        //Update
        ball.Update();
        player.Update();
        cpu_player.Update(ball.y);

        //checking for collisions
        if(CheckCollisionCircleRec(Vector2{ball.x,ball.y},ball.radius,Rectangle{player.x,player.y,player.width,player.height})){
            ball.speed_x *= -1;
        }
        else if(CheckCollisionCircleRec(Vector2{ball.x,ball.y},ball.radius,Rectangle{cpu_player.x,cpu_player.y,cpu_player.width,cpu_player.height})){
            ball.speed_x *= -1;
        }

        //Draw Ball
        ClearBackground(BLACK);
        DrawLine(screen_width/2, 0, screen_width/2, screen_height, WHITE);
        ball.Draw();
        player.Draw();
        cpu_player.Draw();
        DrawText(TextFormat("%i",cpu_score), screen_width/4 - 20, 20, 80, WHITE);
        DrawText(TextFormat("%i",player_score), 3*screen_width/4 - 20, 20, 80, WHITE);
        
        EndDrawing();
    }
    CloseWindow();
    
    return 0;
}
