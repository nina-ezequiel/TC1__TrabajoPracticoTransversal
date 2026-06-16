#include "AF_Converter.h"
#include <stdio.h>
#include <stdlib.h>

/* ------------------------------------------------------------
* Verifica si un subconjunto contiene al menos un estado final del AFND.
* ------------------------------------------------------------ */
static int containsFinal(tData subset, tData finalSet) {
	tData it = tData_getFirst(subset);
	while (it) {
		if (pertainSet(it, finalSet))
			return 1;
		it = tData_getNext(it);
	}
	return 0;
}

/* ------------------------------------------------------------
* Genera el nombre de un estado del AFD a partir de su ID (p0, p1, ...).
* ------------------------------------------------------------ */
static str getStateName(int id) {
	char buf[10];
	sprintf(buf, "p%d", id);
	return loadStr2(buf);
}

/* ------------------------------------------------------------
* Crea un AFD vacío y copia el alfabeto del AFND.
* ------------------------------------------------------------ */
static Af crearAFDVacioConAlfabeto(const Af afnd) {
	Af afd = newEmptyAF();
	if (!afd) return NULL;
	tData sigmaNode = tData_getFirst(getAlphabet(afnd));
	while (sigmaNode) {
		AF_addSymbol(afd, sigmaNode);
		sigmaNode = tData_getNext(sigmaNode);
	}
	return afd;
}

/* ------------------------------------------------------------
* Inicializa los arreglos subConjuntos y estadosAFD con el subconjunto inicial {q0_nd}.
* ------------------------------------------------------------ */
static void inicializarArreglos(Af afd, const Af afnd, tData** subConjuntos, tData** estadosAFD, int* numConjuntos) {
	tData startSet = newEmptyNodeSet();
	tData_addToSet(startSet, copy_tData(getInitial(afnd)));
	*subConjuntos = (tData*)realloc(*subConjuntos, sizeof(tData));
	(*subConjuntos)[0] = startSet;
	*numConjuntos = 1;
	
	str name0 = getStateName(0);
	tData state0 = newNodeStrHard(name0);
	freeStr(&name0);
	AF_addState(afd, state0);
	AF_setInitial(afd, state0);
	*estadosAFD = (tData*)realloc(*estadosAFD, sizeof(tData));
	(*estadosAFD)[0] = state0;
}

/* ------------------------------------------------------------
* Busca o añade un destSet en los arreglos.
* ------------------------------------------------------------ */
static int obtenerOCrearIndice(Af afd, tData destSet, tData** subConjuntos, tData** estadosAFD, int* numConjuntos) {
	for (int k = 0; k < *numConjuntos; k++) {
		if (equal_tData((*subConjuntos)[k], destSet))
			return k;
	}
	int idx = *numConjuntos;
	*subConjuntos = (tData*)realloc(*subConjuntos, (idx + 1) * sizeof(tData));
	(*subConjuntos)[idx] = destSet;
	(*numConjuntos)++;
	
	str newName = getStateName(idx);
	tData newState = newNodeStrHard(newName);
	freeStr(&newName);
	AF_addState(afd, newState);
	*estadosAFD = (tData*)realloc(*estadosAFD, *numConjuntos * sizeof(tData));
	(*estadosAFD)[idx] = newState;
	
	return idx;
}

/* ------------------------------------------------------------
* Agrega una transición al AFD desde currentState con símbolo sigmaNode
* hacia el estado con índice destIdx.
* ------------------------------------------------------------ */
static void agregarTransicionAFD(Af afd, tData currentState, tData sigmaNode, int destIdx, tData* estadosAFD) {
   tData unit = newEmptyNodeSet();
   tData_addToSet(unit, copy_tData(estadosAFD[destIdx]));
   AF_addTransition(afd, currentState, sigmaNode, unit);
   free_tData(unit);
}

/* ------------------------------------------------------------
* Marca como finales los estados del AFD cuyo subconjunto asociado
* contiene al menos un estado final del AFND.
* ------------------------------------------------------------ */
static void marcarFinalesAFD(Af afd, const Af afnd, tData* subConjuntos, tData* estadosAFD, int numConjuntos) {
	for (int i = 0; i < numConjuntos; i++) {
		if (containsFinal(subConjuntos[i], getFinals(afnd))) {
			AF_addFinal(afd, estadosAFD[i]);
		}
	}
}
 
 /* ------------------------------------------------------------
 * Libera la memoria de los arreglos auxiliares.
 * ------------------------------------------------------------ */
static void liberarArreglos(tData* subConjuntos, tData* estadosAFD, int numConjuntos) {
	for (int i = 0; i < numConjuntos; i++) {
		free_tData(subConjuntos[i]);
	}
	free(subConjuntos);
	free(estadosAFD);
}
 
/* ============================================================
* FUNCIÓN PÚBLICA: computeNextSet
* ============================================================ */
tData computeNextSet(const Af af, tData currentStates, Symbol sym) {
	tData nextSet = newEmptyNodeSet();
	tData stateNode = tData_getFirst(currentStates);
	while (stateNode) {
		tData dests = getDestinations(af, stateNode, sym);
		if (dests != NULL) {
			tData dnode = tData_getFirst(dests);
			while (dnode) {
				tData_addToSet(nextSet, copy_tData(dnode));
				dnode = tData_getNext(dnode);
			}
		}
		stateNode = tData_getNext(stateNode);
	}
	if (tData_getFirst(nextSet) == NULL) {
		free_tData(nextSet);
		return NULL;
	}
	return nextSet;
}
 
/* ============================================================
* FUNCIÓN PRINCIPAL: AFNDtoAFD
* ============================================================ */
Af AFNDtoAFD(const Af afnd) {
	if (afnd == NULL) return NULL;
	
	Af afd = crearAFDVacioConAlfabeto(afnd);
	if (!afd) return NULL;
	
	tData* subConjuntos = NULL;
	tData* estadosAFD = NULL;
	int numConjuntos = 0;
	inicializarArreglos(afd, afnd, &subConjuntos, &estadosAFD, &numConjuntos);
	
	for (int i = 0; i < numConjuntos; i++) {
		tData currentSet = subConjuntos[i];
		tData currentState = estadosAFD[i];
		tData sigmaNode = tData_getFirst(getAlphabet(afnd));
		while (sigmaNode) {
			// Usamos la función pública computeNextSet (definida arriba)
			tData destSet = computeNextSet(afnd, currentSet, sigmaNode);
			if (destSet != NULL) {
				int idx = obtenerOCrearIndice(afd, destSet, &subConjuntos, &estadosAFD, &numConjuntos);
				agregarTransicionAFD(afd, currentState, sigmaNode, idx, estadosAFD);
			}
			sigmaNode = tData_getNext(sigmaNode);
		}
	}
	
	marcarFinalesAFD(afd, afnd, subConjuntos, estadosAFD, numConjuntos);
	liberarArreglos(subConjuntos, estadosAFD, numConjuntos);
	
	return afd;
}
