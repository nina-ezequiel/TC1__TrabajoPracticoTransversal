Se eligió la Propuesta 4 para implementar el TAD de autómatas finitos. En esta propuesta, cada transición se representa con una estructura DeltaEntry que guarda un estado origen, un símbolo y un conjunto de estados destino (SET). 
Todas las transiciones se almacenan en un arreglo dinámico (DeltaEntry* delta) y la cantidad de transiciones se lleva en deltaCount.

Las razones de esta elección son:

  Unificación – La misma estructura funciona para AFD y AFND (el destino puede ser uno o varios).
  
  Corrección matemática – Modela exactamente δ: Q × Σ → P(Q).
  
  Reconocimiento unificado – El algoritmo de aceptación maneja por igual ambos tipos de autómata.
  
  Entrada sencilla – El parseo soporta destinos únicos o múltiples (con llaves) de forma natural.
  
  Reutilización – Aprovecha los TAD String y Tree ya implementados.
  
  Extensibilidad – Facilita futuras operaciones (determinización, minimización, etc.).


Además, el uso de delta y deltaCount permite:

  Agregar transiciones dinámicamente con realloc.
  Recorrer todas las transiciones fácilmente con un bucle for.
  Conocer en todo momento cuántas transiciones tiene el autómata.
