#define ENCODER_OPTIMIZE_INTERRUPTS


#include <Encoder.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>


void tela();
void directMotor();
void cmdMotor();



const int  buttonPin = 12;

unsigned long previousMillis = 0;


int ABRE_PWM=5;
int FECHA_PWM=6;

int DIR_EN=7;
int ESQ_EN=8;

int buttonState = false ;         // Estado atual do botão 
int lastButtonState = false ;     // Estado anterior do botão 

long oldPosition  = 0;
long newPosition =  0;


int potValue= 0;          //leitura do potenciometro
int ti = 0;               //tempo de inspiração
int ieRatio = 0;               //tempo de inspiração
int vol = 0;
int tPIP =0;

LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7,3, POSITIVE);
Encoder myEnc(3, 2);

void setup() {


    for(int i=5; i<9; i++){
        pinMode(i,OUTPUT);
    }

  pinMode(buttonPin,INPUT);
  
  digitalWrite(DIR_EN,HIGH);
  digitalWrite(ESQ_EN,HIGH);

  Serial.begin(9600);
  lcd.begin (16,2);
  tela(0);

 }


void loop() {

   
}

void cmdMotor(bool sentido, int velocidade){ //Sentido 0 = Fechando Sentido 1 = Fechando

    if (sentido == 0 ){
        analogWrite(ABRE_PWM, 0);
        analogWrite(FECHA_PWM, velocidade);
    }

    else if (sentido == 1){
        analogWrite(ABRE_PWM, velocidade);
        analogWrite(FECHA_PWM, 0);
    }
    else {
        analogWrite(ABRE_PWM, 0);
        analogWrite(FECHA_PWM, 0);
    }

}

int enterButton(){

  int enter = 0;
  
  buttonState = digitalRead(buttonPin) ; 
  if (buttonState != lastButtonState) {  //se o estado mudou, incrementar o contador  
    
    if (buttonState==HIGH) {  //se o estado atual é alta, então o botão muda de desligado para ligado: 
    enter = 1;    
    } 
    else{ //se o estado atual é baixa, então o botão, varia de ligado para desligado:
    enter = 0;   
    } 
  }
  lastButtonState = buttonState; //salvar o estado atual como o último estado,  
  return enter; 
}


void directMotor(){

    while (myEnc.read() < vol) {
        cmdMotor(0,255);
    }

    cmdMotor(0,0);
    delay(1000);
    Serial.print("Graus maximo fechando: ");
    Serial.println(myEnc.read());


    while (myEnc.read() > 40) {
        cmdMotor(1,64);    
    }

    cmdMotor(0,0);
    delay(1000);
    Serial.print("Graus maximo abrindo: ");
    Serial.println(myEnc.read());


}
  



void tela(char p)
{
  switch(p){

    case 0:

        
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("INICIANDO CONFIG");
        lcd.setCursor(0,1);
        lcd.print("RESPIRADOR");

        cmdMotor(1,64); 
        delay(3000);
        cmdMotor(0,0);
        delay(500);
        myEnc.write(0);
         
        while (myEnc.read() < 50) {
            cmdMotor(0,64);





        /*     
            newPosition = myEnc.read();
            if (newPosition != oldPosition) {
                oldPosition = newPosition;
                Serial.println(newPosition);
            }
        */    
        }

        cmdMotor(0,0);
        delay(1000);
        

        ti = 0;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("CONFIGURACAO");
        lcd.setCursor(0,1);
        lcd.print("Ti:     ms");

        while(enterButton() == false){
        potValue = analogRead(A0);
        ti = map(potValue,0, 1023,0,100); 
        lcd.setCursor(4,1);
        lcd.print(ti*100);
        }

      ti = ti*100;
      tela(1);
      break;

    case 1:

      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("CONFIGURACAO");
      lcd.setCursor(0,1);
      lcd.print("ieRatio:   ");

      while(enterButton() == false){
      potValue = analogRead(A0);
      ieRatio = map(potValue,0, 1010,1,6);

      if(ieRatio == 1){
        lcd.setCursor(9,1);
        lcd.print("1/1");
        }

      if(ieRatio == 2){
        lcd.setCursor(9,1);
        lcd.print("1/2");
        }

      if(ieRatio == 3){
        lcd.setCursor(9,1);
        lcd.print("1/3");
        }

      if(ieRatio == 4){
        lcd.setCursor(9,1);
        lcd.print("1/4");
        }

      if(ieRatio == 5){
        lcd.setCursor(9,1);
        lcd.print("1/5");
        }

      if(ieRatio == 6){
        lcd.setCursor(9,1);
        lcd.print("1/6");
        }
        
      }

      
      tela(2);              
      break;
    
    case 2:

      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("CONFIGURACAO");
      lcd.setCursor(0,1);
      lcd.print("Volume:   %");

      while(enterButton() == false){
      potValue = analogRead(A0);
      vol = map(potValue,0, 1023,30,290); 
      
      lcd.setCursor(8,1);
      lcd.print(vol);
      }
      Serial.println(vol);

      tela(3);
      break;

    case 3:

      tPIP =0;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("CONFIGURACAO");
      lcd.setCursor(0,1);
      lcd.print("tPIP:     ms");

      while(enterButton() == false){
      potValue = analogRead(A0);
      tPIP = map(potValue,0, 1023,0,100); 

      lcd.setCursor(6,1);
      lcd.print(tPIP*10);
      }

      tPIP = tPIP*10;
      
      tela(4);
      break;


    case 4:

      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Pressione p/ ");
      lcd.setCursor(0,1);
      lcd.print("Iniciar");    

      
      while(enterButton() == false){
      
      }

      tela(5);
      break;


    case 5:
      lcd.clear();
      
      while(enterButton() == false){
      directMotor();
      }

      tela(0);
      break;

      
    }
}
