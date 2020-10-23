#include <Arduino.h>
#include <WiFiManager.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <EEPROM.h>

#define redLed D1
#define blueLed D2
#define greenLed D3
#define conexLed D5
#define resetBtn A0

String redValue = "0";
String greenValue = "0";
String blueValue = "0";
String acionar = "0";

int redValueConverted = 0;
int greenValueConverted = 0;
int blueValueConverted = 0;
int x = 0;

ESP8266WebServer server(80);

void setup() 
{
  Serial.begin(115200); //BAUD RATE (VELOCIDADE DE COMUNICAÇÃO VIA SERIAL)
  EEPROM.begin(4096);
  
  pinMode(redLed, OUTPUT);
  pinMode(blueLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(conexLed, OUTPUT);
  pinMode(resetBtn, INPUT_PULLUP);

  WiFiManager wifiManager(0);
  wifiManager.autoConnect("MarcatoHome");  

  if (EEPROM.read(100) < 10) {redValueConverted = EEPROM.read(100);} else {redValueConverted = EEPROM.read(100)*10;}
  if (EEPROM.read(101) < 10) {greenValueConverted = EEPROM.read(101);} else {greenValueConverted = EEPROM.read(101)*10;}
  if (EEPROM.read(102) < 10) {blueValueConverted = EEPROM.read(102);} else {blueValueConverted = EEPROM.read(102)*10;}

  ArduinoOTA.onStart([]() 
  {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_SPIFFS
      type = "filesystem";
    }

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();

  server.on("/", handleRoot); //QUANDO É REQUISITADO A PÁGINA PRINCIPAL DO WEB SERVER CHAMA A FUNÇÃO
  server.on("/controlar", outController); //QUANDO RECEBIDO ESSA REQUISIÇÃO O ESTADO DA SAÍDA DA LÂMPADA É MODIFICADO
  server.begin();
}

void handleRoot() 
{
  String html = "<html><head><title>Marcato Web Server LED PC Controller</title>";
  html += "<style>body {background-color: #cccccc;";
  html += "font-family: Arial, Helvetica, Sans-Serif;";
  html += "Color: #000088; }</style>";
  html += "</head><body>";
  html += "<h1> Marcato Web Server LED PC Controller </h1>";
  html += "<form method='POST' action='/controlar'>";
  html += "<p> RED: <input type=text name=r value=0 /></p>";
  html += "<p> GREEN: <input type=text name=g value=0 /></p>";
  html += "<p> BLUE: <input type=text name=b value=0 /></p>";
  html += "<p><input type=submit value=ENVIAR /></p>";
  html += "</form>";
  html += "</body></html>";

  server.send(200, "text/html", html);
}

void outController() 
{
  String html = "<html><head><title>Recebendo Dados</title>";
  html += "<style>body {background-color: #cccccc;";
  html += "font-family: Arial, Helvetica, Sans-Serif;";
  html += "Color: #000088; }</style>";
  html += "</head><body>";
  html += "<h1>CONTROLE DE FITA LED RGB</h1>";
  html += "<p>";
  html += "<b> RED: ";
  html += server.arg("r");
  html += "</b>";
  html += "<b> GREEN: ";
  html += server.arg("g");
  html += "</b>";
  html += "<b> BLUE: ";
  html += server.arg("b");
  html += "</b>";

  redValue = server.arg("r");
  greenValue = server.arg("g");
  blueValue = server.arg("b");
  acionar = server.arg("a");

  if (server.hasArg("a"))
  {
    if (acionar == "0")
    {
      redValueConverted = 0;
      greenValueConverted = 0;
      blueValueConverted = 0;
    } else if (acionar == "1")
    {
      if (EEPROM.read(100) < 10) {redValueConverted = EEPROM.read(100);} else {redValueConverted = EEPROM.read(100)*10;}
      if (EEPROM.read(101) < 10) {greenValueConverted = EEPROM.read(101);} else {greenValueConverted = EEPROM.read(101)*10;}
      if (EEPROM.read(102) < 10) {blueValueConverted = EEPROM.read(102);} else {blueValueConverted = EEPROM.read(102)*10;}  
    }
  } else
  {
    redValueConverted = map(redValue.toInt(), 0, 233, 0, 1024);
    greenValueConverted = map(greenValue.toInt(), 0, 233, 0, 1024);
    blueValueConverted = map(blueValue.toInt(), 0, 233, 0, 1024);

    if (redValueConverted < 10 ) {EEPROM.write(100, redValueConverted);} else {EEPROM.write(100, redValueConverted/10);}
    if (greenValueConverted < 10 ) {EEPROM.write(101, greenValueConverted);} else {EEPROM.write(101, greenValueConverted/10);}
    if (blueValueConverted < 10 ) {EEPROM.write(102, blueValueConverted);} else {EEPROM.write(102, blueValueConverted/10);}

    EEPROM.commit();
  }

  Serial.print("RED: ");
  Serial.println(redValueConverted);
  Serial.print("GREEN: ");
  Serial.println(greenValueConverted);
  Serial.print("BLUE: ");
  Serial.println(blueValueConverted);

  html += "<form method='GET' action='/'>";
  html += "<p><input type=submit /></p>";
  html += "</form>";
  html += "</p></body></html>";

  server.send(200, "text/html", html);
}

void loop() 
{
  server.handleClient();
  ArduinoOTA.handle();
  if (analogRead(resetBtn) > 100)
  {
    x++;
    delay(50);
    if (x > 50) resetFunction();
  }
  analogWrite (redLed, redValueConverted);
  analogWrite (greenLed, greenValueConverted);
  analogWrite (blueLed, blueValueConverted);
  delay(10);
}

void resetFunction()
{
  for (int i = 0; i <= 4096;)
  {
    EEPROM.write(i, 0);
    i++;
    delay(1);	
  }
  delay(10);
  EEPROM.commit();
  Serial.println("CLEANING MEMORY AND THEN RESETING ...");
  delay(100);
  ESP.reset();  
}