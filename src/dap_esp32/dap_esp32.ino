/*
    Project: Digital Audio Processor
    Type: 24 bits, Digital USB Input STM32F411 and DAC Out PCM5102
    Microprocessor: ESP32 WROOM Devkit
    Rom: 4MB, Minimal SPIFFS large Apps with OTA
    Ram: Internal ~ 350 KBytes
    Author: Junon M.
    License: GPLv3
*/

#include "WiFi.h"
#include "WebServer.h"
#include "SPIFFS.h"
#include "FS.h"
#include <Update.h>
#include "ArduinoJson.h"
#include "driver/gpio.h"
#include <YummyDSP.h>
#include <AudioDriver.h>
#include <esp_task_wdt.h>
//#include <AntiPiracy.h>
#include "the_dog.h"
#include "settings.h"
#ifdef SYMMETRIC_COMP
#include "DualCompressorSymm.h"
#else
#include "DualCompressor.h"
//#include "YAECCompressor.h"
#endif
#include "global_vars.h"
#include "file_manager.h"
#include "serial_cmd.h"
#include "web_server.h"

void setup() {

  esp_task_wdt_init(WDT_TIMEOUT, true);
  esp_task_wdt_add(NULL);

  Serial.begin(115200);

  analogReadResolution(12);
  pinMode(POT_ADC_IN_PIN, INPUT);

  for (int i = 0; i < MAX_NUM_BANDS; i++)
  {
    Equalizer[i] = 0.0f;
    Equalizer_Linear[i] = 0.0f;
    Post_Emphasis_Linear[i] = 0.0f;
  }

  pinMode(CLOCK_OUT_PIN, OUTPUT);

  init_file_manager();

  readConfig();

  init_filters();

  commitConfig();


#ifdef IN_TEST // Start of tests

  delay(500);

  PreEmphasis = -17.0;

  for (int i = 0; i < NumBands; i++)
  {
    int center_freq = getBandCenterFrequency(i);
#ifdef SERIAL_LOG
    Serial.print("center_freq = ");
    Serial.print(center_freq);
    Serial.print(" Curve = ");
    Serial.print(Equalizer[i] + getEqEmphasis(center_freq, PreEmphasis));
    Serial.println("dB");
#endif
  }

  while (1) feedTheDog();

#endif // End of tests

  //--------------------------------------------------------------------------------
  // i2s config
  //--------------------------------------------------------------------------------

  i2sCodec.setI2sPort(I2S_NUM_0);

#if defined(PCM1802_SLAVE_ESP32_MASTER)

  int err = i2sCodec.setFormat(
              SampleRateFreq,
              channelCount,
              I2S_BITS_PER_SAMPLE_32BIT,
              I2S_COMM_FORMAT_I2S,
              CODEC_LJ_RJ_ALIGN,
              384,
              I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_TX);

  err += i2sCodec.setPins(I2S_BCK_PIN, I2S_LRCLK_PIN, I2S_DOUT_PIN, I2S_DIN_PIN, CODEC_ENABLE_PIN);

#elif defined(STM32F411_SLAVE_ESP32_MASTER)

  int err = i2sCodec.setFormat(
              SampleRateFreq,
              channelCount,
              I2S_BITS_PER_SAMPLE_24BIT,
              I2S_COMM_FORMAT_I2S,
              CODEC_I2S_ALIGN,
              384,
              I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_TX);

  err += i2sCodec.setPins(I2S_BCK_PIN, I2S_LRCLK_PIN, I2S_DOUT_PIN, I2S_DIN_PIN, CODEC_ENABLE_PIN);

#elif defined(STM32F411_MASTER_ESP32_SLAVE)

  int err = i2sCodec.setFormat(
              SampleRateFreq,
              channelCount,
              I2S_BITS_PER_SAMPLE_24BIT,
              I2S_COMM_FORMAT_I2S,
              CODEC_I2S_ALIGN,
              384,
              I2S_MODE_SLAVE | I2S_MODE_RX | I2S_MODE_TX);

  err += i2sCodec.setPins(I2S_BCK_PIN, I2S_LRCLK_PIN, I2S_DOUT_PIN, I2S_DIN_PIN, CODEC_ENABLE_PIN);

#elif defined(ESP32_BT_AUDIO_MASTER_ESP32_SLAVE)

  int err = i2sCodec.setFormat(
              SampleRateFreq,
              channelCount,
              I2S_BITS_PER_SAMPLE_32BIT,
              (i2s_comm_format_t)I2S_COMM_FORMAT_I2S,// | I2S_COMM_FORMAT_I2S_MSB),
              CODEC_LJ_RJ_ALIGN,//CODEC_I2S_ALIGN,
              384,
              I2S_MODE_SLAVE | I2S_MODE_RX | I2S_MODE_TX);

  err += i2sCodec.setPins(I2S_BCK_PIN, I2S_LRCLK_PIN, I2S_DOUT_PIN, I2S_DIN_PIN, CODEC_ENABLE_PIN);

#endif


  err += i2sCodec.start();

  i2sCodec.mute(false);

  //--------------------------------------------------------------------------------


#ifdef WEBSERVER_EN
  configureWiFi();
#endif

  // NOTE: The main loop runs on CORE_ONE

#ifdef WEBSERVER_EN
  xTaskCreatePinnedToCore(
    wifiConnectionTask,     // Function that will be performed
    "wifiConnectionTask",   // Task name
    10000,                  // Size of available memory (in WORDs)
    NULL,                   // We will not pass any parameters
    10,                     // Priority
    NULL,                   // We don't need a reference for the task
    CORE_ZERO);             // Core number = 0
  delay(500);               // Time for task to start
#endif

  xTaskCreatePinnedToCore(timeCounterTask, "timeCounterTask", 10000, NULL, 6, NULL, CORE_ZERO); // ZERO
  delay(500);

#ifdef WEBSERVER_EN
  xTaskCreatePinnedToCore(webserverTask, "webserverTask", 10000, NULL, 6, NULL, CORE_ZERO); // ZERO P4
  delay(500);

  //xTaskCreatePinnedToCore(ftpTask, "ftpTask", 10000, NULL, 6, NULL, CORE_ZERO); // ZERO P4
  //delay(500);
#endif

#ifndef WEBSERVER_EN
  xTaskCreatePinnedToCore(serialCommTask, "serialCommTask", 10000, NULL, 6, NULL, CORE_ZERO); // ZERO P10
  delay(500);
#endif

  // run audio in dedicated task on cpu core 1
  // run control task on another cpu  core with lower priority
  xTaskCreatePinnedToCore(audioTask, "audioTask", 10000, NULL, 10, NULL, CORE_ONE);//CORE_ONE P10
  delay(500);

  isLicensed = true; //antiPiracy.licenseIsValid(CHIP_ID);

}


float readPotenciometer() {
  const int precision = 10;
  static int lastPot = 0;
  static float vol = 0.0;
  int pot = analogRead(POT_ADC_IN_PIN);
  if ((pot < (lastPot - precision)) || (pot > (lastPot + precision)))
  {
    lastPot = pot;
    vol = (float)lastPot;
    vol /= 4095.0; // 0.0 a 1.0
  }
  return vol;
}


void init_filters()
{
  //--------------------------------------------------------------------------------
  // Starting the DSP objects
  //--------------------------------------------------------------------------------
  for (int i = 0; i < MAX_NUM_BANDS; i++)
  {
    dspBAND[i].begin(SampleRateFreq);
    LPF_BAND[i].begin(SampleRateFreq, 2);
    HPF_BAND[i].begin(SampleRateFreq, 2);
  }
  //--------------------------------------------------------------------------------

  //--------------------------------------------------------------------------------
  // Setting the frequency of the filters
  //--------------------------------------------------------------------------------
  for (int i = NumBands - 1; i >= 0; i--)
  {
    int hpf = FILTER_FREQ[NumBands - 1][i];
    int lpf = FILTER_FREQ[NumBands - 1][i + 1];

    LPF_BAND[i].setupFilter(FilterNode::LPF, lpf, FiltersQFactor);
    HPF_BAND[i].setupFilter(FilterNode::HPF, hpf, FiltersQFactor);
  }
  //--------------------------------------------------------------------------------


  //--------------------------------------------------------------------------------
  // Adding nodes to DSP objects
  //--------------------------------------------------------------------------------
  for (int i = 0; i < NumBands; i++)
  {
    dspBAND[i].addNode(&LPF_BAND[i]);
    dspBAND[i].addNode(&HPF_BAND[i]);
  }
  //--------------------------------------------------------------------------------

  //--------------------------------------------------------------------------------
  // 19KHz Filters
  //--------------------------------------------------------------------------------
  dspFmFilter.begin(SampleRateFreq);

  for (int i = 0; i < NumFmFilters; i++)
  {
    LPF_FM_FILTER[i].begin(SampleRateFreq, 2);
    LPF_FM_FILTER[i].setupFilter(FilterNode::LPF, 18000, 0.8f);

    dspFmFilter.addNode(&LPF_FM_FILTER[i]);
  }
  //--------------------------------------------------------------------------------


  //--------------------------------------------------------------------------------
  // Starting and configuring the Echo Chamber
  //--------------------------------------------------------------------------------
  dspDelay.begin(SampleRateFreq);
  ECHO.begin(SampleRateFreq, 2);
  ECHO.setDelayMs(30.0f);//150.0f
  ECHO.setMix(Echo, true);
  dspDelay.addNode(&ECHO);
  //--------------------------------------------------------------------------------
}


void timeCounterTask(void * pvParameters)
{
  // Block for 100ms.
  const TickType_t xDelay = 100 / portTICK_PERIOD_MS;
  esp_task_wdt_add(NULL);

  while (true)
  {
    // Obeys the restart command
    if (espRestart) CounterLoopRestart_100ms++;
    if (CounterLoopRestart_100ms >= RestartTimeX100ms) {
      ESP.restart();
    }

    // Turns on audio after N seconds x 10
    if (MuteTimeX100ms > 0) {
      MuteTimeX100ms--;
    } else {
      ForcedMute = false;
    }

    //Volume = readPotenciometer();
    //setAudioVolume();

    feedTheDog();
    vTaskDelay(xDelay);
  }
}


#ifndef WEBSERVER_EN
void serialCommTask(void * pvParameters)
{
  // Block for 10ms.
  const TickType_t xDelay = 10 / portTICK_PERIOD_MS;

  esp_task_wdt_add(NULL);

  while (true)
  {
    commandInterpreter();
    feedTheDog();
    vTaskDelay(xDelay);
  }
}
#endif


void audioTask(void * pvParameters)
{

  esp_task_wdt_add(NULL);

  while (true)
  {
    i2sCodec.readBlock();

    for (int i = 0; i < AudioDriver::BufferSize; i++) {

#ifdef RAW_AUDIO
      inputSampleR = i2sCodec.readRawSample(i, 0);
      inputSampleL = i2sCodec.readRawSample(i, 1);
#else
      inputSampleR = i2sCodec.readSample(i, 0);
      inputSampleL = i2sCodec.readSample(i, 1);
#endif

      if (Compressor)
      {
        // Input level control
        inputSampleR *= InputLevelLinear;
        inputSampleL *= InputLevelLinear;

        for (int j = 0; j < NumBands; j++) {

          // Frequency filters
          Band_R[j] = dspBAND[j].process(inputSampleR, 0);
          Band_L[j] = dspBAND[j].process(inputSampleL, 1);

          if (Band_R[j] > VuArrValue[j]) VuArrValue[j] = Band_R[j];

          // Equalizer
          Band_R[j] *= Equalizer_Linear[j];
          Band_L[j] *= Equalizer_Linear[j];

          // Dynamic compression
          comp_st[j].process(Band_L[j], Band_R[j]);

          // Post-emphasis
          Band_R[j] *= Post_Emphasis_Linear[j];
          Band_L[j] *= Post_Emphasis_Linear[j];

          // Mixer
          outputSampleR += Band_R[j];
          outputSampleL += Band_L[j];
        }

        // 19Khz filter
        if (NumFmFilters > 0)
        {
          outputSampleR = dspFmFilter.process(outputSampleR, 0);
          outputSampleL = dspFmFilter.process(outputSampleL, 1);
        }

        // Echo Chamber
        if (Echo > 0.0f) {
          outputSampleR = dspDelay.process(outputSampleR, 0);
          outputSampleL = dspDelay.process(outputSampleL, 1);
        }
      }
      else // Disable compressor
      {
        // Get audio directly from the input
        outputSampleR = inputSampleR;
        outputSampleL = inputSampleL;

        // Compensates for the drop in intensity
        outputSampleR *= VolCompensationLinear;
        outputSampleL *= VolCompensationLinear;
      }

      if (Reserved1) limiter_st.process(outputSampleL, outputSampleR);

      // End Clipper
      outputSampleR = constrain(outputSampleR, Clipper_Linear_n, Clipper_Linear_p);
      outputSampleL = constrain(outputSampleL, Clipper_Linear_n, Clipper_Linear_p);

      if (Mute || ForcedMute)
      {
        outputSampleR = 0.f;
        outputSampleL = 0.f;
      }
      else
      {
        // End Volume
        outputSampleR *= OutputLevelLinearR;
        outputSampleL *= OutputLevelLinearL;
      }

      //i2sCodec.writeSample(outputSampleR, i, 0);
      //i2sCodec.writeSample(outputSampleL, i, 1);

#ifdef RAW_AUDIO
      i2sCodec.writeRawStereoSample(outputSampleR, outputSampleL, i);
#else
      i2sCodec.writeStereoSample(outputSampleR, outputSampleL, i);
#endif

    }

    i2sCodec.writeBlock();
    feedTheDog();
  }
}


void loop()
{

}
