#include <Arduino.h>
#include <Ultrasonic.h>
#include <ESP32Servo.h>
#include <SPI.h>
#include <MFRC522.h>
#include <WebServer.h>
#include <vector>


//vectores para gestion de targetas i la web
std::vector<String> tAceptadas;
std::vector<String> tPasadas;


//WIFI
const char* ssid = "Ibarra family";
const char* password = "m2o3qC7PkkFT";


WebServer server(80);


//PINES
//led
#define LED_ROJO 16
#define LED_VERDE 4
//sevo
#define SERVO_PIN 17
//sensor ultrasonidos
#define TRIGGER_PIN 33
#define ECHO_PIN 32
//RFID
#define RST_PIN 22
#define SS_PIN 21


//ESPECIFICACIONES
//distancia sensor ultrasonidos
#define DISTANCIA_DESEADA 10 // Distancia deseada en centímetros
//Objeto sensor ultrasonidos
Ultrasonic ultrasonic(TRIGGER_PIN, ECHO_PIN);
//Objeto RFID
MFRC522 rfid(SS_PIN, RST_PIN);
//esp rfid
String content;
//Objeto del servo
Servo myServo;
//esp servo
int pos = 0;
int minUs = 500;
int maxUs = 2700;
ESP32PWM pwm;




//FUNCIONES
bool aceptaTargeta(String id);
void web();
void getTargetas();
void addTargetas();
void deleteTargetas();




void setup() {
 //Puerto serie
 Serial.begin(9600);


 //Leds
 pinMode(LED_ROJO, OUTPUT);
 pinMode(LED_VERDE, OUTPUT);
 
 //Servo
 ESP32PWM::allocateTimer(0);
 ESP32PWM::allocateTimer(1);
 ESP32PWM::allocateTimer(2);
 ESP32PWM::allocateTimer(3);


 myServo.setPeriodHertz(50);
 myServo.write(pos);


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
}


void loop() {


  server.handleClient();


  content.clear(); //borramos el valor de la variable que guarda los uid's


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


  unsigned int distancia = ultrasonic.read(CM); //leemos distancia


  Serial.print("ID: ");
  Serial.println(content);
  Serial.print("Distancia: ");
  Serial.println(distancia);


  tPasadas.push_back(content);


  if ( (aceptaTargeta(content))) //distancia true AND uid true
  {
    digitalWrite(LED_VERDE,HIGH);
   
    while (pos != 90) //abre la puerta
    {
      myServo.write(pos);
      delay(10);
      pos += 1;
    }
    delay(3000);
    distancia = ultrasonic.read(CM); //volvemos a leer distancia
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
  else if( !aceptaTargeta(content))
  {
    digitalWrite(LED_ROJO, HIGH);
    delay(2000);
    digitalWrite(LED_ROJO, LOW);
  }
  delay(2000);
}


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


void web() {
  String html = "<html><head><style>";
  html += "body { font-family: Arial, sans-serif; text-align: center; padding: 50px; }";
  html += "h1 { color: #333; }";
  html += "button, input[type='submit'] { background-color: #4CAF50; color: white; border: none; padding: 10px 20px; text-align: center; text-decoration: none; display: inline-block; font-size: 16px; margin: 10px 2px; cursor: pointer; border-radius: 4px; }";
  html += "button:hover, input[type='submit']:hover { background-color: #45a049; }";
  html += "form { margin: 20px 0; }";
  html += ".uid-list { list-style-type: none; padding: 0; }";
  html += ".uid-list li { background: #f9f9f9; margin: 5px; padding: 10px; border: 1px solid #ddd; border-radius: 4px; }";
  html += ".uid-list li form { display: inline; }";
  html += ".uid-list li button { background-color: #e74c3c; color: white; border: none; padding: 5px 10px; margin-left: 10px; cursor: pointer; border-radius: 4px; }";
  html += ".uid-list li button:hover { background-color: #c0392b; }";
  html += "</style></head><body>";
  html += "<h1>Gestor de tarjetas: PUERTA PARKING</h1>";
  html += "<button onclick=\"location.href='/get_uids'\">Tarjetas pasadas por el lector</button>";
  html += "<form action='/add_uid' method='POST'><input type='text' name='uid' placeholder='UID'><input type='submit' value='Agregar UID de tu tarjeta'></form>";
  html += "<h2>Tarjetas aceptadas</h2><ul class='uid-list'>";
  for (String uid : tAceptadas) {
    html += "<li>" + uid + "<form action='/delete_uid' method='POST' style='display:inline;'><input type='hidden' name='uid' value='" + uid + "'><input type='submit' value='Eliminar' style='background-color: #e74c3c; color: white; border: none; padding: 5px 10px; cursor: pointer; border-radius: 4px;'></form></li>";
  }
  html += "</ul>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}


void getTargetas() {
  String html = "<html><head><style>";
  html += "body { font-family: Arial, sans-serif; text-align: center; padding: 50px; }";
  html += "h1 { color: #333; }";
  html += "button { background-color: #4CAF50; color: white; border: none; padding: 10px 20px; text-align: center; text-decoration: none; display: inline-block; font-size: 16px; margin: 10px 2px; cursor: pointer; border-radius: 4px; }";
  html += "button:hover { background-color: #45a049; }";
  html += ".uid-list { list-style-type: none; padding: 0; }";
  html += ".uid-list li { background: #f9f9f9; margin: 5px; padding: 10px; border: 1px solid #ddd; border-radius: 4px; }";
  html += "</style></head><body>";
  html += "<h1>UID's pasadas por el lector</h1><ul class='uid-list'>";
  for (String uid : tPasadas) {
    html += "<li>" + uid + "</li>";
  }
  html += "</ul>";
  html += "<button onclick=\"location.href='/'\">Volver</button>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}


void addTargetas() {
  if (server.hasArg("uid")) {
    String newUid = server.arg("uid");
    tAceptadas.push_back(newUid);
    server.send(200, "text/html", "<html><head><style>body { font-family: Arial, sans-serif; text-align: center; padding: 50px; } h1 { color: #333; } button { background-color: #4CAF50; color: white; border: none; padding: 10px 20px; text-align: center; text-decoration: none; display: inline-block; font-size: 16px; margin: 10px 2px; cursor: pointer; border-radius: 4px; } button:hover { background-color: #45a049; }</style></head><body><h1>UID Tarjeta agregada correctamente</h1><button onclick=\"location.href='/'\">Volver</button></body></html>");
  } else {
    server.send(400, "text/html", "<html><head><style>body { font-family: Arial, sans-serif; text-align: center; padding: 50px; } h1 { color: #333; } button { background-color: #4CAF50; color: white; border: none; padding: 10px 20px; text-align: center; text-decoration: none; display: inline-block; font-size: 16px; margin: 10px 2px; cursor: pointer; border-radius: 4px; } button:hover { background-color: #45a049; }</style></head><body><h1>Petición inválida</h1><button onclick=\"location.href='/'\">Volver</button></body></html>");
  }
}


void deleteTargetas() {
  if (server.hasArg("uid")) {
    String uidToDelete = server.arg("uid");
    auto it = std::find(tAceptadas.begin(), tAceptadas.end(), uidToDelete);
    if (it != tAceptadas.end()) {
      tAceptadas.erase(it);
      server.send(200, "text/html", "<html><head><style>body { font-family: Arial, sans-serif; text-align: center; padding: 50px; } h1 { color: #333; } button { background-color: #4CAF50; color: white; border: none; padding: 10px 20px; text-align: center; text-decoration: none; display: inline-block; font-size: 16px; margin: 10px 2px; cursor: pointer; border-radius: 4px; } button:hover { background-color: #45a049; }</style></head><body><h1>UID Tarjeta eliminada correctamente</h1><button onclick=\"location.href='/'\">Volver</button></body></html>");
    } else {
      server.send(404, "text/html", "<html><head><style>body { font-family: Arial, sans-serif; text-align: center; padding: 50px; } h1 { color: #333; } button { background-color: #4CAF50; color: white; border: none; padding: 10px 20px; text-align: center; text-decoration: none; display: inline-block; font-size: 16px; margin: 10px 2px; cursor: pointer; border-radius: 4px; } button:hover { background-color: #45a049; }</style></head><body><h1>UID no encontrado</h1><button onclick=\"location.href='/'\">Volver</button></body></html>");
    }
  } else {
    server.send(400, "text/html", "<html><head><style>body { font-family: Arial, sans-serif; text-align: center; padding: 50px; } h1 { color: #333; } button { background-color: #4CAF50; color: white; border: none; padding: 10px 20px; text-align: center; text-decoration: none; display: inline-block; font-size: 16px; margin: 10px 2px; cursor: pointer; border-radius: 4px; } button:hover { background-color: #45a049; }</style></head><body><h1>Petición inválida</h1><button onclick=\"location.href='/'\">Volver</button></body></html>");
  }
}