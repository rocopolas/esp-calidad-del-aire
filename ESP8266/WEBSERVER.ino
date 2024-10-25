#include <WiFi.h> // Incluye la biblioteca WiFi para manejar la conexión a la red.
#include <WebServer.h> // Incluye la biblioteca WebServer para crear un servidor web.
#include "PAGINA1CODE.h" // Incluye el archivo que contiene el código HTML de la página principal.

#include <DHT.h>		// importa la Librerias DHT
#include <DHT_U.h>

const char* ssid = "LABO"; // Define el nombre de la red WiFi a la que se conectará.
const char* password = ""; // Define la contraseña de la red WiFi.
WebServer server(80); // Crea un objeto para el servidor web en el puerto 80.

const int CDA = 27; //calidad del aire
const int VEN = 4; //ventilador
const int BUZ = 26;

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

void setup() {
  Serial.begin(115200); // Inicia la comunicación serial a 115200 baudios.

  dht.begin();

  pinMode(CDA, INPUT); 
  pinMode(VEN, OUTPUT); 
  pinMode(BUZ, OUTPUT); 

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
  // Inicia el servidor web en el puerto 80
  server.begin(); // Comienza a escuchar en el puerto 80 para conexiones entrantes.
}

void loop() {
  server.handleClient(); // Maneja las solicitudes de los clientes que se conectan al servidor.

  calidad_estado = digitalRead(CDA);
  temperatura_estado = dht.readTemperature();
  ventilador_estado = digitalRead(VEN);
  humedad_estado = dht.readHumidity();

  if(calidad_estado > 900 || temperatura_estado > 45){
    digitalWrite(BUZ, HIGH);
    // digitalWrite(VEN, HIGH);
  } else {
    digitalWrite(BUZ, LOW);
    // digitalWrite(VEN, LOW);
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
    CDA_GRA[5] = calidad_estado;
    VEN_GRA[5] = ventilador_estado;
    TEM_GRA[5] = temperatura_estado;
    HUM_GRA[5]=  humedad_estado;

  }

}

void handleRoot() {
  
  String Pagina = PAGINA_PRINCIPAL;
  Pagina.replace("TEMP01",String(temperatura_estado));
  Pagina.replace("CAL01",String(calidad_estado));
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
