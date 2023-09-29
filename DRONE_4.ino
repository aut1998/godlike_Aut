// Basic demo for accelerometer readings from Adafruit MPU6050

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;

#define MOTOR_1 6
#define MOTOR_2 5

float Kp = 0.35 ; 
float Ki = 0.15  ;
float Kd = 0.7   ;

int P = 0 ;
int D = 0 ;
long I ;

float slope[] = { 99,99,99,99,99,99,99,99,99,99,99,99,99,99,99 } ;

void setup(void) {
  Serial.begin(115200);
  
  pinMode( MOTOR_1 ,OUTPUT);
  pinMode( MOTOR_2 ,OUTPUT);
  
  Serial.println("Adafruit MPU6050 test!");

  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_1000_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_94_HZ);
  

  Serial.println("");
  delay(100);
}

void loop() {
  
//  int pwm = 10 ;
   int pwm = map(analogRead(A0),0,1023,25,40);
  int y = 0 ;
  for(int i = 0 ; i <= 20 ; i++ ) 
  {
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

  // sensors_event_t a, g, temp;
  // mpu.getEvent(&a, &g, &temp);

    y += a.acceleration.y*15 ;
  }

  y = y/20 ;

  int out  = PiD( y , 0 );
  if ( out >= 50 ) out = 65 ;
  if( out <= -50  ) out = -65;


  int PWM_1 = 0  ;
  int PWM_2 = 0  ;  

  if ( out > 0 )
  {
    PWM_1 = out+pwm ;
    PWM_2 = pwm ;

  }
  else
  {
    PWM_1 = pwm ;
    PWM_2 = abs(out)+pwm ;
  }

  if( PWM_1 <= 0 ) PWM_1 = 0 ;
  
  if( pwm <= 25 || abs(y) > 100 ) 
  {
    I = 0 ; 
    D = 0 ;
    P = 0 ;
    PWM_1 = 0 ;
    PWM_2 = 0 ;
  }
  if( PWM_2 <= 0 ) PWM_2 = 0 ;

  if( PWM_1 >= 70 ) PWM_1 = 70 ; 
  if( PWM_2 >= 70 ) PWM_2 = 70 ; 

  
  analogWrite( MOTOR_1 , PWM_1 );
  analogWrite( MOTOR_2 , PWM_2 );
  
  Serial.print(P);
  Serial.print("   ");
  Serial.print(I);
  Serial.print("   ");
  Serial.print(D);
  Serial.print("   ");
  Serial.print(PWM_1);
  Serial.print("   ");
  Serial.print(PWM_2);
  Serial.print("  ");
  if(out >= 0 ) Serial.print("+");
  Serial.print(out);


  Serial.print("   ");
 Serial.print(pwm);
  Serial.print("   ");
  Serial.println(y);
  delay(10);

}

int PiD( float IN_PUT_1 , int SET_point  )
  {
    // https://automation360blog.wordpress.com/2018/01/01/pid-by-graph/
    
    P = SET_point - IN_PUT_1 ;
    I += P  ; 
    
    int d   =  SLOPE_D( IN_PUT_1 , 2 ); 
    
    D = IN_PUT_1 ;

//    if ( D <= 0.5 && D >= -0.5 ) 
//      {
//        READY = 1 ;
//      }
    
    float RE = ( Kp * P ) + ( Ki * I ) + ( Kd * d ) ;
    return RE ;
  }


float SLOPE_D( float i , int j )
  {
    slope[10]= slope[9] ;
    slope[9] = slope[8] ;
    slope[8] = slope[7] ;
    slope[7] = slope[6] ;
    slope[6] = slope[5] ;
    slope[5] = slope[4] ;
    slope[4] = slope[3] ;
    slope[3] = slope[2] ;
    slope[2] = slope[1] ;
    slope[1] = slope[0] ;
    slope[0] = i ;

    
    if ( slope[j] == 99 )
      {
        return 0 ;
      }
    else
      {
        return  ( slope[j] - i )/j ; 
      }
  }
