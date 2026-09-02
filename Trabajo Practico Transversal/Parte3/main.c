#include "AF.h"
#include "AF_Operations.h"
#include "AF_Converter.h"

void ejemploAFD1();                     // AFD que reconoce cantidad impar de ceros
void ejemploAFD2();                     // AFD que reconoce números binarios divisibles por 3
void ejemploAFND1();                    // AFND para penúltima letra 'a' (solo muestra y prueba)
void ejemploConversionPenultimaA();     // Construye AFND penúltima 'a' y lo convierte a AFD
void ejemploConversionAFND2();          // Construye AFND de 6 estados (ejemplo TP) y lo convierte a AFD

int main() {
	int opcion;
	do {
		printf("\n========== MENU ==========\n");
		printf("1. Probar AFD\n");
		printf("2. Probar AFND\n");
		printf("3. Crear automata interactivamente y mostrarlo\n");
		printf("4. Convertir AFND -> AFD (ejemplo penultima letra 'a')\n");
		printf("5. Convertir AFND -> AFD (ejemplo del tp)\n");
		printf("6. Salir\n");
		printf("Elija una opcion: ");
		scanf("%d", &opcion);
		limpiarBuffer();
		
		switch (opcion) {
		case 1: {
			printf("\n========== AFD ==========\n");
			ejemploAFD2();
			break;
		}
		case 2: {
			printf("\n========== AFND ==========\n");
			ejemploAFND1();
			break;
		}
		case 3: {
			Af af3 = createAF();
			if (af3) {
				printAF(af3);
				printf("\nAcepta?: %s", acceptFromConsole(af3)? "Si" : "No");
				freeAF(af3);
			} else {
				printf("Error al crear el automata.\n");
			}
			break;
		}
		case 4: {
			printf("\n=== Conversion AFND -> AFD (ejemplo penultima letra 'a') ===\n");
			ejemploConversionPenultimaA();
			break;
		}
		case 5: {
			printf("\n=== Conversion AFND -> AFD (nuevo ejemplo con 6 estados) ===\n");
			ejemploConversionAFND2();
			break;
		}
		case 6:
			printf("Saliendo...\n");
			break;
		default:
			printf("Opcion no valida. Intente de nuevo.\n");
		}
	} while (opcion != 6);
	return 0;
}

/* ==================== Ejemplo AFD (cantidad impar de ceros) ==================== */
void ejemploAFD1() {
	Af afd = newEmptyAF();
	
	State q0 = newNodeStrHard(loadStr2("q0"));
	State q1 = newNodeStrHard(loadStr2("q1"));
	Symbol sym0 = newNodeStrHard(loadStr2("0"));
	Symbol sym1 = newNodeStrHard(loadStr2("1"));
	
	AF_addState(afd, q0);
	AF_addState(afd, q1);
	AF_addSymbol(afd, sym0);
	AF_addSymbol(afd, sym1);
	AF_setInitial(afd, q0);
	AF_addFinal(afd, q1);
	
	tData dest_q0 = newEmptyNodeSet(); tData_addToSet(dest_q0, copy_tData(q0));
	tData dest_q1 = newEmptyNodeSet(); tData_addToSet(dest_q1, copy_tData(q1));
	
	AF_addTransition(afd, q0, sym0, dest_q1);
	AF_addTransition(afd, q0, sym1, dest_q0);
	AF_addTransition(afd, q1, sym0, dest_q0);
	AF_addTransition(afd, q1, sym1, dest_q1);
	
	free_tData(dest_q0);
	free_tData(dest_q1);
	free_tData(q0); free_tData(q1); free_tData(sym0); free_tData(sym1);
	
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

/* ==================== Ejemplo AFD2 (divisible por 3) ==================== */
void ejemploAFD2() {
	Af afd = newEmptyAF();
	
	State q0 = newNodeStrHard(loadStr2("q0"));
	State q1 = newNodeStrHard(loadStr2("q1"));
	State q2 = newNodeStrHard(loadStr2("q2"));
	Symbol sym0 = newNodeStrHard(loadStr2("0"));
	Symbol sym1 = newNodeStrHard(loadStr2("1"));
	
	AF_addState(afd, q0);
	AF_addState(afd, q1);
	AF_addState(afd, q2);
	AF_addSymbol(afd, sym0);
	AF_addSymbol(afd, sym1);
	AF_setInitial(afd, q0);
	AF_addFinal(afd, q0);   // q0 es final
	
	tData dest_q0 = newEmptyNodeSet(); tData_addToSet(dest_q0, copy_tData(q0));
	tData dest_q1 = newEmptyNodeSet(); tData_addToSet(dest_q1, copy_tData(q1));
	tData dest_q2 = newEmptyNodeSet(); tData_addToSet(dest_q2, copy_tData(q2));
	
	AF_addTransition(afd, q0, sym0, dest_q0);
	AF_addTransition(afd, q0, sym1, dest_q1);
	AF_addTransition(afd, q1, sym0, dest_q2);
	AF_addTransition(afd, q1, sym1, dest_q0);
	AF_addTransition(afd, q2, sym0, dest_q1);
	AF_addTransition(afd, q2, sym1, dest_q2);
	
	/*
	
	q0,0,q0
	q0,1,q1
	q1,0,q2
	q1,1,q0
	q2,0,q1
	q2,1,q2
	
	final = q0
	*/
	
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
	printf("\"11 (3 decimal)\"      : %s\n", acceptHardcoded(afd, "11") ? "Aceptada" : "Rechazada");
	printf("\"110 (6 decimal)\"     : %s\n", acceptHardcoded(afd, "110") ? "Aceptada" : "Rechazada");
	printf("\"1001 (9 decimal)\"    : %s\n", acceptHardcoded(afd, "1001") ? "Aceptada" : "Rechazada");
	printf("\"10 (2 decimal)\"      : %s\n", acceptHardcoded(afd, "10") ? "Aceptada" : "Rechazada");
	printf("\"100 (4 decimal)\"     : %s\n", acceptHardcoded(afd, "100") ? "Aceptada" : "Rechazada");
	printf("\"111 (7 decimal)\"     : %s\n", acceptHardcoded(afd, "111") ? "Aceptada" : "Rechazada");
	
	freeAF(afd);
}

/* ==================== Ejemplo AFND1 (penultima letra 'a') ==================== */
void ejemploAFND1() {
	Af afnd = newEmptyAF();
	
	State q0 = newNodeStrHard(loadStr2("q0"));
	State q1 = newNodeStrHard(loadStr2("q1"));
	State q2 = newNodeStrHard(loadStr2("q2"));
	Symbol sym_a = newNodeStrHard(loadStr2("a"));
	Symbol sym_b = newNodeStrHard(loadStr2("b"));
	
	AF_addState(afnd, q0);
	AF_addState(afnd, q1);
	AF_addState(afnd, q2);
	AF_addSymbol(afnd, sym_a);
	AF_addSymbol(afnd, sym_b);
	AF_setInitial(afnd, q0);
	AF_addFinal(afnd, q2);
	
	tData dest_q0 = newEmptyNodeSet(); tData_addToSet(dest_q0, copy_tData(q0));
	tData dest_q1 = newEmptyNodeSet(); tData_addToSet(dest_q1, copy_tData(q1));
	tData dest_q2 = newEmptyNodeSet(); tData_addToSet(dest_q2, copy_tData(q2));
	tData dest_q0_q1 = newEmptyNodeSet();
	tData_addToSet(dest_q0_q1, copy_tData(q0));
	tData_addToSet(dest_q0_q1, copy_tData(q1));
	
	AF_addTransition(afnd, q0, sym_a, dest_q0_q1);
	AF_addTransition(afnd, q0, sym_b, dest_q0);
	AF_addTransition(afnd, q1, sym_a, dest_q2);
	AF_addTransition(afnd, q1, sym_b, dest_q2);
	AF_addTransition(afnd, q2, sym_a, dest_q0);
	AF_addTransition(afnd, q2, sym_b, dest_q0);
	
	free_tData(dest_q0); free_tData(dest_q1); free_tData(dest_q2); free_tData(dest_q0_q1);
	free_tData(q0); free_tData(q1); free_tData(q2); free_tData(sym_a); free_tData(sym_b);
	
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

/* ==================== Conversión: AFND penúltima 'a' -> AFD ==================== */
void ejemploConversionPenultimaA() {
	// Construir el AFND de penultima letra 'a'
	Af afnd = newEmptyAF();
	
	State q0 = newNodeStrHard(loadStr2("q0"));
	State q1 = newNodeStrHard(loadStr2("q1"));
	State q2 = newNodeStrHard(loadStr2("q2"));
	Symbol sym_a = newNodeStrHard(loadStr2("a"));
	Symbol sym_b = newNodeStrHard(loadStr2("b"));
	
	AF_addState(afnd, q0);
	AF_addState(afnd, q1);
	AF_addState(afnd, q2);
	AF_addSymbol(afnd, sym_a);
	AF_addSymbol(afnd, sym_b);
	AF_setInitial(afnd, q0);
	AF_addFinal(afnd, q2);
	
	tData dest_q0 = newEmptyNodeSet(); tData_addToSet(dest_q0, copy_tData(q0));
	tData dest_q1 = newEmptyNodeSet(); tData_addToSet(dest_q1, copy_tData(q1));
	tData dest_q2 = newEmptyNodeSet(); tData_addToSet(dest_q2, copy_tData(q2));
	tData dest_q0_q1 = newEmptyNodeSet();
	tData_addToSet(dest_q0_q1, copy_tData(q0));
	tData_addToSet(dest_q0_q1, copy_tData(q1));
	
	AF_addTransition(afnd, q0, sym_a, dest_q0_q1);
	AF_addTransition(afnd, q0, sym_b, dest_q0);
	AF_addTransition(afnd, q1, sym_a, dest_q2);
	AF_addTransition(afnd, q1, sym_b, dest_q2);
	AF_addTransition(afnd, q2, sym_a, dest_q0);
	AF_addTransition(afnd, q2, sym_b, dest_q0);
	
	free_tData(dest_q0); free_tData(dest_q1); free_tData(dest_q2); free_tData(dest_q0_q1);
	free_tData(q0); free_tData(q1); free_tData(q2); free_tData(sym_a); free_tData(sym_b);
	
	printf("\n--- AFND original ---\n");
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
	
	Af afd = AFNDtoAFD(afnd);
	if (afd) {
		printf("\n--- AFD convertido (renombrado) ---\n");
		printAF(afd);
		printf("\n--- Pruebas AFD (penultima letra = 'a') ---\n");
		printf("\"bab\"   : %s\n", acceptHardcoded(afd, "bab") ? "Aceptada" : "Rechazada");
		printf("\"ab\"    : %s\n", acceptHardcoded(afd, "ab") ? "Aceptada" : "Rechazada");
		printf("\"abaa\"  : %s\n", acceptHardcoded(afd, "abaa") ? "Aceptada" : "Rechazada");
		printf("\"abbab\" : %s\n", acceptHardcoded(afd, "abbab") ? "Aceptada" : "Rechazada");
		printf("\"ba\"    : %s\n", acceptHardcoded(afd, "ba") ? "Aceptada" : "Rechazada");
		printf("\"a\"     : %s\n", acceptHardcoded(afd, "a") ? "Aceptada" : "Rechazada");
		printf("\"bb\"    : %s\n", acceptHardcoded(afd, "bb") ? "Aceptada" : "Rechazada");
		printf("\"aba\"   : %s\n", acceptHardcoded(afd, "aba") ? "Aceptada" : "Rechazada");
		freeAF(afd);
	} else {
		printf("Error en la conversion.\n");
	}
	freeAF(afnd);
}

/* ==================== Conversión: AFND de 6 estados (ejemplo TP) -> AFD ==================== */
void ejemploConversionAFND2() {
	// Estados: q0, q1, q2, q3, q4, q5 (q5 final)
	// Alfabeto: 0, 1
	// Transiciones:
	//   (q0,0) = {q0}
	//   (q0,1) = {q0,q1,q2}
	//   (q1,0) = {q2,q3}
	//   (q1,1) = {}
	//   (q2,0) = {}
	//   (q2,1) = {q4}
	//   (q3,0) = {q5}
	//   (q3,1) = {}
	//   (q4,0) = {}
	//   (q4,1) = {q5}
	//   (q5,0) = {q5}
	//   (q5,1) = {q5}
	// Estado inicial: q0
	// Estado final: q5
	
	Af afnd = newEmptyAF();
	
	State q0 = newNodeStrHard(loadStr2("q0"));
	State q1 = newNodeStrHard(loadStr2("q1"));
	State q2 = newNodeStrHard(loadStr2("q2"));
	State q3 = newNodeStrHard(loadStr2("q3"));
	State q4 = newNodeStrHard(loadStr2("q4"));
	State q5 = newNodeStrHard(loadStr2("q5"));
	
	Symbol sym0 = newNodeStrHard(loadStr2("0"));
	Symbol sym1 = newNodeStrHard(loadStr2("1"));
	
	AF_addState(afnd, q0);
	AF_addState(afnd, q1);
	AF_addState(afnd, q2);
	AF_addState(afnd, q3);
	AF_addState(afnd, q4);
	AF_addState(afnd, q5);
	AF_addSymbol(afnd, sym0);
	AF_addSymbol(afnd, sym1);
	AF_setInitial(afnd, q0);
	AF_addFinal(afnd, q5);
	
	// Conjuntos destino
	tData set_q0 = newEmptyNodeSet(); tData_addToSet(set_q0, copy_tData(q0));
	tData set_q0_q1_q2 = newEmptyNodeSet();
	tData_addToSet(set_q0_q1_q2, copy_tData(q0));
	tData_addToSet(set_q0_q1_q2, copy_tData(q1));
	tData_addToSet(set_q0_q1_q2, copy_tData(q2));
	tData set_q2_q3 = newEmptyNodeSet();
	tData_addToSet(set_q2_q3, copy_tData(q2));
	tData_addToSet(set_q2_q3, copy_tData(q3));
	tData set_q4 = newEmptyNodeSet(); tData_addToSet(set_q4, copy_tData(q4));
	tData set_q5 = newEmptyNodeSet(); tData_addToSet(set_q5, copy_tData(q5));
	tData set_vacio = newEmptyNodeSet();  // para transiciones vacías (no se agregan)
	
	// Agregar transiciones
	AF_addTransition(afnd, q0, sym0, set_q0);               
	AF_addTransition(afnd, q0, sym1, set_q0_q1_q2);         
	AF_addTransition(afnd, q1, sym0, set_q2_q3);            
	AF_addTransition(afnd, q2, sym1, set_q4);               
	AF_addTransition(afnd, q3, sym0, set_q5);               
	AF_addTransition(afnd, q4, sym1, set_q5);               
	AF_addTransition(afnd, q5, sym0, set_q5);               
	AF_addTransition(afnd, q5, sym1, set_q5);               
	
	// Liberar conjuntos temporales
	free_tData(set_q0);
	free_tData(set_q0_q1_q2);
	free_tData(set_q2_q3);
	free_tData(set_q4);
	free_tData(set_q5);
	free_tData(set_vacio);
	free_tData(q0); free_tData(q1); free_tData(q2); free_tData(q3); free_tData(q4); free_tData(q5);
	free_tData(sym0); free_tData(sym1);
	
	printAF(afnd);
	
	printf("\n--- AFND original (6 estados) ---\n");
	printf("\n--- Pruebas de aceptacion (ejemplo de tp) ---\n");
	printf("\"111\"     : %s\n", acceptHardcoded(afnd, "111")? "Aceptada" : "Rechazada");
	printf("\"0\"       : %s\n", acceptHardcoded(afnd, "0")? "Aceptada" : "Rechazada");
	printf("\"00\"      : %s\n", acceptHardcoded(afnd, "00")? "Aceptada" : "Rechazada");
	printf("\"1011\"    : %s\n", acceptHardcoded(afnd, "1011")? "Aceptada" : "Rechazada");
	printf("\"010\"     : %s\n", acceptHardcoded(afnd, "010")? "Aceptada" : "Rechazada");
	printf("\"10100\"   : %s\n", acceptHardcoded(afnd, "10100")? "Aceptada" : "Rechazada");
	
	Af afd = AFNDtoAFD(afnd);
	if (afd) {
		printf("\n--- AFD convertido (renombrado) ---\n");
		printAF(afd);
		printf("\n--- Pruebas de aceptacion (ejemplo de tp) ---\n");
		printf("\"111\"   : %s\n", acceptHardcoded(afd, "111") ? "Aceptada" : "Rechazada");
		printf("\"0\"     : %s\n", acceptHardcoded(afd, "0") ? "Aceptada" : "Rechazada");
		printf("\"00\"    : %s\n", acceptHardcoded(afd, "00") ? "Aceptada" : "Rechazada");
		printf("\"1011\"  : %s\n", acceptHardcoded(afd, "1011") ? "Aceptada" : "Rechazada");
		printf("\"010\"   : %s\n", acceptHardcoded(afd, "010") ? "Aceptada" : "Rechazada");
		printf("\"10100\" : %s\n", acceptHardcoded(afd, "10100") ? "Aceptada" : "Rechazada");
		freeAF(afd);
	} else {
		printf("Error en la conversion.\n");
	}
	freeAF(afnd);
}
