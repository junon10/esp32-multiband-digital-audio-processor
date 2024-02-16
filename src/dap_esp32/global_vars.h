
bool isWifiConnected = false;
bool isLicensed = false;
bool espRestart = false;
bool isGoodFileSystem = true;
uint32_t CounterLoopRestart_100ms = 0, RestartTimeX100ms = DEFAULT_RESTART_TIME;
uint16_t MuteTimeX100ms = DEFAULT_FORCED_MUTE_TIME;

//--------------------------------------------------------------------------------
//Data dataAtual;
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
int TimeZone = -3;
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
String
myAdminUserName = "", myAdminPassword = "",
myApSsid = "", myApPassword = "",
myWifiSsid = "", myWifiPassword = "",
myWifiIp = "", myWifiGateway = "", myWifiSubnet = "", myWifiDns = "",
myTelegramBotToken = "";
bool myWifiWithDhcp = true;
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
float VuArrValue[MAX_NUM_BANDS];

float InputLevel = DEFAULT_INPUT_LEVEL;
float OutputLevel = DEFAULT_OUTPUT_LEVEL;
float Balance = DEFAULT_BALANCE;
float Clipper = DEFAULT_CLIPPER;

bool Compressor = DEFAULT_COMPRESSOR;
bool BandSync = DEFAULT_BAND_SYNC;
bool Mute = DEFAULT_MUTE, ForcedMute = DEFAULT_FORCED_MUTE;
bool Reserved1 = DEFAULT_RESERVED1;
bool Reserved2 = DEFAULT_RESERVED2;

int NumBands = DEFAULT_NUM_BANDS;
float PreEmphasis = DEFAULT_PRE_EMPHASIS;
float PostEmphasis = DEFAULT_POST_EMPHASIS;
float  StepBy = DEFAULT_STEP_BY;
float Echo = DEFAULT_ECHO;
float  FiltersQFactor = DEFAULT_FILTERS_Q_FACTOR;

float Protection[ALL_NUM_BANDS];
float AttackTime[ALL_NUM_BANDS];
float ReleaseTime[ALL_NUM_BANDS];
float Gain[ALL_NUM_BANDS];

float Equalizer[MAX_NUM_BANDS];
float Equalizer_Linear[MAX_NUM_BANDS];
float Post_Emphasis_Linear[MAX_NUM_BANDS];
float Clipper_Linear_p = 0.0f;
float Clipper_Linear_n = 0.0f;
//--------------------------------------------------------------------------------

#define NUM_REGS  1

typedef struct {
  uint8_t write_status;

  float InputLevel;
  float OutputLevel;
  float Clipper;

  bool Compressor;
  bool BandSync;
  bool Mute;
  bool Reserved1;
  bool Reserved2;

  uint8_t NumBands;
  float PreEmphasis;
  float PostEmphasis;
  float StepBy;
  float Echo;

  float FiltersQFactor;
  int8_t timezone;
  float Equalizer[MAX_NUM_BANDS];

  float Gain[ALL_NUM_BANDS];
  float Protection[ALL_NUM_BANDS];
  float AttackTime[ALL_NUM_BANDS];
  float ReleaseTime[ALL_NUM_BANDS];

  char my_admin_user_name[40];
  char my_admin_password[40];
  char my_ap_ssid[40];
  char my_ap_password[40];
  char my_wifi_ssid[40];
  char my_wifi_password[40];
  char my_wifi_with_dhcp;
  char my_wifi_ip[20];
  char my_wifi_gateway[20];
  char my_wifi_subnet[20];
  char my_wifi_dns[20];
} CfgType;

CfgType cfgRegs[NUM_REGS];

//AntiPiracy antiPiracy;

File fsUploadFile; // a File object to temporarily store the received file

AudioDriver i2sCodec;

#ifdef SYMMETRIC_COMP
DualCompressorSymm comp_st[MAX_NUM_BANDS];
DualCompressorSymm limiter_st;
#else
DualCompressor comp_st[MAX_NUM_BANDS];
DualCompressor limiter_st;
//YAECCompressor comp_st[MAX_NUM_BANDS];
//YAECCompressor limiter_st;
#endif

YummyDSP dspBAND[MAX_NUM_BANDS], dspDelay;

FilterNode LPF_BAND[MAX_NUM_BANDS], HPF_BAND[MAX_NUM_BANDS];

DelayNode ECHO;

YummyDSP dspFmFilter;

// filtros de 19KHz
FilterNode LPF_FM_FILTER[NumFmFilters];

float InputLevelLinear = 1.0f;

float OutputLevelLinearL = 0.0f;
float OutputLevelLinearR = 0.0f;

float BalanceLinearL = 0.0f;
float BalanceLinearR = 0.0f;

float VolCompensationLinear = 1.0f;

float Band_R[MAX_NUM_BANDS];
float Band_L[MAX_NUM_BANDS];

float inputSampleR = 0, inputSampleL = 0, outputSampleR = 0, outputSampleL = 0;

float Volume = 0.0;

float decibel_2_linear(float from)
{
  //Serial.println("dB = " + String(from, 2) + "   Linear = " + String(pow(10.0f, from / 10.0f), 2));
  return pow(10.0f, from / 10.0f);
}


float linear_2_decibel(float from)
{
  //Serial.println("Linear = " + String(from, 2) + "   Decibel = " + String(10.0f * log10(from), 2));
  return 10.0f * log10(from);
}


String left(String s, int len)
{
  String S = s;
  while (S.length() < len) S = " " + S;
  return S;
}


String right(String s, int len)
{
  String S = s;
  while (S.length() < len) S += ".";
  return S;
}


float getEqEmphasis(float freq, float att_final)
{
  float max_freq = FILTER_FREQ[NumBands - 1][NumBands];
  float factor = att_final / max_freq;
  return freq * factor;
}


String getStrFilterFreq(int band)
{
  String s = "";

  if ((band >= 0) && (band < NumBands))
  {
    int hpf = FILTER_FREQ[NumBands - 1][band];
    int lpf = FILTER_FREQ[NumBands - 1][band + 1];
    s = left(String(hpf), 6) + "Hz to " + left(String(lpf), 6) + "Hz";
  }
  else
  {
    s = "Error: Invalid index!";
  }
  return s;
}


int getFilterFreq(int index)
{
  int f = 0;
  if ((index >= 0) && (index <= NumBands)) {
    f = FILTER_FREQ[NumBands - 1][index];
  }
  return f;
}


int getBandCenterFrequency(int band_index)
{
  int hpf = FILTER_FREQ[NumBands - 1][band_index];
  int lpf = FILTER_FREQ[NumBands - 1][band_index + 1];
  return hpf + ((lpf - hpf) / 2);
}


void changeEqualization()
{
  float G = 0.0, lastG = 0.0;
  int center_freq = 0;

  // Calculates the greatest gain of the Equalizer + Pre-emphasis
  for (int i = 0; i < NumBands; i++) {
    center_freq = getBandCenterFrequency(i);
    G = Equalizer[i] + getEqEmphasis(center_freq, PreEmphasis);
    if (G > lastG) lastG = G;
  }

  //Applies Equalizer + Pre-emphasis gain
  // Decreases all bands based on the highest level chosen in the equalizer
  for (int i = 0; i < NumBands; i++) {
    center_freq = getBandCenterFrequency(i);
    Equalizer_Linear[i] = decibel_2_linear(Equalizer[i] + getEqEmphasis(center_freq, PreEmphasis) - lastG);
  }

  // Calculates post-emphasis and applies it to the filter
  for (int i = 0; i < NumBands; i++) {
    center_freq = getBandCenterFrequency(i);
    Post_Emphasis_Linear[i] = decibel_2_linear(getEqEmphasis(center_freq, PostEmphasis));
  }
}


// Update FIR filters
void updateFilters()
{
  static int num_bands = NumBands;

  if (num_bands != NumBands)
  {
    num_bands = NumBands;

    ForcedMute = true;
    MuteTimeX100ms = 10; // 1 sec

    //--------------------------------------------------------------------------------
    // Remove all filters
    //--------------------------------------------------------------------------------
    for (int i = 0; i < MAX_NUM_BANDS; i++)
    {
      dspBAND[i].clear();
    }
    //--------------------------------------------------------------------------------

    //--------------------------------------------------------------------------------
    // Reconfigures available filters
    //--------------------------------------------------------------------------------
    for (int i = NumBands - 1; i >= 0; i--)
    {
      int hpf = FILTER_FREQ[NumBands - 1][i];
      int lpf = FILTER_FREQ[NumBands - 1][i + 1];

      LPF_BAND[i].resetFilter(FilterNode::LPF, lpf, FiltersQFactor);
      HPF_BAND[i].resetFilter(FilterNode::HPF, hpf, FiltersQFactor);
    }
    //--------------------------------------------------------------------------------

    //--------------------------------------------------------------------------------
    // Re-adds the configured filters to the DSP object
    //--------------------------------------------------------------------------------
    for (int i = 0; i < NumBands; i++)
    {
      dspBAND[i].addNode(&LPF_BAND[i]);
      dspBAND[i].addNode(&HPF_BAND[i]);
    }
    //--------------------------------------------------------------------------------
  }
}


void setAudioVolume(){
  OutputLevelLinearL = decibel_2_linear(OutputLevel + DEFAULT_VOL_CORRECTION);// * Volume;
  OutputLevelLinearR = decibel_2_linear(OutputLevel + DEFAULT_VOL_CORRECTION);// * Volume;   
}


void commitConfig()
{
  for (int i = 0; i < MAX_NUM_BANDS; i++)
  {
#ifdef WEBSERVER_EN // Individual config
    comp_st[i].setParams(Protection[i], AttackTime[i], ReleaseTime[i], Gain[i], (float)SampleRateFreq, StepBy);
#else
    int j = MAX_NUM_BANDS; // Sync config
    comp_st[i].setParams(Protection[j], AttackTime[j], ReleaseTime[j], Gain[j], (float)SampleRateFreq, StepBy);
#endif

  }

  int k = MAX_NUM_BANDS + 1; 
  limiter_st.setParams(Protection[k], AttackTime[k], ReleaseTime[k], Gain[k], (float)SampleRateFreq, StepBy);
  
  updateFilters();
  ECHO.setMix(Echo, true);
  changeEqualization();
  InputLevelLinear = decibel_2_linear(InputLevel);

  float P = Balance / 100.0f;

  if (Balance < 50.0f) {

    BalanceLinearL = Balance * 0.02f;
    BalanceLinearR = 1.0f;

  } else if (Balance > 50.0f) {

    BalanceLinearL = 1.0f;
    BalanceLinearR = abs(Balance - 100.0f) * 0.02f;

  } else {

    BalanceLinearL = 1.0f;
    BalanceLinearR = 1.0f;

  }

  setAudioVolume();

  OutputLevelLinearL *= BalanceLinearL;
  OutputLevelLinearR *= BalanceLinearR;

  Clipper_Linear_p = decibel_2_linear(Clipper + 10.0f) * COMP_LIMIT;
  Clipper_Linear_n = Clipper_Linear_p * -1.0f;
  
  VolCompensationLinear = decibel_2_linear(DEFAULT_VOL_COMPENSATION) + 1.0f;
}
