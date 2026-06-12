#include "AF.h"

/* -------------------- Funciones auxiliares usando modulos de TAD String ------------------- */

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
	// ---- Destinos (despues de la 2° coma) ----
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

/* -------------------- Creacion y destruccion ------------------- */

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

// Agrega una transición al arreglo, fusionando destinos si ya existe otra con igual (from, symbol)
static void addTransition2(DeltaEntry** delta, int* deltaCount, DeltaEntry newEntry) {
	// Buscar si ya existe una transición con el mismo origen y símbolo
	for (int i = 0; i < *deltaCount; i++) {
		if (equal_tData((*delta)[i].from, newEntry.from) &&
			equal_tData((*delta)[i].symbol, newEntry.symbol)) {
			
			// Fusionar destinos: agregar todos los destinos de newEntry al conjunto existente
			tData destExistentes = (*delta)[i].destinations;
			tData destNode = tData_getFirst(newEntry.destinations);
			while (destNode) {
				// Copiar el destino antes de agregarlo (el original se liberará después)
				tData destCopia = copy_tData(destNode);
				tData_addToSet(destExistentes, destCopia);
				destNode = tData_getNext(destNode);
			}
			// Liberar la nueva entrada (ya no se necesita)
			free_tData(newEntry.from);
			free_tData(newEntry.symbol);
			free_tData(newEntry.destinations);
			return; // No se incrementa deltaCount
		}
	}
	// No existe: agregar nueva entrada al arreglo
	*delta = (DeltaEntry*)realloc(*delta, (*deltaCount + 1) * sizeof(DeltaEntry));
	(*delta)[(*deltaCount)++] = newEntry;
}

static void leerTransiciones(DeltaEntry** delta, int* deltaCount, tData Q_set, tData Sigma_set, State* q0, tData* F) {
	int first = 1;
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
		
		// Agregar 'from' a Q_set
		tData_addToSet(Q_set, copy_tData(entry.from));
		// Agregar todos los destinos a Q_set
		tData destNode = tData_getFirst(entry.destinations);
		while (destNode) {
			tData_addToSet(Q_set, copy_tData(destNode));
			destNode = tData_getNext(destNode);
		}
		// Agregar símbolo a Sigma_set
		tData_addToSet(Sigma_set, copy_tData(entry.symbol));
		
		// Definir estado inicial (primera transición)
		if (first) {
			if (*q0) free_tData(*q0);
			*q0 = copy_tData(entry.from);
			first = 0;
		}
		
		// Guardar transición
		addTransition2(delta, deltaCount, entry);
	}
	freeStr(&finStr);
	
	// Si no se ingresaron transiciones, no preguntamos finales
	if (*deltaCount == 0) {
		*F = NULL;
		return;
	}
	
	// ---- Leer estados finales ----
	while (1) {
		printf("\nAhora ingrese el/los estado/s final/es (separados por comas): ");
		str fStr = loadStr();
		tData Fset = strToSetToken(fStr, ',');
		freeStr(&fStr);
		if (inclusionSet(Fset, Q_set)) {
			*F = Fset;
			break;
		} else {
			printf("\nLos estados ingresados no estan incluidos en Q. Vuelva a intentar: ");
			free_tData(Fset);
		}
	}
}

Af createAFinteractive() {
	Af af = newEmptyAF();
	if (!af) return NULL;
	
	printf("\n=== Creacion interactiva del automata ===\n\n");
	printf("Ingrese las transiciones en el formato:\n\t origen,simbolo,destino\n\n");
	printf("Para multiples destinos ingrese las transiciones en el formato:\n\t origen,simbolo,dest1,dest2,...,dest_n\n\n");
	printf("Escriba 'fin' para terminar.\n");
	printf("La primera transicion determinara el estado inicial.\n\n");
	
	tData Q_set = newEmptyNodeSet();
	tData Sigma_set = newEmptyNodeSet();
	DeltaEntry* delta = NULL;
	int deltaCount = 0;
	State q0 = NULL;
	tData F = NULL;
	
	leerTransiciones(&delta, &deltaCount, Q_set, Sigma_set, &q0, &F);
	
	if (deltaCount == 0) {
		printf("No se ingresaron transiciones. Automata vacio.\n");
		free_tData(Q_set);
		free_tData(Sigma_set);
		freeAF(af);
		return NULL;
	}
	
	// Asignar al autómata
	free_tData(af->Q);
	af->Q = Q_set;
	free_tData(af->Sigma);
	af->Sigma = Sigma_set;
	af->delta = delta;
	af->deltaCount = deltaCount;
	if (af->q0) free_tData(af->q0);
	af->q0 = q0;
	free_tData(af->F);
	af->F = F;
	
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
		tData it = tData_getFirst(currentStates);
		while (it) {
			if (pertainSet(it, af->F))
				return 1;
			it = tData_getNext(it);  
		}
		return 0;
	}
	// Convertir el caracter actual a Symbol (STR de un caracter)
	char symBuf[2] = {input->car, '\0'};
	Symbol sym = newNodeStrHard(loadStr2(symBuf));
	tData nextSet = newEmptyNodeSet();
	tData stateNode = tData_getFirst(currentStates);
	while (stateNode) {
		tData destSet = getDestinations(af, stateNode, sym);
		if (destSet != NULL) {
			tData dest = tData_getFirst(destSet);
			while (dest){
				tData_addToSet(nextSet, copy_tData(dest));
				dest = tData_getNext(dest);
			}
		}
		stateNode = tData_getNext(stateNode);
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
	tData_addToSet(startState, copy_tData(af->q0));
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
	printf("Q = "); printData(getStates(af));
	printf("\nSigma = "); printData(getAlphabet(af));
	printf("\nq0 = "); printData(getInitial(af));
	printf("\nF = "); printData(getFinals(af));
	printf("\ndelta:\n");
	for (int i = 0; i < getDeltaCount(af); i++) {
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


/* -------------------- Funciones de modificacion (encapsulamiento) ------------------- */

// Agrega un estado al conjunto Q del automata
void addState(Af af, State s) {
	if (af == NULL || s == NULL) return;
	tData_addToSet(af->Q, copy_tData(s));
}

// Agrega un simbolo al alfabeto Sigma del automata
void addSymbol(Af af, Symbol s) {
	if (af == NULL || s == NULL) return;
	tData_addToSet(af->Sigma, copy_tData(s));
}

// Define el estado inicial del automata
void setInitial(Af af, State s) {
	if (af == NULL || s == NULL) return;
	if (af->q0) free_tData(af->q0);
	af->q0 = copy_tData(s);
}

// Agrega un estado final al conjunto F del automata
void addFinal(Af af, State s) {
	if (af == NULL || s == NULL) return;
	tData_addToSet(af->F, copy_tData(s));
}

// Agrega una transicion (origen, simbolo) -> conjunto de destinos.
// Si ya existe una transicion con el mismo origen y simbolo, fusiona los destinos.
void addTransition(Af af, State from, Symbol sym, tData dests) {
	if (af == NULL || from == NULL || sym == NULL || dests == NULL) return;
	
	// Crear una nueva entrada temporal
	DeltaEntry newEntry;
	newEntry.from = copy_tData(from);
	newEntry.symbol = copy_tData(sym);
	newEntry.destinations = copy_tData(dests);
	
	// Usar la funcion interna de fusion (agregarTransicion)
	addTransition2(&af->delta, &af->deltaCount, newEntry);
}
