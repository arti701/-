#include <stdio.h>
#include <stdlib.h>

unsigned int size;
unsigned int grid[100][100] = {{0}}; //no variable size(((
char symbols[7] = {' ', 'x', 'o', '\\', '/', '-', '|'};


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

int move(unsigned int who, unsigned int y, unsigned int x){
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

			
			
int gameloop(void){
	unsigned int x, y;
	unsigned int turn;
	unsigned short int i;
	int m_c;
	turn = 0;
	while (1) {
		printf("x:");
		scanf("%u", &x);
		printf("y:");
		scanf("%u", &y);
		if(x >= size || y >= size){
			printf("Unacceptable value \n");
			continue;
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