#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "iPhone de Alex";
const char* password = "popo1234";
const char* serverName = "http://api.thingspeak.com/update?api_key=ZW4DBKXBBO53LX9S";

void setup() {
  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, 16, 17);

  WiFi.begin(ssid, password);
  Serial.println("Conectando a WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Conectado a WiFi!");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    delay(100);  // Pequeño retraso para asegurar la disponibilidad de datos
    if (Serial2.available() > 0) {
      String value1 = cleanInput(Serial2.readStringUntil('\n'));
      String value2 = cleanInput(Serial2.readStringUntil('\n'));
      String value3 = cleanInput(Serial2.readStringUntil('\n'));

      // Verificación de la integridad y existencia de los datos
      if (!value1.isEmpty() && !value2.isEmpty() && !value3.isEmpty()) {
        value1 = validateNumber(value1);
        value2 = validateNumber(value2);
        value3 = validateNumber(value3);

        HTTPClient http;
        http.begin(serverName);

        String httpRequestData = "field1=" + value1 + "&field2=" + value2 + "&field3=" + value3;
        int httpResponseCode = http.POST(httpRequestData);

        Serial.print("Código de respuesta HTTP: ");
        Serial.println(httpResponseCode);
        Serial.print("Enviando valor1: ");
        Serial.println(value1);
        Serial.print("Enviando valor2: ");
        Serial.println(value2);
        Serial.print("Enviando valor3: ");
        Serial.println(value3);

        http.end();
      } else {
        Serial.println("No se recibieron datos válidos, no se envía nada.");
      }
    } else {
      Serial.println("No hay datos disponibles en el puerto serial.");
    }
  } else {
    Serial.println("Error en la conexión WiFi");
    WiFi.reconnect();
  }

  delay(20000);
}

String cleanInput(String data) {
  String result = "";
  for (int i = 0; i < data.length(); i++) {
    char c = data[i];
    if (isdigit(c) || c == '.') {
      result += c;
    }
  }
  return result;
}

String validateNumber(String num) {
  if (num.indexOf('.') == -1) { // Si no contiene punto decimal
    num += ".0"; // Añade '.0' para asegurar el formato decimal
  }
  return num;
}
