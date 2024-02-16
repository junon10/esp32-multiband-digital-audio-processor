/*
    Project: Digital Audio Compressor
    Type: Linked Stereo
    Note: Detects only positive half cycles
    Author: Junon M.
    Date: 2022/12/06 17:15
*/

#include "DualCompressor.h"

DualCompressor::DualCompressor() {

  _protection_p     =  decibel_2_linear(DEFAULT_PROTECTION) * COMP_LIMIT;
  _protection_n     =  _protection_p * -1.0;

  _limit_p = COMP_LIMIT;
  _limit_n = _limit_p * -1.0;

  _attack     =  DEFAULT_ATTACK; // in Delay Cycles 
   
  _release    =  DEFAULT_RELEASE; // in Delay Cycles 
  
  _gain = decibel_2_linear(MAX_COMP_LEVEL); // Maximum signal gain in dB

  _level = decibel_2_linear(DEFAULT_COMP_LEVEL); // Standard Gain without audio, in dB

  // Valor de cada degrau de subida de volume
  _step = decibel_2_linear(DEFAULT_STEP_BY); // _step = 0.0001f to O.1f; 
}


DualCompressor::~DualCompressor() {
}


void DualCompressor::setParams(float Protection, float Attack_Time, float Release_Time, 
float Gain, float Sample_Rate, float Step_By)
{
  _sample_rate = Sample_Rate;
  
  float _cycle_time_seg = 1 / _sample_rate;
  
  _step = decibel_2_linear(Step_By); 

  // Formulas ok {
  // Cycle_Time_s = 1 / SampleRate_Freq_Hz;
  // Cycle_Time_s = 0,000022675s ou 22us
  
  // DelayAttack = Attack_Time_us / Cycle_Time_s / 1000000;
  // DelayAttack = 1000 / 0,000022675 / 1000000;
  // DelayAttack = 45,454545455;
  _attack = timeCalc(Attack_Time, _step) / _cycle_time_seg / 1000.0; // ms

  // DelayRelease = Realease_Time_ms / Cycle_Time_s / 1000;
  // DelayRelease = 30 / 0,000022675 / 1000;
  // DelayRelease = 1363,636363636;
  _release = timeCalc(Release_Time, _step) / _cycle_time_seg / 1000.0;  // ms
  // }

  _protection_p = decibel_2_linear(Protection) * COMP_LIMIT; // X dB below COMP_LIMIT
  _protection_n = _protection_p * -1.0;

  _gain = decibel_2_linear(Gain);

  _min_comp_level = decibel_2_linear(MIN_COMP_LEVEL);

#ifdef DEBUG_COMP
  Serial.println("(1)ATK_DY, (2)RLS_DY, (3)PROT, (4)GAIN, (5)MIN_COMP_LVL, (6)STEP");

  Serial.print("(1)");
  Serial.print(_attack);
  Serial.print(", ");

  Serial.print("(2)");
  Serial.print(_release);
  Serial.print(", ");
  
  Serial.print("(3)");
  Serial.print(_protection,9);
  Serial.print(", "); 

  Serial.print("(4)");
  Serial.print(_gain,6); 
  Serial.print(", ");

  Serial.print("(5)");  
  Serial.print(_min_comp_level,9); 
  Serial.print(", ");

  Serial.print("(6)");
  Serial.print(_step,9);
  Serial.println(";\n");   
#endif
}


float DualCompressor::decibel_2_linear(float from)
{
  return pow(10.0f, from / 10.0f);
}


float DualCompressor::timeCalc(float uS, float Step)
{
  return (uS * 10.0 * Step);
}
