//Project by Romina Marsico, Isabel Gaubatz and Simone Haas
//Arduino sketch for the mini motor driver (using the keys w, a, s, d and p)

#define baudRate 9600  //use the baud rate your bluetooth module is configured to

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

unsigned long HEARTBEAT_INTERVAL = 2000;
unsigned long nextHeartbeatAt = 0;

void setup()
{ 
  Serial.begin(baudRate);  
  
  pinMode(leftDirectionPin, OUTPUT);
  pinMode(rightDirectionPin, OUTPUT);
  pinMode(leftSpeedPin, OUTPUT);
  pinMode(rightSpeedPin, OUTPUT);
} 

void loop()
{ 
  heartbeat();
  if (Serial.available() > 0)  //checks if data has been sent from the computer
  {
    data_packet[packet_index] = Serial.read();  //reads the most recent byte
    Serial.print("Key: ");
    Serial.println(data_packet[packet_index]);  //prints the keys which are pressed
    
    packet_index++;
    if(packet_index==1)  //full packet received
    {
      process_packet();
    }  
  }
}

void process_packet()
{
  packet_index=0;
  
  motorSpeedAbsolut = abs(motorSpeed);
  leftMotorSpeedAbsolut = abs(leftMotorSpeed);
  rightMotorSpeedAbsolut = abs(rightMotorSpeed);  
  
  if((data_packet[0]==119) || (data_packet[0]==115))  //119= w; 115=s; was w or s pressed? (for forward or backwards)
  {
    if(data_packet[0]==115)  //was s pressed?
    {
      motorSpeed = 80;
      motorSpeed = constrain(motorSpeed, -240, 240);
         
      if(motorSpeed >0)  //forward
      {
        digitalWrite(leftDirectionPin, HIGH);
        digitalWrite(rightDirectionPin, HIGH);
      }
      else  //backwards
      {
        digitalWrite(leftDirectionPin, LOW);
        digitalWrite(rightDirectionPin, LOW);
      }     
      analogWrite(leftSpeedPin, motorSpeedAbsolut);
      analogWrite(rightSpeedPin, motorSpeedAbsolut); 
    }
    
    else  //w was pressed
    {
      motorSpeed = -80;
      motorSpeed = constrain(motorSpeed, -240, 240);

      if(motorSpeed >0)  //forward
      {
        digitalWrite(leftDirectionPin, HIGH);
        digitalWrite(rightDirectionPin, HIGH);
      }
      else  //backwards
      {
        digitalWrite(leftDirectionPin, LOW);
        digitalWrite(rightDirectionPin, LOW);
      }
      analogWrite(leftSpeedPin, motorSpeedAbsolut);
      analogWrite(rightSpeedPin, motorSpeedAbsolut);     
    }  

    Serial.print("motorSpeed: ");
    Serial.print(motorSpeed);
    Serial.println();
  }

  if((data_packet[0]==97) || (data_packet[0]==100))  //97=a; 100=d; was a oder d pressed? (for left or right)
  {
    if(data_packet[0]==97)  //was a pressed?
    {
      leftMotorSpeed = constrain(leftMotorSpeed, -240, 240);
    
      if(motorSpeed >= 0)  //forward
      {
        leftMotorSpeed = 50;
        rightMotorSpeed = 80;
        
        analogWrite(leftSpeedPin, leftMotorSpeedAbsolut);
        analogWrite(rightSpeedPin, rightMotorSpeedAbsolut);
      }
      else  //backwards
      {
        leftMotorSpeed = -50;
        rightMotorSpeed = -80;
        
        analogWrite(leftSpeedPin, leftMotorSpeedAbsolut);
        analogWrite(rightSpeedPin, rightMotorSpeedAbsolut);
      }
      
      Serial.print("Speed left: ");
      Serial.print(leftMotorSpeed);
      Serial.println();
      
      Serial.print("Speed right: ");
      Serial.print(rightMotorSpeed);   
      Serial.println();      
    }

    else  //d was pressed
    {
      rightMotorSpeed = constrain(rightMotorSpeed, -240, 240);

      if(motorSpeed >= 0)  //forward
      {
        rightMotorSpeed = 50;
        leftMotorSpeed = 80;
        
        analogWrite(rightSpeedPin, rightMotorSpeedAbsolut);
        analogWrite(leftSpeedPin, leftMotorSpeedAbsolut);
      }
      else  //backwards
      {
        rightMotorSpeed = -50;
        leftMotorSpeed = -80;
        
        analogWrite(rightSpeedPin, rightMotorSpeedAbsolut);
        analogWrite(leftSpeedPin, leftMotorSpeedAbsolut);
      } 
  
      Serial.print("Speed left: ");
      Serial.print(leftMotorSpeed);   
      Serial.println(); 
      
      Serial.print("Speed right: ");
      Serial.print(rightMotorSpeed);   
      Serial.println();      
    } 
  }


  if(data_packet[0]==112) //112=p; was p pressed? (to pause)
  {
    motorSpeed = 0;
    
    analogWrite(leftSpeedPin, motorSpeedAbsolut);
    analogWrite(rightSpeedPin, motorSpeedAbsolut);
  }
}

void heartbeat() {
  unsigned long now = millis();
  if (now > nextHeartbeatAt) {
    digitalWrite(13, HIGH);
    Serial.print("# Bot connected. Uptime: ");
    Serial.println(now);
    nextHeartbeatAt = now + HEARTBEAT_INTERVAL;
    delay(2);
    digitalWrite(13, LOW);
  }
}

/*
    w  =  119
    a  =  97
    s  =  115
    d  =  100
    p  =  112
*/
