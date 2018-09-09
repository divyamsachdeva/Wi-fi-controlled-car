#include <ESP8266WiFi.h>
const char* ssid = "hsv";// enter wifi name 
const char* password = "123214321";// enter password here

int motor_FL = 14; //pin D5 of nodemcu
int motor_FR = 12; //pin D6 of nodemcu
int motor_RL = 13; //pin D7 of nodemcu
int motor_RR = 15; //pin D8 of nodemcu
int switch1 = LOW,switch2 = LOW, Mspeed = 255;
  
WiFiServer server(80);// connects server to port 80 





void setup() 
{
// turning all the switches off initially.
  pinMode(motor_FL, OUTPUT);
  analogWrite(motor_FL, 0);
  pinMode(motor_FR, OUTPUT);
  analogWrite(motor_FR, 0);
  pinMode(motor_RL, OUTPUT);
  analogWrite(motor_RL, 0);
  pinMode(motor_RR, OUTPUT);
  analogWrite(motor_RR, 0);
    //////////////////////////////////////// Connect to WiFi network//////////////////////////////////////////////////////
  Serial.begin(115200);// setting up baud rate of 115200
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);// connecting to the wifi using wifi name and password provided
 
  while (WiFi.status() != WL_CONNECTED) // checks if wifi is connected or not...if conected loop will end
  {
    delay(100);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");//
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////// Start the server/////////////////////////////////////////////////////
  server.begin();
  Serial.println("Server started");
  ////////////////////////////////////////// Print the IP address////////////////////////////////////////////////////
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());// this function "WiFi.localIP()" provides ip address
  Serial.println("/");
 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
 
void loop() {
 /////////// Check if a client has connected//////////////////////////////////////////////
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
 //////////// Wait until the client sends some data//////////////////////////////////////
  
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
  ///////////////////////////////////////////////////////////////////////////////////
 
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
 
  ////////////////////// Match the request///////////////////////////////////////////
 
  if (request.indexOf("/SPEED=H") != -1)
    Mspeed = 255;
  if (request.indexOf("/SPEED=M") != -1)
    Mspeed = 175;
  if (request.indexOf("/SPEED=L") != -1)
    Mspeed = 100;

  if(switch1 == HIGH && switch2 == HIGH)
  { analogWrite(motor_FL, Mspeed);
    analogWrite(motor_FR, Mspeed);
    analogWrite(motor_RL, Mspeed);
    analogWrite(motor_RR, Mspeed);
  }
  if(switch1 == HIGH && switch2 == LOW)
  { analogWrite(motor_FR, Mspeed);
    analogWrite(motor_RR, Mspeed);
  }  
  if(switch1 == LOW && switch2 == HIGH)
  { analogWrite(motor_FL, Mspeed);
    analogWrite(motor_RL, Mspeed);
  }  
  if (request.indexOf("/MOVE=FORWARD") != -1)  {
    analogWrite(motor_FL, Mspeed);
    analogWrite(motor_RL, Mspeed);
    switch1 = HIGH;
    analogWrite(motor_FR, Mspeed);
    analogWrite(motor_RR, Mspeed);
    switch2 = HIGH;
  }
  if (request.indexOf("/MOVE=STOP") != -1)  {
    analogWrite(motor_FL, 0);
    analogWrite(motor_RL, 0);
    switch1 = LOW;
    analogWrite(motor_FR, 0);
    analogWrite(motor_RR, 0);
    switch2 = LOW;
  }
  if (request.indexOf("/SIDE=LEFT") != -1)  {
    analogWrite(motor_FL, 0);
    analogWrite(motor_RL, 0);
    switch1 = LOW;
    analogWrite(motor_FR, Mspeed);
    analogWrite(motor_RR, Mspeed);
    switch2 = HIGH;
  }
  if (request.indexOf("/SIDE=RIGHT") != -1)  {
    analogWrite(motor_FL, Mspeed);
    analogWrite(motor_RL, Mspeed);
    switch1 = HIGH;
    analogWrite(motor_FR, 0);
    analogWrite(motor_RR, 0);
    switch2 = LOW;
  }
  
  // Return the response
  //  client.println("HTTP/1.1 200 OK");
  //  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<p align=middle><center><font size =7 color=blue> ");
  
  client.println(" <br><br><br> ");
  if(switch1 == HIGH && switch2 == HIGH)
    client.println(" RUN ");
  if(switch1 == LOW && switch2 == LOW)
    client.println(" REST ");
  if(switch1 == LOW && switch2 == HIGH)
    client.println(" LEFT ");
  if(switch1 == HIGH && switch2 == LOW)
    client.println(" RIGHT ");
      
  client.println(" <br><br><br><br><br><br> ");
  client.println(" <a href=\"/MOVE=FORWARD\"\"><button><h1>RUN </h1></button></a> ");
  client.println(" <br><br><br> ");
  client.println(" <a href=\"/SIDE=LEFT\"\"><button><h1> LEFT </h1></button></a>&emsp;&emsp;");
  client.println(" <a href=\"/MOVE=STOP\"\"><button><h1> REST </h1></button></a>&emsp;&emsp;");
  client.println(" <a href=\"/SIDE=RIGHT\"\"><button><h1> RIGHT </h1></button></a> ");

  client.println(" <br><br><br> ");
  client.print(" SPEED : ");
  if(Mspeed == 255)
   client.println(" HIGH ");
  if(Mspeed == 175)
   client.println(" MEDIUM ");
  if(Mspeed == 100)
   client.println(" LOW ");
  
  client.println(" <br><br><br> ");
  client.println(" <a href=\"/SPEED=H\"\"><button><h1> HIGH </h1></button></a>&emsp;&emsp; ");
  client.println(" <a href=\"/SPEED=M\"\"><button><h1> MEDIUM </h1></button></a>&emsp;&emsp; ");
  client.println(" <a href=\"/SPEED=L\"\"><button><h1> LOW </h1></button></a> ");
  
  client.println("  ");
  client.print("</font></center></p>");
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
 
}
