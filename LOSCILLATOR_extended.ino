/*  Plays a fluctuating ambient using Mozzi sonification library.
    based on Tim Barrass 2012, CC by-nc-sa codes
    Trying to imitate Loscil drones and typical sound of organo portativo
    
    Composed for Enrico Milani 
    for theatre reading Segreto del Bosco Vecchio by Dino Buzzati
    ideally played with cello phrasings 
    and other mozzibytes imitating nature sounds
    
    in this version there are 4 different kinds of oscillators
    in order to better imitate an organo portativo in its reedy sounds
*/

#include <MozziGuts.h>
#include <Oscil.h>
#include <tables/sin8192_int8.h>
#include <tables/cos2048_int8.h>
#include <tables/triangle_analogue512_int8.h>
#include <tables/triangle_valve_2048_int8.h>
#include <mozzi_midi.h>

#define CONTROL_RATE 128

Oscil<SIN8192_NUM_CELLS, AUDIO_RATE> aSin1(SIN8192_DATA);
Oscil<SIN8192_NUM_CELLS, AUDIO_RATE> aSin2(SIN8192_DATA);
Oscil<TRIANGLE_VALVE_2048_NUM_CELLS, AUDIO_RATE> aTriangle3(TRIANGLE_VALVE_2048_DATA);
Oscil<TRIANGLE_VALVE_2048_NUM_CELLS, AUDIO_RATE> aTriangle4(TRIANGLE_VALVE_2048_DATA);
Oscil<TRIANGLE_ANALOGUE512_NUM_CELLS, AUDIO_RATE> aTriangle5(TRIANGLE_ANALOGUE512_DATA);
Oscil<TRIANGLE_ANALOGUE512_NUM_CELLS, AUDIO_RATE> aTriangle6(TRIANGLE_ANALOGUE512_DATA);
Oscil<COS2048_NUM_CELLS, AUDIO_RATE> aCos7(COS2048_DATA);
Oscil<COS2048_NUM_CELLS, AUDIO_RATE> aCos8(COS2048_DATA);

Oscil<SIN8192_NUM_CELLS, CONTROL_RATE> kVol1(SIN8192_DATA);
Oscil<SIN8192_NUM_CELLS, CONTROL_RATE> kVol2(SIN8192_DATA);
Oscil<TRIANGLE_VALVE_2048_NUM_CELLS, CONTROL_RATE> kVol3(TRIANGLE_VALVE_2048_DATA);
Oscil<TRIANGLE_VALVE_2048_NUM_CELLS, CONTROL_RATE> kVol4(TRIANGLE_VALVE_2048_DATA);
Oscil<TRIANGLE_ANALOGUE512_NUM_CELLS, CONTROL_RATE> kVol5(TRIANGLE_ANALOGUE512_DATA);
Oscil<TRIANGLE_ANALOGUE512_NUM_CELLS, CONTROL_RATE> kVol6(TRIANGLE_ANALOGUE512_DATA);
Oscil<COS2048_NUM_CELLS, CONTROL_RATE> kVol7(COS2048_DATA);
Oscil<COS2048_NUM_CELLS, CONTROL_RATE> kVol8(COS2048_DATA);

char v1,v2,v3,v4,v5,v6,v7,v8;

void setup(){
  aSin1.setFreq(mtof(39));
  aSin2.setFreq(mtof(43));
  aTriangle3.setFreq(mtof(51));
  aTriangle4.setFreq(mtof(55));
  aTriangle5.setFreq(mtof(58));
  aTriangle6.setFreq(mtof(60));
  aCos7.setFreq(mtof(62));
  aCos8.setFreq(mtof(67));

  kVol1.setFreq(0.00584f); 
  kVol2.setFreq(0.01839f);
  kVol3.setFreq(0.00714f);
  kVol4.setFreq(0.00401f);
  kVol5.setFreq(0.00851f);
  kVol6.setFreq(0.00492f);
  kVol7.setFreq(0.00947f);
  kVol8.setFreq(0.00393f);
  
  v1=v2=v3=v4=v5=v6=v7=v8=120;

  startMozzi(CONTROL_RATE);
}

void loop(){
  audioHook();
}

void updateControl(){
   v1 = kVol1.next()>>1;
   v2 = kVol2.next();
   v3 = kVol3.next();
   v4 = kVol4.next();
   v5 = kVol5.next();
   v6 = kVol6.next();
   v7 = kVol7.next();
   v8 = kVol8.next();
}

AudioOutput_t updateAudio(){
  long asig = (long)
    aSin1.next()*v1 +
    aSin2.next()*v2 +
    aTriangle3.next()*v3 +
    aTriangle4.next()*v4 +
    aTriangle5.next()*v5 +
    aTriangle6.next()*v6 +
    aCos7.next()*v7 +
    aCos8.next()*v8;
  
  return MonoOutput::fromAlmostNBit(18, asig);
}
