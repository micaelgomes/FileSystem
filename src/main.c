#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

/**
 *	Definição do tamanho de cada bloco para Alocação contígua
 *	Neste caso será de 1 bytes, para ser possível observar a organização
 *	em blocos sem precisar de um arquivo muito grande
 */
#define TAM_BLOCK 1

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
int menu(char *cmd, char *title, char *content);

/**
 * Controla o buffer e a divisão do argumentos
 * Auxilia os comandos separando o título do conteúdo
 */
int controlBuffer(char *buffer, char **cmd, char **title, char **content);

char* readDirectory();

char* readTable();

char* updateTableFile(char *title, int size);

int getLastFree();

void cat(char *title, char *content);

void ls();

void more(char *title){
	if(title != NULL){
		char c, *result, *token, *table = readTable(), *directory = readDirectory();
		int pos, offset;

		if(table != NULL)
			result = strstr(table, title);

		if(!(result != NULL)){
			printf("Arquivo não encontrado\n");
			return;
		}

		token = strtok(result, ",");
		token = strtok(NULL, ",");
		pos = atoi(token);
		token = strtok(NULL, "]");
		offset = atoi(token);

		// printf("pos: %d off: %d\n", pos, offset);
		// printf("%s\n", directory);

		int i = pos;
		while(i<=offset){
			printf("%c", directory[i]);
			i++;
		}
		printf("\n");

	} else {
		printf("Qual arquivo?\n");
	}
}

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
			flag = menu(cmd, title, content);
			getchar();
		} else {
			getchar();
		}

		strcpy(buffer, "");
	}

	return 0;
}

void info(){
	system("clear");
	printf("## Virtual - FileSystem ##\n\n");
	printf("Options: \n- cat namefile content \n- ls \n- more namefile \n- clear \n- exit\n\n\n");
	printf("Press ENTER key to continue...\n");
	getchar();
	system("clear");
}

int menu(char *cmd, char *title, char *content){
	int flag = TRUE;

	if(!strcmp(cmd,"exit"))
		flag = FALSE;

	else if(!strcmp(cmd,"clear"))
		system("clear");

	else if(!strcmp(cmd,"cat"))
		cat(title, content);
	
	else if(!strcmp(cmd,"ls"))
		ls();

	else if(!strcmp(cmd,"more"))
		more(title);

	else 
		printf("ERRO:: Command Not Found:: %s\n", cmd);

	return flag;	
}	

int controlBuffer(char *buffer, char **cmd, char **title, char **content){
	if(strlen(buffer)){
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

/* CAT */
char* readDirectory(){
	char *buffer, *bufferTmp, c;
	bufferTmp = (char*)malloc(100000*sizeof(char));

	FILE *directory = fopen("HD/directory", "r");

	int i = 0, flag = FALSE;
	while((c = fgetc(directory)) != EOF){
		if(c == '@')
			flag = TRUE;
		if(flag){
    		bufferTmp[i] = c;
    		i++;
		}
	}

	fclose(directory);
	buffer = (char*)malloc(strlen(bufferTmp)*sizeof(char));
	strcpy(buffer, bufferTmp);
	free(bufferTmp);

	return buffer;
}

char* readTable(){
	char *table, *tableTmp, c;
	tableTmp = (char*)malloc(1000*sizeof(char));

	FILE *directory = fopen("HD/directory", "r");

	int i = 0, flag = TRUE;
	while((c = fgetc(directory)) != EOF){
		if(c == '@')
			flag = FALSE;
		if(flag){
    		tableTmp[i] = c;
    		i++;
		}
	}

	fclose(directory);
	table = (char*)malloc(strlen(tableTmp)*sizeof(char));
	strcpy(table, tableTmp);
	free(tableTmp);

	return table;
}

char* updateTableFile(char *title, int size){
	char *newTable, *newTableTmp, extQuantBlock[2], extLastFree[2];
	newTableTmp = (char*)malloc(10000*sizeof(char));

	char *table = readTable();
	int quantBlock = size;
	sprintf(extQuantBlock, "%d", quantBlock);
	sprintf(extLastFree, "%d", getLastFree());

	strcpy(newTableTmp, "[");
	strcat(newTableTmp, title);
	strcat(newTableTmp, ",");
	strcat(newTableTmp, extLastFree);
	strcat(newTableTmp, ",");
	strcat(newTableTmp, extQuantBlock);
	strcat(newTableTmp, "]");

	int tamTable = strlen(newTableTmp) + strlen(table);
	newTable = (char*)malloc(tamTable*sizeof(char));
	
	strcpy(newTable, table);	
	strcat(newTable, newTableTmp);
	free(newTableTmp);

	return newTable;
}

int getLastFree(){
	char *table = readTable();
	char *token, *arquivo;
	char **arquivoDadoSplit = (char **) malloc(3 * sizeof(char *));
	if (!arquivoDadoSplit)
		return ERROR;

	int i = 0;
	token = strtok(table, "[");
	while (token != NULL){
		arquivo = token;
		token = strtok(NULL, "[");
	}

	arquivo[strlen(arquivo) - 1] = '\0';

	i = 0;
	token = strtok(arquivo, ",");
	while (token != NULL){
		arquivoDadoSplit[i++] = token;
		token = strtok(NULL, ",");
	}

	return atoi(arquivoDadoSplit[1]) + atoi(arquivoDadoSplit[2]);
}

void cat(char *title, char *content){
	if(title != NULL){
		char *newBuffer;
		char *table = updateTableFile(title, strlen(content));
		char *buffer = readDirectory();

		int tamBuffer = strlen(table) + strlen(buffer) + strlen(content);
		newBuffer = (char*)malloc(tamBuffer*sizeof(char));

		strcpy(newBuffer, table);
		strcat(newBuffer, buffer);
		strcat(newBuffer, content);

		FILE *directory = fopen("HD/directory", "w");
		fwrite(newBuffer, sizeof(char), strlen(newBuffer), directory);
		fclose(directory);
	} else {
		printf("Sem título.\n");
	}
}

/* LS */
void ls(){
	char *table = readTable();
	int i = 0, tam = strlen(table);

	while(i<tam){
		if(table[i] == '[' || table[i] == ',' || ( table[i] >= '0' && table[i] <= '9')){}
		else if(table[i] == ']')
			printf(" ");
		else
			printf("%c", table[i]);

		i++;
	}
	printf("\n");
}

/* MORE */
