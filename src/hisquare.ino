#include <WiFi.h>
#define pir 26
#define infra 34

const char* ssid = "POCOF3";
const char* password = "edusantos1";

WiFiServer server(80);

String header;

int contador = 0;

unsigned long currentTime = millis();
unsigned long previousTime = 0;
const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);
  pinMode(pir, INPUT);
  pinMode(infra, INPUT);

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  WiFiClient client = server.available(); 

  int deteccao = digitalRead(infra);
  int valorPir = digitalRead(pir);

  if (valorPir == 1 && deteccao == 0) {
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
            // head
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<meta charset=utf-8></head>");
            client.println("<style> body{ background-color: LightSteelBlue; }</style>");
            // body
            client.println("<body><center>");
            client.println("<img src=https://images2.imgbox.com/09/40/5PkSzj04_o.png alt= some text width=250 height=150>");
            client.println("<div><output id=data style=font-family: arial black, avant garde; font-size: 15px;></output></div>");
            client.println("<div><output id=hora style=font-family: arial black, avant garde; font-size: 15px;></output></div>");
            client.println("<p style=font-style:italic;font-weight: bold; font-family: Arial, Helvetica, sans-serif;>o melhor para o seu sucesso </p>");
            client.println(" <script src=https://replit.com/public/js/replit-badge-v2.js theme=dark position=bottom-right></script>");
            client.println("</div><center><form><label for=nome style=color:blue; font-weight: bold;>Praça Liberdade</label><br>");
            client.print("<p>");
            client.print(contador);
            client.println("</p>");
            client.println("</form></body>");

            // JS
            client.println("<script>");
            client.println("function atualizaRelogio() {");
            client.println("var dataElement = document.getElementById('data');");
            client.println("var horaElement = document.getElementById('hora');");
            client.println("var momentoAtual = new Date();");
            client.println("var dataFormatada = momentoAtual.toLocaleDateString();");
            client.println("var horaFormatada = momentoAtual.toLocaleTimeString();");
            client.println("dataElement.innerHTML = dataFormatada;");
            client.println("horaElement.innerHTML = horaFormatada;");
            client.println("setTimeout(atualizaRelogio, 1000);");
            client.println("}");
            client.println("atualizaRelogio();");
            client.println("</script>");

            client.println("</html>");

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




