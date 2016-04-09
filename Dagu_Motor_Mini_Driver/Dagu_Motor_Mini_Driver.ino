//Arduino sketch for the mini motor driver (using the keys w, a, s, d and p)

#define baudRate 115200

//Setup pins for SN754410 Motor chip
int leftDirectionPin = 7;    //Left motor Direction
int leftSpeedPin = 9;        //Left motor Speed
int rightDirectionPin = 8;   //Right motor Direction
int rightSpeedPin = 10;      //Right motor Speed

int motorSpeed = 0;
int motorSpeedAbsolut = 0;

int leftMotorSpeed = 0;
int leftMotorSpeedAbsolut = 0;

int rightMotorSpeed = 0;
int rightMotorSpeedAbsolut = 0;

unsigned char data_packet[1];
unsigned int packet_index=0;

void setup()
{ 
  Serial.begin(baudRate);  //use the baud rate your bluetooth module is configured to
  
  pinMode(leftDirectionPin, OUTPUT);
  pinMode(rightDirectionPin, OUTPUT);
  pinMode(leftSpeedPin, OUTPUT);
  pinMode(rightSpeedPin, OUTPUT);
} 

void loop()
{ 
  if (Serial.available() > 0)  //check if data has been sent from the computer
  {
    data_packet[packet_index] = Serial.read();  //read the most recent byte
    Serial.print("Key: ");
    Serial.println(data_packet[packet_index]);  //ohne die Zeile wäre im Terminal nicht sichtbar, welche Tasten ich selbst eingebe
    
    packet_index++;
    if(packet_index==1)  //full packet received
    {
      process_packet();
    }  
  }
}

void process_packet()
{
  //Serial.println(data_packet[0]);  //Ausgabe des einzelnen Bytes
  
  packet_index=0;
  
  motorSpeedAbsolut = abs(motorSpeed);
  leftMotorSpeedAbsolut = abs(leftMotorSpeed);
  rightMotorSpeedAbsolut = abs(rightMotorSpeed);  
  
  if((data_packet[0]==119) || (data_packet[0]==115))  // wurde w oder s gedrückt?
  {
    if(data_packet[0]==119)  //wurde w gedrückt?
    {
      //motorSpeed=motorSpeed+20;
      motorSpeed = 80;
      motorSpeed = constrain(motorSpeed, -240, 240);
         
      if(motorSpeed >0)  //vorwärts
      {
        digitalWrite(leftDirectionPin, HIGH);
        digitalWrite(rightDirectionPin, HIGH);
      }
      else  //rückwärts
      {
        digitalWrite(leftDirectionPin, LOW);
        digitalWrite(rightDirectionPin, LOW);
      }     
      analogWrite(leftSpeedPin, motorSpeedAbsolut);
      analogWrite(rightSpeedPin, motorSpeedAbsolut); 
    }
    
    else  //es wurde s gedrückt
    {
      //motorSpeed=motorSpeed-20;
      motorSpeed = -80;
      motorSpeed = constrain(motorSpeed, -240, 240);

      if(motorSpeed >0)  //vorwärts
      {
        digitalWrite(leftDirectionPin, HIGH);
        digitalWrite(rightDirectionPin, HIGH);
      }
      else  //rückwärts
      {
        digitalWrite(leftDirectionPin, LOW);
        digitalWrite(rightDirectionPin, LOW);
      }
      analogWrite(leftSpeedPin, motorSpeedAbsolut);
      analogWrite(rightSpeedPin, motorSpeedAbsolut);     
    }  
    
    //leftMotorSpeed = motorSpeed;
    //rightMotorSpeed = motorSpeed;
    
    Serial.print("motorSpeed: ");
    Serial.print(motorSpeed);
    Serial.println();
  }

  if((data_packet[0]==97) || (data_packet[0]==100))  // wurde a oder d gedrückt? (für links oder rechts)
  {
    if(data_packet[0]==97)  //wurde a gedrückt?
    {
      leftMotorSpeed = constrain(leftMotorSpeed, -240, 240);
    
      if(motorSpeed >= 0)  //vorwärts
      {
        //leftMotorSpeed = leftMotorSpeed -20;
        leftMotorSpeed =50;
        rightMotorSpeed = 80;
        
        analogWrite(leftSpeedPin, leftMotorSpeedAbsolut);
        analogWrite(rightSpeedPin, rightMotorSpeedAbsolut);
      }
      else  //rückwärts
      {
        //leftMotorSpeed = leftMotorSpeed +20;
        leftMotorSpeed = -50;
        rightMotorSpeed = -80;
        
        analogWrite(leftSpeedPin, leftMotorSpeedAbsolut);
        analogWrite(rightSpeedPin, rightMotorSpeedAbsolut);
      }
      //analogWrite(leftSpeedPin, motorSpeedAbsolut);
      //analogWrite(rightSpeedPin, motorSpeedAbsolut);
      
      Serial.print("Speed left: ");
      Serial.print(leftMotorSpeed);
      Serial.println();
      
      Serial.print("Speed right: ");
      Serial.print(rightMotorSpeed);   
      Serial.println();      
    }

    else  //es wurde d gedrückt
    {
      rightMotorSpeed = constrain(rightMotorSpeed, -240, 240);

      if(motorSpeed >= 0)  //vorwärts
      {
        //rightMotorSpeed = rightMotorSpeed -20;
        rightMotorSpeed = 50;
        leftMotorSpeed = 80;
        
        analogWrite(rightSpeedPin, rightMotorSpeedAbsolut);
        analogWrite(leftSpeedPin, leftMotorSpeedAbsolut);
      }
      else  //rückwärts
      {
        //rightMotorSpeed = rightMotorSpeed +20;
        rightMotorSpeed = -50;
        leftMotorSpeed = -80;
        
        analogWrite(rightSpeedPin, rightMotorSpeedAbsolut);
        analogWrite(leftSpeedPin, leftMotorSpeedAbsolut);
      }
      //analogWrite(leftSpeedPin, motorSpeedAbsolut);
      //analogWrite(rightSpeedPin, motorSpeedAbsolut);  
  
      Serial.print("Speed left: ");
      Serial.print(leftMotorSpeed);   
      Serial.println(); 
      
      Serial.print("Speed right: ");
      Serial.print(rightMotorSpeed);   
      Serial.println();      
    } 
  }


  if(data_packet[0]==112) // wurde p für PAUSE gedrückt?
  {
    motorSpeed = 0;
    
    analogWrite(leftSpeedPin, motorSpeedAbsolut);
    analogWrite(rightSpeedPin, motorSpeedAbsolut);
  }
}

/*
    w  =  119
    a  =  97
    s  =  115
    d  =  100
    p  =  112
*/
