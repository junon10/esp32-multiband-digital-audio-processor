

//--------------------------------------------------------------------------------
void init_file_manager()
{
  Serial.print(F("\nMounting File System: "));
  if (SPIFFS.begin()) {
    Serial.println(F("Ok"));
  } else {
    Serial.println(F("Failure!"));
    Serial.println(F("Formatting Internal Storage..."));
    if (SPIFFS.format()) {
      Serial.println(F("Formatting Complete!"));
      if (SPIFFS.begin()) {
        Serial.println(F("File System Mounted Successfully!"));
      } else {
        Serial.println(F("Failure!"));
      }
    }
  }
}
//--------------------------------------------------------------------------------


/*
//--------------------------------------------------------------------------------
String readFile(String path) {
  String S = "";

  if (SPIFFS.exists(path)) {
    File f = SPIFFS.open(path, "r");

    if (f && f.size()) {
      while (f.available()) {
        S += char(f.read());
      }
      f.close();
    } else {
#ifdef SERIAL_LOG      
      Serial.println("Error: File " + path + " cannot be read!");
#endif
    }
  } else {
#ifdef SERIAL_LOG    
    Serial.println("Error: File " + path + " does not exist!");
#endif
  }
  return (S);
}
//--------------------------------------------------------------------------------


//--------------------------------------------------------------------------------
bool writeFile(String path, String content) {
  File f;

  if ((f = SPIFFS.open(path, "w")) > 0) { // != NULL
    f.write((uint8_t *)content.c_str(), content.length());
    f.close();
  } else {
#ifdef SERIAL_LOG
    Serial.print(F("Error! Writing to the file "));
    Serial.println(path);
#endif
    return false;
  }
  return true;
}
//--------------------------------------------------------------------------------


//--------------------------------------------------------------------------------
String getSpiffsInfo()
{
  char str[100];
  String S = "";

  S += F("File system information:\n\n");

  File dir = SPIFFS.open("/");

  File file = dir.openNextFile();

  while (file)
  {
    sprintf(str, " %s - %u bytes\n", file.name(), file.size());
    S += str;
    file = dir.openNextFile();
  }

  sprintf(str, "Total Bytes: %u\nUsed Bytes: %u\nFree Bytes: %u\n",
          SPIFFS.totalBytes(),
          SPIFFS.usedBytes(),
          SPIFFS.totalBytes() - SPIFFS.usedBytes());

  S += "\n";
  S += str;
  return S;
}
//--------------------------------------------------------------------------------




//--------------------------------------------------------------------------------
void deleteFile(fs::FS &fs, const char * path) {
  Serial.printf("Deleting file: %s\n", path);
  if (fs.remove(path)) {
    Serial.println("File deleted");
  } else {
    Serial.println("Delete failed");
  }
}
//--------------------------------------------------------------------------------
*/



//--------------------------------------------------------------------------------
// Get default settings
//--------------------------------------------------------------------------------
void loadDefaultConfig()
{
  InputLevel = DEFAULT_INPUT_LEVEL;
  OutputLevel = DEFAULT_OUTPUT_LEVEL;
  Clipper = DEFAULT_CLIPPER;
  
  Compressor = DEFAULT_COMPRESSOR;
  BandSync = DEFAULT_BAND_SYNC;
  Mute = DEFAULT_MUTE;
  Reserved1 = DEFAULT_RESERVED1;
  Reserved2 = DEFAULT_RESERVED2;

  NumBands = DEFAULT_NUM_BANDS;
  PreEmphasis = DEFAULT_PRE_EMPHASIS;
  PostEmphasis = DEFAULT_POST_EMPHASIS;
  StepBy = DEFAULT_STEP_BY;
  Echo = DEFAULT_ECHO;

  FiltersQFactor = DEFAULT_FILTERS_Q_FACTOR;

  for (int i = 0; i < ALL_NUM_BANDS; i++)
  {
    if  (i < MAX_NUM_BANDS) Equalizer[i] = DEFAULT_EQ_BAND;
    Protection[i] = DEFAULT_PROTECTION;
    Gain[i] = DEFAULT_GAIN;
    AttackTime[i] = DEFAULT_ATTACK_TIME;
    ReleaseTime[i] = DEFAULT_RELEASE_TIME;
  }
}
//--------------------------------------------------------------------------------



//--------------------------------------------------------------------------------
// Binary File
//--------------------------------------------------------------------------------
void readConfig()
{
  int idx = 0;

  File f;

  if ((f = SPIFFS.open(DEFAULT_CONFIG_FILENAME, "r")) > 0) // != NULL
  {
    f.read((uint8_t *)cfgRegs, sizeof(CfgType) * NUM_REGS);
    f.close();
  } else {
    Serial.println(F("\nError opening configuration binary file!\n"));
    return;
  }

  if (cfgRegs[idx].write_status == CONFIG_TAG)
  {

    InputLevel = cfgRegs[idx].InputLevel;
    OutputLevel = cfgRegs[idx].OutputLevel;
    Clipper = cfgRegs[idx].Clipper;

    Compressor = cfgRegs[idx].Compressor;
    BandSync = cfgRegs[idx].BandSync;
    Mute = cfgRegs[idx].Mute;
    Reserved1 = cfgRegs[idx].Reserved1;
    Reserved2 = cfgRegs[idx].Reserved2;
        
    NumBands = cfgRegs[idx].NumBands;
    PreEmphasis = cfgRegs[idx].PreEmphasis;
    PostEmphasis = cfgRegs[idx].PostEmphasis;
    StepBy = cfgRegs[idx].StepBy;
    Echo = cfgRegs[idx].Echo;

    FiltersQFactor = cfgRegs[idx].FiltersQFactor;

    for (int i = 0; i < ALL_NUM_BANDS; i++)
    {
      if (i < MAX_NUM_BANDS) Equalizer[i] = cfgRegs[idx].Equalizer[i];
      Protection[i] = cfgRegs[idx].Protection[i];
      Gain[i] = cfgRegs[idx].Gain[i];
      AttackTime[i] = cfgRegs[idx].AttackTime[i];
      ReleaseTime[i] = cfgRegs[idx].ReleaseTime[i];
    }

  }
  else
  {
    loadDefaultConfig();
  }

}
//--------------------------------------------------------------------------------



//--------------------------------------------------------------------------------
// Binary File
//--------------------------------------------------------------------------------
bool saveConfig()
{
  int idx = 0;

  cfgRegs[idx].write_status = CONFIG_TAG;

  cfgRegs[idx].InputLevel = InputLevel;
  cfgRegs[idx].OutputLevel = OutputLevel;
  cfgRegs[idx].Clipper = Clipper;

  cfgRegs[idx].Compressor = Compressor;
  cfgRegs[idx].BandSync = BandSync;
  cfgRegs[idx].Mute = Mute;
  cfgRegs[idx].Reserved1 = Reserved1;
  cfgRegs[idx].Reserved2 = Reserved2;

  cfgRegs[idx].NumBands = NumBands;  
  cfgRegs[idx].PreEmphasis = PreEmphasis;
  cfgRegs[idx].PostEmphasis = PostEmphasis;
  cfgRegs[idx].StepBy = StepBy;
  cfgRegs[idx].Echo = Echo;

  cfgRegs[idx].FiltersQFactor = FiltersQFactor;
  
  for (int i = 0; i < ALL_NUM_BANDS; i++)
  {
    if (i < MAX_NUM_BANDS) cfgRegs[idx].Equalizer[i] = Equalizer[i];
    cfgRegs[idx].Protection[i] = Protection[i];
    cfgRegs[idx].Gain[i] = Gain[i];
    cfgRegs[idx].AttackTime[i] = AttackTime[i];
    cfgRegs[idx].ReleaseTime[i] = ReleaseTime[i];
  }

  //--------------------------------------------------------------------------------
  // Save data to the spiffs file system
  // Supports 10 thousand writes
  //--------------------------------------------------------------------------------
  File f;

  if ((f = SPIFFS.open(DEFAULT_CONFIG_FILENAME, "w")) > 0) // != NULL
  {
    f.write((uint8_t *)cfgRegs, sizeof(CfgType) * NUM_REGS);
    f.close();
    return true;
  } else {
    Serial.println(F("\nError writing configuration binary file!\n"));
    return false;
  }
  //--------------------------------------------------------------------------------

  //--------------------------------------------------------------------------------
  // Save data to external 24c16 eeprom memory
  // Supports more than 1 million writes
  //--------------------------------------------------------------------------------
  //eeprom24c16.writeBytes(0, sizeof(CfgType) * NUM_REGS, (byte *)cfgRegs);
  //--------------------------------------------------------------------------------
}
//--------------------------------------------------------------------------------
