#include <simplecpp>
#include <sstream>
#include <ctime>
#include "shooter.h"
#include "bubble.h"

/* Simulation Vars */
const double STEP_TIME = 0.02;

/* Counter Vars */
int Score=0;
int Health=3;
int Level=1;
long double Time=0;

string str(int N) {
    stringstream ss;
    string str;
    ss<<N;
    ss>>str;
    return str;
}

void move_bullets(vector<Bullet> &bullets){
    // move all bullets
    for(unsigned int i=0; i<bullets.size(); i++){
        if(!bullets[i].nextStep(STEP_TIME)){
            bullets.erase(bullets.begin()+i);
        }
    }
}

void move_bubbles(vector<Bubble> &bubbles){
    // move all bubbles
    for (unsigned int i=0; i < bubbles.size(); i++)
    {
        bubbles[i].nextStep(STEP_TIME);
    }
}

vector<Bubble> create_bubbles(int &Lvl)
{
    // create bubbles for different levels in the game
    vector<Bubble> bubbles;

    switch(Lvl) {

    case 1:

        //Bubbles for level 1
        bubbles.push_back(Bubble(100, 100, BUBBLE_DEFAULT_RADIUS, -BUBBLE_DEFAULT_VX, BUBBLE_DEFAULT_VY, COLOR(0,0,255)));
        bubbles.push_back(Bubble(200, 150, BUBBLE_DEFAULT_RADIUS, BUBBLE_DEFAULT_VX, BUBBLE_DEFAULT_VY, COLOR(0,0,255)));
        break;

    case 2:

        //bubbles for level 2
        bubbles.push_back(Bubble(100, 100, 2*BUBBLE_DEFAULT_RADIUS, -BUBBLE_DEFAULT_VX, BUBBLE_DEFAULT_VY, COLOR(255,215,0)));
        bubbles.push_back(Bubble(200, 150, 2*BUBBLE_DEFAULT_RADIUS, BUBBLE_DEFAULT_VX, BUBBLE_DEFAULT_VY, COLOR(255,215,0)));
        bubbles.push_back(Bubble(400, 120, 2*BUBBLE_DEFAULT_RADIUS, BUBBLE_DEFAULT_VX, BUBBLE_DEFAULT_VY, COLOR(255,215,0)));
        break;

    case 3:

        //bubbles for level 3
        bubbles.push_back(Bubble(100, 100, 4*BUBBLE_DEFAULT_RADIUS, -BUBBLE_DEFAULT_VX, BUBBLE_DEFAULT_VY, COLOR(138,43,226)));
        bubbles.push_back(Bubble(200, 150, 4*BUBBLE_DEFAULT_RADIUS, BUBBLE_DEFAULT_VX, BUBBLE_DEFAULT_VY, COLOR(138,43,226)));
        bubbles.push_back(Bubble(400, 120, 4*BUBBLE_DEFAULT_RADIUS, BUBBLE_DEFAULT_VX, BUBBLE_DEFAULT_VY, COLOR(138,43,226)));
        break;

    }

    return bubbles;

}

//check the collisions of the bubbles
void check_collisions(vector<Bubble> &bubbles, vector<Bullet> &bullets, Shooter &shooter) {

    shooter.shooter_hit()=false;

    //Checking collisions individually for each Bubble
    for (unsigned int i=0; i < bubbles.size(); i++) {

        //Checking collisions individually for each Bullet
        for(unsigned int j=0; j<bullets.size(); j++) {

            //checking collisions between Bubble and the Bullet
            if((abs(bubbles[i].get_center_x()-bullets[j].get_center_x())<bubbles[i].get_radius()+bullets[j].get_width()/2)
                &&
                (abs(bubbles[i].get_center_y()-bullets[j].get_center_y())<bubbles[i].get_radius()+bullets[j].get_height()/2)) {

                    if(bubbles[i].get_radius()>BUBBLE_DEFAULT_RADIUS) {
                        bubbles.push_back(Bubble(bubbles[i].get_center_x(), bubbles[i].get_center_y(), bubbles[i].get_radius()/2,BUBBLE_DEFAULT_VX,0,bubbles[i].get_color()));
                        bubbles.push_back(Bubble(bubbles[i].get_center_x(), bubbles[i].get_center_y(), bubbles[i].get_radius()/2,-BUBBLE_DEFAULT_VX,0,bubbles[i].get_color()));
                    }

                    Score += 100; // Increse the score counter by 100 for each bubble when hit
                    bubbles.erase(bubbles.begin()+i);
                    bullets.erase(bullets.begin()+j);

            }
        }

        //checking collision of the Bubble with the shooter
        if((abs(bubbles[i].get_center_x()-shooter.get_center_x())<bubbles[i].get_radius()+2*shooter.get_head_radius())
            &&
           (abs(bubbles[i].get_center_y()-shooter.get_center_Y())<bubbles[i].get_radius()+2*shooter.get_head_radius()) ) {

                if(!bubbles[i].bubble_hit()) {

                    Health--; // Reduce health pts by 1 when hit

                    bubbles[i].bubble_hit()=true;
                }

                shooter.shooter_hit()=true;

            }
        else bubbles[i].bubble_hit()=false;

    }

}

int main()
{
    initCanvas("Bubble Trouble", WINDOW_X, WINDOW_Y);

    Line b1(0, PLAY_Y_HEIGHT, WINDOW_X, PLAY_Y_HEIGHT);
    b1.setColor(COLOR(0, 0, 255));

    // Intialize the shooter
    Shooter shooter(SHOOTER_START_X, SHOOTER_START_Y, SHOOTER_VX);

    XEvent event;

    string msg_cmd("Cmd: _");

    // Intialize the Counter Vars on the initCanvas
    Text charPressed(LEFT_MARGIN, BOTTOM_MARGIN, msg_cmd);
    Text LEVEL(WINDOW_X/2.0,BOTTOM_MARGIN,"Level: "+str(Level)+"/3");
    Text SCORE(WINDOW_X-LEFT_MARGIN,BOTTOM_MARGIN,"Score: "+str(Score));
    Text HEALTH(WINDOW_X-LEFT_MARGIN,WINDOW_Y-BOTTOM_MARGIN,"Health: "+str(Health)+"/3");
    Text TIME(LEFT_MARGIN,WINDOW_Y-BOTTOM_MARGIN,"Time: "+str(Time)+"/50");
    Text Display_progress(WINDOW_X/2.0,WINDOW_Y/2.0,"Level "+str(Level)+"!");
    Display_progress.setColor(COLOR(0,0,255));

    // Main game loop
    while((Time<=51)&&(Health>0)&&(Level<=3)) {

        //Update the counter vars
        Health=3;
        Time=0;

        //Display which Level we are playing on the Screen
        LEVEL.setMessage("Level: "+str(Level)+"/3");
        Display_progress.show();
        Display_progress.setMessage("Level "+str(Level)+"!");
        wait(3);
        Display_progress.hide();

        // Initialize the bubbles
        vector<Bubble> bubbles = create_bubbles(Level);

        // Initialize the bullets (empty)
        vector<Bullet> bullets;

        // Initialize the clock
        clock_t start_time = clock();

        while (bubbles.size()&&(Time<=51)&&(Health>0))
        {
            bool pendingEvent = checkEvent(event);
            if (pendingEvent)
            {
                // Get the key pressed
                char c = charFromEvent(event);
                msg_cmd[msg_cmd.length() - 1] = c;
                charPressed.setMessage(msg_cmd);

                // Update the shooter
                if(c == 'a')
                    shooter.move(STEP_TIME, true);
                else if(c == 'd')
                    shooter.move(STEP_TIME, false);
                else if(c == 'w')
                    bullets.push_back(shooter.shoot(0));
                else if((c==' ')&&(Score>=400)) {
                // The Death's Barrage power up
                    bullets.push_back(shooter.shoot(15));
                    bullets.push_back(shooter.shoot(0));
                    bullets.push_back(shooter.shoot(-15));
                    Score-=400;
                }
                else if(c == 'q')
                    return 0;
            }

            // Check any collisions between the bubbles, bullets and shooter
            check_collisions(bubbles, bullets, shooter);

            // If shooter is hit change it's color
            if(shooter.shooter_hit()) shooter.change_color(COLOR(220,20,60));
            else shooter.change_color(COLOR(0,255,0));

            // Update the bubbles
            move_bubbles(bubbles);

            // Update the bullets
            move_bullets(bullets);

            // Display the counter vars on the screen
            SCORE.setMessage("Score: "+str(Score));
            TIME.setMessage("Time: "+str(Time)+"/50");
            HEALTH.setMessage("Health: "+str(Health)+"/3");

            wait(0.005);

            //Update the Time
            Time=float(clock()-start_time)/CLOCKS_PER_SEC;

        }

        //Update the Level
        Level++;

    }

    Display_progress.show();

    if((Level>3)&&(Time<=51)&&(Health>0)) {
        //Declare you have won the game
        Display_progress.setMessage("You Win!!");
        Display_progress.setColor(COLOR(0,255,0));
    }
    else {
        //Declare you have lost
        Display_progress.setMessage("Game Over");
        Display_progress.setColor(COLOR(255,0,0));
    }

    wait(10);
}
