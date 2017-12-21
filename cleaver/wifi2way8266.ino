#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

IPAddress apIP(192, 168, 1, 1);
String ssid;
String password;
int connect = 0;
int n;
String newssid;
String newpassword;
String host;
ESP8266WebServer server(80);
int count = 0;
byte value;

void setup(void){
  newssid = "ESP000"+String(ESP.getChipId());
  newpassword = "000"+String(ESP.getChipId());
  Serial.begin(115200);
  Serial.println("");
  WiFi.disconnect();
  delay(1000);
  WiFi.mode(WIFI_OFF);
  delay(1000);
  // set both access point and station
  WiFi.mode(WIFI_AP_STA);
  Serial.println("start scan");
  n = WiFi.scanNetworks();
  Serial.println("scan done");
  WiFi.softAPConfig(apIP,apIP,IPAddress(255, 255, 255, 0));
  WiFi.softAP(newssid.c_str(), newpassword.c_str());
  Serial.print(newssid);
  Serial.print(" server ip: ");
  Serial.println(WiFi.softAPIP());

  server.on("/ip", handleIp);
  server.on("/", handleRoot);  
  server.onNotFound(handleNotFound);  
  server.begin();
  Serial.println("HTTP server started");
}
 
void loop(void){
  server.handleClient();
}

void handleIp() {
    host = "http://"+String(server.arg(0));
    String s;
    Serial.print("[HTTP]begin...\n");
    HTTPClient http;
    http.begin(host);
    Serial.println(host);
    Serial.print("[HTTP]GET...\n");
    int httpCode = http.GET();
    if(httpCode > 0) {
      // Serial.printf("[HTTP]GET...code: %d\n", httpCode);
      if(httpCode == HTTP_CODE_OK){
        s = http.getString();
        // Serial.println(s);
        // int equal = s.indexOf('=');
        // int value = s.substring(equal+1).toInt();
      }
    } else {
      s += host+"<br>";
      s += "[HTTP]GET...failed "+http.errorToString(httpCode)+"<br>";
      Serial.printf("[HTTP]GET...failed, error: %s\n",http.errorToString(httpCode).c_str());
    }
    http.end();
    s += "<br><br><br>";
    s += "ESP ChipId : 000"+String(ESP.getChipId())+"<br>";
    s += "this node connect to SSID :"+ssid+"<br>";
    s += "<form action='/ip'>Test IP:<input type='text' name='ip'><br>";
    s += "<input type='submit' value='Submit'><br></form>";    
    server.send(200, "text/html", s);
}

void handleRoot() {
  Serial.print("handleRoot: ");
  Serial.println(count);
  String s = "ESP ChipId : 000"+String(ESP.getChipId())+"<br>request count: ";
  s += ++count;
  s += "<br>";
  if (connect == 0) {
    // Serial.println("start scan");
    // int n = WiFi.scanNetworks();
    // Serial.println("scan done");
    if (n == 0) {
      Serial.println("no networks found");
    } else {
      Serial.print(n);
      Serial.println(" networks found");
    }
    s += "<table>";
    for (int i = 0; i<n; ++i) {
      s += "<tr><form action='http://192.168.1.1/";
      s += WiFi.SSID(i);
      s += "'><th>";
      s += WiFi.SSID(i);
      s += "</th><th>Password :";
      s += "<input type='text' name='ip'>";
      s += "<input type='submit' value='Submit'>";
      s += "</th></form></tr>";
    }
    s += "</table>";
  } else {
    s += "this node connect to SSID :"+ssid+"<br>";
    s += "<form action='/ip'>Test IP:<input type='text' name='ip'><br>";
    s += "<input type='submit' value='Submit'><br></form>";
  }
  server.send(200, "text/html", s);
}

void handleNotFound() {
  if (String(server.uri()) == "/favicon.ico") {
    
  } else {
    String s = "ESP ChipId : 000"+String(ESP.getChipId())+"<br>";
    int narg = server.args();
    Serial.println("Number of args "+String(narg));
    for (int i=0; i<narg; i++){
      Serial.print("Arg Name"+server.argName(i)+":");
      Serial.println(server.arg(i));
    }
    Serial.print("request :");
    Serial.println(server.uri());
    ssid = String(server.uri()).substring(1);
    password = String(server.arg(0));
    count++;

    if (connect == 0) {
      s = "request : "+ssid;
      WiFi.begin(ssid.c_str(), password.c_str());
      while (WiFi.status() != WL_CONNECTED) {
        Serial.print('.');
        delay(1000);
      }
      Serial.print("Connected to: ");
      Serial.print(WiFi.SSID());
      Serial.print(", IP address: ");
      Serial.println(WiFi.localIP());
      connect = 1;
    }
    s += "<br>";
    s += "this node connect to SSID :"+ssid+"<br>";
    s += "<form action='/ip'>Test IP:<input type='text' name='ip'><br>";
    s += "<input type='submit' value='Submit'><br></form>";
    server.send(200, "text/html", s);
  }
}
