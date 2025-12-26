#include <stdio.h>

int size = 3;
int grid[100][100] = {{0}}; //no variable size(((
char symbols[7] = {' ', 'x', 'o', '\\', '/', '-', '|'};
int player[2] = {0, 1};


int render(void){
	 short int i;
	 short int j;
	 int a;
	printf("\n  ");
	for(a = 0; a < size; a++){
		if (a<10) {printf(" %d", a);}
		else {printf("%d", a);}}
	printf("\n");
	for(i = 0; i < size; i++){
		printf("  ");
		for(a = 0; a < size; a++){
			printf("._");
			}
		if (i<10) {printf(".\n %d", i);}
		else {printf(".\n%d", i);}
		for(j = 0; j < size; j++){
			printf("|%c", symbols[grid[i][j]]);
			}
		printf("|\n");
		}
	printf("  ");
	for(a = 0; a < size; a++){
			printf("._");
			}
	printf(". \n \n");
	return 0;
	}

int move( int who,  int x,  int y){ //x and y swapped so it looks more intuitive, should probably switch to letter system but oh well there's only so much letters <<< WHY DID I DO THIS I"M GONNA KMS I WAS SO FLUFFING ENRAGED WHEN IT HAPPENED (and now it works perfectly???
	 short int a;
	 short int b;
	 char has_won;
	//input check returned value being > 100 means error
	if(x >= size || y >= size) return 101; //wrong coords
	if(who != 1 && who != 2) return 102; // wrong person
	if(grid[x][y] != 0) {return 103;} // spot taken
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
	//check vertical?
	has_won = 1;
	for(a = 0; a < size; a++){
			if(grid[x][a] != who) has_won = 0;
		}
	if(has_won) return who + 30;
	//check horizontal?
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


int bot_move( int who){ //bot only cares about the current moment and calculates short term best point,, i think game's more fun that way
	 char a; //direction, used to iterate through the surroundings of the iterated spot
	int x_dir, x_l_dir; // x + direction, may be negative(although we woild want it to)
	int y_dir, y_l_dir; //y + direction
	 int x, y; // iterated spot
	 int saved_spot; // currently chosen spot
	short int pref, saved_pref; // how preferable iterated/chosen spot is, used to campare them
	saved_spot = 0; // hopefully it will change, i'm not sure how it will be handled
	saved_pref = -1;
	for(x=0; x < size; x++){
		for(y=0;y<size; y++){
			pref = 0; //restart pref for this spot
			for(a=0; a<8; a++){
				y_dir = y - 1 + a%3;
				y_l_dir = y + 2*(y + y_dir);
				if(a<3){//top row
					x_dir = x - 1;
					x_l_dir = x - 2;}
				else if (a > 5){//bottom row
					x_dir = x + 1;
					x_l_dir = x + 2;}
				else if(a == 4){//middle spot, we don't care
					continue;}
				else{x_dir = x;}//middle row
				//printf("a a is %d xd is %d yd is %d xld is %d yld is %d x is %d y is %d\n", a, x_dir, y_dir, x_l_dir, y_l_dir, x, y);
				if(x_dir < 0 || x_dir >= size || y_dir < 0 || y_dir >= size){//out of reach
					continue;}
				if(x_l_dir < 0 || x_l_dir >= size){x_l_dir = x;}
				if(y_l_dir < 0 || y_l_dir >= size){y_l_dir = y;}
				//printf("b a is %d xd is %d yd is %d xld is %d yld is %d\n", a, x_dir, y_dir, x_l_dir, y_l_dir);
				if(grid[x_dir][y_dir] == who){pref += 1;// bot likes when there are its symbols nearby
				if(grid[x_l_dir][y_l_dir] == who){pref += 2;}} //it's better in a row
				else if(grid[x_dir][y_dir] == ((who + 1) % 2)+1){pref += 1;// it also likes to block other's move
				if(grid[x_l_dir][y_l_dir] == ((who + 1) % 2)+1){pref += 2;}}
				//printf(" pref is %d\n", pref);
				}//we are done with iterating this spot's surroundings, let's see if we like it
			if(saved_pref < pref || grid[saved_spot / 100][saved_spot % 100] !=0){
				//printf("pref saved\n");
				saved_pref = pref;
				saved_spot = x * 100 + y;}//we need to store two values in one int
			}
		}
	// printf("grid value %d x %d y %d\n", grid[saved_spot/100][saved_spot%100], saved_spot/100, saved_spot%100);
	if(grid[saved_spot/100][saved_spot%100] !=0){
		printf("Entered failstate");
		for(x=0; x<size; x++){
			for(y=0; y<size; y++){
				if(grid[x][y] == 0) return x * 100 + y;
			}
		}
	}
	// printf("pref is %d\n", saved_pref);
	return saved_spot;
	}
			
			
int settings(void){
	int input_1 = 0;
	printf("Current settings are: \n\t Players: ");
	if(player[0] == 0){
		printf(" human vs");}
	else printf(" bot vs");
	if(player[1] == 0){
		printf(" human \n\t");}
	else printf(" bot \n\t");
	printf("Grid size is %d \n\t", size);
	printf("\n Want to change the settings? Input \"1\" to do so or \"0\" to play\n");
	scanf("%d", &input_1);
	if(input_1 == 1){
		
		printf("\tPlayer 1 is a human(0) or a bot(1)? \n\t");
		scanf("%d", &input_1);
		while(input_1 != 0 && input_1 != 1){
			printf("\tWrong value \n\t");
			scanf("%d", &input_1);
		}
		player[0] = input_1;
		
		printf("\tPlayer 2 is a human(0) or a bot(1)? \n\t");
		scanf("%d", &input_1);
		while(input_1 != 0 && input_1 != 1){
			printf("\tWrong value \n\t");
			scanf("%d", &input_1);
		}
		player[1] = input_1;
		
		printf("\tWhat should the size of the grid be? (2-100) \n\t");
		scanf("%d", &input_1);
		while(input_1 < 2 || input_1 > 100){
			printf("\tWrong value \n\t");
			scanf("%d", &input_1);
		}
		size = input_1;
		
	}
	else{
	return 0;}
}
			
			
			
int gameloop(void){
	 int x, y;
	 int turn;
	 short int i;
	int m_c;
	 int b_m;
	turn = 0;
	while (1) {
		if(player[turn%2] == 0){
			printf("x:");
			scanf("%d", &x);
			printf("y:");
			scanf("%d", &y);
			if(x >= size || y >= size){
				printf("Unacceptable values \n");
				continue;}
			}
		else{
			b_m = bot_move(turn);
			x = b_m/100;
			y = b_m%100;
			printf("bot (%d) has chosen spot %d %d \n", turn%2 + 1, x, y);
			}
		
		// printf("%d %d %d \n", x, y, turn);
		m_c = move(turn % 2 + 1, x, y);
		if(m_c > 100){
			if(m_c == 103){
				printf("Input error: spot already taken \n");
				//printf("FKLJSH %d %d\n",grid[b_m/100][b_m%100], b_m);
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
	render();
	printf("pro tip: x is vertical, y is horizontal \n");
	gameloop();
	return 0;}
