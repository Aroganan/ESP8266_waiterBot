#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// Define the GPIO pin where the LED is connected
const int ledPin_green = 5;
const int ledPin_blue = 4;

// Create an instance of the web server on port 80
ESP8266WebServer server(80);

// Variable to store the LED state
bool waiterState = false;

// HTML code for the web page
String htmlPage = "<html>\
<head>\
<title>Waiter Bot initial control</title>\
</head>\
<body>\
<div style='display: block; justify-content: center; align-items: center; height:100vh; width: 100%; margin: 30px;'>\
<h1>Start the Bot Function</h1>\
<p >Bot Status: <strong id='ledState'>OFF</strong></p>\
<button onclick='start_bot()'>Start Bot</button>\
</div>\
<script>\
function start_bot() {\
    var xhr = new XMLHttpRequest();\
    xhr.open('GET', '/start_bot', true);\
    xhr.onreadystatechange = function() {\
        if (xhr.readyState == 4 && xhr.status == 200) {\
            document.getElementById('ledState').innerText = xhr.responseText;\
        }\
    };\
    xhr.send();\
}\
</script>\
</body>\
</html>";

void setup() {
  // Initialize the GPIO pin as an output
  pinMode(ledPin_green, OUTPUT);
  digitalWrite(ledPin_green, LOW);

  pinMode(ledPin_blue, OUTPUT);
  digitalWrite(ledPin_blue, HIGH);

  // Start the ESP8266 in AP mode
  WiFi.softAP("Waiter Bot control", "waiter123");

  Serial.begin(115200);
  Serial.println();
  Serial.print("Access Point IP address: ");
  Serial.println(WiFi.softAPIP());

  // Define the routes
  server.on("/", HTTP_GET, handleRoot);
  server.on("/start_bot", HTTP_GET, start_bot);

  // Start the web server
  server.begin();
  Serial.println("Web server started.");
}

void handleRoot() {
  server.send(200, "text/html", htmlPage);
}

void start_bot() {
  waiterState = !waiterState;
  digitalWrite(ledPin_green, waiterState ? HIGH : LOW);
  digitalWrite(ledPin_blue, waiterState ? LOW : HIGH);
  String waiterStatus = waiterState ? "ON" : "OFF";
  server.send(200, "text/plain", waiterStatus);
}

void loop() {
  server.handleClient();
}
