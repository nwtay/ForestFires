#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include "neillsimplescreen.h"
#define L 2500
/*Probability of lightning strike = 1/L */
#define PROBLIGHTNING 0.0004
#define G 250
/*Probability of tree growth = 1/G */
#define PROBGROWTH 0.004
#define GRIDWIDTH 80
#define GRIDHEIGHT 30
#define NUMITERATIONS 1000
#define WAITSECS 0.1

enum bool {false, true};
typedef enum bool bool;
enum state {empty, tree, fire};
typedef enum state state;
typedef int row;
typedef int column;

/*Initialises the grid to be empty*/
void init_array(state forest1[GRIDHEIGHT][GRIDWIDTH]);
/*Assertion tests*/
void test(void);
/*Converts the current array to the next one*/
void next_array(state forest2[GRIDHEIGHT][GRIDWIDTH]);
/*Applies model rules to each cell, returns celltype*/
state apply_rules(state forest3[GRIDHEIGHT][GRIDWIDTH], row y, column x);
/*Creates a random double that is between 0 and 1*/
double rand_dbl(void);
/*Returns true if cell (y,x) is struck by lightning,ie (rand < prob)*/
bool isstruck(double random, double probability);
/*Returns true if in 8-neighbourhood of fire cell*/
bool nearfire(state forest4[GRIDHEIGHT][GRIDWIDTH], row currY, column currX);
/*Returns true if tree grows spontatneously in cell,ie rand < prob*/
bool grows(double random, double probability);
/*Safety: true if cell is within the forest boundaries*/
bool inbounds(row y, column x);



int main(void)
{
   int iteration = 0;
   state forest[GRIDHEIGHT][GRIDWIDTH];
   srand(time(NULL));
   test();
   init_array(forest);
   neillclrscrn();
   for(iteration=0;iteration<=NUMITERATIONS;iteration++){
      neillcursorhome();
      next_array(forest);
      neillbusywait(WAITSECS);
   }
   neillfgcol(white);
   return 0;
}


void init_array(state forest1[GRIDHEIGHT][GRIDWIDTH])
{
   row y;
   column x;
   for(y=0;y<GRIDHEIGHT;y++){
      printf("\n");
      for(x=0;x<GRIDWIDTH;x++){
         forest1[y][x] = empty;
         printf(" ");
      }
   }
}

void next_array(state forest2[GRIDHEIGHT][GRIDWIDTH])
{
   row y;
   column x;
   state new[GRIDHEIGHT][GRIDWIDTH];
   printf("\n\n");
   for(y=0;y<GRIDHEIGHT;y++){
      printf("\n");
      for(x=0;x<GRIDWIDTH;x++){
         new[y][x] = forest2[y][x];
         new[y][x] = apply_rules(new,y,x);
         if(new[y][x]==fire){
            neillfgcol(red);
            printf("*");
         }
         else if(new[y][x]==tree){
            neillfgcol(green);
            printf("@");
         }
         else{
            printf(" ");
         }
         forest2[y][x] = new[y][x];
      }
   }
}

state apply_rules(state forest3[GRIDHEIGHT][GRIDWIDTH], row y, column x)
{
   state c = forest3[y][x];
   switch(c){
      case fire:
         forest3[y][x] = empty;
         break;
      case empty:
         if((grows(rand_dbl(), PROBGROWTH))){
            forest3[y][x] = tree;
         }
         else{
            forest3[y][x] = empty;
         }
         break;
      case tree:
         if((isstruck(rand_dbl(),PROBLIGHTNING))||
         (nearfire(forest3,y,x))){
            forest3[y][x] = fire;
         }
         else{
            forest3[y][x] = tree;
         }
         break;
   }
   return forest3[y][x];
}

double rand_dbl(void)
{
   return (double)rand()/(double)(RAND_MAX+1.0);
}

bool isstruck(double random, double probability)
{
   /*Logic:True when random double < probability*/
   if(random>probability){
      return false;
   }
   return true;
}

bool grows(double random, double probability)
{
   /*Logic:True when random double < probability*/
   if(random>probability){
      return false;
   }
   return true;
}

bool inbounds(row y, column x)
{
   if((y<0)||(y>=GRIDHEIGHT)||(x<0)||(x>=GRIDWIDTH)){
      return false;
   }
   return true;
}


bool nearfire(state forest4[GRIDHEIGHT][GRIDWIDTH], row currY, column currX)
{
   int cnt = 0;
   row y;
   column x;
   /*Checking if fire in 1 cell in each direction
   around middle (currenty,currentx)*/
   for(y = -1; y <=1; y ++){
      for(x = -1; x <=1; x++){
         if(inbounds(currY + y, currX + x)){
            if(forest4[currY + y][currX + x]==fire){
               cnt++;
            }
         }
      }
   }
   if(cnt == 0){
      return false;
   }
   return true;
}


void test(void)
{
   /*Creating bunch of arrays for tests*/
   state new1[GRIDHEIGHT][GRIDWIDTH];
   state gridtest[GRIDHEIGHT][GRIDWIDTH];
   state gridtest2[3][GRIDWIDTH] = {{empty,empty,empty},{tree,fire,empty},
   {tree,fire,empty}};
   state gridtest3[10][GRIDWIDTH] = {{empty,empty,tree},{empty,empty,empty},
   {fire,empty,empty}};
   state gridtest4[GRIDHEIGHT][GRIDWIDTH] = {{tree,fire,tree},
   {fire,tree,tree},{empty,tree,fire}};
   state gridtest5[GRIDHEIGHT][GRIDWIDTH];
   state gridtest6[3][GRIDWIDTH] = {{fire,fire,fire},{tree,tree,tree},
   {empty,empty,empty}};
   state gridtest7[10][GRIDWIDTH] = {{fire,fire,tree,tree,fire,tree,
   fire,tree,tree,tree}};
   int i;
   row y;
   long unsigned int n;
   /*number iterations for tree to grow at least once,
   10000 is rule of thumb*/
   long unsigned int n1 = (1/PROBGROWTH)*10000;
   /*number iterations for lightning at least once,
   10000 is rule of thumb*/
   long unsigned int n2 = (1/PROBLIGHTNING)*10000;
   column x;
   int growthcnt = 0;
   int lightningcnt = 0;
   /*Creating array full of fire cells for testing*/
   for(y=0;y<GRIDHEIGHT;y++){
      for(x=0;x<GRIDWIDTH;x++){
         gridtest5[y][x]=fire;
      }
   }
   /*Initialise tests*/
   init_array(gridtest);
   for(y=0;y<GRIDHEIGHT;y++){
      for(x=0;x<GRIDWIDTH;x++){
         assert(gridtest[y][x]==empty);
         assert(gridtest[y][x]!=tree);
         assert(gridtest[y][x]!=fire);
      }
   }
   assert(gridtest[10][20]==empty);
   assert(gridtest[0][2]==empty);
   assert(gridtest[GRIDHEIGHT-1][GRIDWIDTH-1]==empty);
   /*Checking array copy from function next_array*/
   for(y=0;y<GRIDHEIGHT;y++){
      for(x=0;x<GRIDWIDTH;x++){
         new1[y][x]=gridtest[y][x];
      }
   }
   assert(new1[10][10]==gridtest[10][10]);
   assert(new1[0][0]== gridtest[0][0]);
   assert(new1[GRIDHEIGHT-1][GRIDWIDTH-1]==
   gridtest[GRIDHEIGHT-1][GRIDWIDTH-1]);
   /*inbounds tests*/
   assert(inbounds(-1,-1)==false);
   assert(inbounds(0,0)==true);
   assert(inbounds(20,20)==true);
   assert(inbounds((GRIDHEIGHT-1),(GRIDWIDTH-1))==true);
   assert(inbounds((GRIDHEIGHT),(GRIDWIDTH))==false);
   assert(inbounds((GRIDHEIGHT+1),(GRIDWIDTH+1))!=true);
   assert(inbounds(-1,(GRIDWIDTH))==false);
   assert(inbounds(10,(GRIDWIDTH+5))==false);
   /*Tests that rand_dbl function returns
   value >=0.0**<=1.0*/
   for(i=0;i<100000;i++){
      assert((rand_dbl()>=0.0)&&(rand_dbl()<=1.0));
   }
   /*Testing that a tree grows at least 1 in n1 times*/
   for(n=0;n<n1;n++){
      if(grows(rand_dbl(),PROBGROWTH)){
         growthcnt++;
      }
   }
   assert(growthcnt>0);
   /*Testing that lightning stikres at least
   1 in n2 times*/
   for(n=0;n<n2;n++){
      if(isstruck(rand_dbl(),PROBLIGHTNING)){
         lightningcnt++;
      }
   }
   assert(lightningcnt>0);
   /*Is struck tests*/
   assert(isstruck(0.99,0.90)==false);
   assert(isstruck(0.95,0.94)==false);
   assert(isstruck(0.95,0.1)==false);
   assert(isstruck(0.8,0.9)==true);
   assert(isstruck(0.5,0.501)==true);
   assert(isstruck(0.95,0.1)!=true);
   assert(isstruck(0.00,0.1)==true);
   assert(isstruck(1.0,0.1)!=true);
   /*Spontatenous tree growth function tests*/
   assert(grows(0.90,0.90)==true);
   assert(grows(0.75,0.80)==true);
   assert(grows(0.13,0.1)==false);
   assert(grows(0.8,0.9)==true);
   assert(grows(0.1,0.101)==true);
   assert(grows(0.40,0.99)!=false);
   assert(grows(0.00,0.1)==true);
   assert(grows(1.0,0.1)!=true);
   /*nearfire tests*/
   for(y=0;y<GRIDHEIGHT-1;y++){
      for(x=0;x<GRIDWIDTH-1;x++){
         assert(nearfire(gridtest,y,x)==false);
         assert(nearfire(gridtest5,y,x)==true);
      }
   }
   assert(nearfire(gridtest2,1,1)==true);
   assert(nearfire(gridtest4,GRIDHEIGHT-2,GRIDWIDTH-2)
   ==false);
   assert(nearfire(gridtest5,0,GRIDWIDTH-1)==true);
   assert(nearfire(gridtest5,GRIDHEIGHT-1,0)==true);
   assert(nearfire(gridtest5,GRIDHEIGHT-1,GRIDWIDTH-1)
   ==true);
   assert(nearfire(gridtest6,1,0)==true);
   assert(nearfire(gridtest7,0,9)==false);
   /*Testing that apply rules fn turns fire into empty*/
   assert(apply_rules(gridtest5,0,0)==empty);
   assert(apply_rules(gridtest5,10,9)==empty);
   assert(apply_rules(gridtest2,1,1)==empty);
   assert(apply_rules(gridtest3,2,0)==empty);
   /*Testing that apply rules fn turns struck tree
   into fire*/
   for(i=0;i<10000;i++){
      if(rand_dbl()<PROBLIGHTNING){
         if(isstruck(rand_dbl(),PROBLIGHTNING)){
            assert(apply_rules(gridtest4,0,0)==fire);
            assert(apply_rules(gridtest2,1,0)==fire);
            assert(apply_rules(gridtest4,2,0)!=fire);
         }
      }
   }
   /*Testing that apply rules fn ignites tree
   in neighbourhood*/
   assert(apply_rules(gridtest7,0,7)==fire);
   assert(apply_rules(gridtest7,0,9)!=fire);
   assert(apply_rules(gridtest7,0,2)==fire);

}
