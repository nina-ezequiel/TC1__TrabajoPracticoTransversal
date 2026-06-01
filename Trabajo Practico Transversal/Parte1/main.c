#include "Tree.h"

void demostracionCompleta() {
	
	printf("======================================================\n");
	printf("            DEMOSTRACION COMPLETA DEL TDA\n");
	printf("======================================================\n\n");
	
	// ==================== PARTE 1: STRINGS ====================
	printf("1. OPERACIONES CON STRINGS\n");
	printf("-----------------------------------------\n");
	
	// Crear strings
	tData strHola = newNodeStrHard(loadStr2("Hola"));
	tData strMundo = newNodeStrHard(loadStr2("Mundo"));
	tData strHolaMundo = newNodeStrHard(loadStr2("HolaMundo"));
	
	printf("String 1: "); printData(strHola);
	printf("String 2: "); printData(strMundo);
	printf("String 3: "); printData(strHolaMundo);
	
	// Concatenar
	str resultadoConcat = concatStr(strHola->str, strMundo->str);
	printf("Concatenacion 'Hola' + 'Mundo': ");
	printStr(resultadoConcat);
	printf("\n");
	freeStr(&resultadoConcat);
	
	// Before/After token
	str antes = beforeToken(strHolaMundo->str, 'M');
	printf("Antes de 'M' en 'HolaMundo': ");
	printStr(antes);
	printf("\n");
	freeStr(&antes);
	
	str despues = afterToken(strHolaMundo->str, 'M');
	printf("Despues de 'M' en 'HolaMundo': ");
	printStr(despues);
	printf("\n");
	freeStr(&despues);
	
	// isinStr
	int contiene = isinStr(strHolaMundo->str, strHola->str);
	printf("'HolaMundo' contiene 'Hola'? %s\n", contiene ? "SI" : "NO");
	
	// equalStr
	int igual = equalStr(strHola->str, strHola->str);
	printf("'Hola' es igual a 'Hola'? %s\n", igual ? "SI" : "NO");
	
	// sizeStr
	printf("Longitud de 'HolaMundo': %d\n", sizeStr(strHolaMundo->str));
	
	// Liberar strings
	free_tData(strHola);
	free_tData(strMundo);
	free_tData(strHolaMundo);
	
	
	// ==================== PARTE 2: LISTAS ====================
	printf("\n\n2. OPERACIONES CON LISTAS\n");
	printf("-----------------------------------------\n");
	
	// Crear lista 1
	tData lista1 = newEmptyNodeList();
	tData elem1 = newNodeStrHard(loadStr2("A"));
	tData elem2 = newNodeStrHard(loadStr2("B"));
	tData elem3 = newNodeStrHard(loadStr2("C"));
	
	append(&(lista1->data), elem1);
	append(&(lista1->data), elem2);
	append(&(lista1->data), elem3);
	
	printf("Lista 1: "); 
	printData(lista1);
	
	// Crear lista 2
	tData lista2 = newEmptyNodeList();
	tData elem4 = newNodeStrHard(loadStr2("D"));
	tData elem5 = newNodeStrHard(loadStr2("E"));
	
	append(&(lista2->data), elem4);
	append(&(lista2->data), elem5);
	
	printf("Lista 2: "); 
	printData(lista2);
	
	// Concatenar listas
	tData listaConcat = concatData(lista1, lista2);
	printf("Concatenacion Lista1.Lista2: ");
	printData(listaConcat);
	
	// Buscar en lista
	tData elemBuscar = newNodeStrHard(loadStr2("B"));
	tData encontrado = search(lista1, elemBuscar);
	printf("Buscando 'B' en Lista1: ");
	if (encontrado) {
		printf("ENCONTRADO -> ");
		printData(encontrado);
		free_tData(encontrado);
	} else {
		printf("NO ENCONTRADO\n");
	}
	
	tData elemNoExistente = newNodeStrHard(loadStr2("Z"));
	encontrado = search(lista1, elemNoExistente);
	printf("Buscando 'Z' en Lista1: ");
	if (encontrado) {
		printf("ENCONTRADO -> ");
		printData(encontrado);
		free_tData(encontrado);
	} else {
		printf("NO ENCONTRADO\n");
	}
	
	// Liberar
	free_tData(elemBuscar);
	free_tData(elemNoExistente);
	free_tData(lista1);
	free_tData(lista2);
	free_tData(listaConcat);
	
	// ==================== PARTE 3: CONJUNTOS ====================
	printf("\n\n3. OPERACIONES CON CONJUNTOS\n");
	printf("-----------------------------------------\n");
	
	// Crear conjunto A
	tData conjuntoA = newEmptyNodeSet();
	tData ce1 = newNodeStrHard(loadStr2("Rojo"));
	tData ce2 = newNodeStrHard(loadStr2("Verde"));
	tData ce3 = newNodeStrHard(loadStr2("Azul"));
	
	insert_set(&(conjuntoA->data), ce1);
	insert_set(&(conjuntoA->data), ce2);
	insert_set(&(conjuntoA->data), ce3);
	insert_set(&(conjuntoA->data), copy_tData(ce1)); // Duplicado (no se agrega)
	
	printf("\nConjunto A: "); 
	printData(conjuntoA);
	printf("Cardinalidad: %d\n", cardinalitySet(conjuntoA));
	
	// Crear conjunto B
	tData conjuntoB = newEmptyNodeSet();
	tData ce4 = newNodeStrHard(loadStr2("Verde"));
	tData ce5 = newNodeStrHard(loadStr2("Amarillo"));
	
	insert_set(&(conjuntoB->data), ce4);
	insert_set(&(conjuntoB->data), ce5);
	
	printf("Conjunto B: "); 
	printData(conjuntoB);
	
	// Crear conjunto C
	tData conjuntoC = newEmptyNodeSet();
	tData ce6 = newNodeStrHard(loadStr2("Verde"));
	tData ce7 = newNodeStrHard(loadStr2("Rojo"));
	
	insert_set(&(conjuntoC->data), ce6);
	insert_set(&(conjuntoC->data), ce7);
	
	printf("Conjunto C: "); 
	printData(conjuntoC);
	
	// Pertenencia
	tData elemPertenece = newNodeStrHard(loadStr2("Rojo"));
	int pert = pertainSet(elemPertenece, conjuntoA);
	printf("'Rojo' pertenece a A? %s\n", pert ? "SI" : "NO");
	free_tData(elemPertenece);
	
	// Inclusion
	int incl = inclusionSet(conjuntoB, conjuntoA);
	printf("B esta incluido en A ? %s\n", incl ? "SI" : "NO");
	
	// Union
	tData unionAB = unionSet(conjuntoA, conjuntoB);
	printf("Union A y B: ");
	printData(unionAB);
	
	// Interseccion
	tData interAB = intersectionSet(conjuntoA, conjuntoB);
	printf("Interseccion A y B: ");
	printData(interAB);
	
	// Diferencia
	tData diffAB = differenceSet(conjuntoA, conjuntoB);
	printf("Diferencia A - B: ");
	printData(diffAB);
	
	// Remover elemento
	tData elemRemover = newNodeStrHard(loadStr2("Azul"));
	printf("Removiendo 'Azul' de A...\n");
	removeFromSet(conjuntoA, elemRemover);
	printf("A despues de remover: ");
	printData(conjuntoA);
	free_tData(elemRemover);
	
	int iguales = equal_tData(conjuntoA,conjuntoC);
	printf("Ahora A y C con iguales? %s", iguales ? "SI" : "NO");
	
	// Liberar
	free_tData(conjuntoA);
	free_tData(conjuntoB);
	free_tData(conjuntoC);
	free_tData(unionAB);
	free_tData(interAB);
	free_tData(diffAB);
	
	// ==================== PARTE 4: PRODUCTO CARTESIANO ====================
	printf("\n\n4. PRODUCTO CARTESIANO\n");
	printf("-----------------------------------------\n");
	
	// Caso 1: LISTA X CONJUNTO
	printf("\nCaso 1: LISTA X CONJUNTO\n");
	tData listaParaProd = newEmptyNodeList();
	append(&(listaParaProd->data), newNodeStrHard(loadStr2("X")));
	append(&(listaParaProd->data), newNodeStrHard(loadStr2("Y")));
	printf("Lista: "); printData(listaParaProd);
	
	tData conjuntoParaProd = newEmptyNodeSet();
	insert_set(&(conjuntoParaProd->data), newNodeStrHard(loadStr2("1")));
	insert_set(&(conjuntoParaProd->data), newNodeStrHard(loadStr2("2")));
	printf("Conjunto: "); printData(conjuntoParaProd);
	
	tData prodCart1 = cartesianProduct(listaParaProd, conjuntoParaProd);
	printf("Producto Cartesiano (Lista X Conjunto): ");
	printData(prodCart1);
	printf("Cardinalidad del resultado: %d\n", cardinalitySet(prodCart1));
	
	free_tData(listaParaProd);
	free_tData(conjuntoParaProd);
	free_tData(prodCart1);
	
	// Caso 2: CONJUNTO X LISTA
	printf("\nCaso 2: CONJUNTO X LISTA\n");
	tData conjuntoProd2 = newEmptyNodeSet();
	insert_set(&(conjuntoProd2->data), newNodeStrHard(loadStr2("A")));
	insert_set(&(conjuntoProd2->data), newNodeStrHard(loadStr2("B")));
	printf("Conjunto: "); printData(conjuntoProd2);
	
	tData listaProd2 = newEmptyNodeList();
	append(&(listaProd2->data), newNodeStrHard(loadStr2("1")));
	append(&(listaProd2->data), newNodeStrHard(loadStr2("2")));
	append(&(listaProd2->data), newNodeStrHard(loadStr2("3")));
	printf("Lista: "); printData(listaProd2);
	
	tData prodCart2 = cartesianProduct(conjuntoProd2, listaProd2);
	printf("Producto Cartesiano (Conjunto X Lista): ");
	printData(prodCart2);
	printf("Cardinalidad del resultado: %d\n", cardinalitySet(prodCart2));
	
	free_tData(conjuntoProd2);
	free_tData(listaProd2);
	free_tData(prodCart2);
	
	// Caso 3: CONJUNTO X CONJUNTO
	printf("\nCaso 3: CONJUNTO X CONJUNTO\n");
	tData setA = newEmptyNodeSet();
	insert_set(&(setA->data), newNodeStrHard(loadStr2("Rojo")));
	insert_set(&(setA->data), newNodeStrHard(loadStr2("Verde")));
	printf("Conjunto A: "); printData(setA);
	
	tData setB = newEmptyNodeSet();
	insert_set(&(setB->data), newNodeStrHard(loadStr2("Grande")));
	insert_set(&(setB->data), newNodeStrHard(loadStr2("Pequenio")));
	printf("Conjunto B: "); printData(setB);
	
	tData prodCart3 = cartesianProduct(setA, setB);
	printf("Producto Cartesiano (A X B): ");
	printData(prodCart3);
	printf("Cardinalidad: %d\n", cardinalitySet(prodCart3));
	
	free_tData(setA);
	free_tData(setB);
	free_tData(prodCart3);
	
	// Caso 4: LISTA X LISTA (con duplicados)
	printf("\nCaso 4: LISTA X LISTA (con elementos duplicados)\n");
	tData listaDup1 = newEmptyNodeList();
	append(&(listaDup1->data), newNodeStrHard(loadStr2("a")));
	append(&(listaDup1->data), newNodeStrHard(loadStr2("b")));
	append(&(listaDup1->data), newNodeStrHard(loadStr2("a")));
	printf("Lista 1: "); printData(listaDup1);
	
	tData listaDup2 = newEmptyNodeList();
	append(&(listaDup2->data), newNodeStrHard(loadStr2("1")));
	append(&(listaDup2->data), newNodeStrHard(loadStr2("2")));
	printf("Lista 2: "); printData(listaDup2);
	
	tData prodCart4 = cartesianProduct(listaDup1, listaDup2);
	printf("Producto Cartesiano (Lista1 X Lista2): ");
	printData(prodCart4);
	printf("Nota: Los pares duplicados se evitan automaticamente por ser un SET.\n");
	
	free_tData(listaDup1);
	free_tData(listaDup2);
	free_tData(prodCart4);
	
	// ==================== PARTE 5: COPIA E IGUALDAD ====================
	printf("\n\n5. COPIA E IGUALDAD DE ESTRUCTURAS\n");
	printf("-----------------------------------------\n");
	
	tData original = newEmptyNodeList();
	append(&(original->data), newNodeStrHard(loadStr2("Hola")));
	append(&(original->data), newNodeStrHard(loadStr2("Mundo")));
	printf("Original: "); printData(original);
	
	tData copia = copy_tData(original);
	printf("Copia: "); printData(copia);
	
	int sonIguales = equal_tData(original, copia);
	printf("Original y copia son iguales? %s\n", sonIguales ? "SI" : "NO");
	
	// Modificar la copia para demostrar que son independientes
	tData nuevoElem = newNodeStrHard(loadStr2("Nuevo"));
	append(&(copia->data), nuevoElem);
	printf("Copia modificada: "); printData(copia);
	printf("Original sin cambios: "); printData(original);
	
	free_tData(original);
	free_tData(copia);
	
	// ==================== PARTE 6: CONVERSIONES STR -> LIST ====================
	printf("\n\n6. CONVERSIONES STR <-> LIST\n");
	printf("-----------------------------------------\n");
	
	// Ejemplo 1: STR -> LIST
	printf("\nEjemplo 1: STR a LIST\n");
	tData strEjemplo = newNodeStrHard(loadStr2("HolaMundo"));
	printf("String original: "); printData(strEjemplo);
	
	tData listaConvertida = strToList(strEjemplo);
	printf("Convertido a lista (cada caracter como elemento): ");
	printData(listaConvertida);
	
	// Ejemplo 2: LIST -> STR
	printf("\nEjemplo 2: LIST a STR\n");
	tData listaParaConvertir = newEmptyNodeList();
	append(&(listaParaConvertir->data), newNodeStrHard(loadStr2("Pro")));
	append(&(listaParaConvertir->data), newNodeStrHard(loadStr2("gra")));
	append(&(listaParaConvertir->data), newNodeStrHard(loadStr2("macion")));
	printf("Lista original: "); printData(listaParaConvertir);
	
	tData strConvertido = listToStr(listaParaConvertir);
	printf("Convertido a string: ");
	printData(strConvertido);
	
	// Ejemplo 3: LIST mixta (con elementos no STR)
	printf("\nEjemplo 3: LIST mixta (ignora no-strings)\n");
	tData listaMixta = newEmptyNodeList();
	append(&(listaMixta->data), newNodeStrHard(loadStr2("Hola")));
	append(&(listaMixta->data), newEmptyNodeList());   // LIST vacía
	append(&(listaMixta->data), newNodeStrHard(loadStr2(" ")));
	append(&(listaMixta->data), newEmptyNodeSet());    // SET vacío
	append(&(listaMixta->data), newNodeStrHard(loadStr2("Mundo")));
	printf("Lista mixta: "); printData(listaMixta);
	
	tData strMixto = listToStr(listaMixta);
	printf("Convertido a string (solo STR): ");
	printData(strMixto);
	
	// Ejemplo 4: Ciclo completo STR -> LIST -> STR
	printf("\nEjemplo 4: Ciclo completo (verificar integridad)\n");
	tData strOriginalCiclo = newNodeStrHard(loadStr2("ABCD1234"));
	printf("String original: "); printData(strOriginalCiclo);
	
	tData intermedia = strToList(strOriginalCiclo);
	printf("LIST: "); printData(intermedia);
	
	tData strFinal = listToStr(intermedia);
	printf("STR final: "); printData(strFinal);
	
	if (equal_tData(strOriginalCiclo, strFinal)) {
		printf("EXITO: El string se conserva despues de la conversion\n");
	} 
	else {
		printf("ERROR: El string cambio despues de la conversion\n");
	}
	
	// Liberar memoria de esta sección
	free_tData(strEjemplo);
	free_tData(listaConvertida);
	free_tData(listaParaConvertir);
	free_tData(strConvertido);
	free_tData(listaMixta);
	free_tData(strMixto);
	free_tData(strOriginalCiclo);
	free_tData(intermedia);
	free_tData(strFinal);
	
	// ==================== PARTE 7: CONVERSIONES STR <-> SET ====================
	printf("\n\n7. CONVERSIONES STR <-> SET\n");
	printf("-----------------------------------------\n");
	
	// Ejemplo 1: STR -> SET (elimina duplicados)
	printf("\nEjemplo 1: STR a SET\n");
	tData strConDuplicados = newNodeStrHard(loadStr2("HOLA"));
	printf("String original (con caracteres repetidos): ");
	printData(strConDuplicados);
	
	tData setConvertido = strToSet(strConDuplicados);
	printf("Convertido a conjunto (sin duplicados): ");
	printData(setConvertido);
	printf("Cardinalidad del conjunto: %d\n", cardinalitySet(setConvertido));
	
	// Ejemplo 2: SET -> STR (convierte todos los strings internos a un solo string)
	printf("\nEjemplo 2: SET a STR\n");
	tData setParaConvertir = newEmptyNodeSet();
	insert_set(&(setParaConvertir->data), newNodeStrHard(loadStr2("H")));
	insert_set(&(setParaConvertir->data), newNodeStrHard(loadStr2("O")));
	insert_set(&(setParaConvertir->data), newNodeStrHard(loadStr2("L")));
	insert_set(&(setParaConvertir->data), newNodeStrHard(loadStr2("A")));
	printf("Conjunto original: ");
	printData(setParaConvertir);
	
	tData strDesdeSet = setToStr(setParaConvertir);
	printf("Convertido a string: ");
	printData(strDesdeSet);
	
	// Ejemplo 3: Ciclo completo STR -> SET -> STR (se pierde orden y duplicados)
	printf("\nEjemplo 3: Ciclo completo STR -> SET -> STR\n");
	tData strOriginalCicloSet = newNodeStrHard(loadStr2("PROGRAMACION"));
	printf("String original: "); printData(strOriginalCicloSet);
	
	tData setIntermedio = strToSet(strOriginalCicloSet);
	printf("\nSET intermedio (caracteres unicos): ");
	printData(setIntermedio);
	
	tData strRecuperado = setToStr(setIntermedio);
	printf("STR recuperado (orden arbitrario): ");
	printData(strRecuperado);
	
	printf("Nota: En un conjunto se pierde el orden y los caracteres repetidos.\n");
	
	// Liberar memoria de esta sección
	free_tData(strConDuplicados);
	free_tData(setConvertido);
	free_tData(setParaConvertir);
	free_tData(strDesdeSet);
	free_tData(strOriginalCicloSet);
	free_tData(setIntermedio);
	free_tData(strRecuperado);
	
	printf("\n======================================================\n");
	printf("                FIN DE LA DEMOSTRACION\n");
	printf("======================================================\n");
}

int main() {
	demostracionCompleta();
	
	printf("\nPresione Enter para salir...");
	getchar();
	
	return 0;
}
