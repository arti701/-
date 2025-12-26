#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>

unsigned int size;
unsigned int grid[100][100] = {{0}}; //no variable size(((
char symbols[7] = {' ', 'x', 'o', '\\', '/', '-', '|'};
char player[2] = {0, 1};
int bot_ramdonness = 40;


int render(void){
	unsigned short int i;
	unsigned short int j;
	unsigned int a;
	printf("  ");
	for(a = 0; a < size; a++){
		if (a<10) {printf(" %u", a);}
		else {printf("%u", a);}}
	printf("\n");
	for(i = 0; i < size; i++){
		printf("  ");
		for(a = 0; a < size; a++){
			printf("._");
			}
		if (i<10) {printf(".\n %u", i);}
		else {printf(".\n%u", i);}
		for(j = 0; j < size; j++){
			printf("|%c", symbols[grid[i][j]]);
			}
		printf("|\n");
		}
	printf("  ");
	for(a = 0; a < size; a++){
			printf("._");
			}
	printf(". \n");
	return 0;
	}

int move(unsigned int who, unsigned int y, unsigned int x){ //x and y swapped so it looks more intuitive, should probably switch to letter system but oh well there's only so much letters
	unsigned short int a;
	unsigned short int b;
	unsigned char has_won;
	//input check returned value being > 100 means error
	if(x >= size || y >= size) return 101; //wrong coords
	if(who != 1 && who != 2) return 102; // wrong person
	if(grid[x][y] != 0) return 103; // spot taken
	grid[x][y] = who;
	//win condition check, 1 or 2 means win
	if(x == y){//diagonal topleft
		has_won = 1;
		for(a = 0; a < size; a++){
			if(grid[a][a] != who) has_won = 0;
			}
		if(has_won) return who + 10; //using base 10 for readability, we store winstate in each digit to render crosslines
		}
	if(x == size - y - 1){//diagonal bottomleft
		has_won = 1;
		for(a = 0; a < size; a++){
			if(grid[a][size - a - 1] != who) has_won = 0;
			}
		if(has_won) return who + 20;
		}
	// not on a diagonal
	//check vertical
	has_won = 1;
	for(a = 0; a < size; a++){
			if(grid[x][a] != who) has_won = 0;
		}
	if(has_won) return who + 30;
	//check horizontal
	has_won = 1;
	for(a = 0; a < size; a++){
			if(grid[a][y] != who) has_won = 0;
		}
	if(has_won) return who + 40;
	// no win detected then, oh well,l let's check for a tie
	has_won = 1; //there really is no need for a new variable
	for(a = 0; a < size; a++){
		for(b = 0; b < size; b++){
			if(grid[a][b] == 0){has_won = 0;}
			}
		}
	if(has_won) return 50;//means no spaces left, a tie
	
	return 0;
	}


unsigned int bot_move(unsigned int who){ //bot only cares about the current moment and calculates short term best point,, i think game's more fun that way
	unsigned char a; //direction, used to iterate through the surroundings of the iterated spot
	int x_dir; // x + direction, may be negative(although we woild want it to)
	int y_dir; //y + direction
	unsigned int x, y; // iterated spot
	unsigned int saved_spot; // currently chosen spot
	short int pref, saved_pref; // how preferable iterated/chosen spot is, used to campare them
	saved_spot = 0; // hopefully it will change, i'm not sure how it will be handled
	saved_pref = -1;
	for(x=0; x < size; x++){
		for(y=0;y<size; y++){
			pref = 0; //restart pref for this spot
			for(a=0; a<8; a++){
				x_dir = x - 1 + a%3;
				if(a<3){//top row
					y_dir = y - 1;}
				else if (a > 6){//bottom row
					y_dir = y + 1;}
				else if(a == 5){//middle spot, we don't care
					continue;}
				else{y_dir = y;}//middle row
				if(x_dir < 0 || x_dir >= size || y_dir < 0 || y_dir >= size){//out of reach
					continue;}
				if(grid[x_dir][y_dir] == who){pref += 2;}// bot likes when there are it's symbols nearby
				else if(grid[x_dir][y_dir] == (who + 1) % 2){pref += 1;}// it also likes to block other's move, but less
				}//we are done with iterating this spot's sirroundings, let's see if we like it
			if((saved_pref <= pref && rand()%100 < bot_ramdonness) || grid[saved_spot / 100][saved_spot % 100] !=0){//we may randomly chose a different, equally preferable spot, so there's less bias towards leftmost corner, also don't choose the taken spot
				saved_pref = pref;
				saved_spot = x * 100 + y;}//we need to store two values in one
			}
		}
	return saved_spot;
	}
			
			
int gameloop(void){
	unsigned int x, y;
	unsigned int turn;
	unsigned short int i;
	int m_c;
	unsigned int b_m;
	turn = 0;
	srand(time(NULL));
	while (1) {
		if(player[turn%2] == 0){
			printf("x:");
			scanf("%u", &x);
			printf("y:");
			scanf("%u", &y);
			if(x >= size || y >= size){
				printf("Unacceptable value \n");
				continue;}
			}
		else{
			b_m = bot_move(turn);
			x = b_m / 100;
			y = b_m % 100;
			printf("bot has chosen %u %u \n", x, y);
			}
		
		// printf("%u %u %u \n", x, y, turn);
		m_c = move(turn % 2 + 1, x, y);
		if(m_c > 100){
			if(m_c == 103){
				printf("Input error: spot already taken \n");
				}
			else {printf("This error really shouldn't have happened... code %d \n", m_c);}
			}
		else if(m_c > 5){
			if(m_c / 10 == 1){
				for(i=0; i < size; i++){
					grid[i][i] = 3;
					}
				}
			else if(m_c / 10 == 2){
				for(i=0; i < size; i++){
					grid[i][size - i - 1] = 4;
					}
				}
			else if(m_c / 10 == 3){
				for(i=0; i < size; i++){
					grid[x][i] = 5;
					}
				}
				
			else if(m_c / 10 == 4) {
				for(i=0; i < size; i++){
					grid[i][y] = 6;
					}
				}
			else {printf("A DRAW(tie)!!!!!"); render(); break;}
			render();
			printf("%c has won!!!!!! yayyy", symbols[turn%2+1]);
			break;
			}
		else {turn++; render();}
		}
	return 0;
	}

	
int main(void){
	printf("Input size:");
	scanf("%d", &size);
	render();
	printf("pro tip: x is horizontal, y is verical \n");
	gameloop();
	return 0;}