#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include "RFID_Keys.h"
#include "Functions.h"

#define RST_PIN         9          
#define SS_PIN          10
#define greenPin        2
#define redPin          3
#define servoPin        4
#define buzzerPin       5        

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
Servo myservo;
RFID_Keys accessUID_keys;         // Am creat o lista UID_keys goala
int nr_keys;
int i = 0;
int index_key = 0;
byte values[4] = {0, 0, 0, 0};
int read_first_byte = 1;


void setup() {
  lcd.begin(16,2);
  idle_lcd(lcd);
  myservo.attach(servoPin);
  pinMode(greenPin,OUTPUT);
  pinMode(redPin,OUTPUT);
  pinMode(buzzerPin, OUTPUT);
	Serial.begin(9600);		// Initialize serial communications with the PC
	while (!Serial);		// Do nothing if no serial port is opened
	SPI.begin();			
	mfrc522.PCD_Init();		// Init MFRC522
	delay(10);				// Optional delay. Some boards need more time after init to be ready
}

void loop() {


  while(Serial.available() > 0){        //Verificam seriala daca exista bytes

    if(read_first_byte == 1){           //Citim primul byte pentru a afla numrul de UIDs din fisierul UID_list.txt care il primim prin seriala
      nr_keys = Serial.read();
      nr_keys = int((char(nr_keys) - '0'));
      read_first_byte = 0;
      Serial.print("Nr keys: ");
      Serial.print(nr_keys, "\n");
      accessUID_keys.allocate_memory(nr_keys);    //Alocam memorie pentru lista de UID_chei (adica o matrice)
    }
    
    byte bytes = Serial.read();

    if(bytes != 255){                   //Avem o erorare cand primim byte urile prin seriala din scriptul python, deoarece primul byte primit este 255
                                        //Iar apoi primim byte urile corecte pentru cheile respective
      values[i] = bytes;                //Salvam byte-ul in vectorul values care este un vector de byte
      i++;
    }

    if(i == 4){                         //Daca avem 4 byte uri in vector value inseamna ca am gasit o cheie UID
      i = 0;                            //Initializam vectorul value
      accessUID_keys.add_key(values,index_key);     //Adaugam cheia la lista_UID(matrice)
      for(int j = 0; j<4; j++){
        values[j] = 0;                  //Initializam vectorul value
      }
      index_key++;                      //Incrementam variabila index_key pentru a stii numarul cheii citite
    }
    
  }

  
	if ( ! mfrc522.PICC_IsNewCardPresent()) {       //Resetam functia loop daca nu exista un tag/card pe senzor/reader
		return;
	}

	if ( ! mfrc522.PICC_ReadCardSerial()) {          //Verificam daca tag-ul/card-ul a fost citit cu success si daca nu resetam functia loop
		return;
	}


	if(accessUID_keys.check_key(mfrc522.uid.uidByte)){    //Verificam daca tag-ul/card-ul citit este prezent in lista de UID_chei
    digitalWrite(greenPin,HIGH);        //Aprindem led-ul verde
    myservo.write(110);                 //Deschidem Usa cu ajutorul servomotorului
    granted_lcd(lcd);
    for(int i = 0; i < 2; i++){         //Emitem un semnal sonor dublu 
      tone(buzzerPin, 2000);
      delay(150);
      noTone(buzzerPin);
      delay(150);
    }
    delay(850);                         //Delay pentru a deschide usa suficient de mult
    myservo.write(90);                  //Oprirea servomotorului
    delay(2000);                        //Asteptam 2 secunde pentru a permite intrarea
    digitalWrite(greenPin,LOW);         //Stingem led-ul verde
    closing_lcd(lcd);                   //Afisam pe lcd ca usa se inchide
    myservo.write(70);                  //Inchidem Usa
    delay(1450);                        //Delay pentru a permite usa sa se inchide, deorece avem de a face cu un servo motor continual
                                        //un servomotor positional ar fi mai ideal
    myservo.write(90);                  //Oprirea servomotorului
    
	}else{                                //Daca tag-ul/card-ul citi nu exista in lista de UID_chei atunci
    digitalWrite(redPin,HIGH);          //Aprindem led-ul rosu
    denied_lcd(lcd);                    //Afisam pe lcd ca cipul/cardul respectiv este invalid
    tone(buzzerPin, 500);               //Emitem un semnal sonor mai jos si prelungit
    delay(1500);                        //Asteptam 1.5 secunde
    noTone(buzzerPin);                  //Oprim semnalul sonor
    digitalWrite(redPin,LOW);           //Stingem led-ul rosu
	}
 
	mfrc522.PICC_HaltA();                 //Resetam rfid reader-ul pentru a putea citi urmatorul  tag/card
  idle_lcd(lcd);                        //Afisam pe lcd textul standar
}
