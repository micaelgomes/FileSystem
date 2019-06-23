#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 *	Definição do tamanho de cada bloco para Alocação contígua
 *	Neste caso será de 4 bytes, para ser possível observar a organização
 *	em blocos sem precisar de um arquivo muito grande
 */
#define TAM_BLOCK 4

#define TRUE 1
#define FALSE 0
#define ERROR -1

/**
 * Exibi o nome do Programa e os possíveis comandos
 */
void info();

/**
 * Controla a execução dos comandos, além de finalizar o programa
 */
int menu(char *cmd, char *title, char *content, int size);

/**
 * Controla o buffer e a divisão do argumentos
 * Auxilia os comandos separando o título do conteúdo
 */
int controlBuffer(char *buffer, char **cmd, char **title, char **content);

void readFile();

void writeFile();

int main(){
	int flag = TRUE, statusBuffer;
	char *buffer, *cmd, *title, *content;

	info();
	while(flag){
		printf("$ ");
		fflush(stdin);
		scanf("%[^\n]s", buffer);	
		statusBuffer = controlBuffer(buffer, &cmd, &title, &content);

		if(statusBuffer){
			flag = menu(cmd, title, content, sizeof(content));
			getchar();
		} else {
			printf("Buffer ERROR\n");
			exit(ERROR);
		}

		printf("%s - %s - %s\n", cmd, title, content);
	}

	return 0;
}

void info(){
	system("clear");
	printf("## Virtual - FileSystem ##\n\n");
	printf("Options: \n- cat \n- ls \n- more \n- clear \n- exit\n\n\n");
	printf("Press ENTER key to continue...\n");
	getchar();
	system("clear");
}

int menu(char *cmd, char *title, char *content, int size){
	int flag = TRUE;

	if(!strcmp(cmd,"exit"))
		flag = FALSE;

	else if(!strcmp(cmd,"clear"))
		system("clear");

	else if(!strcmp(cmd,"cat"))
		printf("cat not yet\n");
	
	else if(!strcmp(cmd,"ls"))
		printf("ls not yet\n");

	else if(!strcmp(cmd,"more"))
		printf("more not yet\n");

	else 
		printf("ERRO:: Command Not Found:: %s\n", cmd);

	return flag;	
}	

int controlBuffer(char *buffer, char **cmd, char **title, char **content){
	if(buffer != NULL){
		const char delimiter[2] = " ";
		const char end[2] = "\0";
		char *token;

		token = strtok(buffer, delimiter);
		*cmd = token;

		token = strtok(NULL, delimiter);
		*title = token;

		token = strtok(NULL, end);
		*content = token;

		return TRUE;
	}

	return FALSE;
}
