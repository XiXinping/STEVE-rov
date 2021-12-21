#include <Adafruit_GFX.h>                          
#include "Adafruit_LEDBackpack.h"                  
                                               
Adafruit_7segment matrix = Adafruit_7segment();    
                                               
int cnt;                                           
                                               
String inString = "";                              
                                               
void setup() {                                     
	Serial.begin(9600);                            
	matrix.begin(0x70);                            
}                                                  
                                               
void loop() {                                      
	if (Serial.available() > 0) {                  
	    inString = Serial.readStringUntil('\n');   
                                               
	    int x = inString.toInt();                  
                                               
	    matrix.print(x, DEC);                      
	    matrix.writeDisplay();                     
	}                                              
                                               
}                                                  

