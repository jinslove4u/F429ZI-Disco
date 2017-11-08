/*
   Touch example for STM32F429 Discvery

 Most of the STM32 microcontrollers have more than one serial port.

 The STM32GENERIC Arduino framwork allows you to access this ports.

 The naming is

 Serial       = SerialUSB  this is equal to SerialUSB the virtual serial USB port
 SerialUART1               physical serial interface   
 SerialUART2               physical serial interface
 ...                       ...

 The SerialUARTx are in the data sheets sometimes called USARTx.
 Check the datasheed of your on board microcontroller to which pins this USARTS are connected.
 *  XV11Lidar 이용 레이더 만들기
 *  Mod By JINsMac
 *  2014
 *
 * 20160518 거리내 접근 거리표시
 * 20160527 레이더 구간 접근 확인시 구간별 색칠 표시
 * 20161223 거리값 확대 축소 추가
 * 20161224 확대시 이미지 간섭 제거 완료
 * 20161224 RPM 추가
 * 20161224 Gyro 값 보기 추가
 * 20171010 아두이노 환경으로 포팅 
 * 
 * 
 * 
 * 
uint8_t BSP_GYRO_Init(void);
void    BSP_GYRO_Reset(void);
uint8_t BSP_GYRO_ReadID(void);
void    BSP_GYRO_ITConfig(GYRO_InterruptConfigTypeDef *pIntConfigStruct);
void    BSP_GYRO_EnableIT(uint8_t IntPin);
void    BSP_GYRO_DisableIT(uint8_t IntPin);
void    BSP_GYRO_GetXYZ(float* pfData);

CS   PC1
SCK  PF7
MISO PF8
MOSI PF9
INT1 PA1
INT2 PA2

 * 
*/

#include <Wire.h>
#include "stm32_ub_touch_stmpe811.h"
#include "BSP_f429i_discovery.h"

#include "LTDC_F429_Discovery.h"
//#include "TFT_F429_Discovery.h"

#include "MadgwickAHRS.h"

Madgwick filter;

LTDC_F429_Discovery tft;
TwoWire Wire3(I2C3,PC9,PA8);

#define GREENLED                PG13
#define REDLED                  PG14


const int DesiredRPM=300;  // Setting Desired RPM Here.
const int MotorPWMPin=4;
///#define MotorPWMPin PA4   // pwm출력포트와 핀 추가 

#define mySerialPort Serial          // equal to SerialUSB
//#define mySerialPort SerialUART1 
//#define mySerialPort SerialUART2


 uint8_t GyroID; 
float  GyroDATA[3]; 
 int sampleNo = 1000; 
  int i = 0,x_ax = 0;
  float x_offset = 0;
  float y_offset = 0;
  float z_offset = 0;

  uint32_t drifttimestart;
  uint32_t drifttimeend;
  uint32_t drifttimedelta;
  float x_drift = 0;
  float y_drift = 0;
  float z_drift = 0;

uint32_t currenttime = 0;
  uint32_t lasttime = 0;
  float timediff = 0;
  float xabs = 0;
  float yabs = 0;
  float zabs = 0;

  float  gyroRaw[3];
  float  gyro_cal[3] = {0, 0, 0};
#define  GMult 250.0f / 32768.0f

unsigned long microsPerReading, microsPrevious;
unsigned long microsNow;
/// <----------------porting Start ------------>
#define ABS(x)  ((x) > 0 ? (x) : -(x))
#define CURSOR_SIZE 60
#define DEG2RAD  PI/180
#define RAD2DEG  180/PI

#define BUTTON  PA0  // missile  F429-disco default Button. 

     
     int Old_x, Old_y;
   float x2, y2, ang =0; 



// static void LCD_Config(void);
 static void LCD_AF_GPIOConfig(void);
// static void delay(__IO uint32_t nCount);

 const float RAD = (M_TWOPI / 360.0f);
    uint16_t deg;
       float radians, displayDistance, ratio = 5.0f;
    uint16_t x[360], y[360];
    uint16_t Xposition, Yposition;

     uint8_t AcceX[0];

     uint8_t textSPEED[5];
     uint8_t textBuffer[30];
     uint8_t textRatio[5];
    uint16_t MaxDistance  = 5000 ;   // default = 3800

///      static TP_STATE* TP_State;
/// static void TP_Config(void);


/// <----------------porting End ------------>

         int GCTL1,GCTL2,RCTL1,RCTL2, GON, RON ;

uint8_t UB_I2C3_ReadByte(uint8_t addressI2cDevice, uint8_t registerId)
{
  uint8_t result;
  addressI2cDevice = addressI2cDevice >> 1;

  Wire3.beginTransmission( addressI2cDevice  );
  Wire3.write( registerId );
  uint8_t error;
  error = Wire3.endTransmission();

  Wire3.requestFrom( addressI2cDevice, (uint8_t) 1 , (uint8_t) true );

  while ( Wire3.available() < 1 );

  result = Wire3.read() ;
  return result;
}

int16_t UB_I2C3_WriteByte(uint8_t addressI2cDevice, uint8_t adr, uint8_t wert)
{
  uint16_t error=0;
  
  addressI2cDevice = addressI2cDevice >> 1;

  Wire3.beginTransmission( addressI2cDevice  );
  Wire3.write( adr );
  Wire3.write( wert );
  error = Wire3.endTransmission();

  return error;
}



void LED_work(void)
{
  
    analogWrite(GREENLED, GCTL1);
///    analogWrite(REDLED, RCTL1);

    if(GCTL1 < 254 && GON == 0) GCTL1++ ; else {GCTL1-- ; GON = 1;}    
    if(GCTL1 < 1 && GON == 1 )  GON= 0;  

    if(RCTL1 > 1 && RON == 0) RCTL1-- ; else {RCTL1++ ; RON = 1;}    
    if(RCTL1 > 254 && RON == 1 )  RON= 0;  
}

void GYRO_GetXYZ_dps(float * GyroDATA)
{
  BSP_GYRO_GetXYZ(GyroDATA);
  GyroDATA[0] = GyroDATA[0]*250/0x8000*0.00875;
  GyroDATA[1] = GyroDATA[1]*250/0x8000*0.00875;
  GyroDATA[2] = GyroDATA[2]*250/0x8000*0.00875;
  // 
  // 오차값이 0.05이하일때는 0으로 처리 
  if(abs(GyroDATA[0])<0.01 && abs(GyroDATA[1])<0.01 && abs(GyroDATA[2])<0.01)
  {
    GyroDATA[0] = 0;  //PITCH
    GyroDATA[1] = 0;  //ROLL
    GyroDATA[2] = 0;  //YAW
  }
}

void driftValue(void){

  for(int i = 0;i<sampleNo;i++)
  {
    BSP_GYRO_GetXYZ(GyroDATA);
    x_offset += GyroDATA[0];
    y_offset += GyroDATA[1];
    z_offset += GyroDATA[2];
  }
  x_offset = x_offset/sampleNo;
  y_offset = y_offset/sampleNo;
  z_offset = z_offset/sampleNo;


  for(int i = 0;i<sampleNo;i++)
  {
    if(i==0)
      drifttimestart = micros();
    else if(i==sampleNo-1)
      drifttimeend = micros();
    GYRO_GetXYZ_dps(GyroDATA);
    x_drift += GyroDATA[0]-x_offset;
    y_drift += GyroDATA[1]-y_offset;
    z_drift += GyroDATA[2]-z_offset;
  }
  drifttimedelta = drifttimeend - drifttimestart;
  x_drift = x_drift/drifttimedelta;
  y_drift = y_drift/drifttimedelta;
  z_drift = z_drift/drifttimedelta;
    tft.setCursor(0, 270);
    tft.print(x_drift); tft.print(" , "); tft.print(y_drift); tft.print(" , "); tft.print(z_drift);
}



void Calibration(void)
{
  int cal_int = 0;
  uint8_t axis = 0;

  for (cal_int = 0; cal_int < 2000; cal_int ++){
//      accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);    
      GYRO_GetXYZ_dps(GyroDATA);
      gyro_cal[0] += GyroDATA[0];
      gyro_cal[1] += GyroDATA[1];
      gyro_cal[2] += GyroDATA[2];
  }
  for(int axis=0; axis<3; axis++)
  {
    gyro_cal[axis] /= 2000.0f;
  }
}

void GYRO_Common(void)
{
      GYRO_GetXYZ_dps(GyroDATA);
//    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);    
    gyroRaw[0] = (GyroDATA[0] - gyro_cal[0]);
    gyroRaw[1] = (GyroDATA[1] - gyro_cal[1]);
    gyroRaw[2] = (GyroDATA[2] - gyro_cal[2]); 
}

void setup()
{
    delay(1000);
    pinMode(GREENLED, OUTPUT);
    pinMode(REDLED, OUTPUT);
    pinMode(BUTTON, INPUT);
  
    Serial1.begin(115200);  // serial 1  PA9/PA10
  
    Wire3.begin();  // touch Pannel 
    tft.begin();
    tft.fillScreen(LTDC_BLACK);
  //tft.setRotation(0);
    tft.setCursor(0, 260);
    tft.println("Arduino F429i-Disco LiDAR by JINsMac");
    tft.setCursor(0, 240);
    if (BSP_GYRO_Init() == 0 ) tft.println ("Gyro OK"); else tft.println("Gyro fail "); 
    GyroID = BSP_GYRO_ReadID(); 
    if (GyroID == I_AM_L3GD20) tft.println("I have a Gyro L3GD20");
    else tft.println("I have no Gyro");
///    BSP_GYRO_GetXYZ(GyroDATA);

    Calibration();  // 자이로 초기값 축적
    driftValue();
    tft.setCursor(0, 270);
//  tft.print(x_offset); tft.print(" , "); tft.print(y_offset); tft.print(" , "); tft.print(z_offset);
    filter.begin(65);
    
    if(UB_Touch_Init()==ERROR)
    {
      tft.println("touch interface error");
      tft.println("stopped!");
      while(1);
    }
    GCTL1 =0, RCTL1 = 255,  GON =0, RON = 0;
}

void loop()
{
///    UB_Touch_Read();  
    int x = Touch_Data.xp;
    int y = Touch_Data.yp;
    int touchState=Touch_Data.status;

//    tft.setTextColor(LTDC_WHITE);
//    tft.setCursor(0, 10);
//    tft.print("-initiation-");
    
    microsNow = micros();
    if (microsNow - microsPrevious >= microsPerReading) {
    GYRO_Common();
    tft.fillRect(0,280,200,20,LTDC_BLACK);
    tft.setTextColor(LTDC_WHITE);
    tft.setCursor(0, 280);
    tft.print(GyroDATA[0]); tft.print(" , "); tft.print(GyroDATA[1]); tft.print(" , "); tft.print(GyroDATA[2]);
    tft.drawPixel(x_ax,GyroDATA[0]+80,LTDC_RED);
    tft.drawPixel(x_ax,GyroDATA[1]+80,LTDC_BLUE);
    tft.drawPixel(x_ax,GyroDATA[2]+80,LTDC_YELLOW);
    tft.drawLine(0,80,240,80,LTDC_WHITE);

    filter.updateGyro((double)gyroRaw[0], (double)gyroRaw[1], (double)gyroRaw[2]);
//    roll    = filter.getRoll();
//    pitch   = filter.getPitch();
//    heading = filter.getYaw();      
/*
    currenttime = micros();
    timediff = currenttime - lasttime;
    lasttime = currenttime;
    GYRO_GetXYZ_dps(GyroDATA);
    tft.setCursor(0, 290);
    tft.print(GyroDATA[0]-x_offset); tft.print(" , "); tft.print(GyroDATA[1]-y_offset); tft.print(" , "); tft.print(GyroDATA[2]-z_offset);
    // 5.8 experimentell bestimmt
    xabs += (GyroDATA[0]-(x_drift*timediff))*timediff/1000/5.8*90;
    yabs += (GyroDATA[1]-(y_drift*timediff))*timediff/1000/5.8*90;
    zabs += (GyroDATA[2]-(z_drift*timediff))*timediff/1000/5.8*90;
    tft.setCursor(0, 300);
    tft.print(xabs); tft.print(" , "); tft.print(yabs); tft.print(" , "); tft.print(zabs);
*/    
    microsPrevious = microsPrevious + microsPerReading;
    tft.setCursor(0, 290);
    tft.print(filter.getPitch()); tft.print(" , "); tft.print(filter.getRoll()); tft.print(" , "); tft.print(filter.getYaw());    
    tft.drawPixel(x_ax,filter.getPitch()+150,LTDC_BLUE);
    tft.drawPixel(x_ax,filter.getRoll()+150,LTDC_RED);
    tft.drawPixel(x_ax,filter.getYaw()-30,LTDC_YELLOW);
    tft.drawLine(0,150,240,150,LTDC_WHITE);    
  
    x_ax++; 

    }
    if (x_ax >= 239) x_ax = 0; 
    if ( touchState ==TOUCH_PRESSED && x < 240 )    
    {
      if(Old_x != x || Old_y != y) // reduce flickering
      {
        if(x < 240) { // 240인 경우 비정상 터치좌표 
          Old_x = x;
          Old_y = y;
        }
        tft.setTextColor(LTDC_WHITE);
        tft.setCursor(180, 10);
        tft.print(x); tft.print(" , "); tft.print(y);
      }
    }
    
    if(digitalRead(BUTTON))  // scale button
    {
    
    }
    LED_work();    

}

void serialEvent() {
  while (Serial1.available()) {
    unsigned char inChar = (unsigned char)Serial1.read();
  }
}

