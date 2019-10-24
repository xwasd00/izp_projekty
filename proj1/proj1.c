/*******************************************************************/
/*								   */
/*                1. projekt do IZP - Práce s textem               */
/*                     vytvořil: Michal Sova                       */
/*                        login: xsovam00                          */
/*                     vytvořeno: říjen 2018                       */
/*                                                                 */
/*******************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

#define LENGTH 1002

void help();
int goto_cmd(char* content, int* cmd_num);
int insert(char* content);
int my_remove(char* line, char* content);
int append(char* line, char* content, int line_len);
int before(char* line, char* content);
int s_pattern_replacement(char* line, char* cmd);
int quit(char* content, bool* end);
int my_switch(char* cmd, int* cmd_num, bool* newline, bool* write, bool* end, int* p_skip_lines, char* line);
int get_number(char* content);
int find_pattern(char* line, char* pattern, char* replacement, bool multi_replace);
int replace(char* line, char* pattern, char* replacement);
int clean_str(char* str);
int number_of_lines(char* content, int* p_skip_lines);
int write_or_not(char* line, bool write, int* p_skip_lines);

int main(int argc, char* argv[]){
	if(argc != 2){
		help();
		return 0;
	}
	FILE* commands;	
	char line[LENGTH];	
	int cmd_num = 0;
	char cmd[100][LENGTH];
	int skip_lines = 0;
	bool write = false;
	bool newline = false;
	bool end = false;
	if((commands = fopen(argv[1], "r")) == NULL){
		fprintf(stderr, "Nepovedlo se otevřít soubor %s.\n", argv[1]);
		return -1;
	}
	//Uloží všechny příkazy do pole cmd[][]
	for(int i = 0; i < 100; i++){
		clean_str(cmd[i]);
		if(fgets(cmd[i], LENGTH, commands) == NULL){
			continue;
		}
		//Pokud je na řádku víc znaků než kolik se načítá program skončí s chybou
		if(cmd[i][LENGTH - 1] != '\0'){
			fprintf(stderr, "Přetečení %d. řádku v souboru: %s\n", i, argv[1]);
			return -1;
		}
	}
	fclose(commands);
	//Načítá postupně řádek po řádku soubory ze vstupu
	clean_str(line);
	while(fgets(line, LENGTH, stdin) != NULL){
		if(line[LENGTH - 1] != '\0'){
			fprintf(stderr, "Přetečení řádku ve vstupu.\n");
			return -1;
		}
		//Pokud byl použit příkaz nX či dX, program vypíše příslušný počet řádků
		if(skip_lines != 0){
			write_or_not(line, write, &skip_lines);
        	}
		else{
			newline = false;
            		write = true;
			//načítá příkaz po příkazu
            		while(cmd[cmd_num][0] != '\0'){
				//Pokud byl předchozí příkaz n nebo d, vyskočí z cyklu ještě před načtením dalšího příkazu
				if(newline == true){
					break;
                		}
                		if(my_switch(&cmd[cmd_num][0], &cmd_num, &newline, &write, &end, &skip_lines, line) == -1){
			    		return -1;
			    	}
				//Při příkazu q ukončí program
                		if(end){
                    			return 0;
                		}
            		}
			//V případě n vytiskne funkce již upravený řádek ze vstupu
            		write_or_not(line, write, &skip_lines);
		}
	}
	return 0;
}



void help(){
	printf("Program se spouští příkazem: $ ./proj1 NAZEV_SOUBORU_S_PRIKAZY.txt\n");
	printf("program načítá vstup a postupně podle souboru s příkazy upravuje vstup.\n");
}


int my_switch(char* cmd, int* cmd_num, bool* newline, bool* write, bool* end, int* p_skip_lines, char* line){
	//Switch srovnává první znak z řádku příkazu
	switch(cmd[0]){
		case 'i' :
			if(insert(&cmd[1]) == -1){
				return -1;
			}
			break;

		case 'b' :
			if(before(line, &cmd[1]) == -1){
				return -1;
			}
			break;

		case 'a' :			
			if(append(&line[strlen(line) -1], &cmd[1], strlen(line)) == -1){
				return -1;
			}
			break;

		case 'd' :
			if(number_of_lines(&cmd[1], p_skip_lines) == -1){
				fprintf(stderr, "Špatně zadaný počet řádků v příkazu dX.\n");
				return -1;
			}
			*write = false;
			*newline = true;
			break;

		case 'r' :
			if (my_remove(line, &cmd[1]) == -1){
				return -1;
			}
			break;

		case 's' :
            		if(s_pattern_replacement(line, cmd) == -1){
                		return -1;
            		}
			break;

		case 'S' :
        		if(s_pattern_replacement(line, cmd) == -1){
	                	return -1;
      			}
			break;

		case 'n' :			
			if(number_of_lines(&cmd[1], p_skip_lines) == -1){
				fprintf(stderr, "Špatně zadaný počet řádků v příkazu nX.\n");
				return -1;
			}
			*newline = true;
			break;

		case 'q' :
			return quit(&cmd[1], end);
            		break;

		case 'g' :
		        if(goto_cmd(&cmd[1], cmd_num) == -1){
                		return -1;
            		}
			break;

		default :
			fprintf(stderr, "Špatně zadaný příkaz.\n");
			return -1;
			break;
	}
	(*cmd_num)++;
	return 0;
}

int write_or_not(char* line, bool write, int* p_skip_lines){
	if((*p_skip_lines) > 0){
		(*p_skip_lines)--;
	}
	if(write){
		printf("%s", line);
	}
	return 0;
}

int goto_cmd(char* content, int* cmd_num){
	int num = 0;
	num = get_number(content);
	if(num < 1){
		fprintf(stderr, "Špatně zadaný počet řádků v příkazu gX.\n");
		return -1;
	}
	*cmd_num = num -2;
	return 0;
}

//Funkce number_of_lines udává kolik řádků ze vstupu se má vytisknout v případě nX či přeskočit v případě dX
int number_of_lines(char* content, int* p_skip_lines){
	int num = 0;
	int i = 0;
	if(content[i] == '\n'){
		return 0;
	}
	num = get_number(content);
	if(num < 1){
		return -1;
	}
	*p_skip_lines = num;
	return 0;
}

int insert(char* content){
	if(content[0] == '\0'){
		fprintf(stderr, "Špatně zadaný příkaz i.\n");
		return -1;
	}
	printf("%s", content);
	return 0;
}

int my_remove(char* line, char* content){
	if(content[0] != '\n'){
		fprintf(stderr, "Špatně zadaný příkaz r.\n");
		return -1;
	}
	line[strlen(line) - 1] = '\0';	
	return 0;
}

int before(char* line, char* content){
	if(content[0] == '\0'){
		fprintf(stderr, "Špatně zadaný příkaz bCONTENT.\n");
		return -1;
	}
	if(strlen(line) + strlen(content) > (unsigned)LENGTH){
		fprintf(stderr, "Přetečení řádku v příkazu bCONTENT.\n");
		return -1;
	}
	char s[LENGTH];
	clean_str(s);
	strcpy(s, line);
	strcpy(line, content);
	strcpy(&line[strlen(content) - 1], s);
	return 0;
}

int append(char* line, char* content, int line_len){
	if(content[0] == '\0'){
		fprintf(stderr, "Špatně zadaný příkaz aCONTENT.\n");
		return -1;
	}
	if(line_len + strlen(content) > (unsigned)LENGTH){
		fprintf(stderr, "Přetečení řádku v příkazu aCONTENT.\n");
		return -1;
	}
	//Pokud vstupní řádek má znak konce řádku, tímto se nahradí
	if(line[0] == '\n'){
		strcpy(line, content);
	}
	else{
		strcpy(&line[1], content);
		line[strlen(line) - 1] = '\0';
	}
	return 0;
}

int get_number(char* content){
	int number = 0;
	int j = 0;
	while(content[j] != '\n' && content[j] != EOF){
		if(content[j] >= '0' && content[j] <= '9'){
			number = number * 10 + content[j] - '0';
			j++;
		}
		else{
			return -1;
		}
	}
	return number;
}

int s_pattern_replacement(char* line, char* cmd){
	char pattern[LENGTH];
	clean_str(pattern);
	char replacement[LENGTH];
	clean_str(replacement);
	int i = 2;
	int j = 0;
	int position = 0;
	bool multi_replace = false;
	if(cmd[0] == 'S'){
		multi_replace = true;
	}
	if(cmd[1] == '\0' || cmd[1] == '\n' || cmd[2] == '\0' || cmd[1] == '\n' || cmd[2] == cmd[1]){
		fprintf(stderr, "Špatně zadaný příkaz v s_pattern_replacement.\n");
		return -1;
	}
	//Načte pattern z příkazu
	while(cmd[i] != cmd[1] && cmd[i] != '\0' && cmd[i] != '\n'){
		pattern[j] = cmd[i];
		i++;
		j++;
	}
	if(pattern[0] == '\0'){
		fprintf(stderr, "Špatný příkaz v s_pattern_replacement:žádný pattern.\n");
		return -1;
	}
	i++;
	j = 0;
	//Načte replacement z příkazu
	while(cmd[i] != '\n' && cmd[i] != '\0'){
		replacement[j] = cmd[i];
	        i++;
 		j++;
	}
	if(replacement[0] == '\0'){
		printf("Špatný příkaz v s_pattern_replacement:žádný replacement.\n");
		return -1;
	}
	//Pokud je příkaz 'S',  vyhledá všechny pattern ve vstupním řádku a nahradí je
	if(multi_replace){
        	if(find_pattern(line, pattern, replacement, multi_replace) == -1){
			fprintf(stderr, "Přetečení řádku v S:pattern:replacement\n");
			return -1;
		}
	}
	else{
		//Funkce find_pattern vrací při chybě -1
        	position = find_pattern(line, pattern, replacement, multi_replace);
		if(position == -1){
			printf("Přetečení řádku v s:pattern:replacement nebo s_pattern_replacement");
			return -1;
		}
		//Když find_pattern najde ve vstupním řádku pattern, vrací pozici 1. znaku pattern
		//Když však funkce find_pattern nic nenajde, vrací -2
        	if(position != -2){
        		replace(&line[position], pattern, replacement);
        	}
    	}
	return 0;
}

int find_pattern(char* line, char* pattern, char* replacement, bool multi_replace){
	bool pattern_found = false;
	for(unsigned i = 0; i < strlen(line); i++){
		//Tento cyklus najde pouze 1. znak řetězce pattern ve vstupním řádku
        	if(line[i] == pattern[0]){
			//tento cyklus porovná zbytek řetězce pattern
        		for(unsigned j = 0; j < strlen(pattern); j++){
                		if(line[i + j] == pattern[j]){
                    			pattern_found = true;
                		}
				else{
                    			pattern_found = false;
                		}
            		}
            		if(pattern_found){
				if(strlen(line) + strlen(replacement) - strlen(pattern) >= (unsigned)LENGTH){
					return -1;
				}
				//V případě 'S' find_pattern rovnou vymění pattern za replacement
				//a pokračuje v hledání
                		if(multi_replace){
                			replace(&line[i], pattern, replacement);
					pattern_found = false;
					i = i + strlen(replacement) - strlen(pattern);
                		}
				//V případě 's' find_pattern vrátí pozici,
				//na které je 1. znak pattern ve vstupním řádku
                		else{
                    			return i;
                		}
            		}
        	}
	}
	return -2;
}

int replace(char* line, char* pattern, char* replacement){
	char s[LENGTH];
	clean_str(s);
	strcpy(s, &line[strlen(pattern)]);
	strcpy(line, replacement);
	strcpy(&line[strlen(replacement)], s);
	return 0;
}

int clean_str(char* str){
	for(int i = 0; i < LENGTH; i++){
		str[i] = '\0';
	}
	return 0;
}

int quit(char* content, bool* end){
	if(content[0] == '\n'){
		*end = true;
		return 0;
	}
	fprintf(stderr, "Špatně zadaný příkaz q.\n");
	return -1;
}

