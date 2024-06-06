# PROYECTO_FINAL_PD_XavierHidalgo

## SISTEMA DE **CONTROL DE ACCESO** PARA UNA **PUERTA DE PARKING**

Alumnos: Juan Esteban Palacios, Xavier Hidalgo 
Profesor de prácticas: Manuel Lopez Palma



### **INTRODUCCIÓN:**

Nuestro proyeco se ha enfocado en el desarrollo de un sistema que permite el acceso a un parking mediante la lectura de tarjetas RFID autorizadas.

Además, generamos un servidor web en el que se encuantra una página web que permite la gestión de las tarjetas RFID. En resumen, la web permite la visualización de UID's de tarjetas que han intentado acceder al parking así como la autorización y desautorización de tarjetas para el acceso al parking.

Hemos desarrollado el sistema con la ayuda del microprocesador ESP-WROOM-32. Lo hemos programado con el IDE platformio desde Visual Studio Code (Microsoft) utilizando la librería y el framework de Arduino.

·Imagen: MAQUETA DEL SISTEMA:

![Im1](https://github.com/XaviHidalgo/PROYECTO_FINAL_PD_XavierHidalgo/blob/main/imagenes/maqueta.jpg)

·Imagen: INTERFACE DE LA PÁGINA WEB:

![Im1](https://github.com/XaviHidalgo/PROYECTO_FINAL_PD_XavierHidalgo/blob/main/imagenes/web1.jpg)


### **OBJETIVOS:**

1- Desarrollar un sistema que funcione y sea lo más realista posible.

2- Desarrollar una página web que actualice la accesibilidad de usuarios al parking rápidamente y permita visualizar todos los UID’s de las tarjetas que han intentado acceder.

3- Desarrollar un programa optimizado que haga funcionar correctamente el sistema.

4- Entender las librerías que utilizamos en el programa.

5- Entender cómo trabajan los componentes electrónicos utilizados.


### **COMPONENTES ELECTRÓNICOS: su función y funcionamiento**

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

Función: Abrir la puerta del parking.

Funcionamiento:
  El servomotor es un motor DC con engranajes que funciona a partir de pulsos eléctricos.
  Los engranajes sirven para limitar la rotación del motor (normalmente hasta 180º) y reducir la velocidad de giro.

  Como hemos escrito, el motor funciona a partir de pulsos, estos pulsos reciben un nombre, PWM (Pulse Width Modulation) o modulación por ancho de pulso. La       
  duración de los pulsos determina la rotación que debe hacer el servo, siendo esta normalmente:

  Pulso de 1 ms → 0 grados
  Pulso de 1.5 ms →  90 grados
  Pulso de 2 ms → 180 grados


· 2 LED's (rojo y verde):

![Im1](https://github.com/XaviHidalgo/PROYECTO_FINAL_PD_XavierHidalgo/blob/main/imagenes/led.jpg)

Función: Dar información visual al usuario sobre si tiene acceso o no al parking. Si pasa una tarjeta autorizada se encenderá el LED verde. En caso contrario, el rojo.


### **CONEXIONES: periféricos - microprocesador:**

Sensor de Ultrasonidos - ESP-WROOM-32

VCC --> 5 V
GND --> GND
Trig --> GPIO 33
Echo --> GPIO 32

Lector RFID - ESP-WROOM-32

VCC --> 3.3 V
GND --> GND
RST --> GPIO 22
SDA --> GPIO 21
MOSI --> GPIO 23
MISO --> GPIO 19
SCK --> GPIO 18

Servomotor - ESP-WROOM-32

VCC --> 5 V
GND --> GND
Signal --> GPIO 17

LED's - ESP-WROOM-32

Signal (ROJO) --> GPIO 16
Signal (VERDE) --> GPIO 04
GND --> GND


### **Partes principales de nuestro PROGRAMA:**

Como describe el título, en este apartado solo veremos las partes principales o más importantes del programa que hemos hecho. Esto lo hacemos para no llenar el informe con especificaciones que carezcan de explicación. Por ejemplo, partes como la assignación de pines no están incluidas a continuación. 

Explicaremos las partes en orden por el que se ejecutan:


· Librerías:

``` cpp
#include <Arduino.h>
#include <Ultrasonic.h>
#include <ESP32Servo.h>
#include <SPI.h>
#include <MFRC522.h>
#include <WebServer.h>
#include <vector>
```

Estas son las librerías que hemos utilizado en nuestro programa para en funcionamiento del sistema. 
Encontramos las necesarias para tener control sobre los periféricos:
  Ultrasonic.h : Sensor de ultrasonidos.
  MFRC522.h : Lector RFID.
  ESP32Servo.h : Servomotor.

SPI.h para establecer conexión con el lector RFID, ya que este usa este protocolo para comunicarse.
WebServer.h para crear un servidor web.
vector.h para crear vectores donde almacenaremos los UID's de tarjetas.


· Vectores (C++) para la gestión de tarjetas y web:

```cpp
std::vector<String> tAceptadas;
std::vector<String> tPasadas;
```

En ambos vectores se almacenan UID's de tarjetas en forma de String y, más concretamente, en hexadecimal.
En el vector tAceptadas guardamos los UID's de las tarjetas que pueden abrir la puerta, es decir, las autorizadas.
En el vector tPasadas guardamos los UID's de cualquier tarjeta que pase por el lector.
El contenido y actualización de ambos vectores se puede gestionar desde la página web, por ejemplo, para añadir un UID al vector tAceptadas para autorizar una tarjeta con el respectivo debemos hacerlo desde la página web, más adelante lo veremos.


· Funciones:

Inicialización:

``` cpp
bool aceptaTargeta(String id);
void web();
void getTargetas();
void addTargetas();
void deleteTargetas();
```

(sabemos que tarjetas se escribe con "j", en el pasado parece que no éramos conscientes).

A continucación mostramos el programa de la función aceptaTargeta() y su explicación, pero antes preferimos explicar las 4 funciones restantes que se encargan del diseño y funcionamiento de la web. No mostraremos el programa completo de estas 4 restantes ya que son códigos bastante largos de, al fin y al cabo, programas HTML (casi todo diseño) que creemos que enborronarían demasiado el informe, si los quieren ver, los pueden encontrar en el main.cpp que hay dentro del repositorio.

·Imagen: INTERFACE DE LA PÁGINA WEB:

![Im1](https://github.com/XaviHidalgo/PROYECTO_FINAL_PD_XavierHidalgo/blob/main/imagenes/web1.jpg)

- web(): 

Incluye el diseño de la página web (que pueden ver arriba). Eso supone el título con su tamaño de letra, la creación de los 3 botones con su color entre   otros, la creación de la entrada de texto (para los UID's) con su tamaño, etc. Supongo que ya entienden porqué no incluymos el código, es casi todo diseño.
  
Las tres siguientes funciones ya son más interesantes:

- getTargetas(), addTargetas(), delateTargetas():

Estas son las funciones que se encargan de utilizar los vectores anteriormente definidos.
La primera, getTargetas(), utiliza tPasadas y se ejecuta cuando se pulsa el primer botón de la web  "Targetas pasadas por el lector". Como podrán intuir, despliega todo el contenido de tPasadas, esto lo hace con el siguiente for:

```cpp
for (String uid : tPasadas) {
    html += "<li>" + uid + "</li>";
  }
  html += "</ul>";
  html += "<button onclick=\"location.href='/'\">Volver</button>";
  html += "</body></html>";
  server.send(200, "text/html", html);
```

Las dos últimas funciones utilizan el vector tAceptadas. addTargetas() se ejecuta cuando se pulsa el botón "Añadir UID de tu tarjeta" y, añade el contenido que haya en la entrada de texto de su izquierda al vector (con la función push_back() de .vector.h). delateTarjetas() se ejecuta cuando se pulsa el botón rojo que pone "Eliminar" y, elimina del vector el UID que haya a la izquierda del botón (con la función .erase() de vector.h). Este botón solo saldrá si hay algo dentro de tAceptadas.

A continuación, la última función:

aceptaTargeta(String id):

``` cpp
bool aceptaTargeta(String id)
{
  for (int i = 0; i < tAceptadas.size(); i++)
  {
      if(id == tAceptadas[i])
      {
          return true;
      }
  }
  return false;
}
```
La función devuelve un valor booleano y pide un String, más concretamente un UID.
Devolverá TRUE si el UID entrado coincide con alguno que haya dentro del vector tAceptadas.
Devolverá FALSE en caso contrario.
Básicamente es la función que permite o no la apertura de la puerta.


· VOID SETUP:

Aqui iniciamos el puerto série y todos los objetos-periféricos, conectamos el MP a un WiFi, agregamos todas las funciones que se encargan del funcionamiento de la web al servidor y lo iniciamos:

```cpp
//Puerto serie
 Serial.begin(9600);
 //Leds
 pinMode(LED_ROJO, OUTPUT);
 pinMode(LED_VERDE, OUTPUT);

 // Inicialización del servo
 myServo.attach(SERVO_PIN, minUs, maxUs);

// Inicialización bus SPI
 SPI.begin();

 //Inicializacion RFID
 rfid.PCD_Init();

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP());

  // Web server setup
  server.on("/", web);
  server.on("/get_uids", getTargetas);
  server.on("/add_uid", HTTP_POST, addTargetas);
  server.on("/delete_uid", HTTP_POST, deleteTargetas);
  server.begin();
  Serial.println("HTTP server started");
```


· VOID LOOP:

En este apartado, está la parte más importante del programa, el funcionamiento del sistema.

El sistema, en primer inicio, esta buscando constantemente una respuesta del lector RFID, es decir, se queda en un bucle preguntando si se ha pasado una tarjeta por el lector (no hará nada hasta que no detecte una respuesta pase una). En cuanto se pasa una tarjeta, el programa avanza y, guarda el UID en de dicha en hexadecimal (lo convertimos con un for) en una variable tipo String que llamamos "content" ("content" se invoca antes del void_setup()).

El programa descrito en el párrafo anterior es el siguiente (donde "rfid" es la variable de nuestro objeto que hace funcionar el lector RFID):

``` cpp
if (!rfid.PICC_IsNewCardPresent()) //busca targeta
  {
    return;
  }
  if (!rfid.PICC_ReadCardSerial()) { //lee uid de targeta
    return;
  }


  for (byte i = 0; i < rfid.uid.size; i++) { //guardamos uid de la targeta en la variable "content"
    content.concat(String(rfid.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(rfid.uid.uidByte[i], HEX));
  }
  content.toUpperCase();

  tPasadas.push_back(content);
```

(como podéis ver en el trozo de código anterior, se añade el UID en el vector tPasadas, para que se pueda ver este desde la página web)

Tras leer una tarjeta y en consecuencia su correspondiente UID, pueden pasar dos cosas, que este esté autorizado y entonces se abra la puerta o, por lo contrario, que no lo esté y no se abra.
En el primer caso:

``` cpp
if ( (aceptaTargeta(content))) // uid true
  {
    digitalWrite(LED_VERDE,HIGH);
   
    while (pos != 90) //abre la puerta
    {
      myServo.write(pos);
      delay(10);
      pos += 1;
    }
    delay(3000);
    distancia = ultrasonic.read(CM); //leemos distancia
    while(distancia <= DISTANCIA_DESEADA) //mientras no se vaya el coche, espera
    {
      distancia = ultrasonic.read(CM);
      delay(1000);
    }

    while (pos > 0) //se ha ido el coche, cierra la puerta
    {
      myServo.write(pos);
      delay(10);
      pos -= 1;
    }

    digitalWrite(LED_VERDE,LOW);
  }
```

Como hemos dicho anteriormente, si aceptaTargeta() es TRUE, la targeta está autorizada (incluyda dentro del vector tAceptadas). Entonces, el acceso está autorizado y ejecutamos de la siguiente manera: 
-Encendemos LED verde.
-Movemos servomotor (abrimos).
-Esperamos a que avance el coche (3 segundos).
-Leemos distancia y, hasta que esta no sea menor a 10cm, supondremos que el coche sigue cruzando.
-Cuando la distancia sea mayor a 10cm (el coche ha anvanzado), movemos servomotor (cerramos).
-Apagamos LED verde.

En segundo caso:

``` cpp
else if( !aceptaTargeta(content))
  {
    digitalWrite(LED_ROJO, HIGH);
    delay(2000);
    digitalWrite(LED_ROJO, LOW);
  }
```

Si aceptaTargeta() es FALSE, ejecutamos de la siguiente manera:
-Encendemos LED ROJO.
-Eperamos 2 segundos.
-Apagamos LED ROJO.

Finalmente, tras leer un UID y abrir o no la puerta en consecuencia, esperamos 2 segundos hasta hacer la siguiente lectura.
