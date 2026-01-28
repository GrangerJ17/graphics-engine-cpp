#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <iostream>
#include <string>
#include <math.h>
#include <cmath>
using namespace std;

#define HEIGHT 800
#define WIDTH 800
#define FPS 60.0f
#define _USE_MATH_DEFINE



SDL_Window *window;
SDL_Renderer *renderer;
SDL_Event event;

float cam_move = 0;

float angle = 0.0f;
float dz = 3;
const float dt = 1.0f/FPS;
float dx = 0;

float last_mouse_x = 0.0f;
float last_mouse_y = 0.0f;
float sensitivity = 0.01f;

float rot_speed = 1.0f;
float move_speed = 4.0f;
float focal = 1.5f;

bool change = false;



const std::vector<std::vector<float>> vs = {
    // Far

    {0.5, 0.5, 0.5},
    {0.5, -0.5, 0.5},
    {-0.5, -0.5, 0.5},
    {-0.5, 0.5, 0.5},

    // Near

    {0.5, 0.5, -0.5},
    {0.5, -0.5, -0.5},
    {-0.5, -0.5, -0.5},
    {-0.5, 0.5, -0.5}
    
    
};

const std::vector<std::vector<int>> fs = {
    {0, 1, 2, 3},
    {4, 5, 6, 7},
    {4, 0},
    {5, 1},
    {6, 2},
    {7, 3}

};




void create_point(std::vector<float> points, float size){
    float x = points[0];
    float y = points[1];


    SDL_FRect point = SDL_FRect {
        ((x+1)/2*WIDTH)-size/2,
        ((1-(y+1)/2)*HEIGHT)-size/2,
        10,
        10
    };

    SDL_RenderFillRect(renderer, &point);
}



std::vector<float> z_division(std::vector<float> points, float dz){
    float z = points[2];

    
    std::vector<float> z_points = {((points[0]/z)*focal), ((points[1]/z)*focal)};
    
    // if(change){
    //     std::cout << "curr x: " << points[0] << std::endl;
    //     std::cout << "curr y: " << points[1] << std::endl;
    //     std::cout << "curr z: " << z << std::endl;  
    //     std::cout << "rendered x: " << z_points[0] << std::endl;
    //     std::cout << "rendered y: " << z_points[1] << std::endl;
    //     std::cout << std::endl;
    // }

    return z_points;
}

std::vector<float> rotate_xz(std::vector<float> points, float angle){
    float c = cos(angle);
    float s = sin(angle);

    float x = points[0] * c - points[2] * s;
    float z = points[0] * s + points[2] * c;

  

    return { x+dx, points[1], z+dz };
}

void create_line(int back_point, int front_point, float angle, float dz){
    std::vector<float> p1 = z_division(rotate_xz((vs[back_point]), angle) , dz);
    std::vector<float> p2 = z_division(rotate_xz((vs[front_point]), angle) , dz);
    
    SDL_RenderLine(renderer, (((p1[0])+1)/2*WIDTH), ((1-(p1[1]+1)/2)*HEIGHT), (((p2[0])+1)/2*WIDTH) , ((1-(p2[1]+1)/2)*HEIGHT));
}

void render_screen(){
    SDL_SetRenderDrawColor(renderer, 80, 255, 80, 255);
    
    for (std::vector<int> face_points : fs){
        int i = 0; 
        for (int points : face_points){
            create_line(face_points[i], face_points[(i + 1) % face_points.size()], angle, dz);

            i++;
        }
         change = false;

    }


}

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    SDL_CreateWindowAndRenderer("CPU Surface", HEIGHT, WIDTH, 0, &window, &renderer);

    SDL_Surface *surface =
        SDL_CreateSurface(HEIGHT, WIDTH, SDL_PIXELFORMAT_RGBA32);

    
    SDL_Cursor* Cursor{
    SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_DEFAULT)
    };

    SDL_SetCursor(Cursor);
   
   while (1) {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT){
            goto quit;
        } 
        
        if (event.type == SDL_EVENT_KEY_DOWN) {
            SDL_Keycode key = SDL_GetKeyFromScancode(event.key.scancode, event.key.mod, false);

            if (key == 's') {
                dz += 1.0f*dt;
            };

            if (key == 'd'){
                dx -= 1.0f*dt;
            };

            if (key == 'a'){
                dx += 1.0f*dt;
            };

            if (key == 'w'){
                dz -= 1.0f*dt;
            };


            if(key == 'q'){
                angle -= rot_speed * dt;
                float s = sin(rot_speed * dt);
                float c = cos(rot_speed * dt);
                float old_dx = dx;
                float old_dz = dz;

                dx = ((old_dx*c)+(old_dz*s));
                dz = ((old_dx*-s)+(old_dz*c));
                change = true;
                
            }

            if(key == 'e'){
                angle += rot_speed * dt;
                float s = sin(rot_speed * dt);
                float c = cos(rot_speed * dt);
                float old_dx = dx;
                float old_dz = dz;

                dx = ((old_dx*c)-(old_dz*s));
                dz = ((old_dx*s)+(old_dz*c));  
                change = true;
            }

            // std::cout << dz << endl;


            


            
            
            
        }

        if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        if (event.button.button == SDL_BUTTON_LEFT) {
            cam_move = 1;
            last_mouse_x = event.button.x;
        }
    }
        
        if (event.type == SDL_EVENT_MOUSE_BUTTON_UP) {
            if (event.button.button == SDL_BUTTON_LEFT) {
                cam_move = 0;
            }
        } 
        
        if (event.type == SDL_EVENT_MOUSE_MOTION) {
            if (cam_move) {
               float rot_direction = event.button.x - last_mouse_x;
               std::cout << "Rotation direction: " << rot_direction << std::endl;
               if(rot_direction > 0){
                    angle += rot_speed *sensitivity;
                    float s = sin(rot_speed * sensitivity);
                    float c = cos(rot_speed * sensitivity);
                    float old_dx = dx;
                    float old_dz = dz;

                    dx = ((old_dx*c)-(old_dz*s));
                    dz = ((old_dx*s)+(old_dz*c));
               } else if (rot_direction < 0) {
                        angle -= rot_speed * sensitivity;
                        float s = sin(rot_speed * sensitivity);
                        float c = cos(rot_speed * sensitivity);
                        float old_dx = dx;
                        float old_dz = dz;

                        dx = ((old_dx*c)+(old_dz*s));
                        dz = ((old_dx*-s)+(old_dz*c));

               }
            }

            last_mouse_x = event.button.x;
         
        }

    }
    

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    
    
    render_screen();

    SDL_RenderPresent(renderer);
    SDL_Delay(16);
    }

    quit: 
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);

        SDL_Quit();

    return 0;

   

}
