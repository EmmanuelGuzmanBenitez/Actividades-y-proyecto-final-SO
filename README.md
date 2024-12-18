# Administración de memoria.
## 3.1 ¿Cuál es la diferencia entre fragmentación interna y externa?
La fragmentación interna ocurre cuando un bloque de memoria asignado a un proceso es más grande de lo necesario, dejando partes no utilizadas dentro del bloque. Aunque el espacio está reservado, no se puede usar para otros procesos. 
Esto afecta desperdiciando memoria dentro de los bloques asignados además de que afecta el rendimiento porque se reduce la cantidad de memoria disponible para otros procesos.

La fragmentación externa sucede cuando hay pequeños fragmentos de memoria libres dispersos entre bloques asignados. Estos fragmentos no son lo suficientemente grandes como para satisfacer nuevas solicitudes de memoria, aunque la suma total podría ser suficiente.
Esto afecta a la memoria generando más tiempo de procesamiento para encontrar bloques adecuados.

### Investiga y explica las políticas de reemplazo de páginas en sistemas operativos. ¿Cuál consideras más eficiente y por qué?
En sistemas operativos que usan memoria virtual, las políticas de reemplazo de páginas determinan qué página se debe sacar de la memoria principal para dar espacio a una nueva cuando ocurre un fallo de página.
* FIFO (First In, First Out): La página que llegó primero a la memoria es la que se reemplaza.
Es simple de implementar y no requiere cálculos complejos.

* LRU (Least Recently Used): Se reemplaza la página que no se ha usado en el mayor tiempo. Es una aproximación práctica al uso futuro y tiene mejor rendimiento que FIFO en la mayoría de los casos.

* Optimal (OPT): Se reemplaza la página que no se necesitará por más tiempo en el futuro. Es teóricamente la política más eficiente y según minimiza los fallos de página.

* NRU (Not Recently Used): Clasifica las páginas según dos bits (referencia y modificación). Las páginas que no se han usado ni modificado recientemente son las candidatas a reemplazo. Es eficiente en tiempo y espacio.

* Clock (o Second Chance): Similar a FIFO, pero da una "segunda oportunidad" a las páginas si han sido usadas recientemente además de que usa un puntero en forma circular. Es una buena combinación entre eficiencia y simplicidad y se cree es más eficiente que FIFO y más fácil de implementar que LRU.

* Según mi opinión, ¿Cuál es más eficiente y por qué?
Según yo LRU suele ser más eficiente porque se basa en patrones de acceso recientes, lo cual es una buena estimación del comportamiento futuro.
Aunque Clock también se me hace buena opción, ya que tiene un balance entre rendimiento y complejidad, además de que mejora FIFO sin requerir la complejidad de LRU.
