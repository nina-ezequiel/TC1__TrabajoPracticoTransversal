#include "AF.h"

void ejemploAFD1(); // cantidad impar de ceros
void ejemploAFD2(); // numero binaro divisible por 3
void ejemploAFND1(); // penultima letra 'a'

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
			ejemploAFD2();
			break;
		}
		case 2: {
			printf("\n========== AFND ==========\n");
			ejemploAFND1();
			break;
		}
		case 3: {
			// ==================== Creación interactiva (solo mostrar) ====================
			printf("\n--- Creacion interactiva del automata ---\n");
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
			printf("Opción no valida. Intente de nuevo.\n");
		}
	} while (opcion != 4);
	
	return 0;
}

void ejemploAFD1(){
	
	Af afd = newEmptyAF();
	
	// Estados
	tData q0 = newNodeStrHard(loadStr2("q0"));
	tData q1 = newNodeStrHard(loadStr2("q1"));
	insert_set(&(afd->Q->data), q0);
	insert_set(&(afd->Q->data), q1);
	
	// Alfabeto
	tData sym0 = newNodeStrHard(loadStr2("0"));
	tData sym1 = newNodeStrHard(loadStr2("1"));
	insert_set(&(afd->Sigma->data), sym0);
	insert_set(&(afd->Sigma->data), sym1);
	
	// Inicial
	afd->q0 = copy_tData(q0);
	
	// Final
	insert_set(&(afd->F->data), copy_tData(q1));
	
	// Transiciones
	DeltaEntry d1, d2, d3, d4;
	d1.from = copy_tData(q0);
	d1.symbol = copy_tData(sym0);
	d1.destinations = newEmptyNodeSet();
	insert_set(&(d1.destinations->data), copy_tData(q1));
	
	d2.from = copy_tData(q0); d2.symbol = copy_tData(sym1);
	d2.destinations = newEmptyNodeSet();
	insert_set(&(d2.destinations->data), copy_tData(q0));
	
	d3.from = copy_tData(q1); d3.symbol = copy_tData(sym0);
	d3.destinations = newEmptyNodeSet();
	insert_set(&(d3.destinations->data), copy_tData(q0));
	
	d4.from = copy_tData(q1); d4.symbol = copy_tData(sym1);
	d4.destinations = newEmptyNodeSet();
	insert_set(&(d4.destinations->data), copy_tData(q1));
	
	afd->delta = malloc(4 * sizeof(DeltaEntry));
	afd->delta[0] = d1; 
	afd->delta[1] = d2;
	afd->delta[2] = d3;
	afd->delta[3] = d4;
	afd->deltaCount = 4;
	
	printAF(afd);
	printf("\n--- Pruebas de aceptacion ---\n");
	printf("\"\"      : %s\n", acceptHardcoded(afd, "") ? "Aceptada" : "Rechazada");
	printf("\"0\"     : %s\n", acceptHardcoded(afd, "0") ? "Aceptada" : "Rechazada");
	printf("\"00\"    : %s\n", acceptHardcoded(afd, "00") ? "Aceptada" : "Rechazada");
	printf("\"1\"     : %s\n", acceptHardcoded(afd, "1") ? "Aceptada" : "Rechazada");
	printf("\"010\"   : %s\n", acceptHardcoded(afd, "010") ? "Aceptada" : "Rechazada");
	printf("\"10100\" : %s\n", acceptHardcoded(afd, "10100") ? "Aceptada" : "Rechazada");
	
	freeAF(afd);
}

void ejemploAFD2(){
	
	Af afd = newEmptyAF();
	
	// Estados
	tData q0 = newNodeStrHard(loadStr2("q0"));
	tData q1 = newNodeStrHard(loadStr2("q1"));
	tData q2 = newNodeStrHard(loadStr2("q2"));
	insert_set(&(afd->Q->data), q0);
	insert_set(&(afd->Q->data), q1);
	insert_set(&(afd->Q->data), q2);
	
	// Alfabeto
	tData sym0 = newNodeStrHard(loadStr2("0"));
	tData sym1 = newNodeStrHard(loadStr2("1"));
	insert_set(&(afd->Sigma->data), sym0);
	insert_set(&(afd->Sigma->data), sym1);
	
	// Inicial
	afd->q0 = copy_tData(q0);
	
	// Final
	insert_set(&(afd->F->data), copy_tData(q0));
	
	// Transiciones
	DeltaEntry d1, d2, d3, d4, d5, d6;
	
	d1.from = copy_tData(q0); 
	d1.symbol = copy_tData(sym0);
	d1.destinations = newEmptyNodeSet();
	insert_set(&(d1.destinations->data), copy_tData(q0));
	
	d2.from = copy_tData(q0); 
	d2.symbol = copy_tData(sym1);
	d2.destinations = newEmptyNodeSet();
	insert_set(&(d2.destinations->data), copy_tData(q1));
	
	d3.from = copy_tData(q1); 
	d3.symbol = copy_tData(sym0);
	d3.destinations = newEmptyNodeSet();
	insert_set(&(d3.destinations->data), copy_tData(q2));
	
	d4.from = copy_tData(q1); 
	d4.symbol = copy_tData(sym1);
	d4.destinations = newEmptyNodeSet();
	insert_set(&(d4.destinations->data), copy_tData(q0));
	
	d5.from = copy_tData(q2); 
	d5.symbol = copy_tData(sym0);
	d5.destinations = newEmptyNodeSet();
	insert_set(&(d5.destinations->data), copy_tData(q1));
	
	d6.from = copy_tData(q2); 
	d6.symbol = copy_tData(sym1);
	d6.destinations = newEmptyNodeSet();
	insert_set(&(d6.destinations->data), copy_tData(q2));
	
	afd->delta = malloc(6 * sizeof(DeltaEntry));
	afd->delta[0] = d1; 
	afd->delta[1] = d2;
	afd->delta[2] = d3;
	afd->delta[3] = d4;
	afd->delta[4] = d5;
	afd->delta[5] = d6;
	afd->deltaCount = 6;
	
	printAF(afd);
	printf("\n--- Pruebas de aceptacion ---\n");
	printf("\"10\"      : %s\n", acceptHardcoded(afd, "10") ? "Aceptada" : "Rechazada");
	printf("\"111\"     : %s\n", acceptHardcoded(afd, "111") ? "Aceptada" : "Rechazada");
	printf("\"1000\"    : %s\n", acceptHardcoded(afd, "1000") ? "Aceptada" : "Rechazada");
	printf("\"1100\"     : %s\n", acceptHardcoded(afd, "1100") ? "Aceptada" : "Rechazada");
	printf("\"1111\"   : %s\n", acceptHardcoded(afd, "1111") ? "Aceptada" : "Rechazada");
	printf("\"10010\" : %s\n", acceptHardcoded(afd, "10010") ? "Aceptada" : "Rechazada");
	
	freeAF(afd);
	
}
	
void ejemploAFND1(){
	
	Af afnd = newEmptyAF();
	
	tData q0 = newNodeStrHard(loadStr2("q0"));
	tData q1 = newNodeStrHard(loadStr2("q1"));
	tData q2 = newNodeStrHard(loadStr2("q2"));
	insert_set(&(afnd->Q->data), q0);
	insert_set(&(afnd->Q->data), q1);
	insert_set(&(afnd->Q->data), q2);
	
	tData sym_a = newNodeStrHard(loadStr2("a"));
	tData sym_b = newNodeStrHard(loadStr2("b"));
	insert_set(&(afnd->Sigma->data), sym_a);
	insert_set(&(afnd->Sigma->data), sym_b);
	
	afnd->q0 = copy_tData(q0);
	insert_set(&(afnd->F->data), copy_tData(q2));
	
	// Transiciones
	DeltaEntry t1, t2, t3, t4, t5, t6;
	t1.from = copy_tData(q0); 
	t1.symbol = copy_tData(sym_a);
	t1.destinations = newEmptyNodeSet();
	insert_set(&(t1.destinations->data), copy_tData(q0));
	insert_set(&(t1.destinations->data), copy_tData(q1));
	
	t2.from = copy_tData(q0); t2.symbol = copy_tData(sym_b);
	t2.destinations = newEmptyNodeSet();
	insert_set(&(t2.destinations->data), copy_tData(q0));
	
	t3.from = copy_tData(q1); t3.symbol = copy_tData(sym_a);
	t3.destinations = newEmptyNodeSet();
	insert_set(&(t3.destinations->data), copy_tData(q2));
	
	t4.from = copy_tData(q1); t4.symbol = copy_tData(sym_b);
	t4.destinations = newEmptyNodeSet();
	insert_set(&(t4.destinations->data), copy_tData(q2));
	
	t5.from = copy_tData(q2); t5.symbol = copy_tData(sym_a);
	t5.destinations = newEmptyNodeSet();
	insert_set(&(t5.destinations->data), copy_tData(q0));
	
	t6.from = copy_tData(q2); t6.symbol = copy_tData(sym_b);
	t6.destinations = newEmptyNodeSet();
	insert_set(&(t6.destinations->data), copy_tData(q0));
	
	afnd->delta = malloc(6 * sizeof(DeltaEntry));
	afnd->delta[0] = t1; 
	afnd->delta[1] = t2;
	afnd->delta[2] = t3;
	afnd->delta[3] = t4;
	afnd->delta[4] = t5; 
	afnd->delta[5] = t6;
	afnd->deltaCount = 6;
	
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
	
	
