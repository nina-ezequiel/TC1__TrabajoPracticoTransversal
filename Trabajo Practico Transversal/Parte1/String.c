#include "String.h"

// Crea un nuevo nodo
str crearNodo(char c) {
	str nvo = (str)malloc(sizeof(T_nodo));
	nvo->car = c;
	nvo->next = NULL;
	return nvo;
}

// libera memoria
void freeStr(str *s) {
	while (*s != NULL) {
		str cop = *s;
		*s = (*s)->next;
		free(cop);
	}
	*s = NULL;
}

// Crea cadena vacía
str createStr() {
	return NULL;
}

// Lee cadena desde teclado
str loadStr() {
	str ini = NULL;
	str ult = NULL;
	char c;
	while ((c = getchar()) != '\n' && c != EOF) {
		str nvo = crearNodo(c);
		if (ini == NULL) {
			ini = nvo;
			ult = nvo;
		} else {
			ult->next = nvo;
			ult = nvo;
		}
	}
	return ini;
}

// Crea cadena desde const char*
str loadStr2(const char* cadena) {
	str ini = NULL;
	str ult = NULL;
	for (int i = 0; cadena[i] != '\0'; i++) {
		str nvo = crearNodo(cadena[i]);
		if (ini == NULL) {
			ini = nvo;
			ult = nvo;
		} else {
			ult->next = nvo;
			ult = nvo;
		}
	}
	return ini;
}

// Muestra la cadena
void printStr(str s) {
	if(s != NULL){
		str temp = s;
		printf("\"");
		while (temp != NULL) {
			printf("%c", temp->car);
			temp = temp->next;
		}
		printf("\"");
	}
	else{
		printf("\nString Vacio ...");
	}
}

// Copia una cadena
str copyStr(str s){
	str ini = NULL;
	str fin = NULL;
	str temp = s;
	while(temp != NULL){
		str nvo = crearNodo(temp->car);
		if(ini == NULL){
			ini = nvo;
			fin = nvo;
		}
		else{
			fin->next = nvo;
			fin = nvo;
		}
		temp = temp->next;
	}
	return ini;
}
		
// Concatena dos cadenas
str concatStr(str s1, str s2) {
	str cop1 = copyStr(s1);
	str cop2 = copyStr(s2);
	if (cop1 == NULL && cop2 == NULL) {
		return NULL;
	}
	if (cop1 == NULL) {
		return cop2;
	}
	if (cop2 == NULL) {
		return cop1;
	}
	str temp = cop1;
	while(temp->next != NULL){
		temp = temp->next;
	}
	temp->next = cop2;
	
	return cop1;
}

// Parte antes del token
str beforeToken(str s, char token) {
	if (s == NULL) 
		return NULL;
	str temp = s;
	str ini = NULL;
	str ult = NULL;
	while (temp != NULL && temp->car != token) {
		str nvo = crearNodo(temp->car);
		if (ini == NULL) {
			ini = ult = nvo;
		} else {
			ult->next = nvo;
			ult = nvo;
		}
		temp = temp->next;
	}
	if (temp == NULL) {
		return NULL;
	}
	return ini;
}

// Parte después del token
str afterToken(str s, char token) {
	if (s == NULL) 
		return NULL;
	str temp = s;
	while (temp != NULL && temp->car != token) {
		temp = temp->next;
	}
	if (temp == NULL || temp->next == NULL) {
		return NULL;
	}
	str cop = temp->next;			
	str ini = NULL;
	str fin = NULL;
	while (cop != NULL) {
		str nvo = crearNodo(cop->car);
		if (ini == NULL) {
			ini = fin = nvo;
		} else {
			fin->next = nvo;
			fin = nvo;
		}
		cop = cop->next;
	}        
	
	return ini;
}

// Verifica si s2 está en s1
int isinStr(str s1, str s2) {
	if (s2 == NULL) 
		return 1;
	if (s1 == NULL) 
		return 0;
	str temp = s1;
	while (temp != NULL) {
		if (temp->car == s2->car) {
			str ss1 = temp;
			str ss2 = s2;
			while (ss1 != NULL && ss2 != NULL && ss1->car == ss2->car) {
				ss1 = ss1->next;
				ss2 = ss2->next;
			}
			if (ss2 == NULL) 
				return 1;
		}
		temp = temp->next;
	}
	return 0;
}

// Verifica si s1 y s2 son iguales
int equalStr(str s1,str s2){
	if(isinStr(s1,s2) && isinStr(s2,s1))
		return 1;
	else
		return 0;
}

// Retorna tamanio del string
int sizeStr(str s){
	int c = 0;
	while(s != NULL){
		c++;
		s = s->next;
	}
	return c;
}

// limpia buffer
void limpiarBuffer() {
	int c;
	while ((c = getchar()) != '\n' && c != EOF);
}
