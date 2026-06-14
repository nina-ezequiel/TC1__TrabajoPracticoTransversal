#include "AF_Operations.h"

/* -------------------- parseDeltaEntry ------------------- */
DeltaEntry parseDeltaEntry(str s) {
	DeltaEntry entry = {NULL, NULL, NULL};
	if (s == NULL) return entry;
	
	// Origen (antes de la 1ª coma)
	str fromStr = beforeToken(s, ',');
	if (fromStr == NULL) return entry;
	entry.from = newNodeStrHard(fromStr);
	freeStr(&fromStr);
	
	// Simbolo (entre 1ª y 2ª coma)
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
	
	// Destinos (despues de la 2ª coma)
	str destPart = afterToken(afterFirst, ',');
	freeStr(&afterFirst);
	if (destPart == NULL) {
		free_tData(entry.from);
		free_tData(entry.symbol);
		return entry;
	}
	entry.destinations = strToSetToken(destPart, ',');
	freeStr(&destPart);
	return entry;
}

/* -------------------- addTransition (fusion) ------------------- */
void addTransition(DeltaEntry** delta, int* deltaCount, DeltaEntry newEntry) {
	// Buscar si ya existe una transicion con el mismo origen y simbolo
	for (int i = 0; i < *deltaCount; i++) {
		if (equal_tData((*delta)[i].from, newEntry.from) &&
			equal_tData((*delta)[i].symbol, newEntry.symbol)) {
			// Fusionar destinos
			tData destExistentes = (*delta)[i].destinations;
			tData destNode = tData_getFirst(newEntry.destinations);
			while (destNode) {
				tData destCopia = copy_tData(destNode);
				tData_addToSet(destExistentes, destCopia);
				destNode = tData_getNext(destNode);
			}
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

/* -------------------- readTransitions ------------------- */
void readTransitions(DeltaEntry** delta, int* deltaCount,
	tData Q_set, tData Sigma_set,
	State* q0, tData* F) {
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
		// Agregar simbolo a Sigma_set
		tData_addToSet(Sigma_set, copy_tData(entry.symbol));
		
		// Definir estado inicial (primera transicion)
		if (first) {
			if (*q0) free_tData(*q0);
			*q0 = copy_tData(entry.from);
			first = 0;
		}
		
		// Guardar transicion (fusionando)
		addTransition(delta, deltaCount, entry);
	}
	freeStr(&finStr);
	
	if (*deltaCount == 0) {
		*F = NULL;
		return;
	}
	
	// Leer estados finales
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
 
/* -------------------- printAF ------------------- */
void printAF(const Af af) {
	printf("\n=== AUTOMATA ===\n");
	printf("Q = "); printData(getStates(af));
	printf("\nSigma = "); printData(getAlphabet(af));
	printf("\nq0 = "); printData(getInitial(af));
	printf("\nF = "); printData(getFinals(af));
	printf("\ndelta:\n");
	for (int i = 0; i < getDeltaCount(af); i++) {
		const DeltaEntry* d = &getDelta(af)[i];
		printf("  d(");
		printData(d->from);
		printf(", ");
		printData(d->symbol);
		printf(") = ");
		printData(d->destinations);
		printf("\n");
	}
	printf("\nNumero de Transiciones %d: ", af->deltaCount);
	printf("\nEs Determinista?: %s\n", isDeterministic(af) ? "Si" : "No");
}
