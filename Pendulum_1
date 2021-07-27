/*  27 07 2021 

SONIC PENDULUM
    needs 
    Leonardo Micro Pro
    MPU6050 gyro acc
    MOZZI byte
    Li-Po battery
    make up a pendulum with these elements
    
    code based on Tim Barrass 2012, CC by-nc-sa codes
    and other non blocking codes for MPU6050
*/

#include <MozziGuts.h>
#include <Oscil.h>
#include <tables/triangle_analogue512_int8.h>
#include <tables/sin512_int8.h>
#include <mozzi_midi.h>
#include <twi_nonblock.h>

#define CONTROL_RATE 32
float gain;

static volatile byte acc_status = 0;
#define ACC_IDLE 0
#define ACC_READING 1
#define ACC_WRITING 2

int accbytedata[14];

#define MPU6050_ADDR                  0x68
#define MPU6050_SMPLRT_DIV_REGISTER   0x19
#define MPU6050_CONFIG_REGISTER       0x1a
#define MPU6050_GYRO_CONFIG_REGISTER  0x1b
#define MPU6050_ACCEL_CONFIG_REGISTER 0x1c
#define MPU6050_PWR_MGMT_1_REGISTER   0x6b
#define MPU6050_ACCEL_OUT_REGISTER    0x3b
#define TEMP_LSB_2_DEGREE     340.0   
#define TEMP_LSB_OFFSET       12412.0

Oscil<SIN512_NUM_CELLS, AUDIO_RATE> aSin1(SIN512_DATA);
Oscil<SIN512_NUM_CELLS, AUDIO_RATE> aSin2(SIN512_DATA);
Oscil<SIN512_NUM_CELLS, AUDIO_RATE> aSin3(SIN512_DATA);
Oscil<SIN512_NUM_CELLS, AUDIO_RATE> aSin4(SIN512_DATA);
Oscil<SIN512_NUM_CELLS, AUDIO_RATE> aSin5(SIN512_DATA);
Oscil<SIN512_NUM_CELLS, AUDIO_RATE> aSin6(SIN512_DATA);
Oscil<SIN512_NUM_CELLS, AUDIO_RATE> aSin7(SIN512_DATA);
Oscil<SIN512_NUM_CELLS, AUDIO_RATE> aSin8(SIN512_DATA);
Oscil<SIN512_NUM_CELLS, AUDIO_RATE> aSin9(SIN512_DATA);
Oscil<SIN512_NUM_CELLS, AUDIO_RATE> aSin10(SIN512_DATA);
Oscil<SIN512_NUM_CELLS, AUDIO_RATE> aSin11(SIN512_DATA);
Oscil<SIN512_NUM_CELLS, AUDIO_RATE> aSin12(SIN512_DATA);

void setup_accelero(){
  initialize_twi_nonblock();

  acc_writeTo(MPU6050_SMPLRT_DIV_REGISTER, 0x00);
  acc_writeTo(MPU6050_CONFIG_REGISTER, 0x00);
  acc_writeTo(MPU6050_ACCEL_CONFIG_REGISTER, 0x00);
    acc_writeTo(MPU6050_GYRO_CONFIG_REGISTER, 0x00);
  acc_writeTo(MPU6050_PWR_MGMT_1_REGISTER, 0x01);
  
  acc_status = ACC_IDLE;
}

void initiate_read_accelero(){
  txAddress = MPU6050_ADDR; //MMA7660_ADDR;
  txBufferIndex = 0;
  txBufferLength = 0;

  txBuffer[txBufferIndex] = MPU6050_ACCEL_OUT_REGISTER; // Start reading from register for X
  ++txBufferIndex;
  txBufferLength = txBufferIndex;

  twi_initiateWriteTo(txAddress, txBuffer, txBufferLength);
  acc_status = ACC_WRITING;
}

void initiate_request_accelero(){
  txBufferIndex = 0;
  txBufferLength = 0;

  byte read = twi_initiateReadFrom(MPU6050_ADDR, 14);
  acc_status = ACC_READING;
}

void finalise_request_accelero() {
  byte read = twi_readMasterBuffer( rxBuffer, 14 ); 
  rxBufferIndex = 0;
  rxBufferLength = read;

  byte i = 0;
  while( rxBufferLength - rxBufferIndex > 0) { // device may send less than requested (abnormal)
    accbytedata[i] = rxBuffer[rxBufferIndex];
    ++rxBufferIndex;
    i++;
  }

  acc_status = ACC_IDLE;
}

void acc_writeTo(byte address, byte val) {
  twowire_beginTransmission(MPU6050_ADDR); // start transmission to device
  twowire_send( address );
  twowire_send( val );
  twowire_endTransmission();
}

void setup(){
  aSin1.setFreq(2350);
  aSin2.setFreq(24);
  aSin3.setFreq(2350);
  aSin4.setFreq(2);
  aSin5.setFreq(16);
  aSin6.setFreq(1865);
  aSin7.setFreq(784);
  aSin8.setFreq(1568);
  aSin9.setFreq(2094);
  aSin10.setFreq(4700);
  aSin11.setFreq(6);
  aSin12.setFreq(6274);

  startMozzi(CONTROL_RATE);
  
  Serial.begin(115200); 
  setup_accelero();
}

int accx;
int accy;
int accz;
float temp;
int gyrox;
int gyroy;
int gyroz;

unsigned long ms = millis();
unsigned long readTime = ms;

void loop(){
  audioHook();
}


void updateControl(){
  ms = millis();
  if (ms > readTime) {
    readTime += 10;
 
    switch( acc_status ){
    case ACC_IDLE:
      accx = (accbytedata[0] << 8 | accbytedata[1]) >> 7; // accelerometer x reading, reduced to 8 bit
      accy = (accbytedata[2] << 8 | accbytedata[3]) >> 7; // accelerometer y reading, 8 bit
      accz = (accbytedata[4] << 8 | accbytedata[5]) >> 7; // accelerometer z reading
      temp = ((accbytedata[6] << 8 | accbytedata[7]) + TEMP_LSB_OFFSET) / TEMP_LSB_2_DEGREE;; // temperature reading
      gyrox = (accbytedata[8] << 8 | accbytedata[9]) >> 7; // gyro x reading, reduced to 8 bit
      gyroy = (accbytedata[10] << 8 | accbytedata[11]) >> 7; // gyro y reading, 8 bit
      gyroz = (accbytedata[12] << 8 | accbytedata[13]) >> 7; // gyro z reading
      Serial.print("\taX ");Serial.print(accx);
      Serial.print("\taY ");Serial.print(accy);
      Serial.print("\taZ ");Serial.print(accz);
      Serial.print("\tTemp ");Serial.print(temp);
      Serial.print("\tgX ");Serial.print(gyrox);
      Serial.print("\tgY ");Serial.print(gyroy);
      Serial.print("\tgZ ");Serial.print(gyroz);
      Serial.println();
      initiate_read_accelero();
  
        break;
    case ACC_WRITING:
      if ( TWI_MTX != twi_state ){
        initiate_request_accelero();
      }
      break;
    case ACC_READING:
      if ( TWI_MRX != twi_state ){
        finalise_request_accelero();
      }
      break;
    }
  }
}



AudioOutput_t updateAudio(){
  long asig = (long)
    aSin1.next() * (gyroz/2) +
    aSin2.next() * (gyroy/2) +
    aSin3.next() * (gyrox/2) +
    aSin4.next() * (gyroz+gyrox+gyroy) +
    aSin5.next() * (gyroy/2) +
    aSin6.next() * (gyrox/2) +
    aSin7.next() * (gyroz/2)+
    aSin8.next() * (gyrox/2) +
    aSin9.next() * (gyroz/2) +
    aSin10.next() * (gyroz/2) +
    aSin11.next() * (gyroy/2) +
    aSin12.next() * (gyroz/2) ;
  
  return MonoOutput::fromAlmostNBit(18, asig);
}
