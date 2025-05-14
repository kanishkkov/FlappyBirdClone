//Pre-processor directives
#include <FEHLCD.h>
#include <FEHRandom.h>
#include <FEHSD.h>
#include <FEHUtility.h>
#include <FEHImages.h>
#include <fstream>
using namespace std;
//Method prototypes
void drawMainMenu();
void displayStats(int highScore);
void displayInstructions();
void displayCredits();
void displayGameOver(int score);
void loadingGame();
int displayGameMode();
//Creates instances of an object Bird
//Author: Kanishk Kovuru
class Bird
{
 private:
 //xCoord of Bird
 int xCoord;
 //yCoord of Bird
 int yCoord;
 //radius of Bird
 int radius;
 public:
 //Bird Class Constructor
 Bird(int x, int y, int r);
 //Displays Bird
 void display();
 //Up and Down movement of code
 void movement(float g, float *v);
 //Accessor Method
 int getYcoord();
};
//Creates instances of an object Pole
//Author: Charles Cao
class Pole
{
 public:
 //Constructor
 Pole(float Htop, float V);
 void DrawWhite();
 void DrawBlack();

 //distance from top pole and bottom pole
 float gap;
 //height of top pole
 float Htop;
 //height of bottom pole
 float Hbottom;
 //width of poles
 float width;
 //Change in xCoord of poles
 float V;
};
//Constructor
//Author: Kanishk Kovuru
Bird :: Bird(int x, int y, int r)
{
 xCoord = x;
 yCoord = y;
 radius = r;
}
//Displays Bird
//Author: Kanishk Kovuru
void Bird :: display()
{
 LCD.SetFontColor(PURPLE);
 LCD.FillCircle(xCoord, yCoord, radius);
}
//Gravity and Up movement of Bird
//Author: Charles Cao and Kanishk Kovuru
void Bird :: movement(float gravity, float *velocity)
{

 float x,y;
 float *velocityAddress = velocity;

 //Erases previous location of bird
 LCD.SetFontColor(WHITE);
 LCD.FillCircle(xCoord, yCoord, radius);


 if (!LCD.Touch(&x, &y))
 {
 //Bird goes down if no touch
 *velocityAddress += gravity;
 }
 else if (LCD.Touch(&x, &y))
 {
 //Bird goes up if touch
 *velocityAddress = -1.3;
 }
 //Changes yCoord attribute of Bird
 yCoord += *velocityAddress;

 //Displays current location of bird
 LCD.SetFontColor(PURPLE);
 display();


 Sleep(0.008);


}
//Accessor method to use yCoord in main function
//Author: Kanishk Kovuru
int Bird :: getYcoord(){
 return yCoord;
}
//Constructor for Pole
//Author: Kanishk Kovuru
Pole :: Pole (float topHeight, float VTemp)
{
 gap = 60;
 Htop = topHeight;
 Hbottom = 180 - topHeight;
 width = 20;
 V = VTemp;
}
//Draws Pole
//Author: Charles Cao
void Pole::DrawBlack()
{
 LCD.SetFontColor(BLACK);
 LCD.FillRectangle(300-V, 0, width, Htop);
 LCD.FillRectangle(300-V, Htop+gap, width, Hbottom);
}
//Draws over previous Pole
//Author: Charles Cao
void Pole::DrawWhite()
{

 LCD.SetFontColor(WHITE);
 LCD.FillRectangle(300-V, 0, width, Htop);
 LCD.FillRectangle(300-V, Htop+gap, width, Hbottom);
}
//Author: Kanishk Kovuru and Charles Cao
int main(void)
{
 int highScore = 0;
 int poleSpeed = 0;

 while(1){
 int x, y, xTwo, yTwo;
 int mainMenuExit = 0;
 int gameScore = 0;
 int *scoreAddress = &gameScore;
 while (mainMenuExit == 0)
 {
 //Draw Main Menu
 drawMainMenu();
 LCD.Update();
 //Test for Touch
 while (!LCD.Touch(&x, &y))
 {}
 while (LCD.Touch(&xTwo, &yTwo))
 {}
 //Detect where user touches the screen
 if (((x > 80) && (x < 240)) && ((y > 45) && (y < 75)))
 {
 LCD.Clear();
 mainMenuExit = 1;
 poleSpeed = displayGameMode();
 }
 else if (((x > 80) && (x < 240)) && ((y > 85) && (y < 115)))
 {

 //Iterates through file to find high Score
 FILE *scoresRead;
 scoresRead = fopen("scoreList.txt", "r");
 int score;
 while (fscanf (scoresRead, "%i", &score) != EOF){
 if (score > highScore){
 highScore = score;
 }
 }
 fclose(scoresRead);
 displayStats(highScore);
 }
 else if (((x > 80) && (x < 240)) && ((y > 125) && (y < 155)))
 {
 displayInstructions();
 }
 else if (((x > 80) && (x < 240)) && ((y > 165) && (y < 195)))
 {
 displayCredits();
 }
 }
 loadingGame();
 //Set game values
 float gravity = 0.2;
 float velocity = 0;
 float *addressVelocity = &velocity;
 int heightTop = rand() % (120 - 60 + 1) + 75;
 int disp = 0;


 LCD.Clear(WHITE);

 //Original location of Bird
 Bird flap(100, 160, 7);
 flap.display();
 Pole newPole(heightTop, disp);
 //Timer used for scoring
 int start = time(NULL);
 bool gameOver = true;

 //Author: Charles Cao
 while ((flap.getYcoord() < 225) && (flap.getYcoord() > 25) && gameOver)
 {
 //Test if Bird and Pole hit
 if ((300 - newPole.V < 107) && (300 - newPole.V > 73)){
 if (((flap.getYcoord()-7) < newPole.Htop) ||(flap.getYcoord()+7) >
(newPole.Htop + newPole.gap)){
 gameOver = false;
 }
 LCD.SetFontColor(PURPLE);
 LCD.WriteAt("SCORE:", 10, 80);
 LCD.WriteAt(gameScore, 20, 100);
 flap.movement(gravity, addressVelocity);
 LCD.SetFontColor(WHITE);
 LCD.WriteAt(gameScore, 20, 100);
 }
 // Movement of Pole
 if (newPole.V < 300){
 LCD.SetFontColor(PURPLE);
 LCD.WriteAt("SCORE:", 10, 80);
 LCD.WriteAt(gameScore, 20, 100);
 flap.movement(gravity, addressVelocity);
 LCD.SetFontColor(WHITE);
 LCD.WriteAt(gameScore, 20, 100);
 gameScore = time(NULL) - start;
 newPole.DrawBlack();
 Sleep(0.01);
 newPole.DrawWhite();
 newPole.V += poleSpeed;
 }
 // Create new Pole when pole gets to left edge
 else{
 newPole.Htop = rand() % (120 - 60 + 1) + 75;
 newPole.Hbottom = 180 - newPole.Htop;
 newPole.V = 0;
 LCD.SetFontColor(PURPLE);
 LCD.WriteAt("SCORE:", 10, 80);
 LCD.WriteAt(gameScore, 20, 100);
 flap.movement(gravity, addressVelocity);
 LCD.SetFontColor(WHITE);
 LCD.WriteAt(gameScore, 20, 100);
 gameScore = time(NULL) - start;
 }
 
 }

 //Write Score to file
 //Author: Kanishk Kovuru
 ofstream scoresWrite;
 scoresWrite.open("scoreList.txt");
 scoresWrite << gameScore << endl;
 scoresWrite.close();

 //Display game over screen
 displayGameOver(gameScore);
 }
 while(1) {
 LCD.Update();
 }
}
//Draw Main Menu Screen
//Author: Kanishk Kovuru
void drawMainMenu()
{
 //Clear screen to black
 LCD.Clear(BLACK);
 //Add Title of Game
 LCD.SetFontColor(CYAN);
 LCD.WriteAt("FLAPPY BIRD", 95, 25);
 //Add "Play" Menu System
 LCD.SetFontColor(WHITE);
 LCD.DrawRectangle(80, 45, 160, 30);
 LCD.SetFontColor(GREEN);
 LCD.WriteAt("Play", 135, 53);
 //Add "Statistics" Menu System
 LCD.SetFontColor(WHITE);
 LCD.DrawRectangle(80, 85, 160, 30);
 LCD.SetFontColor(GREEN);
 LCD.WriteAt("Stats", 130, 93);
 //Add "Instructions" Rectangle
 LCD.SetFontColor(WHITE);
 LCD.DrawRectangle(80, 125, 160, 30);
 LCD.SetFontColor(GREEN);
 LCD.WriteAt("Instructions", 87, 133);
 //Add "Credits" Rectangle
 LCD.SetFontColor(WHITE);
 LCD.DrawRectangle(80, 165, 160, 30);
 LCD.SetFontColor(GREEN);
 LCD.WriteAt("Creds", 130, 173);
}
//Display Stats Screen
//Author: Kanishk Kovuru
void displayStats(int highScore)
{
 LCD.Clear(BLACK);
 LCD.SetFontColor(WHITE);
 LCD.WriteAt("Your High Score: ", 60, 20);
 LCD.WriteAt(highScore, 160, 50);
 LCD.SetFontColor(GREEN);
 LCD.DrawRectangle(85, 205, 150, 25);
 LCD.WriteAt("Return", 125, 210);
 LCD.Update();
 //Testing touch on return button
 int x, y, xTwo, yTwo;
 int leave = 0;
 while (leave == 0)
 {
 while (!LCD.Touch(&x, &y))
 {}
 while (LCD.Touch(&xTwo, &yTwo))
 {}
 if ((x > 85 && x < 235) && (y > 205 && y < 230))
 {
 leave = 1;
 }
 }
}
//Display Instructions Screen
//Author: Kanishk Kovuru
void displayInstructions()
{
 LCD.Clear(BLACK);
 LCD.SetFontColor(WHITE);
 LCD.WriteAt("Click = Up", 60, 20);
 LCD.WriteAt("Release = Down", 60, 40);
 LCD.WriteAt("Hit Pipe = End", 60, 60);
 LCD.WriteAt("Hit top or bottom = End", 30, 80);
 LCD.WriteAt("Each Second = +1", 50, 100);

 LCD.SetFontColor(GREEN);
 LCD.DrawRectangle(85, 205, 150, 25);
 LCD.WriteAt("Return", 125, 210);
 LCD.Update();
 //Testing touch on return button
 int x, y, xTwo, yTwo;
 int leave = 0;
 while (leave == 0)
 {
 while (!LCD.Touch(&x, &y))
 {}
 while (LCD.Touch(&xTwo, &yTwo))
 {}
 if ((x > 85 && x < 235) && (y > 205 && y < 230))
 {
 leave = 1;
 }
 }
}
//Displays Credits Screen
//Author: Kanishk Kovuru
void displayCredits()
{
 LCD.Clear(BLACK);
 LCD.SetFontColor(WHITE);
 LCD.WriteAt("Developers:", 60, 20);
 LCD.WriteAt("Kanishk Kovuru", 60, 40);
 LCD.WriteAt("Charles Cao", 60, 60);
 LCD.WriteAt("References:", 60, 100);
 LCD.WriteAt("FEH Proteus Guide", 60, 120);

 LCD.WriteAt("Return", 125, 210);
 LCD.Update();
 LCD.SetFontColor(GREEN);
 LCD.DrawRectangle(85, 205, 150, 25);
 LCD.WriteAt("Return", 125, 210);
 LCD.Update();
 //Testing touch on return button
 int x, y, xTwo, yTwo;
 int leave = 0;
 while (leave == 0)
 {
 while (!LCD.Touch(&x, &y))
 {}
 while (LCD.Touch(&xTwo, &yTwo))
 {}
 if ((x > 85 && x < 235) && (y > 205 && y < 230))
 {
 leave = 1;
 }
 }
}
//Displays game over screen
//Author: Kanishk Kovuru
void displayGameOver(int gameScore){
 LCD.Clear(BLACK);
 LCD.SetFontColor(WHITE);
 LCD.WriteAt("GAME OVER", 100, 100);
 LCD.WriteAt("Score: ", 90, 130);
 LCD.WriteAt(gameScore, 165, 130);
 Sleep(3.0);
}
//Displays a loading screen
//Author: Kanishk Kovuru
void loadingGame(){
 LCD.Clear(BLACK);
 LCD.SetFontColor(WHITE);
 LCD.WriteAt("Loading...", 100, 100);
 LCD.DrawRectangle(100, 130, 115, 10);
 int x = 0;
 int left = 100;
 Sleep(1.0);
 //Loading Bar
 while (x <= 22)
 {
 LCD.FillRectangle(left, 130, 5, 10);
 left+=5;
 x++;
 Sleep(0.2);
 }
 Sleep(0.5);
}
//Display game Mode
//Author: Kanishk Kovuru
int displayGameMode()
{
 LCD.Clear(BLACK);
 LCD.SetFontColor(GREEN);
 LCD.DrawRectangle(85, 170, 150, 25);
 LCD.WriteAt("EASY", 135, 175);

 LCD.SetFontColor(RED);
 LCD.DrawRectangle(85, 205, 150, 25);
 LCD.WriteAt("HARD", 135, 210);
 LCD.Update();
 //Testing touch on Easy or Hard button
 int x, y, xTwo, yTwo;
 int leave = 0;
 while (leave == 0)
 {
 while (!LCD.Touch(&x, &y))
 {}
 while (LCD.Touch(&xTwo, &yTwo))
 {}
 if ((x > 85 && x < 235) && (y > 205 && y < 230))
 {
 return 6;
 }
 else if ((x > 85 && x < 235) && (y > 170 && y < 195))
 {
 return 2;
 }
 }
}
