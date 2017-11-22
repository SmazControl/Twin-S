// Robo India Tutorial 
// Digital Input and Output on LED 
// Hardware: NodeMCU
// D2 -- OLED SCL
// D3 -- OLED SDA
// D7 -- RX
// D8 -- TX
#include <SoftwareSerial.h>
#include <EEPROM.h>
#include <Wire.h>
#include "SSD1306.h"
SSD1306 display(0x3c, 0, 4);  // SDA(D3), SCL(D2) Standard I2C 
SoftwareSerial Twin_S(13, 15);   // RX(D7), TX(D8)

int EEPROM_write(int index, String text) 
{
  for (int i = index; i < text.length() + index; ++i) 
  {
    EEPROM.write(i, text[i - index]);  
  }
  EEPROM.write(index + text.length(), 0);
  EEPROM.commit();
  return text.length() + 1;
}

String EEPROM_read(int index, int length) 
{
  String text = "";
  char ch = 1;
  for (int i = index; (i < (index + length)) && ch; ++i)
  {
    if (ch = EEPROM.read(i))
    {
      text.concat(ch);
    }
  }
  return text;
}

String ER="      ";

void setup() 
{
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  display.setColor(WHITE);
  Serial_OLED("Twin-S Start..");
  EEPROM.begin(512);
  Serial.begin(9600);                  
  Twin_S.begin(9600);
  delay(100);
  Serial.println("");
  Serial.println("Twin-S");  
  Twin_S.println("Twin-S");
  EEPROM_write(0,"Twin-S");
  EEPROM_write(6,ER);
}

int input = 0;
String data="";
char inputchar;
int line = 0;
String text1 = "";
String text2 = "";
String text3 = "";

void loop()  //Can't put any delay in this loop() 
{
  Twin_Check();
}

void Serial_OLED(String Text)
{
  if (line==45) 
  {
    display.clear();
    display.drawString(0, 0, text1); 
    display.drawString(0, 15, text2); 
    display.drawString(0, 30, text3); 
    display.drawString(0, 45, Text);
    text1 = text2;
    text2 = text3;
    text3 = Text;
  } else
  {
    display.drawString(0, line, Text);
    line = line + 15;
  }
  display.display();
}

void Twin_Check()
{
  inputchar = Twin_S.read();
  input = int(inputchar);
  if((input!=255)&&(input!=10)&&(input!=0)&&(input!=13)) {
    // Serial_OLED(inputchar + " " +String(input));
    Serial.print(inputchar);
    Serial.print(" ");
    Serial.println(String(input));
    data += inputchar;
  }
  
  if (input==10) {
    ER = EEPROM_read(6,6);
    if (ER!="Twin-S") // Restart
    {
      if (data=="Twin-S" || data=="Twin-X" || data=="Twin-C" || data=="Twin-D")
      {
        Serial_OLED(data);      
      }
      if (data=="Twin-S")
      {
        Twin_S.println("");
        delay(2000);
        Twin_S.println("Twin-X");
        Serial_OLED("pair...");
        Serial.println("pair...");
        EEPROM_write(6,"Twin-S");
      } else
      {
        if (data=="Twin-X")
        {
          Serial_OLED("pair...recheck");
          Serial.println("pair...recheck");
          EEPROM_write(6,"Twin-S");                  
          Twin_S.println("");
          delay(2000);
          Twin_S.println("Twin-C");
        }
      }
    } else // already connected
    {
      if (data=="Twin-S" || data=="Twin-X" || data=="Twin-C" || data=="Twin-D")
      {
        Serial_OLED(data);      
      }
      if (data=="Twin-S")
      {
        Twin_S.println("");
        delay(2000);
        Twin_S.println("Twin-X");
        Serial_OLED("repair...");
        Serial.println("repair...");
      } else
      {
        if (data=="Twin-X")
        {
          Serial_OLED("repair...recheck");
          Serial.println("repair...recheck");
          Twin_S.println("");
          delay(2000);
          Twin_S.println("Twin-C");
        } else
        {
          if (data=="Twin-C") //recheck
          {
            Serial_OLED("C-Connected");
            Serial.println("C-Connected");
            Twin_S.println("");
            delay(2000);
            Twin_S.println("Twin-D");      
          } else 
          {
            if (data=="Twin-D") //final
            {
              Serial_OLED("D-Connected");
              Serial.println("D-Connected");          
            }
          } 
        }
      }
    }
    data = "";
  }
}
