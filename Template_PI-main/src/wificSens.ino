#include <WiFi.h>
#define pir 26
#define infra 34

// Replace with your network credentials
const char* ssid = "POCOF3";
const char* password = "peruperuano";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

int contador = 0;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);
  pinMode(pir, INPUT);
  pinMode(infra, INPUT);

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){
  WiFiClient client = server.available();   
     int deteccao = digitalRead(infra);
     int valorPir = digitalRead(pir);

      if(valorPir == 1 && deteccao == 0){
         contador++;
         delay(750);
    }

  if (client) {                            
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          
    String currentLine = "";              
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  
      currentTime = millis();
      if (client.available()) {            
        char c = client.read();            
        Serial.write(c);                    
        header += c;
        if (c == '\n') {                    
         
          if (currentLine.length() == 0) {
            
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            
            
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<meta charset=utf-8>");
            
           
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}</style></head>");
            
           
            client.println("<body><h1 style=color: red;>HiSquare</h1>");
            client.println("<p style=font-style:italic;font-weight: bold; font-family: Arial, Helvetica, sans-serif;>o melhor para o seu sucesso </p>");
            client.print("<p>tem isso ai de gente: ");
            client.println(contador);
            client.println("</p>");
        
            
            
            client.println("</body></html>");
            
            
            client.println();
            
            break;
          } else { 
            currentLine = "";
          }
        } else if (c != '\r') { 
          currentLine += c;    
        }
      }
    }
   
    header = "";

    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
