/*
    Project: Digital Audio Compressor
    Type: Linked Stereo
    Note: Detects only positive half cycles
    Author: Junon M.
    Date: 2022/12/06 17:15
    License: GPLv3
*/

#ifndef DualCompressor_h
#define DualCompressor_h

#include <Arduino.h>

//--------------------------------------------------------------------------------
// Reference for digital audio peaks
// Uncomment the line corresponding to the audio streaming bitrate
//--------------------------------------------------------------------------------

// For int 24-bit audio: RAW AUDIO
//const float COMP_LIMIT = 1000000000.0f; // 1 bi

// For float 32bit audio:
const float COMP_LIMIT = 5.0f; // 5.0f

// For int 16bit audio:
//const float COMP_LIMIT = 50000.0f;

//--------------------------------------------------------------------------------

//#define DEBUG_COMP         1

class DualCompressor
{
  private:

    // Maximum gain
    const float MAX_COMP_LEVEL = 70.0f; // em dB

    // Default gain when not playing
    const float DEFAULT_COMP_LEVEL = 10.0f; // in dB (10dB = 1.0x)

    // Minimum gain or maximum attenuation
    const float MIN_COMP_LEVEL = -70.0f; // in dB (0.1x)

    const float DEFAULT_PROTECTION = 7.0f; // 0dB

    const int32_t DEFAULT_ATTACK = 100; // in Delay Cycles

    const int32_t DEFAULT_RELEASE = 1500; // in Delay Cycles

    // Volume up steps
    const float DEFAULT_STEP_BY = -10.0f; // in dB (-10dB = 0.1x)

    
    float _sample_rate;
    float _limit_p;
    float _limit_n;
    float _protection_p;
    float _protection_n;
    float _min_comp_level;
    int32_t _attack;
    int32_t _release;
    float _gain;
    float _step;
    int32_t _attack_cnt = 0;
    int32_t _release_cnt = 0;
    float _level;
    float decibel_2_linear(float from);
    float timeCalc(float uS, float Step);

  public:

    DualCompressor();

    virtual ~DualCompressor();

    void setParams(float Protection, float Attack_Time, float Release_Time, float Gain, float Sample_Rate = 44100, float Step_By = 0.1f);

    inline void process(float &ch_L_input_output, float &ch_R_input_output)
    {
      float valueL = ch_L_input_output * _level;
      float valueR = ch_R_input_output * _level;
      
      if ((valueL > _limit_p) && (valueR > _limit_p)){
        
        if (_attack_cnt > _attack){

          _attack_cnt = 0;
          _level = _level - _step;

          if (_level < _min_comp_level) _level = _min_comp_level;

        }

        if ((valueL > _protection_p) && (valueR > _protection_p)){

          _level = _protection_p / fabs(ch_L_input_output);
        }
        
      }else{
        
        if (_release_cnt > _release){
          _release_cnt = 0;
          _level = _level + _step;
          if (_level > _gain) _level = _gain;
        }

      }
      
      _attack_cnt++;
      _release_cnt++;

      valueL = ch_L_input_output * _level;
      valueR = ch_R_input_output * _level;

      ch_L_input_output = valueL;
      ch_R_input_output = valueR;
    }

    inline float vrms(float vpico) {
      return (vpico * (1 / sqrt(2)));
    }
};

#endif /* DualCompressor_h */
