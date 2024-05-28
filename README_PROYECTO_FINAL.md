# PROYECTO_FINAL_PD_XavierHidalgo

## SISTEMA DE CONTROL DE ACCESO PARA UNA PUERTA DE PARKING

### Alumnos: Juan Esteban Palacios, Xavier Hidalgo 
### Profesor: Manuel Lopez Palma



**INTRODUCCIÓN:**

Nuestro proyecto se enfoca en el desarrollo de un sistema de control de acceso para una puerta de parking.

Para ello hemos utilizado casi todo lo que hemos hecho en las clases de prácticas. De la misma forma, hemos utilizado el mismo microprocesador (ESP-WROOM-32) e IDE (Platformio desde Visual Studio Code). Para escribir el programa, también, hemos utilizado el framework y las librerías de Arduino.

Nuestro microprocesador consta de tres periféricos principales:
-Lector RFID: Se encarga de autorizar la apertura de la puerta de parking.
-Sensor de ultrasonidos: Se encarga de la detección de vehículos.
-Sevo motor: Se encarga de abrir la puerta.

Además:
-2 LED's (rojo y verde): Se encargan de mostrar una respuesta visual al usuario y, quedan bonitos.
-Conexión WIFI (del mismo microprocesador): Se encarga de crear una interfaz WEB para la gestión remota de targetas.


¿Cuándo se abrirá la puerta?
Siempre y cuando se pase por el lector una targeta autorizada.

¿Como puedo autorizar o desautorizar mi targeta?
Desde la página WEB, en ella encontramos:
-Un registro de todos los UID's de las targetas que han pasado por el lector.
-Un apartado para autorizar una targeta a partir de su UID.
-Un apartado para desautorizar una targeta a partir de su UID.
