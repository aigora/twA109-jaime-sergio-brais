/*
 * LOW es BAJO, y en el caso de los le3d lo usamos para mandar a GND la corriente y que se encienda
 * HIGH es ALTO, 
 */

#include <LiquidCrystal.h> 
#include <SPI.h>
#include <MFRC522.h>

 
#define ENABLE 9
#define DIRA 10
#define DIRB 8
 
  // Ejemplo de clave valida


//configuracion al crear el objeto lcd
//1º parametro = RS
//2º parametro = E
//3º parametro = D4
//4º parametro = D5
//5º parametro = D6
//6º parametro = D7

LiquidCrystal lcd(23,25,27,29,31,33); 

//VARIABLES GENERICAS
const int RST_PIN = 5;        // Pin 9 para el reset del RC522
const int SS_PIN = 53;   // Pin 10 para el SS (SDA) del RC522
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Crear instancia del MFRC522
byte validKey1[4] = { 0xE0, 0x4D, 0xD0, 0x57 };
bool alarmaStandby=true;
bool esperandoClave=false;
int pinZumbador=22;
int pinLedApagado=34;
int pinLedActivoSensor=30;
int pinLedActivoPulsador=38;
int pinPulsadorEncendido=42;
int pinLlama=0;
//int pinMotor=50;
//int pinPulsadorApagado=26;


//Esta funcion emite pitidos tantas veces como deseemos y con una duracion
//determinada
void emitirPitidos(int numVeces, int duracion){
  for(int cont=0; cont<numVeces; cont++){
    digitalWrite(pinZumbador, HIGH);   
    delay(duracion);              
    digitalWrite(pinZumbador, LOW);    
    delay(duracion);              
  }
}
bool isEqualArray(byte* arrayA, byte* arrayB, int length)
{
  for (int index = 0; index < length; index++)
  {
    if (arrayA[index] != arrayB[index]) return false;
  }
  return true;
}


//Configuramos los pines
//Inicializamos los estados de los pines
//configuramos pantalla
void setup() { 
  Serial.begin(9600);
  SPI.begin();        // Iniciar SPI
  mfrc522.PCD_Init(); // Iniciar MFRC522
  pinMode(pinLedApagado, OUTPUT);
  pinMode(pinLedActivoSensor, OUTPUT);
  pinMode(pinLedActivoPulsador, OUTPUT);
  //pinMode(pinLlama, INPUT);
  pinMode(pinPulsadorEncendido, INPUT);
//  pinMode(pinPulsadorApagado, INPUT);
  pinMode(pinZumbador, OUTPUT);
  pinMode(ENABLE,OUTPUT);
  pinMode(DIRA,OUTPUT);
  pinMode(DIRB,OUTPUT);
//  pinMode(pinMotor, OUTPUT);


  digitalWrite(pinLedApagado, HIGH);    
  digitalWrite(pinLedActivoSensor, LOW);    
  digitalWrite(pinLedActivoPulsador, LOW);



  lcd.begin(16,2); // Inicializamos la interfaz del LCD y especificamos las dimensiones de la pantalla 

  emitirPitidos(3, 150);
  
  lcd.print("Alarma apagada"); // Imprime inicializando en el LCD
  lcd.setCursor(0,1);


}

void loop() { 
  int valida1=0;
  int valida2=0;
  int i;
  pinLlama = analogRead(A0);
  Serial.println(pinLlama);
  //SI LA ALARMA ESTA "APAGADA"

  if(alarmaStandby){
    digitalWrite(pinLedApagado, HIGH);    
    digitalWrite(pinLedActivoSensor, LOW);    
    digitalWrite(pinLedActivoPulsador, LOW);    
  }

  
  if(digitalRead(pinPulsadorEncendido)==HIGH)  //PARA ACTIVAR LA ALRAMA
  {
    while(valida1!=1)
    {
    lcd.clear(); 
    lcd.setCursor(0,0); 
    lcd.print("Alarma activada"); 
    lcd.setCursor(0,1); 
    lcd.print("manualmente");
    digitalWrite(ENABLE,HIGH); // enable on
    for (i=0;i<5;i++) {
    digitalWrite(DIRA,HIGH); //one way
    digitalWrite(DIRB,LOW);
    }

 // Detectar tarjeta
  if (mfrc522.PICC_IsNewCardPresent())
  {
    //Seleccionamos una tarjeta
    if (mfrc522.PICC_ReadCardSerial())
    {
      // Comparar ID con las claves válidas
      if (isEqualArray(mfrc522.uid.uidByte, validKey1, 4))
        valida1=1;
 
      // Finalizar lectura actual
      mfrc522.PICC_HaltA();
    }
  }
  delay(250);

    
    digitalWrite(pinLedActivoSensor, HIGH);
    digitalWrite(pinLedApagado, LOW);
    digitalWrite(pinLedActivoPulsador, LOW);
  //  digitalWrite(pinMotor, LOW);
    emitirPitidos(1,200);
    } 
        lcd.clear(); 
    lcd.setCursor(0,0); 
    lcd.print("Alarma apagada"); 
    lcd.setCursor(0,1); 
    lcd.print(" "); 
    digitalWrite(ENABLE,LOW);
    digitalWrite(pinLedActivoSensor, LOW);
    digitalWrite(pinLedApagado, HIGH);
    digitalWrite(pinLedActivoPulsador, LOW);
    //  digitalWrite(pinMotor, HIGH);

  }
    if(pinLlama < 500) //PARA ACTIVAR ALARMA
  {
    while(valida2!=1){
          lcd.clear(); 
    lcd.setCursor(0,0); 
    lcd.print("Alarma activada"); 
    lcd.setCursor(0,1); 
    lcd.print("por sensor");
        digitalWrite(ENABLE,HIGH); // enable on
    for (i=0;i<5;i++) {
    digitalWrite(DIRA,HIGH); //one way
    digitalWrite(DIRB,LOW);
    }
     // Detectar tarjeta
  if (mfrc522.PICC_IsNewCardPresent())
  {
    //Seleccionamos una tarjeta
    if (mfrc522.PICC_ReadCardSerial())
    {
      // Comparar ID con las claves válidas
      if (isEqualArray(mfrc522.uid.uidByte, validKey1, 4))
        valida2=1;
 
      // Finalizar lectura actual
      mfrc522.PICC_HaltA();
    }
  }
  delay(250);
    digitalWrite(pinLedActivoSensor, LOW);
    digitalWrite(pinLedApagado, LOW);
    digitalWrite(pinLedActivoPulsador, HIGH);
    emitirPitidos(1,200);

    }
    lcd.clear(); 
    lcd.setCursor(0,0); 
    lcd.print("Alarma apagada"); 
    lcd.setCursor(0,1); 
    lcd.print(" ");
       digitalWrite(ENABLE,LOW); 
    digitalWrite(pinLedActivoSensor, LOW);
    digitalWrite(pinLedApagado, HIGH);
    digitalWrite(pinLedActivoPulsador, LOW);

  }
  }
