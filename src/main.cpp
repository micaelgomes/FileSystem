#include <iostream>
#include <fstream> 	
#include <string>
#include <cmath>

/**
 *	Definição do tamanho de cada bloco para Alocação contígua
 *	Neste caso será de 4 bytes, para ser possível observar a organização
 *	em blocos sem precisar de um arquivo muito grande
 */
#define TAM_BLOCK 4

using namespace std;

/**
 * Exibi o nome do Programa e os possíveis comandos
 */
void info();

/**
 * Controla a execução dos comandos, além de finalizar o programa
 */
bool menu(string cmd, string title, string content, int size);

/**
 * Controla o buffer e a divisão do argumentos
 * Auxilia os comandos separando o título do conteúdo
 */
bool controlBuffer(string buffer, string *cmd, string *title, string *content);

bool cat(string title, string content, int size){
	fstream directory;
	directory.open("HD/directory", fstream::in | fstream::out | fstream::app);
	directory << title << ceil(size/TAM_BLOCK) << content << endl;
	directory.close();

	return true;
}

int main(){
	bool flag = true, statusBuffer;
	string buffer, cmd, title, content;

	info();
	while(flag){
		cout << "$ ";
		getline(cin, buffer);
		statusBuffer = controlBuffer(buffer, &cmd, &title, &content);

		if(statusBuffer)
			flag = menu(cmd, title, content, content.length()); 
	}

	return 0;
}

void info(){
	system("clear");
	cout << "## Virtual - FileSystem ##" << endl;
	cout << "Options: cat, ls & more\n" << endl;
}

bool menu(string cmd, string title, string content, int size){
	bool flag = true;

	if(!cmd.compare("exit"))
		flag = false;

	else if(!cmd.compare("clear"))
		system("clear");

	else if(!cmd.compare("cat"))
		cat(title, content, size);
	
	else if(!cmd.compare("ls"))
		cout << "not Yet" << endl;

	else if(!cmd.compare("more"))
		cout << "not Yet" << endl;

	else 
		cout << "ERRO:: Command Not Found:: " << cmd << endl;

	return flag;	
}	

bool controlBuffer(string buffer, string *cmd, string *title, string *content){
	if(!buffer.empty()){
		string delimiter = " ";
		string command = buffer.substr(0, buffer.find(delimiter));
		buffer.erase(0, buffer.find(delimiter) + delimiter.length());

		string tl = buffer.substr(0, buffer.find(delimiter));
		buffer.erase(0, buffer.find(delimiter) + delimiter.length());	

		*cmd = command; 
		*title = tl;
		*content = buffer;
	
		return true;
	}

	return false;
}
