#include <WiFi.h>
#include <WebServer.h>
//Enable pin - for PWM - to control motor speed
//#define ENA 25 //Enable Pin A
#define IN1 27 
#define IN2 26
//#define ENB 14 //Enable Pin B
#define IN3 12
#define IN4 13

int motorSpeed = 200; //controlling voltage that motor gets
//Value can be changed from 0 to 255 - PWM

const char* ssid = "ESP32-RobotControl";
const char* password = "123@robo";

WebServer server(80); //Connecting to HTTP server

// ----- Motor Control -----
//Forward direction
void moveForward(int duration) {
  digitalWrite(IN1, HIGH); //Left wheel forward
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); //Right wheel forward
  digitalWrite(IN4, LOW);
 // analogWrite(ENA, motorSpeed);
 // analogWrite(ENB, motorSpeed);
  delay(duration);
  stoppingMotor();
}
//Backward direction
void moveBackward(int duration) {
  digitalWrite(IN1, LOW); //Left wheel backward
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW); //Right wheel backward
  digitalWrite(IN4, HIGH);
  //analogWrite(ENA, motorSpeed);
  //analogWrite(ENB, motorSpeed);
  delay(duration);
  stoppingMotor();
}
// Smooth Left Turn (pivot on left wheel)
void turnLeft(int duration) {
  digitalWrite(IN1, LOW);   // Left wheel stopped
  digitalWrite(IN2, LOW);
 // analogWrite(ENA, 0);

  digitalWrite(IN3, HIGH);  // Right wheel forward
  digitalWrite(IN4, LOW);
 // analogWrite(ENB, motorSpeed);

  delay(duration);
  stoppingMotor();
}

// Smooth Right Turn (pivot on right wheel)
void turnRight(int duration) {
  digitalWrite(IN1, HIGH);  // Left wheel forward
  digitalWrite(IN2, LOW);
  //analogWrite(ENA, motorSpeed);

  digitalWrite(IN3, LOW);   // Right wheel stopped
  digitalWrite(IN4, LOW);
  //analogWrite(ENB, 0);

  delay(duration);
  stoppingMotor();
}

//Clockwise rotation - Left wheel forward and Right wheel backward
void rotateClockwise(int duration) {
  digitalWrite(IN1, HIGH); //Left wheel forward
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); //Right wheel backward
  digitalWrite(IN4, HIGH);
  //analogWrite(ENA, motorSpeed);
  //analogWrite(ENB, motorSpeed);
  delay(duration);
  stoppingMotor();
}

//Counter-Clockwise rotation - Right wheel forward and Left wheel backward
void rotateAntiClockwise(int duration) {
  digitalWrite(IN1, LOW); //Left wheel backward
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);//Right wheel forward
  digitalWrite(IN4, LOW);
  //analogWrite(ENA, motorSpeed);
  //analogWrite(ENB, motorSpeed);
  delay(duration);
  
  stoppingMotor();
}

void stoppingMotor() {
  digitalWrite(IN1, LOW); //Left wheel stopped
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); //Right wheel stopped
  digitalWrite(IN4, LOW);
 // analogWrite(ENA, 0);
 // analogWrite(ENB, 0);
}

// ----- HTML Page -----
String htmlPage() {
  return String(R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>ESP32 Robot Controller</title>
  <meta name="viewport" content="width=device-width, initial-scale=4.0">
  <style>
    body { 
      text-align: center; 
      font-family: Arial, Helvetica, sans-serif;
    }
    button {
      width: 120px; 
      height: 60px; 
      margin: 10px;
      font-size: 20px; 
      border-radius: 12px;
    }
    .row { 
      display: flex;
      justify-content: center;
      align-items: center;
    }
    .main-controls {
      display: flex;
      flex-direction: column;
      align-items: center;
      margin-top: 20px;
    }
    .rotate-controls {
      margin-top: 20px;
    }
  </style>
</head>
<body>
  <h3>ESP32 Robot Controller</h3>
  <div class="main-controls">
    <div class="row">
      <button onclick="askSteps('forward')">Forward</button>
    </div>
    <div class="row">
      <button onclick="askSteps('left')">Left</button>
      <button onclick="send('stop')">Stop</button>
      <button onclick="askSteps('right')">Right</button>
    </div>
    <div class="row">
      <button onclick="askSteps('backward')">Backward</button>
    </div>
  </div>
  <div class="rotate-controls">
   <!-- <button onclick="askRotate('cwrotate')">Rotate Clockwise</button>
    <button onclick="askRotate('ccwrotate')">Rotate Anti-Clockwise</button> -->
  </div>

  <script>
    function send(cmd) {
      fetch('/' + cmd);
    }
    function askSteps(cmd) {
      let steps = prompt("Enter duration (ms) for " + cmd + " movement:", "1000");
      if (steps != null) {
        fetch('/' + cmd + '?time=' + steps);
      }
    }
    function askRotate(cmd) {
      let t = prompt("Enter rotation time (ms):", "2000");
      if (t != null) {
        fetch('/' + cmd + '?time=' + t);
      }
    }
  </script>
</body>
</html>
)rawliteral");
}
// ----- Handle Requests -----
void handleRoot() {
  server.send(200, "text/html", htmlPage());
}

void handleCommand(String cmd) {
  int duration = 1000; // default 1 second
  if (server.hasArg("time")) {
    duration = server.arg("time").toInt();
  }

  if (cmd == "forward") moveForward(duration);
  else if (cmd == "backward") moveBackward(duration);
  else if (cmd == "left") turnLeft(duration);
  else if (cmd == "right") turnRight(duration);
  else if (cmd == "cwrotate") rotateClockwise(duration);
  else if (cmd == "ccwrotate") rotateAntiClockwise(duration);
  else if (cmd == "stop") stoppingMotor();

  server.send(200, "text/plain", "OK");
}

void setup() {
  Serial.begin(115200);

 // pinMode(ENA, OUTPUT);
 // pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
 
//ESP32 in AP mode
  WiFi.softAP(ssid, password);
  Serial.println("WiFi started");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.on("/forward", [](){ handleCommand("forward"); });
  server.on("/backward", [](){ handleCommand("backward"); });
  server.on("/left", [](){ handleCommand("left"); });
  server.on("/right", [](){ handleCommand("right"); });
  server.on("/cwrotate", [](){ handleCommand("cwrotate"); });
  server.on("/ccwrotate", [](){ handleCommand("ccwrotate"); });
  server.on("/stop", [](){ handleCommand("stop"); });
  server.begin();
  Serial.println("Web server started");
}

void loop() {
  server.handleClient();
}