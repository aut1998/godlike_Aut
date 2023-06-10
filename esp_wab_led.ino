
#include <ESP8266WiFi.h>

const char* ssid = "XXXX";
const char* password = "XXXXXX";

int ledPin1 = D4; // ขา D1

int ch1 = 0;
WiFiServer server(80);

void setup() {
  Serial.begin(9600);
  delay(100);

  // ประกาศขา เป็น Output
  pinMode(ledPin1, OUTPUT);  

  // เริ่มต้น ให้ Logic ตำแหน่งขาเป็น LOW
  digitalWrite(ledPin1, HIGH);   

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Wait until the client sends some data
  Serial.println("new client");
  while (!client.available()) {
    delay(1);
  }

  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  // Match the request

    // รับ Link ช่อง 1 

  if (request.indexOf("/LED1=ON") != -1) {
    digitalWrite(ledPin1, LOW);
    ch1 = 1;
  }
  if (request.indexOf("/LED1=OFF") != -1) {
    digitalWrite(ledPin1, HIGH);
    ch1 = 0;
  }
//

  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); // do not forget this one
  client.println("");
  client.println("");

  client.print("Led pin D1 : ");
  if (ch1 == 1) {
    client.print("On<br>");
  } else {
    client.print("Off<br>");
  }

  client.println("");

client.println("CH1 <a href=\"/LED1=ON\"\"><button> On </button></a><a href=\"/LED1=OFF\"\"><button> OFF </button></a><br>");  


  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
}
