#ifndef AF_H
#define AF_H

#include "Tree.h"

typedef tData State;   // Estado = STR (ej: "q0")
typedef tData Symbol;  // Símbolo = STR de un carácter (ej: "0")

// Entrada de la función de transición d
typedef struct {
	State from;           // estado origen
	Symbol symbol;        // símbolo leído
	tData destinations;   // conjunto (SET) de estados destino
} DeltaEntry;

// Automata finito (AFD o AFND)
typedef struct {
	tData Q;              // conjunto de estados (SET)
	tData Sigma;          // alfabeto (SET)
	DeltaEntry* delta;    // arreglo dinamico de transiciones
	int deltaCount;       // número de transiciones
	State q0;             // estado inicial
	tData F;              // conjunto de estados finales (SET)
} Automata;

// Creacion y destruccion
Automata* newEmptyAF(void);
Automata* createAFinteractive(void);
void freeAF(Automata* af);

// Consulta de los componentes de la 5-upla
tData getStates(const Automata* af);
tData getAlphabet(const Automata* af);
State getInitial(const Automata* af);
tData getFinals(const Automata* af);
DeltaEntry* getDelta(const Automata* af);
int getDeltaCount(const Automata* af);

// Propiedades y reconocimiento
int isDeterministic(const Automata* af);
int acceptString(Automata* af, str cadena);
int acceptHardcoded(Automata* af, const char* cadena);
int acceptFromConsole(Automata* af);

// Utilidad
void printAF(const Automata* af);

#endif
