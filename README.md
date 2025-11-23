# Juego interactivo web de aprendizaje de programación mediante edición de vóxeles (manuel.santamarina, v.druiz)

Nuestra propuesta consiste en un juego de aprendizaje de programación que permite a través de un cliente web escribir código que luego será interpretado y procesado por un cursor que se moverá a través de él generando, pintando o eliminando bloques. El escenario consiste de un espacio geométrico de n³ bloques que podrán contener cualquier valor RGBA escrito en hexadecimal, siendo el 00 absoluto en el canal alfa equivalente a la no existencia del bloque en esas coordenadas. 

Los bloques estarán ordenados en base a un espacio de coordenadas de mano izquierda, siendo inicialmente la esquina presente a la izquierda en la figura 1 el origen de coordenadas del escenario, creciendo hacia la derecha la X y la Y, y la Z decreciendo hacia abajo.

![Fig. 1](image.png)

En cuanto al lenguaje, tendremos una serie de instrucciones básicas de movimiento del "cursor" (que estará representado por un modelo más amigable).

Nuestro compilador, que estará basado en WebAssembly y compilado a partir de la salida de BISON, generará una serie de pasos a seguir que serán visualizados y renderizados en la web.

El lenguaje estará compuesto por las siguientes instrucciones: 
- Forwards
- Backwards 
- Right
- Left
- Up
- Down
- setCube(RGBA)

Es cierto que las instrucciones de movimiento podrían tomar como argumento la repetición, pero si planteamos esta herramienta como una herramienta de aprendizaje debemos evitar las funciones inicialmente, pudiendo añadirlas a futuro.


Aparte de eso, contaremos con unas estructuras de control que nos permitirán hacer los bucles y condicionales.

if
while (?)
for (?)
loop/break(?)

Nuestro lenguaje no cuenta con "true" o "false" explícitamente, sino que los condicionales se evalúan y controlan la ejecución sin hacer una reducción a true o false.


Trataremos todos los errores sintácticos y semánticos que detectemos a través de feedback de cara al usuario a través de la UI, indicando la línea que produce el error. En vez de mandar el mensaje con la representación del estado al código JavaScript, tendremos mensajes especiales para señalar el error.