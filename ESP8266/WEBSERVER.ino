#include <WiFi.h> // Incluye la biblioteca WiFi para manejar la conexión a la red.
#include <WebServer.h> // Incluye la biblioteca WebServer para crear un servidor web.
#include "PAGINA1CODE.h" // Incluye el archivo que contiene el código HTML de la página principal.

#include <DHT.h>		// importa la Librerias DHT
#include <DHT_U.h>

const char* ssid = "LOL-2.4"; // Define el nombre de la red WiFi a la que se conectará.
const char* password = "00425174082"; // Define la contraseña de la red WiFi.
WebServer server(80); // Crea un objeto para el servidor web en el puerto 80.

const int CDA = 32; //calidad del aire
const int VEN = 4; //ventilador
const int BUZ = 26;

const int verde = 21;
const int amarillo = 19;
const int rojo = 18;

#define DHTPIN 25
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

int calidad_estado;
float temperatura_estado;
int ventilador_estado;
float humedad_estado;


int CDA_GRA[6] = {0,0,0,0,0,0}; 
int VEN_GRA[6] = {0,0,0,0,0,0}; 
int TEM_GRA[6] = {0,0,0,0,0,0}; 
int HUM_GRA[6] = {0,0,0,0,0,0}; 

unsigned long previousMillis = 0; 
const long interval = 5000;

unsigned long previousMillis2 = 0; 
const long interval2 = 1000;

// 0 -> automatico
// 1 -> manual
int modo = 0;

unsigned long ventiladorTimerStart = 0;  // Renombrar la variable a un nombre único
unsigned long timerDuration = 0;
bool ventiladorTemporizado = false;

void startTimer() {
  modo = 1;
  String tiempoStr = server.arg("tiempo");  // Obtiene el tiempo en segundos
  timerDuration = tiempoStr.toInt() * 1000;  // Convierte a milisegundos
  ventiladorTemporizado = true;  // Activa el temporizador
  digitalWrite(VEN, HIGH);  // Enciende el ventilador
  ventiladorTimerStart = millis();  // Guarda el tiempo actual

  String jsonResponse = "{\"status\": \"Temporizador activado\", \"tiempo\": \"" + String(tiempoStr) + "\"}";
  server.send(200, "application/json", jsonResponse);  // Responde al cliente
}


void setup() {
  Serial.begin(115200); // Inicia la comunicación serial a 115200 baudios.

  dht.begin();

  pinMode(CDA, INPUT); 
  pinMode(VEN, OUTPUT); 
  pinMode(BUZ, OUTPUT); 

  pinMode(rojo, OUTPUT); 
  pinMode(verde, OUTPUT); 
  pinMode(amarillo, OUTPUT); 

  digitalWrite(VEN, LOW);
  
  // Conexión a la red WiFi
  WiFi.begin(ssid, password); // Inicia la conexión a la red WiFi con el SSID y la contraseña especificados.
  while (WiFi.status() != WL_CONNECTED) { // Espera a que se establezca la conexión WiFi.
    delay(500); // Espera 500 milisegundos antes de verificar nuevamente.
    Serial.print("."); // Imprime un punto en el monitor serial para indicar que está intentando conectar.
  }
  Serial.println("\nConectado a la red WiFi."); // Imprime un mensaje cuando se ha conectado a la red.
  Serial.print("IP: "); // Imprime el IP local del dispositivo.
  Serial.println(WiFi.localIP()); // Muestra la IP local en el monitor serial.

  // Configuración de las rutas del servidor web
  server.on("/", HTTP_GET, handleRoot); // Asigna la función handleRoot a la ruta raíz.
  server.on("/toggleVentilador", HTTP_GET, toggleVentilador);
  server.on("/startTimer", HTTP_GET, startTimer);


  // Inicia el servidor web en el puerto 80
  server.begin(); // Comienza a escuchar en el puerto 80 para conexiones entrantes.
  
}

void loop() {
  server.handleClient(); // Maneja las solicitudes de los clientes que se conectan al servidor.

  int calidad_estado1 = analogRead(CDA);
  calidad_estado = map(calidad_estado1, 3000, 4096, 0, 100);
  temperatura_estado = dht.readTemperature();
  ventilador_estado = digitalRead(VEN);
  humedad_estado = dht.readHumidity();

  // Verifica si el temporizador está activo y si ha pasado el tiempo
  if (ventiladorTemporizado && (millis() - ventiladorTimerStart >= timerDuration)) {
    modo = 0;
    digitalWrite(VEN, LOW);  // Apaga el ventilador
    ventiladorTemporizado = false;  // Desactiva el temporizador
    Serial.println("Ventilador apagado automáticamente tras el temporizador.");
  }

  unsigned long currentMillis2 = millis();
  if (currentMillis2 - previousMillis2 >= interval2) {
    previousMillis2 = currentMillis2;
    if(calidad_estado > 80){
      digitalWrite(BUZ, HIGH);
      if(modo == 0){
        digitalWrite(VEN, HIGH);
      }
      
      digitalWrite(rojo, HIGH);
      digitalWrite(amarillo, LOW);
      digitalWrite(verde, LOW);
    } else if (calidad_estado > 30 && calidad_estado < 80) {
      digitalWrite(BUZ, LOW);
      if(modo == 0){
        digitalWrite(VEN, LOW);
      }
      digitalWrite(rojo, LOW);
      digitalWrite(amarillo, HIGH);
      digitalWrite(verde, LOW);
    } else if (calidad_estado >= 0 && calidad_estado < 50){
      digitalWrite(BUZ, LOW);
      if(modo == 0){
        digitalWrite(VEN, LOW);
      }
      digitalWrite(rojo, LOW);
      digitalWrite(amarillo, LOW);
      digitalWrite(verde, HIGH);
    }
  }
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    for (int i = 0; i < 5; i++) {
      CDA_GRA[i] = CDA_GRA[i + 1];
      VEN_GRA[i] = VEN_GRA[i + 1];
      TEM_GRA[i] = TEM_GRA[i + 1];
      HUM_GRA[i] = HUM_GRA[i + 1];
    }

    if(calidad_estado < 0){CDA_GRA[5] = 1;}else{CDA_GRA[5] = calidad_estado;}

    if(ventilador_estado != 0){VEN_GRA[5] = 100;}else{VEN_GRA[5] = 1;}
    TEM_GRA[5] = temperatura_estado;
    HUM_GRA[5]=  humedad_estado;

    Serial.println("---------------------------");
    Serial.print("GAS PPM: ");
    Serial.println(calidad_estado);
    Serial.println(calidad_estado1);
    Serial.print("Temperatura: ");
    Serial.println(temperatura_estado);
    Serial.print("Ventilador estado: ");
    Serial.println(ventilador_estado);
    Serial.print("Humedad: ");
    Serial.print(humedad_estado);
    Serial.println("%");
    Serial.println("---------------------------");

  }

}

void handleRoot() {
  
  String Pagina = PAGINA_PRINCIPAL;
  Pagina.replace("TEMP01",String(temperatura_estado));
  if(calidad_estado < 0){Pagina.replace("CAL01",String(1));}else{Pagina.replace("CAL01",String(calidad_estado));}
  Pagina.replace("VEN01",String(ventilador_estado));
  Pagina.replace("HUM01",String(humedad_estado));

  Pagina.replace("CAL006",String(CDA_GRA[5]));
  Pagina.replace("CAL005",String(CDA_GRA[4]));
  Pagina.replace("CAL004",String(CDA_GRA[3]));
  Pagina.replace("CAL003",String(CDA_GRA[2]));
  Pagina.replace("CAL002",String(CDA_GRA[1]));
  Pagina.replace("CAL001",String(CDA_GRA[0]));

  Pagina.replace("TEM006",String(TEM_GRA[5]));
  Pagina.replace("TEM005",String(TEM_GRA[4]));
  Pagina.replace("TEM004",String(TEM_GRA[3]));
  Pagina.replace("TEM003",String(TEM_GRA[2]));
  Pagina.replace("TEM002",String(TEM_GRA[1]));
  Pagina.replace("TEM001",String(TEM_GRA[0]));

  Pagina.replace("VEN006",String(VEN_GRA[5]));
  Pagina.replace("VEN005",String(VEN_GRA[4]));
  Pagina.replace("VEN004",String(VEN_GRA[3]));
  Pagina.replace("VEN003",String(VEN_GRA[2]));
  Pagina.replace("VEN002",String(VEN_GRA[1]));
  Pagina.replace("VEN001",String(VEN_GRA[0]));

  Pagina.replace("HUM006",String(HUM_GRA[5]));
  Pagina.replace("HUM005",String(HUM_GRA[4]));
  Pagina.replace("HUM004",String(HUM_GRA[3]));
  Pagina.replace("HUM003",String(HUM_GRA[2]));
  Pagina.replace("HUM002",String(HUM_GRA[1]));
  Pagina.replace("HUM001",String(HUM_GRA[0]));

  server.send(200, "text/html", Pagina); // Envía la página principal al cliente que hace la solicitud.
}

void toggleVentilador() {
  static bool ventiladorEncendido = false; // Estado del ventilador

  // Cambiar el estado del ventilador
  ventiladorEncendido = !ventiladorEncendido;
  
  // Encender o apagar el ventilador
  if (ventiladorEncendido) {
    digitalWrite(VEN, HIGH);  // Encender el ventilador
    modo = 1;
  } else {
    digitalWrite(VEN, LOW);   // Apagar el ventilador
    modo = 0;
  }

  // Enviar una respuesta en formato JSON
  String jsonResponse = "{\"ventilador\": " + String(ventiladorEncendido ? 1 : 0) + "}";
  server.send(200, "application/json", jsonResponse);  // Responder con el estado del ventilador
}

