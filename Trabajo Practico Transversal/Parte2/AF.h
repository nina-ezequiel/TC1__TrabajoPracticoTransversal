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

typedef Automata* Af;

// Creación y destrucción
Af newEmptyAF(void);
Af createAFinteractive(void);
void freeAF(Af af);

// Consulta de los componentes de la 5-upla
tData getStates(const Af af);
tData getAlphabet(const Af af);
State getInitial(const Af af);
tData getFinals(const Af af);  
DeltaEntry* getDelta(const Af af);
int getDeltaCount(const Af af);

// Propiedades y reconocimiento
int isDeterministic(const Af af);
int acceptString(const Af af, str cadena);
int acceptHardcoded(const Af af, const char* cadena);
int acceptFromConsole(const Af af);

// Utilidad
void printAF(const Af af);

#endif
