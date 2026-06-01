#include "AF.h"
#include <stdio.h>

int main() {
	int opcion;
	do {
		printf("\n========== MENU ==========\n");
		printf("1. Probar AFD (cantidad impar de ceros)\n");
		printf("2. Probar AFND (penultima letra = 'a')\n");
		printf("3. Crear automata interactivamente y mostrarlo (sin probar cadenas)\n");
		printf("4. Salir\n");
		printf("Elija una opcion: ");
		scanf("%d", &opcion);
		limpiarBuffer(); // limpiar el salto de línea
		
		switch (opcion) {
		case 1: {
			// ==================== AFD: impar cantidad de ceros ====================
			printf("\n========== AFD: cantidad impar de ceros ==========\n");
			Automata* af1 = newEmptyAF();
			
			// Estados
			tData q0_1 = newNodeStrHard(loadStr2("q0"));
			tData q1_1 = newNodeStrHard(loadStr2("q1"));
			insert_set(&(af1->Q->data), q0_1);
			insert_set(&(af1->Q->data), q1_1);
			
			// Alfabeto
			tData sym0 = newNodeStrHard(loadStr2("0"));
			tData sym1 = newNodeStrHard(loadStr2("1"));
			insert_set(&(af1->Sigma->data), sym0);
			insert_set(&(af1->Sigma->data), sym1);
			
			// Inicial
			af1->q0 = copy_tData(q0_1);
			// Final
			insert_set(&(af1->F->data), copy_tData(q1_1));
			
			// Transiciones
			DeltaEntry d1, d2, d3, d4;
			d1.from = copy_tData(q0_1); d1.symbol = copy_tData(sym0);
			d1.destinations = newEmptyNodeSet();
			insert_set(&(d1.destinations->data), copy_tData(q1_1));
			
			d2.from = copy_tData(q0_1); d2.symbol = copy_tData(sym1);
			d2.destinations = newEmptyNodeSet();
			insert_set(&(d2.destinations->data), copy_tData(q0_1));
			
			d3.from = copy_tData(q1_1); d3.symbol = copy_tData(sym0);
			d3.destinations = newEmptyNodeSet();
			insert_set(&(d3.destinations->data), copy_tData(q0_1));
			
			d4.from = copy_tData(q1_1); d4.symbol = copy_tData(sym1);
			d4.destinations = newEmptyNodeSet();
			insert_set(&(d4.destinations->data), copy_tData(q1_1));
			
			af1->delta = malloc(4 * sizeof(DeltaEntry));
			af1->delta[0] = d1; 
			af1->delta[1] = d2;
			af1->delta[2] = d3;
			af1->delta[3] = d4;
			af1->deltaCount = 4;
			
			printAF(af1);
			printf("\n--- Pruebas de aceptación ---\n");
			printf("\"\"      : %s\n", acceptHardcoded(af1, "") ? "Aceptada" : "Rechazada");
			printf("\"0\"     : %s\n", acceptHardcoded(af1, "0") ? "Aceptada" : "Rechazada");
			printf("\"00\"    : %s\n", acceptHardcoded(af1, "00") ? "Aceptada" : "Rechazada");
			printf("\"1\"     : %s\n", acceptHardcoded(af1, "1") ? "Aceptada" : "Rechazada");
			printf("\"010\"   : %s\n", acceptHardcoded(af1, "010") ? "Aceptada" : "Rechazada");
			printf("\"10100\" : %s\n", acceptHardcoded(af1, "10100") ? "Aceptada" : "Rechazada");
			
			freeAF(af1);
			break;
		}
		case 2: {
			// ==================== AFND: penúltima letra = 'a' ====================
			printf("\n========== AFND: cadenas cuya penultima letra es 'a' ==========\n");
			Automata* af2 = newEmptyAF();
			
			tData q0_2 = newNodeStrHard(loadStr2("q0"));
			tData q1_2 = newNodeStrHard(loadStr2("q1"));
			tData q2_2 = newNodeStrHard(loadStr2("q2"));
			insert_set(&(af2->Q->data), q0_2);
			insert_set(&(af2->Q->data), q1_2);
			insert_set(&(af2->Q->data), q2_2);
			
			tData sym_a = newNodeStrHard(loadStr2("a"));
			tData sym_b = newNodeStrHard(loadStr2("b"));
			insert_set(&(af2->Sigma->data), sym_a);
			insert_set(&(af2->Sigma->data), sym_b);
			
			af2->q0 = copy_tData(q0_2);
			insert_set(&(af2->F->data), copy_tData(q2_2));
			
			// Transiciones
			DeltaEntry t1, t2, t3, t4, t5, t6;
			t1.from = copy_tData(q0_2); t1.symbol = copy_tData(sym_a);
			t1.destinations = newEmptyNodeSet();
			insert_set(&(t1.destinations->data), copy_tData(q0_2));
			insert_set(&(t1.destinations->data), copy_tData(q1_2));
			
			t2.from = copy_tData(q0_2); t2.symbol = copy_tData(sym_b);
			t2.destinations = newEmptyNodeSet();
			insert_set(&(t2.destinations->data), copy_tData(q0_2));
			
			t3.from = copy_tData(q1_2); t3.symbol = copy_tData(sym_a);
			t3.destinations = newEmptyNodeSet();
			insert_set(&(t3.destinations->data), copy_tData(q2_2));
			
			t4.from = copy_tData(q1_2); t4.symbol = copy_tData(sym_b);
			t4.destinations = newEmptyNodeSet();
			insert_set(&(t4.destinations->data), copy_tData(q2_2));
			
			t5.from = copy_tData(q2_2); t5.symbol = copy_tData(sym_a);
			t5.destinations = newEmptyNodeSet();
			insert_set(&(t5.destinations->data), copy_tData(q0_2));
			
			t6.from = copy_tData(q2_2); t6.symbol = copy_tData(sym_b);
			t6.destinations = newEmptyNodeSet();
			insert_set(&(t6.destinations->data), copy_tData(q0_2));
			
			af2->delta = malloc(6 * sizeof(DeltaEntry));
			af2->delta[0] = t1; 
			af2->delta[1] = t2;
			af2->delta[2] = t3;
			af2->delta[3] = t4;
			af2->delta[4] = t5; 
			af2->delta[5] = t6;
			af2->deltaCount = 6;
			
			printAF(af2);
			printf("\n--- Pruebas AFND (penúltima letra = 'a') ---\n");
			printf("\"bab\"   : %s\n", acceptHardcoded(af2, "bab") ? "Aceptada" : "Rechazada");
			printf("\"ab\"    : %s\n", acceptHardcoded(af2, "ab") ? "Aceptada" : "Rechazada");
			printf("\"abaa\"  : %s\n", acceptHardcoded(af2, "abaa") ? "Aceptada" : "Rechazada");
			printf("\"abbab\" : %s\n", acceptHardcoded(af2, "abbab") ? "Aceptada" : "Rechazada");
			printf("\"ba\"    : %s\n", acceptHardcoded(af2, "ba") ? "Aceptada" : "Rechazada");
			printf("\"a\"     : %s\n", acceptHardcoded(af2, "a") ? "Aceptada" : "Rechazada");
			printf("\"bb\"    : %s\n", acceptHardcoded(af2, "bb") ? "Aceptada" : "Rechazada");
			printf("\"aba\"   : %s\n", acceptHardcoded(af2, "aba") ? "Aceptada" : "Rechazada");
			
			freeAF(af2);
			break;
		}
		case 3: {
			// ==================== Creación interactiva (solo mostrar) ====================
			printf("\n--- Creacion interactiva del automata ---\n");
			Automata* af3 = createAFinteractive();
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
			printf("Opción no válida. Intente de nuevo.\n");
		}
	} while (opcion != 4);
	
	return 0;
}
