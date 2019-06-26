#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

/**
 *	Definição do tamanho de cada bloco para Alocação contígua
 *	Neste caso será de 1 bytes
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
 * lê e trata a String de entrada, dividindo comandos e parâmetros
 */
int controlBuffer(char *buffer, char **cmd, char **title, char **content);

/**
 * Retorna o conteúdo do Arquivo
 */
char* readDirectory();

/**
 * Retorna a Tabela de endereço
 */
char* readTable();

/**
 * Atualiza a tabela de endereço, indexando o novo arquivo ao final da mesma
 */
char* updateTableFile(char *title, int size);

/**
 * Pega o último bloco livre para endereçar o próximo conteúdo
 */
int getLastFree();

/**
 * Cria o Arquivo no diretório
 */
void cat(char *title, char *content);

/**
 * Lista os Aquivos do diretório com base na tabela
 */
void ls();

/**
 * Exibi o conteúdo do Arquivo, recebe apenas o título como refência
 */
void more(char *title);


// MAIN
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
	printf("## Virtual - FileSystem ##\nv0.0.2\n\n");
	printf("Options: \n- cat namefile content \n- ls \n- more namefile \n- clear \n- exit\n\n\n");
	printf("\nOBS -> Evite números no título dos Arquivos e acentos.");
	printf("\n    -> Eventualmente pode ocorrer erro por falta de memória.\n\n\n\n");
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
	bufferTmp = (char*)calloc(8000,sizeof(char));

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

	if(!strcmp(bufferTmp, ""))
		strcpy(bufferTmp, "@");

	fclose(directory);
	buffer = (char*)malloc(strlen(bufferTmp)*sizeof(char));
	strcpy(buffer, bufferTmp);
	free(bufferTmp);

	return buffer;
}

char* readTable(){
	char *table, *tableTmp, c;
	tableTmp = (char*)calloc(5000,sizeof(char));

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

	if(!strcmp(tableTmp, ""))
		strcpy(tableTmp, "[]");

	fclose(directory);
	table = (char*)malloc(strlen(tableTmp)*sizeof(char));
	strcpy(table, tableTmp);
	free(tableTmp);

	return table;
}

char* updateTableFile(char *title, int size){
	char *newTable, *newTableTmp, extQuantBlock[5], extLastFree[5];
	newTableTmp = (char*)malloc(5000*sizeof(char));

	char *table = readTable();
	sprintf(extQuantBlock, "%d", size);
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

	int lastPos = atoi(arquivoDadoSplit[1]) + atoi(arquivoDadoSplit[2]);

	return lastPos;
}

void cat(char *title, char *content){
	if(title != NULL && content != NULL){
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
		printf("Sem título ou Sem conteúdo.\n");
	}
}

/* LS */
void ls(){
	char *table = readTable();
	int i = 0, tam = strlen(table);

	while(i<tam){
		if(table[i] == '[' || table[i] == ',' || table[i] == '@' || ( table[i] >= '0' && table[i] <= '9')){}
		else if(table[i] == ']'){
			printf(" ");
		} else
			printf("%c", table[i]);

		i++;
	}
	printf("\n");
}

/* MORE */
void more(char *title){
	if(title != NULL){
		char c, *result, *token, *table = readTable(), *directory;
		directory = readDirectory();
		
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

		int i = pos-1;
		int total = pos + offset - 1;
		while(i < total){
			printf("%c", directory[i]);
			i++;
		}
		printf("\n");

	} else {
		printf("Qual arquivo?\n");
	}
}