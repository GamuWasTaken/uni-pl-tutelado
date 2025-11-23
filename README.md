# Juego interactivo web de aprendizaje de programación mediante edición de vóxeles (manuel.santamarina, v.druiz)

Nuestra propuesta consiste en un juego de aprendizaje de programación que permite a través de un cliente web escribir código que luego será interpretado y procesado por un cursor que se moverá a través de él generando, pintando o eliminando bloques. El escenario consiste de un espacio geométrico de n³ bloques que podrán contener cualquier valor RGBA escrito en hexadecimal, siendo el 00 absoluto en el canal alfa equivalente a la no existencia del bloque en esas coordenadas. 

Los bloques estarán ordenados en base a un espacio de coordenadas de mano izquierda, siendo inicialmente la esquina presente a la izquierda en la figura 1 el origen de coordenadas del escenario, creciendo hacia la derecha la X y la Y, y la Z decreciendo hacia abajo.

![Fig. 1](image.png)

En cuanto al lenguaje, tendremos una serie de instrucciones básicas de movimiento del "cursor" (que estará representado por un modelo más amigable).

Nuestro compilador, que estará compilado a WebAssembly a partir de la salida de BISON, generará una lista de estados que serán visualizados y renderizados en la web.

Para interactuar con el estado, usarán built-ins:
- Mover
  - Arriba
  - Abajo
  - Izquierda
  - Derecha
  - Adelante
  - Atras
- Modificar cubo (RGBA)

Aparte de eso, mecanismos para hacer bucles y condicionales.
- Ejecución condicional
- Iteración

Nuestro lenguaje no cuenta con booleanos explícitamente, sino que los condicionales se evalúan y controlan la ejecución sin hacer una reducción a true o false.

Trataremos todos los errores sintácticos y semánticos que detectemos a través de feedback de cara al usuario a través de la UI, indicando la línea que produce el error. En vez de mandar el mensaje con la representación del estado al código JavaScript, tendremos mensajes especiales para señalar el error.
