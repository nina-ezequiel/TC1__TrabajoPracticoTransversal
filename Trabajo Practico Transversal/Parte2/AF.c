#include "AF.h"

/* -------------------- Funciones auxiliares con TAD String ------------------- */

// Parsea una transicion en formato "origen,simb,destino" o "origen,simb,dest1,dest2,...,dest_n"
// Retorna una DeltaEntry. En caso de error, los campos quedan NULL.
static DeltaEntry parseDeltaEntry(str s) {
	DeltaEntry entry = {NULL, NULL, NULL};
	if (s == NULL) 
		return entry;
	// ---- Origen (antes de la 1° coma) ----
	str fromStr = beforeToken(s, ',');
	if (fromStr == NULL) 
		return entry;
	entry.from = newNodeStrHard(fromStr);
	freeStr(&fromStr);
	// ---- Símbolo (entre 1° y 2° coma) ----
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
	// ---- Destinos (después de la 2° coma) ----
	str destPart = afterToken(afterFirst, ',');
	freeStr(&afterFirst);
	if (destPart == NULL) {
		free_tData(entry.from);
		free_tData(entry.symbol);
		return entry;
	}
	tData destSet = strToSetToken(destPart,',');
	entry.destinations = destSet;
	freeStr(&destPart);
	return entry;
}

/* -------------------- Creación y destrucción ------------------- */

Af newEmptyAF() {
	Af af = (Af)malloc(sizeof(Automata));
	if (!af) return NULL;
	af->Q = newEmptyNodeSet();
	af->Sigma = newEmptyNodeSet();
	af->delta = NULL;
	af->deltaCount = 0;
	af->q0 = NULL;
	af->F = newEmptyNodeSet();
	return af;
}

Af createAFinteractive() {
	Af af = newEmptyAF();
	if (!af) 
		return NULL;
	printf("\n=== Creacion interactiva del automata ===\n");
	printf("Ingrese las transiciones en el formato: origen,simbolo,destino\n");
	printf("  (para multiples destinos escriba: origen,simbolo,dest1,dest2,...,dent_n)\n");
	printf("Escriba 'fin' para terminar.\n");
	printf("La primera transicion determinara el estado inicial.\n\n");
	// Conjuntos temporales para Q y Sigma (usando el TAD Tree)
	tData Q_set = newEmptyNodeSet();     // acumula estados
	tData Sigma_set = newEmptyNodeSet(); // acumula símbolos
	int first = 1;                       // primera transición
	str finStr = loadStr2("fin");
	while (1) {
		printf("d(estado,simbolo) = ");
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
		printf("No se ingresaron transiciones. Automata vacio.\n");
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
	while (1) {
		printf("\nAhora ingrese el/los estado/s final/es (separados por comas): ");
		str fStr = loadStr();
		tData Fset = strToSetToken(fStr, ',');
		freeStr(&fStr);
		if (inclusionSet(Fset, af->Q)) {
			free_tData(af->F);
			af->F = Fset;
			break;  // salir del bucle
		} 
		else {
			printf("\nLos estados ingresados no estan incluidos en Q. Vuelva a intentar: ");
			free_tData(Fset);
		}
	}
	return af;
}

void freeAF(Af af) {
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

tData getStates(const Af af) { return af->Q; }

tData getAlphabet(const Af af) { return af->Sigma; }

State getInitial(const Af af) { return af->q0; }

tData getFinals(const Af af) { return af->F; }

DeltaEntry* getDelta(const Af af) { return af->delta; }

int getDeltaCount(const Af af) { return af->deltaCount; }

/* -------------------- Determinismo ------------------- */

int isDeterministic(const Af af) {
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
static tData getDestinations(const Af af, State from, Symbol sym) {
	for (int i = 0; i < af->deltaCount; i++) {
		if (equal_tData(af->delta[i].from, from) && equal_tData(af->delta[i].symbol, sym)) {
			return af->delta[i].destinations;   // El SET pertenece al autómata
		}
	}
	return NULL;   // Sin transición
}

// Backtracking sobre conjuntos de estados
static int acceptRecursive(const Af af, tData currentStates, str input) {
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

int acceptString(const Af af, str cadena) {
	if (!af || !cadena) return 0;
	// Conjunto inicial: {q0}
	tData startState = newEmptyNodeSet();
	insert_set(&(startState->data), copy_tData(af->q0));
	int accepted = acceptRecursive(af, startState, cadena);
	free_tData(startState);
	return accepted;
}

int acceptHardcoded(const Af af, const char* cadena) {
	str s = loadStr2(cadena);
	int res = acceptString(af, s);
	freeStr(&s);
	return res;
}

int acceptFromConsole(const Af af) {
	printf("\nIngrese la cadena a evaluar: ");
	str s = loadStr();
	int res = acceptString(af, s);
	freeStr(&s);
	return res;
}

/* -------------------- Impresion ------------------- */

void printAF(const Af af) {
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
