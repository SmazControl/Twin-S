#include <ESP32WebServer.h> //https://github.com/nhatuan84/esp32-webserver
#include <WiFi.h>
#include <HTTPClient.h>

IPAddress apIP(192, 168, 4, 1);
String ssid;
String password;
int connect = 0;
uint64_t chipid;
int n;
String newssid;
String newpassword;
String host;

ESP32WebServer server(80);
HTTPClient http;

int count = 0;
String payload;
byte value;

void setup(void){
  chipid = ESP.getEfuseMac();
  newssid = "ESP"+String((uint32_t)chipid);
  newpassword = String((uint32_t)chipid);
  Serial.begin(115200);
  Serial.println("");
  WiFi.disconnect();
  delay(1000);
  WiFi.mode(WIFI_OFF);
  delay(1000);
  // set both access point and station
  WiFi.mode(WIFI_AP_STA);
  delay(100);
  Serial.println("scan start");
  n = WiFi.scanNetworks();
  Serial.println("scan done");
  WiFi.softAPConfig(apIP,apIP,IPAddress(255, 255, 255, 0));
  WiFi.softAP(newssid.c_str(), newpassword.c_str());
  Serial.print(newssid);
  Serial.print(" server ip: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);  
  server.onNotFound(handleNotFound);  
  server.begin();
  Serial.println("HTTP server started");
  payload = "";
}
 
void loop(void){
  server.handleClient();
}

void handleRoot() {
  Serial.print("handleRoot: ");
  Serial.println(count);
  chipid = ESP.getEfuseMac();
  String s = "ESP ChipId : "+String((uint32_t)chipid)+"<br>request count: ";
  s += ++count;
  s += "<br>";
  if (connect == 0) {
    // Serial.println("scan start");
    // int n = WiFi.scanNetworks();
    // Serial.print("scan done");
    if (n == 0) {
      Serial.println("no networks found");
    } else {
      Serial.print(n);
      Serial.println(" networks found");
    }
    for (int i = 0; i<n; ++i) {
      s += "<a href='http://192.168.4.1/";
      s += WiFi.SSID(i);
      s += "'>";
      s += WiFi.SSID(i);
      s += "</a><br>";
    }
  } else {
    s += "this node connect to SSID :"+ssid+"<br>";
  }
  server.send(200, "text/html", s);
}

void handleNotFound() {
  if (String(server.uri()) == "/favicon.ico") {
    
  } else {
    Serial.print("request :");
    Serial.println(server.uri());
    ssid = String(server.uri()).substring(1);
    password = String(server.uri()).substring(4);
    count++;

    if (connect == 0) {
      payload = "request : "+ssid;
      WiFi.begin(ssid.c_str(), password.c_str());
      while (WiFi.status() != WL_CONNECTED) {
        Serial.print('.');
        delay(1000);
      }
      Serial.print("Connected to: ");
      Serial.print(WiFi.SSID());
      Serial.print(", IP address: ");
      Serial.println(WiFi.localIP());
      host = "http://"+String(WiFi.localIP());
      connect = 1;
    }
    Serial.print("[HTTP]begin...\n");
    http.begin(host.c_str());
    Serial.print("[HTTP]GET...\n");
    int httpCode = http.GET();
    if(httpCode > 0) {
      // Serial.printf("[HTTP]GET...code: %d\n", httpCode);
      if(httpCode == HTTP_CODE_OK){
        payload = http.getString();
        // Serial.println(payload);
        int equal = payload.indexOf('=');
        int value = payload.substring(equal+1).toInt();
      }
    } else {
      Serial.printf("[HTTP]GET...failed, error: %s\n",http.errorToString(httpCode).c_str());
    }
    http.end();
    server.send(200, "text/html", payload);
  }
}
