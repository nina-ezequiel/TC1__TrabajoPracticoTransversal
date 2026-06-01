#include <stdio.h>
#include <stdlib.h>

typedef struct nodo {
	char car;       
	struct nodo *next; 
} T_nodo;
typedef T_nodo* str;

str createStr(); 						// Crea y retorna una cadena vacía (str = NULL)
str loadStr(); 							// Carga una cadena desde el ingreso por teclado
str loadStr2(const char* cadena); 		// Crea una cadena a partir de una cadena previamente escrita (const char*)
void printStr(str s); 					// Muestra la cadena en la salida estándar (pantalla)
str concatStr(str s1, str s2); 			// Concatena dos cadenas y retorna el resultado
str beforeToken(str s, char token); 	// Retorna la parte de la cadena antes de la primera aparición del token
str afterToken(str s, char token); 		// Retorna la parte de la cadena después de la primera aparición del token
int isinStr(str s1, str s2); 			// Verifica si s2 está contenido en s1 (1 si es verdadero, 0 si es falso)
int equalStr(str s1,str s2);			// Verifica si s1 y s2 son iguales (1 si es verdadero, 0 si es falso)
int sizeStr(str s);						// Retorna el tamanio del string
void limpiarBuffer();					// Modulo auxiliar para limpiar el buffer
str crearNodo(char c); 					// Crea un nuevo nodo con el carácter especificado
void freeStr(str* s); 					// Libera toda la memoria utilizada por una cadena
str copyStr(str s);             		// Copia cadena
