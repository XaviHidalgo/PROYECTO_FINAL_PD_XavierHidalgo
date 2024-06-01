# PROYECTO_FINAL_PD_XavierHidalgo


##                       TÍTULO:
## SISTEMA DE **CONTROL DE ACCESO** PARA UNA **PUERTA DE PARKING**


### Alumnos: Juan Esteban Palacios, Xavier Hidalgo 
### Profesor de prácticas: Manuel Lopez Palma



**INTRODUCCIÓN:**

Nuestro proyeco se ha enfocado en el desarrollo de un sistema que permite el acceso a un parking mediante la lectura de tarjetas RFID autorizadas.

Además, generamos un servidor web en el que se encuantra una página web que permite la gestión de las tarjetas RFID. En resumen, la web permite la visualización de UID's de tarjetas que han intentado acceder al parking así como la autorización y desautorización de tarjetas para el acceso al parking.

Hemos desarrollado el sistema con la ayuda del microprocesador ESP-WROOM-32. Lo hemos programado con el IDE platformio desde Visual Studio Code (Microsoft) utilizando la librería y el framework de Arduino.

·Imagen: MAQUETA DEL SISTEMA:

![Im1](https://github.com/XaviHidalgo/PROYECTO_FINAL_PD_XavierHidalgo/blob/main/imagenes/maqueta.jpg)

·Imagen: INTERFACE DE LA PÁGINA WEB:

![Im1](https://github.com/XaviHidalgo/PROYECTO_FINAL_PD_XavierHidalgo/blob/main/imagenes/web1.jpg)


**OBJETIVOS:**

1- Desarrollar un sistema que funcione y sea lo más realista posible.

2- Desarrollar una página web que actualice la accesibilidad de usuarios al parking rápidamente y permita visualizar todos los UID’s de las tarjetas que han intentado acceder.

3- Desarrollar un programa optimizado que haga funcionar correctamente el sistema.

4- Entender las librerías que utilizamos en el programa.

5- Entender cómo trabajan los componentes electrónicos utilizados.


**COMPONENTES ELECTRÓNICOS: su función y funcionamiento**

· Microprocesador ESP-WROOM-32:

![Im1](https://github.com/XaviHidalgo/PROYECTO_FINAL_PD_XavierHidalgo/blob/main/imagenes/esp-wroom-32.jpg)


· Sensor de Ultrasonidos HC-SR40:

![Im1](https://github.com/XaviHidalgo/PROYECTO_FINAL_PD_XavierHidalgo/blob/main/imagenes/ultrasonido.jpg)

Función: Detectar la salida del vehículo para tras ello cerrar la puerta del parking. De esta forma nos aseguramos de no cerrar la puerta cuando el vehículo este cruzando y nos ahorramos accidentes.

Funcionamiento: 
  1- El microprocesador envía una señal de disparo por el PIN Trigger (de ahí su nombre) del componente.
  2- El sensor traduce la señal en un pulso de sonido que viaja por el aire a unos 40kHz.
  3- El pulso rebota en un obstáculo.
  4- El sensor recibe el pulso y lo traduce en una señal eléctrica que envía al microprocesador por el PIN Echo.
  5- El microprocesador calcula el tiempo trancurrido desde que ha enviado una señal por el PIN Trigger hasta que recibe una por el PIN Echo. Con el tiempo y la         velocidad del sonido a 40kHz, el microprocesador calcula la distancia a la que se encuantra el sensor del objeto en el que ha rebotado el pulso. 


· Lector RFID MFRC522:

![Im1](https://github.com/XaviHidalgo/PROYECTO_FINAL_PD_XavierHidalgo/blob/main/imagenes/rfid.jpg)

Función: Autorizar el acceso al parking leiendo el UID de la tarjeta que se pase. Es la llave que abre la puerta (hace girar el servomotor).

Funcionamiento:
  El lector esta connectado vía SPI al microprocesador.
  1- El lector RFID emite un campo EM a través de su antena (a una frecuencia de 13.56 MHz en el caso del MFRC522).
  2- Cuando una tarjeta entra en este campo, recibe energía del lector mediante inducción electromagnética por su antena.
  3- Con esa energía alimenta su microchip y este modula una señal EM de respuesta en la que viaja el UID (representado en 4 bytes) de la tarjeta.
  4- El lector detecta variaciones en el campo EM y lee la señal que ha enviado la targeta.
  5- Envía la señal al microprocesador vía SPI y este la digitaliza para extraer lo que son comunmente 4 bytes.


· Mini Servomotor SG90:

![Im1](https://github.com/XaviHidalgo/PROYECTO_FINAL_PD_XavierHidalgo/blob/main/imagenes/servo.jpg)

· 2 LED's (rojo y verde):

![Im1](https://github.com/XaviHidalgo/PROYECTO_FINAL_PD_XavierHidalgo/blob/main/imagenes/led.jpg)
