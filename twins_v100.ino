// Robo India Tutorial 
// Digital Input and Output on LED 
// Hardware: NodeMCU
#include <SoftwareSerial.h>
SoftwareSerial Twin_S(13, 15);   // RX, TX

void setup() 
{
  Serial.begin(9600);                  
  Twin_S.begin(9600);
  delay(100);
  Serial.println("");
  Serial.println("Twin-S");  
  Twin_S.println("Twin-S");

}
int input = 0;
String data="";
char inputchar;
void loop() 
{
  inputchar = Twin_S.read();
  input = int(inputchar);
  if((input!=255)&&(input!=10)&&(input!=0)&&(input!=13)) {
    Serial.print(inputchar);
    Serial.print(" ");
    Serial.println(input);
    data += inputchar;
  }
  
  if (input==10) {
    Serial.println(data);
    data = "";
  }
}
