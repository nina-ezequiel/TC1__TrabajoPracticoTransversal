#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* -------------------- Funciones auxiliares con TAD String ------------------- */

// Convierte una cadena "elem1,elem2,elem3" en un conjunto (SET) de strings
static tData splitToSet(str s, char delim) {
	tData set = newEmptyNodeSet();
	if (set == NULL || s == NULL) 
		return set;
	str resto = copyStr(s);
	while (resto != NULL) {
		str token = beforeToken(resto, delim);
		if (token != NULL) {
			tData elem = newNodeStrHard(token);
			insert_set(&(set->data), elem);
			freeStr(&token);
		}
		str nuevoResto = afterToken(resto, delim);
		freeStr(&resto);
		resto = nuevoResto;
	}
	return set;
}

// Parsea una transicion en formato "origen,simb,destino" o "origen,simb,{dest1,dest2}"
// Retorna una DeltaEntry. En caso de error, los campos quedan NULL.
static DeltaEntry parseDeltaEntry(str s) {
	DeltaEntry entry = {NULL, NULL, NULL};
	if (s == NULL) 
		return entry;
	// ---- Origen (antes de la 1ª coma) ----
	str fromStr = beforeToken(s, ',');
	if (fromStr == NULL) 
		return entry;
	entry.from = newNodeStrHard(fromStr);
	freeStr(&fromStr);
	// ---- Símbolo (entre 1ª y 2ª coma) ----
	str afterFirst = afterToken(s, ',');
	if (afterFirst == NULL) {
		free_tData(entry.from);
		return entry;
	}
	str symStr = beforeToken(afterFirst, ',');
	if (symStr == NULL) {
		free_tData(entry.from);
		freeStr(&afterFirst);
		return entry;
	}
	entry.symbol = newNodeStrHard(symStr);
	freeStr(&symStr);
	// ---- Destinos (después de la 2ª coma) ----
	str destPart = afterToken(afterFirst, ',');
	freeStr(&afterFirst);
	if (destPart == NULL) {
		free_tData(entry.from);
		free_tData(entry.symbol);
		return entry;
	}
	// Eliminar espacios iniciales
	while (destPart != NULL && (destPart->car == ' ' || destPart->car == '\t')) {
		str temp = destPart->next;
		free(destPart);
		destPart = temp;
	}
	if (destPart != NULL && destPart->car == '{') {
		// Formato con llaves: {dest1,dest2,...}
		str temp = destPart->next;
		free(destPart);
		destPart = temp;   // saltamos '{'
		
		// Buscar la llave de cierre
		str closeBrace = destPart;
		while (closeBrace != NULL && closeBrace->car != '}')
			closeBrace = closeBrace->next;
		
		if (closeBrace != NULL) {
			// Copiar hasta el '}'
			str destCopy = copyStr(destPart);
			str destClean = beforeToken(destCopy, '}');
			freeStr(&destCopy);
			entry.destinations = splitToSet(destClean, ',');
			freeStr(&destClean);
		} 
		else {
			entry.destinations = splitToSet(destPart, ',');
		}
	} 
	else {
		// Un solo destino (sin llaves)
		tData destSet = newEmptyNodeSet();
		tData dest = newNodeStrHard(destPart);
		insert_set(&(destSet->data), dest);
		entry.destinations = destSet;
	}
	freeStr(&destPart);
	return entry;
}

/* -------------------- Creación y destrucción ------------------- */

Automata* newEmptyAF() {
	Automata* af = (Automata*)malloc(sizeof(Automata));
	if (!af) return NULL;
	af->Q = newEmptyNodeSet();
	af->Sigma = newEmptyNodeSet();
	af->delta = NULL;
	af->deltaCount = 0;
	af->q0 = NULL;
	af->F = newEmptyNodeSet();
	return af;
}

Automata* createAFinteractive() {
	Automata* af = newEmptyAF();
	if (!af) 
		return NULL;
	printf("\n=== Creacion interactiva del automata ===\n");
	printf("Ingrese las transiciones en el formato: origen,simbolo,destino\n");
	printf("  (para multiples destinos use llaves: origen,simbolo,{dest1,dest2})\n");
	printf("Escriba 'fin' para terminar.\n");
	printf("La primera transición determinara el estado inicial.\n\n");
	// Conjuntos temporales para Q y Sigma (usando el TAD Tree)
	tData Q_set = newEmptyNodeSet();     // acumula estados
	tData Sigma_set = newEmptyNodeSet(); // acumula símbolos
	int first = 1;                       // primera transición
	str finStr = loadStr2("fin");
	while (1) {
		printf("d(estado,símbolo) = ");
		str s = loadStr();
		if (equalStr(s, finStr)) {
			freeStr(&s);
			break;
		}
		DeltaEntry entry = parseDeltaEntry(s);
		freeStr(&s);
		if (!entry.from || !entry.symbol || !entry.destinations) {
			printf("Formato invalido. Intente de nuevo.\n");
			if (entry.from) free_tData(entry.from);
			if (entry.symbol) free_tData(entry.symbol);
			if (entry.destinations) free_tData(entry.destinations);
			continue;
		}
		// ---- Agregar 'from' a Q ----
		insert_set(&(Q_set->data), copy_tData(entry.from));
		// ---- Agregar todos los destinos a Q ----
		tData destNode = entry.destinations->data;
		while (destNode) {
			insert_set(&(Q_set->data), copy_tData(destNode));
			destNode = destNode->next;
		}
		// ---- Agregar símbolo a Sigma ----
		insert_set(&(Sigma_set->data), copy_tData(entry.symbol));
		// ---- Definir estado inicial (primera transicion) ----
		if (first) {
			if (af->q0) free_tData(af->q0);
			af->q0 = copy_tData(entry.from);
			first = 0;
		}
		// ---- Guardar la transicion en el automata ----
		af->delta = (DeltaEntry*)realloc(af->delta, (af->deltaCount + 1) * sizeof(DeltaEntry));
		af->delta[af->deltaCount++] = entry;
	}
	freeStr(&finStr);
	// Si no se ingreso ninguna transición, liberar y retornar NULL
	if (first) {
		printf("No se ingresaron transiciones. Autómata vacío.\n");
		free_tData(Q_set);
		free_tData(Sigma_set);
		freeAF(af);
		return NULL;
	}
	// Asignar Q y Sigma al autómata
	free_tData(af->Q);
	af->Q = Q_set;
	free_tData(af->Sigma);
	af->Sigma = Sigma_set;
	// ---- Pedir estados finales ----
	printf("\nAhora ingrese los estados finales (separados por comas): ");
	str fStr = loadStr();
	tData Fset = splitToSet(fStr, ',');
	freeStr(&fStr);
	if (Fset) {
		free_tData(af->F);
		af->F = Fset;
	} 
	else {
		af->F = newEmptyNodeSet();
	}
	return af;
}

void freeAF(Automata* af) {
	if (!af) return;
	free_tData(af->Q);
	free_tData(af->Sigma);
	for (int i = 0; i < af->deltaCount; i++) {
		free_tData(af->delta[i].from);
		free_tData(af->delta[i].symbol);
		free_tData(af->delta[i].destinations);
	}
	free(af->delta);
	free_tData(af->q0);
	free_tData(af->F);
	free(af);
}

/* -------------------- Getters ------------------- */
tData getStates(const Automata* af) { 
	return af->Q; 
}

tData getAlphabet(const Automata* af) { 
	return af->Sigma; 
}

State getInitial(const Automata* af) {
	return af->q0; 
}

tData getFinals(const Automata* af) { 
	return af->F; 
}

DeltaEntry* getDelta(const Automata* af) { 
	return af->delta; 
}

int getDeltaCount(const Automata* af) { 
	return af->deltaCount; 
}

/* -------------------- Determinismo ------------------- */

int isDeterministic(const Automata* af) {
	if (!af) return 0;
	for (int i = 0; i < af->deltaCount; i++) {
		// El conjunto destino debe tener exactamente un estado
		if (cardinalitySet(af->delta[i].destinations) != 1)
			return 0;
		// No debe haber otra transicion con el mismo (from, symbol)
		for (int j = i+1; j < af->deltaCount; j++) {
			if (equal_tData(af->delta[i].from, af->delta[j].from) && equal_tData(af->delta[i].symbol, af->delta[j].symbol))
				return 0;
		}
	}
	return 1;
}

/* -------------------- Aceptación de cadenas ------------------- */

// Retorna el conjunto de destinos para (from, symbol), o NULL si no existe
static tData getDestinations(const Automata* af, State from, Symbol sym) {
	for (int i = 0; i < af->deltaCount; i++) {
		if (equal_tData(af->delta[i].from, from) && equal_tData(af->delta[i].symbol, sym)) {
			return af->delta[i].destinations;   // El SET pertenece al autómata
		}
	}
	return NULL;   // Sin transición
}

// Backtracking sobre conjuntos de estados
static int acceptRecursive(const Automata* af, tData currentStates, str input) {
	if (input == NULL){   // fin de cadena
		tData it = currentStates->data;
		while (it) {
			if (pertainSet(it, af->F))
				return 1;
			it = it->next;
		}
		return 0;
	}
	// Convertir el caracter actual a Symbol (STR de un caracter)
	char symBuf[2] = {input->car, '\0'};
	Symbol sym = newNodeStrHard(loadStr2(symBuf));
	tData nextSet = newEmptyNodeSet();
	tData stateNode = currentStates->data;
	while (stateNode) {
		tData destSet = getDestinations(af, stateNode, sym);
		if (destSet != NULL) {
			tData dest = destSet->data;
			while (dest){
				insert_set(&(nextSet->data), copy_tData(dest));
				dest = dest->next;
			}
		}
		stateNode = stateNode->next;
	}
	free_tData(sym);
	int result = acceptRecursive(af, nextSet, input->next);
	free_tData(nextSet);
	return result;
}

int acceptString(Automata* af, str cadena) {
	if (!af || !cadena) return 0;
	// Conjunto inicial: {q0}
	tData startState = newEmptyNodeSet();
	insert_set(&(startState->data), copy_tData(af->q0));
	int accepted = acceptRecursive(af, startState, cadena);
	free_tData(startState);
	return accepted;
}

int acceptHardcoded(Automata* af, const char* cadena) {
	str s = loadStr2(cadena);
	int res = acceptString(af, s);
	freeStr(&s);
	return res;
}

int acceptFromConsole(Automata* af) {
	printf("\nIngrese la cadena a evaluar: ");
	str s = loadStr();
	int res = acceptString(af, s);
	freeStr(&s);
	return res;
}

/* -------------------- Impresion ------------------- */
void printAF(const Automata* af) {
	printf("\n=== AUTOMATA ===\n");
	printf("Q = "); printData(af->Q);
	printf("\nSigma = "); printData(af->Sigma);
	printf("\nq0 = "); printData(af->q0);
	printf("\nF = "); printData(af->F);
	printf("\ndelta:\n");
	for (int i = 0; i < af->deltaCount; i++) {
		printf("  d(");
		printData(af->delta[i].from);
		printf(", ");
		printData(af->delta[i].symbol);
		printf(") = ");
		printData(af->delta[i].destinations);
		printf("\n");
	}
	printf("\nNumero de Transiciones %d: ", af->deltaCount);
	printf("\nEs Determinista?: %s\n", isDeterministic(af) ? "Si" : "No");
}
