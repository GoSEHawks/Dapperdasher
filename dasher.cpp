#include"raylib.h"


struct AnimData
{
    Rectangle rec;
    Vector2 pos;
    int Frame;
    float updateTime;
    float runningTime;
};

bool isOnGround(AnimData data, int windowHight)
{
    return data.pos.y >= windowHight - data.rec.height;
}
AnimData upDateAnimdata(AnimData data, float deltaTime, int maxFrame)
{
    // update running time
    data.runningTime += deltaTime;
    if (data.runningTime >= data.updateTime)
    {
        data.runningTime = 0.0;
        //update animation frame
        data.rec.x = data.Frame * data.rec.width;
        data.Frame ++;
        if (data.Frame > maxFrame)
        {
            data.Frame =0;
        }
        
    }
    return data;
}
int main(){
    //window demensions array
    int windowDemensions[2];
    windowDemensions[0] = 512;
    windowDemensions[1] = 380;

    // make window
    InitWindow(windowDemensions[0],windowDemensions[1]," .Dasher.");
    const int gravity{1'000};

    //nebula vareables
    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");
    //animdata for nebula
    const int sizeOfNebulea{2};
    AnimData nebulea[sizeOfNebulea] { };

    for (int i = 0; i < sizeOfNebulea; i++)
    {
        nebulea[i].rec.x = 0.0;
        nebulea[i].rec.y = 0.0;
        nebulea[i].rec.width = nebula.width/8;
        nebulea[i].rec.height = nebula.height/8;
        nebulea[i].pos.y = windowDemensions[1] - nebula.height/8;
        nebulea[i].Frame = 0;
        nebulea[i].runningTime = 0.0;
        nebulea[i].updateTime = 0.0;
        nebulea[i].pos.x = windowDemensions[0] + i * 400;
    }
    float finishLine{nebulea[sizeOfNebulea -1].pos.x};


    // nebula velocity (pixels per sec)
    int nebVel{-250};

    // scarfy vareables
    Texture2D scarfy = LoadTexture("textures/scarfy.png");
    AnimData scarfyData;
    scarfyData.rec.width = scarfy.width/6;
    scarfyData.rec.height = scarfy.height;
    scarfyData.rec.x = 0;
    scarfyData.rec.y = 0;
    scarfyData.pos.x = windowDemensions[0]/2 -scarfyData.rec.width/2;
    scarfyData.pos.y = windowDemensions[1] - scarfyData.rec.height;
    scarfyData.Frame = 0;
    scarfyData.updateTime = 1.0/12.0;
    scarfyData.runningTime = 0.0;
    
    // is rectangele in the air
    bool isInAir{};
    // jump velocity
    const int jumpVel{-650};

    int velocety{0};
    // back grounds
    Texture2D backGroud = LoadTexture("textures/far-buildings.png");
    float bgEx{};
    Texture2D midGround = LoadTexture("textures/back-buildings.png");
    float mgEx{};
    Texture2D foreGround = LoadTexture("textures/foreground.png");
    float fgEx{};


    bool collison{};

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        // delta time 
        const float dT{GetFrameTime()};
        //start drawing
        BeginDrawing();
        ClearBackground(WHITE);
        // scroll back ground
        bgEx -= 20 * dT;
        if (bgEx <= -backGroud.width *2)
        {
            bgEx = 0.0;
        }
        //scroll mindGorund
        mgEx -= 40* dT;
        if (mgEx <= -midGround.width *2)
        {
            mgEx = 0.0;
        }
        //scroll foreground
        fgEx -= 80*dT;
        if (fgEx <= -foreGround.width *2)
        {
            fgEx = 0.0;
        }
        
        

        // draw background
        Vector2 bg1Pos {bgEx,0.0};
        DrawTextureEx(backGroud,bg1Pos, 0.0, 2.0, BLUE);
        Vector2 bg2Pos{bgEx + backGroud.width *2, 0.0};
        DrawTextureEx(backGroud, bg2Pos , 0.0, 2.0, BLUE);
        // mid ground draw
        Vector2 mg1Pos {mgEx,0.0};
        DrawTextureEx(midGround,mg1Pos, 0.0, 2.0, GRAY);
        Vector2 mg2Pos{mgEx + midGround.width *2, 0.0};
        DrawTextureEx(midGround, mg2Pos , 0.0, 2.0, GRAY);
        //fore gorund draw
        Vector2 fg1Pos {fgEx,0.0};
        DrawTextureEx(foreGround,fg1Pos, 0.0, 2.0, RED);
        Vector2 fg2Pos{fgEx + foreGround.width *2, 0.0};
        DrawTextureEx(foreGround, fg2Pos , 0.0, 2.0, RED);
        // preform ground check

        if (isOnGround(scarfyData, windowDemensions[1]))
        {
            velocety = 0;
            isInAir = false;
        }
        else
        {
        //apply gravity
        velocety += gravity * dT;
        isInAir = true;
        }
        
        //jump check
        if (IsKeyPressed(KEY_SPACE) && !isInAir)
        {
            velocety += jumpVel;
        }
    
       for (int i = 0; i < sizeOfNebulea ; i++)
       {
            // upadate nebula possition
            nebulea[i].pos.x += nebVel * dT;
          
       }
       // update finish line
       finishLine += nebVel * dT;
       
       

        scarfyData.pos.y += velocety * dT;

        // update scarfys animation 
        if (!isInAir)
        {
            scarfyData = upDateAnimdata( scarfyData, dT, 5);    
        }

        for (int i = 0; i < sizeOfNebulea; i++)
        {
            
            nebulea[i] = upDateAnimdata(nebulea[i],dT,7);
        }
        for (AnimData  nebula : nebulea)
        {
            float pad{40};
            Rectangle nebRec{
                nebula.pos.x + pad,
                nebula.pos.y + pad,
                nebula.rec.width - 2*pad,
                nebula.rec.height - 2*pad
            };
            Rectangle scarfyRec{
                scarfyData.pos.x,
                scarfyData.pos.y,
                scarfyData.rec.width,
                scarfyData.rec.height
            };
            if ((CheckCollisionRecs(nebRec,scarfyRec)))
            {
                collison = true;
            }
            
        }
        
         if(collison)
         {
            // lose the game 
            DrawText(" GAME  OVER! ", windowDemensions[0]/8, windowDemensions[1]/4, 60, ORANGE);
         }
         else if (scarfyData.pos.x >= finishLine)
         {
             //win game
            DrawText(" YOU  WIN! ", windowDemensions[0]/8, windowDemensions[1]/4, 60, GREEN);
         }
         else
         {

            for (int i = 0; i < sizeOfNebulea ; i++)
            {
                // draw nebula
                DrawTextureRec(nebula, nebulea[i].rec, nebulea[i].pos, YELLOW); 
            }
            
            // draw scarfy
            DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);
         }

        //stop drwaing
        EndDrawing();
    }
    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    UnloadTexture(backGroud);
    UnloadTexture(foreGround);
    UnloadTexture(midGround);
    CloseWindow();
    
}