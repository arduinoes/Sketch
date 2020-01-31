/*
    Autor: Carlos Miranda
    2020/01/31
*/

#include "ThingSpeak.h" // https://github.com/mathworks/thingspeak-arduino
#include <ESP8266WiFi.h>
#include "DHTesp.h"

DHTesp dht;

//----------------  Completa con tus datos   ---------------------
char ssid[] = "studio";           // Nombre de la red WiFi 
char pass[] = "88888888";         // Contraseña de la red WiFi
unsigned long myChannelNumber = 964762;  // Nº de canal de ThinkSpeak
const char * myWriteAPIKey = "1W3SDA9T9H1MYWAM"; // Copia y pega API Key Write 
//----------------------------------------------------------------

WiFiClient  client;

void setup() {
  
  Serial.begin(9600); //Iniciamos el monitor serie
  while (!Serial) {;} // Esperamos por la conexión serie.
  dht.setup(2, DHTesp::DHT11); // Asignamos el pin/sensor
  
  WiFi.mode(WIFI_STA);// Establecemos el modo como estación.
  ThingSpeak.begin(client); // Cliente Biblioteca
}

void loop() {

  // Establece la conexión WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("\nConectando a: ");
    Serial.print(ssid);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);
      Serial.print(".");
      delay(5000);     
    } 
    Serial.print("Conectado.");
    Serial.print("\nDirección IP: ");
    Serial.print(WiFi.localIP());
    Serial.print("\n.");
  } // Fin conexión WiFi

  // Inicio biblioteca DHTesp.h
  delay(dht.getMinimumSamplingPeriod()); // Tiempo mínimo entre muestras
  
  float temperatura = dht.getTemperature(); // Valor de la temperatura
  float humedad = dht.getHumidity();        // Valor de la humedad
  
  Serial.print(dht.getStatusString()); // Comprueba si se produce algún error
  Serial.print("\nTemperatura: ");  // Imprime resultados en el monitor serie
  Serial.print(temperatura);
  Serial.print("\nHumedad: ");
  Serial.print(humedad);

  // Inicio biblioteca ThingSpeak
  ThingSpeak.setField(1, temperatura); // Configuramos los campos
  ThingSpeak.setField(2, humedad);
  
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200){
    Serial.println("\nCanal actualizado.");
  }
  else{
    Serial.println("Problema con la actualización. HTTP código de error " + String(x));
  }
  
  delay(20000); // Actualiza cada 20 segundos
}
