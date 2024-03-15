#include "Functions.h"

void idle_lcd(LiquidCrystal_I2C lcd){
  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print("Door Locked!");
  lcd.setCursor(1,1);
  lcd.print("Scan Your Card");
}

void denied_lcd(LiquidCrystal_I2C lcd){
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("Access  Denied");
  lcd.setCursor(3,1);
  lcd.print("Wrong Card");
}

void granted_lcd(LiquidCrystal_I2C lcd){
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("Access Granted");
  lcd.setCursor(1,1);
  lcd.print("Open The Door!");
}

void closing_lcd(LiquidCrystal_I2C lcd){
  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print("Closing Door!");
  lcd.setCursor(2,1);
  lcd.print("Please Wait!");
}
