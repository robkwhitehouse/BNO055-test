/*
 * 
 *    This example sketch shows you how to use the library with the getHeadingAuto(address) function, which detects impacts and loads offsets values from the provided eeprom address automatically.
 *    Keep in mind, that you still need to manually save correct offset values into eeprom with saveOffsets(address).
 *
 */

#include <BohleBots_BNO055.h>
#include <Wire.h> //include wire.h for i2c communication
#include <LiquidCrystal_I2C.h>

#define COLUMNS           16
#define ROWS             2

#define LCD_SPACE_SYMBOL 0x20  //space symbol from the lcd ROM, see p.9 of GDM2004D datasheet

LiquidCrystal_I2C lcd(PCF8574_ADDR_A21_A11_A01, 4, 5, 6, 16, 11, 12, 13, 14, POSITIVE);

BNO bno;  //create bno from the Class BNO

void setup()
{  
  Wire.begin(); //Start I2C communication as master
  Wire.setClock(20000);  //Using a long lead - slow the I2C clock down
  Serial.begin(115200); //Begin Serial Communication for debugging
  while(!Serial);	//wait for a serial connection being fully established
  bno.startBNO(200, false); //enables high_g interrupts and puts the compass into fusion output mode NDOF. First parameter controls the threshold for the interrupt (0-255), the second one enables INT pin forwarding.
  while (lcd.begin(COLUMNS, ROWS, LCD_5x8DOTS) != 1) //colums - 20, rows - 4, pixels - 5x8
  {
    Serial.println(F("PCF8574 is not connected or lcd pins declaration is wrong. Only pins numbers: 4,5,6,16,11,12,13,14 are legal."));
    delay(5000);
  }

  Serial.println(F("LCD Display found...")); //(F()) saves string to flash & keeps dynamic memory free
  lcd.clear();
  
  while(!bno.isCalibrated())	//wait until everything is fully calibrated once....
  {
     bno.serialPrintCalibStat();	//print the current calibration levels via serial
     delay(1000);
  }
  Serial.println("Fully Calibrated!");  
  bno.saveOffsets(100);  //....then save the Offsets into the eeprom at the specified address 
}

void loop() 
{
  unsigned heading;

  heading = bno.getHeadingAuto(100);
  Serial.print(F("Heading = ")); //give out the current heading (this function automatically reloads the offset values stored at the eeprom address provided)
  Serial.println(heading, DEC);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Heading = ");
  lcd.setCursor(11,0);
  lcd.print(heading);
  delay(500);
}
