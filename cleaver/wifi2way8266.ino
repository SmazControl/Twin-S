#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

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
  // WiFi.softAPConfig(apIP,apIP,IPAddress(255, 255, 255, 0));
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
    host = String(server.arg(0));
    String s;
    Serial.print("[HTTP]begin...\n");
    Serial.println(host);
    WiFiClient client;
    if (!client.connect(host.c_str(), 80)) {
      Serial.print("Connection failed");
      s = s + "Connection failed";
      return;
    }
    String url = "";
    client.print(String("GET") + url + "HTTP/1.1\r\n" +
      "Host: " + host + "\r\n" + 
      "Connection: close\r\n\r\n");
    delay(10);
    while(client.available()){
      String line = client.readStringUntil('\r');
      Serial.print(line);
      s = s + line;
    }
    client.stop();
    Serial.print("[HTTP]GET...\n");
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
    if (n == 0) {
      Serial.println("no networks found");
    } else {
      Serial.print(n);
      Serial.println(" networks found");
    }
    s += "<table>";
    for (int i = 0; i<n; ++i) {
      s += "<tr><form action='/";
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
