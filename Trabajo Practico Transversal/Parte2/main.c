#include "AF.h"

void ejemploAFD1();
void ejemploAFD2();
void ejemploAFND1();

int main() {
	int opcion;
	do {
		printf("\n========== MENU ==========\n");
		printf("1. Probar AFD\n");
		printf("2. Probar AFND\n");
		printf("3. Crear automata interactivamente y mostrarlo (sin probar cadenas)\n");
		printf("4. Salir\n");
		printf("Elija una opcion: ");
		scanf("%d", &opcion);
		limpiarBuffer(); // limpiar el salto de línea
		
		switch (opcion) {
		case 1: {
			printf("\n========== AFD ==========\n");
			ejemploAFD1();
			break;
		}
		case 2: {
			printf("\n========== AFND ==========\n");
			ejemploAFND1();
			break;
		}
		case 3: {
			Af af3 = createAFinteractive();
			if (af3) {
				printAF(af3);
				freeAF(af3);
			} else {
				printf("Error al crear el automata.\n");
			}
			break;
		}
		case 4:
			printf("Saliendo...\n");
			break;
		default:
			printf("Opcion no valida. Intente de nuevo.\n");
		}
	} while (opcion != 4);
	return 0;
}

void ejemploAFD1() {
	Af afd = newEmptyAF();
	
	// Crear estados y símbolos (como tData)
	State q0 = newNodeStrHard(loadStr2("q0"));
	State q1 = newNodeStrHard(loadStr2("q1"));
	Symbol sym0 = newNodeStrHard(loadStr2("0"));
	Symbol sym1 = newNodeStrHard(loadStr2("1"));
	
	// Agregar usando funciones de encapsulamiento (deben existir en AF.h)
	addState(afd, q0);
	addState(afd, q1);
	addSymbol(afd, sym0);
	addSymbol(afd, sym1);
	setInitial(afd, q0);
	addFinal(afd, q1);
	
	// Construir conjuntos destino unitarios
	tData dest_q0 = newEmptyNodeSet();
	tData_addToSet(dest_q0, copy_tData(q0));
	tData dest_q1 = newEmptyNodeSet();
	tData_addToSet(dest_q1, copy_tData(q1));
	
	// Agregar transiciones
	addTransition(afd, q0, sym0, dest_q1);
	addTransition(afd, q0, sym1, dest_q0);
	addTransition(afd, q1, sym0, dest_q0);
	addTransition(afd, q1, sym1, dest_q1);
	
	// Liberar conjuntos temporales (addTransition ya copió los destinos)
	free_tData(dest_q0);
	free_tData(dest_q1);
	// Liberar los originales de estados y símbolos (ya se copiaron dentro del autómata)
	free_tData(q0);
	free_tData(q1);
	free_tData(sym0);
	free_tData(sym1);
	
	printAF(afd);
	printf("\n--- Pruebas de aceptacion (Cantidad Impar de Ceros) ---\n");
	printf("\"\"      : %s\n", acceptHardcoded(afd, "") ? "Aceptada" : "Rechazada");
	printf("\"0\"     : %s\n", acceptHardcoded(afd, "0") ? "Aceptada" : "Rechazada");
	printf("\"00\"    : %s\n", acceptHardcoded(afd, "00") ? "Aceptada" : "Rechazada");
	printf("\"1\"     : %s\n", acceptHardcoded(afd, "1") ? "Aceptada" : "Rechazada");
	printf("\"010\"   : %s\n", acceptHardcoded(afd, "010") ? "Aceptada" : "Rechazada");
	printf("\"10100\" : %s\n", acceptHardcoded(afd, "10100") ? "Aceptada" : "Rechazada");
	
	freeAF(afd);
}

void ejemploAFD2() {
	Af afd = newEmptyAF();
	
	State q0 = newNodeStrHard(loadStr2("q0"));
	State q1 = newNodeStrHard(loadStr2("q1"));
	State q2 = newNodeStrHard(loadStr2("q2"));
	Symbol sym0 = newNodeStrHard(loadStr2("0"));
	Symbol sym1 = newNodeStrHard(loadStr2("1"));
	
	addState(afd, q0);
	addState(afd, q1);
	addState(afd, q2);
	addSymbol(afd, sym0);
	addSymbol(afd, sym1);
	setInitial(afd, q0);
	addFinal(afd, q0);   // q0 es final
	
	tData dest_q0 = newEmptyNodeSet(); tData_addToSet(dest_q0, copy_tData(q0));
	tData dest_q1 = newEmptyNodeSet(); tData_addToSet(dest_q1, copy_tData(q1));
	tData dest_q2 = newEmptyNodeSet(); tData_addToSet(dest_q2, copy_tData(q2));
	
	addTransition(afd, q0, sym0, dest_q0);
	addTransition(afd, q0, sym1, dest_q1);
	addTransition(afd, q1, sym0, dest_q2);
	addTransition(afd, q1, sym1, dest_q0);
	addTransition(afd, q2, sym0, dest_q1);
	addTransition(afd, q2, sym1, dest_q2);
	
	free_tData(dest_q0);
	free_tData(dest_q1);
	free_tData(dest_q2);
	free_tData(q0);
	free_tData(q1);
	free_tData(q2);
	free_tData(sym0);
	free_tData(sym1);
	
	printAF(afd);
	printf("\n--- Pruebas de aceptacion (Numero Binario Divisible por 3)---\n");
	printf("\"10\"      : %s\n", acceptHardcoded(afd, "10") ? "Aceptada" : "Rechazada");
	printf("\"111\"     : %s\n", acceptHardcoded(afd, "111") ? "Aceptada" : "Rechazada");
	printf("\"1000\"    : %s\n", acceptHardcoded(afd, "1000") ? "Aceptada" : "Rechazada");
	printf("\"1100\"     : %s\n", acceptHardcoded(afd, "1100") ? "Aceptada" : "Rechazada");
	printf("\"1111\"   : %s\n", acceptHardcoded(afd, "1111") ? "Aceptada" : "Rechazada");
	printf("\"10010\" : %s\n", acceptHardcoded(afd, "10010") ? "Aceptada" : "Rechazada");
	
	freeAF(afd);
}

void ejemploAFND1() {
	Af afnd = newEmptyAF();
	
	State q0 = newNodeStrHard(loadStr2("q0"));
	State q1 = newNodeStrHard(loadStr2("q1"));
	State q2 = newNodeStrHard(loadStr2("q2"));
	Symbol sym_a = newNodeStrHard(loadStr2("a"));
	Symbol sym_b = newNodeStrHard(loadStr2("b"));
	
	addState(afnd, q0);
	addState(afnd, q1);
	addState(afnd, q2);
	addSymbol(afnd, sym_a);
	addSymbol(afnd, sym_b);
	setInitial(afnd, q0);
	addFinal(afnd, q2);
	
	// Conjuntos destino
	tData dest_q0 = newEmptyNodeSet(); tData_addToSet(dest_q0, copy_tData(q0));
	tData dest_q1 = newEmptyNodeSet(); tData_addToSet(dest_q1, copy_tData(q1));
	tData dest_q2 = newEmptyNodeSet(); tData_addToSet(dest_q2, copy_tData(q2));
	tData dest_q0_q1 = newEmptyNodeSet();
	tData_addToSet(dest_q0_q1, copy_tData(q0));
	tData_addToSet(dest_q0_q1, copy_tData(q1));
	
	addTransition(afnd, q0, sym_a, dest_q0_q1);
	addTransition(afnd, q0, sym_b, dest_q0);
	addTransition(afnd, q1, sym_a, dest_q2);
	addTransition(afnd, q1, sym_b, dest_q2);
	addTransition(afnd, q2, sym_a, dest_q0);
	addTransition(afnd, q2, sym_b, dest_q0);
	
	free_tData(dest_q0);
	free_tData(dest_q1);
	free_tData(dest_q2);
	free_tData(dest_q0_q1);
	free_tData(q0);
	free_tData(q1);
	free_tData(q2);
	free_tData(sym_a);
	free_tData(sym_b);
	
	printAF(afnd);
	printf("\n--- Pruebas AFND (penultima letra = 'a') ---\n");
	printf("\"bab\"   : %s\n", acceptHardcoded(afnd, "bab") ? "Aceptada" : "Rechazada");
	printf("\"ab\"    : %s\n", acceptHardcoded(afnd, "ab") ? "Aceptada" : "Rechazada");
	printf("\"abaa\"  : %s\n", acceptHardcoded(afnd, "abaa") ? "Aceptada" : "Rechazada");
	printf("\"abbab\" : %s\n", acceptHardcoded(afnd, "abbab") ? "Aceptada" : "Rechazada");
	printf("\"ba\"    : %s\n", acceptHardcoded(afnd, "ba") ? "Aceptada" : "Rechazada");
	printf("\"a\"     : %s\n", acceptHardcoded(afnd, "a") ? "Aceptada" : "Rechazada");
	printf("\"bb\"    : %s\n", acceptHardcoded(afnd, "bb") ? "Aceptada" : "Rechazada");
	printf("\"aba\"   : %s\n", acceptHardcoded(afnd, "aba") ? "Aceptada" : "Rechazada");
	
	freeAF(afnd);
}
