//--------------------------------------------------------------------------------
// Include version control here
//--------------------------------------------------------------------------------
const String VERSION = "0.2.71 2024/02/14";
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
// Change this number so that the system takes the default values, and
// do not accept saved files
//--------------------------------------------------------------------------------
#define   CONFIG_TAG     123 // 0 a 255
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
// Equation Test Mode
//--------------------------------------------------------------------------------
//#define  IN_TEST   1
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
// Serial log
//--------------------------------------------------------------------------------
//#define  SERIAL_LOG   1
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
// Debug mode
//--------------------------------------------------------------------------------
//#define  IN_DEVELOPMENT   1
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
// Configuration files saved as Json
//--------------------------------------------------------------------------------
#define SAVE_AS_JSON   1
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
// Configuration via webserver or USB serial port
//--------------------------------------------------------------------------------
#define  WEBSERVER_EN    1
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
// Compressor Model
//--------------------------------------------------------------------------------
//#define  SYMMETRIC_COMP  1
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
// Language/Idioma
//--------------------------------------------------------------------------------
#define  ENG_LANG         1
//#define  PTBR_LANG        1
//--------------------------------------------------------------------------------


//--------------------------------------------------------------------------------
// Choose depending on the input hardware!
// Uncomment just one option
//--------------------------------------------------------------------------------

// External ADC PCM1802
//#define  PCM1802_SLAVE_ESP32_MASTER    1

// Arduino IDE Esp32 lib version 1.0.6
// Direct USB input for Esp32 i2s slave
#define  STM32F411_MASTER_ESP32_SLAVE  1

// Direct USB input for Esp32 i2s master
//#define  STM32F411_SLAVE_ESP32_MASTER  1

// Arduino IDE Esp32 lib version 2.0.6
// Esp32 Bluetooth and STM32F411 Master input
// Also valid for Mp3 Player with Esp32 (32bit audio)
//#define  ESP32_BT_AUDIO_MASTER_ESP32_SLAVE 1

//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
// Audio without floating point processing
//--------------------------------------------------------------------------------
//#define RAW_AUDIO  true
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
// APPLICATION_TITLE
//--------------------------------------------------------------------------------
const String APPLICATION_TITLE = "DIGITAL AUDIO PROCESSOR";
const String LOGIN_PAGE_TITLE = "DAP - LOGIN";
const String ROOT_PAGE_TITLE = "DIGITAL AUDIO PROCESSOR";
const String ALL_SETTINGS_PAGE_TITLE = "DAP - SETTINGS";
const String NETWORK_PAGE_TITLE = "DAP - NETWORK SETTINGS";
const String BACKUP_AND_RESTALL_PAGE_TITLE = "DAP - BACKUP AND RESTORE";
const String FILE_UPLOAD_PAGE_TITLE = "DAP - FILE UPLOAD";
const String FILE_DELETE_PAGE_TITLE = "DAP - DELETE FILES";
const String FIRMWARE_UPDATE_PAGE_TITLE = "DAP - FIRMWARE UPDATE";
const String PRODUCT_ACTIVATION_PAGE_TITLE = "DAP - PRODUCT ACTIVATION";
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
// um ID exclusivo do Chip...
const uint32_t chipID = (uint32_t)(ESP.getEfuseMac() >> 32);

// montado para ser usado no HTML
const String CHIP_ID = String(chipID, HEX);
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
// Default values for wifi connections
//--------------------------------------------------------------------------------
#ifdef IN_DEVELOPMENT

#define DEFAULT_ADMIN_USERNAME    "admin"
#define DEFAULT_ADMIN_PASSWORD    "admin"

#define DEFAULT_AP_SSID           "DAP"
#define DEFAULT_AP_PASSWORD       "123mudar"

#define DEFAULT_WIFI_SSID         ""
#define DEFAULT_WIFI_PASSWORD     ""

#define DEFAULT_WIFI_WITH_DHCP    false
#define DEFAULT_WIFI_IP           "192.168.0.200"
#define DEFAULT_WIFI_GATEWAY      "192.168.0.1"
#define DEFAULT_WIFI_SUBNET       "255.255.255.0"
#define DEFAULT_WIFI_DNS          "8.8.8.8"

#else

#define DEFAULT_ADMIN_USERNAME    "admin"
#define DEFAULT_ADMIN_PASSWORD    "admin"

#define DEFAULT_AP_SSID           "DAP"
#define DEFAULT_AP_PASSWORD       "123mudar"

#define DEFAULT_WIFI_SSID         ""
#define DEFAULT_WIFI_PASSWORD     ""

#define DEFAULT_WIFI_WITH_DHCP    true
#define DEFAULT_WIFI_IP           ""
#define DEFAULT_WIFI_GATEWAY      ""
#define DEFAULT_WIFI_SUBNET       ""
#define DEFAULT_WIFI_DNS          ""

#endif

//--------------------------------------------------------------------------------
// Digital audio processor configuration
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
// MAX
//--------------------------------------------------------------------------------
const float MAX_INPUT_LEVEL =            90.0f;
const float MAX_OUTPUT_LEVEL =           10.0f;
const float MAX_BALANCE =               100.0f;
const float MAX_CLIPPER =                40.0f;

const int   MAX_NUM_BANDS =                 10;
const float MAX_PRE_EMPHASIS =           30.0f;
const float MAX_POST_EMPHASIS =          30.0f;
const float MAX_STEP_BY =                -3.0f; // 0dB = 1x, -10dB = 0.1x
const float MAX_ECHO =                    1.0f;

const float MAX_PROTECTION =             30.0f;
const float MAX_GAIN =                   80.0f; // 80.0 dB
const float MAX_ATTACK_TIME =          1000.0f; // 1000ms = 1s
const float MAX_RELEASE_TIME =        50000.0f; // 50000ms = 50s

const float MAX_EQ_BAND =                12.0f;
const float MAX_FILTERS_Q_FACTOR =       10.0f;
//--------------------------------------------------------------------------------


//--------------------------------------------------------------------------------
// MIN
//--------------------------------------------------------------------------------
const float MIN_INPUT_LEVEL =           -50.0f;
const float MIN_OUTPUT_LEVEL =          -70.0f;
const float MIN_BALANCE =                 0.0f;
const float MIN_CLIPPER =               -40.0f;

const int   MIN_NUM_BANDS =                  1;
const float MIN_PRE_EMPHASIS =          -30.0f; 
const float MIN_POST_EMPHASIS =         -30.0f;
const float MIN_STEP_BY =                -90.f; // -90dB = 0.000000001x
const float MIN_ECHO =                    0.0f;

const float MIN_PROTECTION =              0.0f;
const float MIN_GAIN =                    0.0f;
const float MIN_ATTACK_TIME =             0.0f;
const float MIN_RELEASE_TIME =            5.0f;

const float MIN_EQ_BAND =               -12.0f;
const float MIN_FILTERS_Q_FACTOR =        0.1f;
//--------------------------------------------------------------------------------


//--------------------------------------------------------------------------------
// DEFAULT
//--------------------------------------------------------------------------------
const float DEFAULT_INPUT_LEVEL =        16.0f; // -10.0dB
const float DEFAULT_OUTPUT_LEVEL =      -20.0f; // Hurricane = -20dB; Headphones = 0dB;
const float DEFAULT_BALANCE =            50.0f; 
const float DEFAULT_CLIPPER =             0.0f;

const bool  DEFAULT_COMPRESSOR =          true;
const bool  DEFAULT_BAND_SYNC =           true;
const bool  DEFAULT_MUTE =               false;
const bool  DEFAULT_RESERVED1 =          false; // Final Limiter = false
const bool  DEFAULT_RESERVED2 =           true;

const int   DEFAULT_NUM_BANDS =              8;
const float DEFAULT_PRE_EMPHASIS =        0.0f; // 0dB at 20Hz, 6dB at 18000Hz 
const float DEFAULT_POST_EMPHASIS =      -6.0f; // 0dB at 20Hz, 6dB at 18000Hz 
const float DEFAULT_STEP_BY =            -14.f; // -10dB = 0.1x
const float DEFAULT_ECHO =                0.0f;

const float DEFAULT_PROTECTION =          7.0f;
const float DEFAULT_GAIN =               50.0f;
const float DEFAULT_ATTACK_TIME =         30.f; // 1000.0us (or 500.0us in old equation)
const float DEFAULT_RELEASE_TIME =        30.f; //   20.0ms (or  30.0ms in old equation)

const float DEFAULT_EQ_BAND =             0.0f;
const float DEFAULT_FILTERS_Q_FACTOR =    1.0f;

const bool  DEFAULT_FORCED_MUTE =         true;
const int   DEFAULT_FORCED_MUTE_TIME =     100; // in seconds / 10 ou 100ms
const int   DEFAULT_RESTART_TIME =         100; // in seconds / 10 ou 100ms

#ifdef RAW_AUDIO
const float DEFAULT_VOL_CORRECTION =    -30.0f; // -30.0dB
#else
const float DEFAULT_VOL_CORRECTION =    -10.0f; // -10.0dB
#endif

// Audio intensity compensation for compressor off
const float DEFAULT_VOL_COMPENSATION =   20.0f; //  in dB
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------
#define   NumFmFilters     0  // 0 to 2
//--------------------------------------------------------------------------------

const int ALL_NUM_BANDS = MAX_NUM_BANDS + 2;

//--------------------------------------------------------------------------------
// Digital frequency filters configuration
//--------------------------------------------------------------------------------

// The lines (N_BAND) are the number of bands
// The columns (N_FREQ) are the frequencies available for each band
//
const int N_BAND = 10, N_FREQ = 11;

/* 
// 01 Initial 
int FILTER_FREQ[N_BAND][N_FREQ] = {
  {10, 18000,     0,     0,     0,     0,     0,     0,     0,     0,     0}, //  1 BAND
  {10,  3000, 18000,     0,     0,     0,     0,     0,     0,     0,     0}, //  2 BAND
  {10,   800, 10000, 18000,     0,     0,     0,     0,     0,     0,     0}, //  3 BAND
  {10,   400,  1500, 10000, 18000,     0,     0,     0,     0,     0,     0}, //  4 BAND
  {10,   400,  1500,  3000, 10000, 18000,     0,     0,     0,     0,     0}, //  5 BAND
  {10,   400,  1500,  3000, 10000, 15000, 18000,     0,     0,     0,     0}, //  6 BAND
  {10,   400,  3000, 11000, 14000, 16000, 17000, 18000,     0,     0,     0}, //  7 BAND
  {10,   400,  5000, 10000, 12000, 14000, 15000, 16000, 18000,     0,     0}, //  8 BAND
  {10,   400,  2000,  6000, 10000, 12000, 14000, 16000, 17000, 18000,     0}, //  9 BAND Calibrado!
  {10,   400,   800,  1500,  3000,  6000,  8000, 11000, 14000, 16000, 18000}  // 10 BAND
};
*/

/*
// 02 HD
int FILTER_FREQ[N_BAND][N_FREQ] = {
  {10, 18000,     0,     0,     0,     0,     0,     0,     0,     0,     0}, //  1 BAND
  {10,  3000, 18000,     0,     0,     0,     0,     0,     0,     0,     0}, //  2 BAND
  {10,  2000, 10000, 18000,     0,     0,     0,     0,     0,     0,     0}, //  3 BAND
  {10,  1000,  2000, 12000, 18000,     0,     0,     0,     0,     0,     0}, //  4 BAND
  {10,  1000,  2000,  7000, 12000, 18000,     0,     0,     0,     0,     0}, //  5 BAND
  {10,  1000,  3000, 11000, 16000, 17000, 18000,     0,     0,     0,     0}, //  6 BAND
  {10,  1000,  3000, 11000, 15000, 16000, 17000, 18000,     0,     0,     0}, //  7 BAND
  {10,  1000,  3000,  9000, 13000, 14000, 15000, 16000, 18000,     0,     0}, //  8 BAND
  {10,  1000,  3000,  6000, 10000, 13000, 15000, 16000, 17000, 18000,     0}, //  9 BAND
  {10,   400,   800,  1500,  3000,  6000,  8000, 11000, 14000, 16000, 18000}  // 10 BAND
};
*/

/*
// 03 HD 2
int FILTER_FREQ[N_BAND][N_FREQ] = {
  {10, 18000,     0,     0,     0,     0,     0,     0,     0,     0,     0}, //  1 BAND
  {10,  6000, 18000,     0,     0,     0,     0,     0,     0,     0,     0}, //  2 BAND
  {10,  1000, 13000, 18000,     0,     0,     0,     0,     0,     0,     0}, //  3 BAND
  {10,  1000, 16000, 17000, 18000,     0,     0,     0,     0,     0,     0}, //  4 BAND
  {10,  1000, 10000, 16000, 17000, 18000,     0,     0,     0,     0,     0}, //  5 BAND
  {10,  1000, 10000, 15000, 16000, 17000, 18000,     0,     0,     0,     0}, //  6 BAND
  {10,  1000, 10000, 14000, 15000, 16000, 17000, 18000,     0,     0,     0}, //  7 BAND
  {10,  1000,  6000, 12000, 14000, 15000, 16000, 17000, 18000,     0,     0}, //  8 BAND
  {10,  1000,  6000, 12000, 13000, 14000, 15000, 16000, 17000, 18000,     0}, //  9 BAND
  {10,  1000,  5000, 10000, 12000, 13000, 14000, 15000, 16000, 17000, 18000}  // 10 BAND
};
*/


// 04 HD 3
int FILTER_FREQ[N_BAND][N_FREQ] = {
  {20, 18000,     0,     0,     0,     0,     0,     0,     0,     0,     0}, //  1 BAND
  {20,  6000, 18000,     0,     0,     0,     0,     0,     0,     0,     0}, //  2 BAND
  {20,   200,  3000, 18000,     0,     0,     0,     0,     0,     0,     0}, //  3 BAND
  {20,   200,   500, 17000, 18000,     0,     0,     0,     0,     0,     0}, //  4 BAND
  {20,   200,   500,  3000, 14000, 18000,     0,     0,     0,     0,     0}, //  5 BAND
  {20,   200,   500,  2000,  6000, 15000, 18000,     0,     0,     0,     0}, //  6 BAND
  {20,   200,   500,  2000,  6000, 14000, 16000, 18000,     0,     0,     0}, //  7 BAND
  {20,   200,   500,  2000,  6000, 14000, 16000, 17000, 18000,     0,     0}, //  8 BAND
  {20,   200,   500,  2000,  6000, 14000, 15000, 16000, 17000, 18000,     0}, //  9 BAND
  {20,   200,   500,  2000,  5000, 13000, 14000, 15000, 16000, 17000, 18000}  // 10 BAND
};


/*
// 05 FM 1
int FILTER_FREQ[N_BAND][N_FREQ] = {
  {10, 13000,     0,     0,     0,     0,     0,     0,     0,     0,     0}, //  1 BAND
  {10,  6000, 13000,     0,     0,     0,     0,     0,     0,     0,     0}, //  2 BAND
  {10,  6000, 12000, 13000,     0,     0,     0,     0,     0,     0,     0}, //  3 BAND
  {10,  2000, 11000, 12000, 13000,     0,     0,     0,     0,     0,     0}, //  4 BAND
  {10,  2000, 10000, 11000, 12000, 13000,     0,     0,     0,     0,     0}, //  5 BAND
  {10,  2000,  6000, 10000, 11000, 12000, 13000,     0,     0,     0,     0}, //  6 BAND
  {10,  2000,  4000,  6000, 10000, 11000, 12000, 13000,     0,     0,     0}, //  7 BAND
  {10,  2000,  4000,  6000,  9000, 10000, 11000, 12000, 13000,     0,     0}, //  8 BAND
  {10,  2000,  3000,  6000,  8000,  9000, 10000, 11000, 12000, 13000,     0}, //  9 BAND
  {10,  2000,  3000,  5000,  7000,  8000,  9000, 10000, 11000, 12000, 13000}  // 10 BAND
};
*/

/*
// 06 FM 2
int FILTER_FREQ[N_BAND][N_FREQ] = {
  {20, 14000,     0,     0,     0,     0,     0,     0,     0,     0,     0}, //  1 BAND
  {20,  6000, 14000,     0,     0,     0,     0,     0,     0,     0,     0}, //  2 BAND
  {20,   200,  3000, 14000,     0,     0,     0,     0,     0,     0,     0}, //  3 BAND
  {20,   200,   500, 12000, 14000,     0,     0,     0,     0,     0,     0}, //  4 BAND
  {20,   200,   500,  3000, 12000, 14000,     0,     0,     0,     0,     0}, //  5 BAND
  {20,   200,   500,  2000,  6000, 13000, 14000,     0,     0,     0,     0}, //  6 BAND
  {20,   200,   500,  2000,  6000, 12000, 13000, 14000,     0,     0,     0}, //  7 BAND
  {20,   200,   500,  2000,  6000, 11000, 12000, 13000, 14000,     0,     0}, //  8 BAND
  {20,   200,   500,  2000,  6000, 10000, 11000, 12000, 13000, 14000,     0}, //  9 BAND
  {20,   200,   500,  2000,  5000,  9000, 10000, 11000, 12000, 13000, 14000}  // 10 BAND
};
*/

//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
#ifdef SAVE_AS_JSON
  #define DEFAULT_CONFIG_FILENAME  "/config.json"
#else
  #define DEFAULT_CONFIG_FILENAME  "/config.bin"
#endif  
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
// I2S Basic Settings
//--------------------------------------------------------------------------------
const int SampleRateFreq = 44100;
const int channelCount =   2;
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
// I2S Input/Output Pinout
//--------------------------------------------------------------------------------
#define I2S_BCK_PIN        26
#define I2S_LRCLK_PIN      27
#define I2S_DOUT_PIN       14
#define I2S_DIN_PIN        13
#define CODEC_ENABLE_PIN   5
#define CLOCK_OUT_PIN      23

#define POT_ADC_IN_PIN     36
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
// Tempo de WDT
//--------------------------------------------------------------------------------
#define WDT_TIMEOUT        10
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
// Número do núcleo
//--------------------------------------------------------------------------------
static uint8_t
CORE_ZERO = 0, // Available for tasks
CORE_ONE = 1; // Core where the main loop runs
//--------------------------------------------------------------------------------
