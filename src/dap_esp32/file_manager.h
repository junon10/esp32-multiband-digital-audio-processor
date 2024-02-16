

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

  if ((f = SPIFFS.open(path, "w")) != NULL) {
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
  TimeZone = -3;

  for (int i = 0; i < ALL_NUM_BANDS; i++)
  {
    if  (i < MAX_NUM_BANDS) Equalizer[i] = DEFAULT_EQ_BAND;
    Protection[i] = DEFAULT_PROTECTION;
    Gain[i] = DEFAULT_GAIN;
    AttackTime[i] = DEFAULT_ATTACK_TIME;
    ReleaseTime[i] = DEFAULT_RELEASE_TIME;
  }

  myAdminUserName = DEFAULT_ADMIN_USERNAME;
  myAdminPassword = DEFAULT_ADMIN_PASSWORD;

  myApSsid = DEFAULT_AP_SSID;
  myApSsid += "_";
  myApSsid += CHIP_ID;

  myApPassword = DEFAULT_AP_PASSWORD;

  myWifiSsid = DEFAULT_WIFI_SSID;
  myWifiPassword = DEFAULT_WIFI_PASSWORD;

  myWifiWithDhcp = DEFAULT_WIFI_WITH_DHCP;
  myWifiIp = DEFAULT_WIFI_IP;
  myWifiGateway = DEFAULT_WIFI_GATEWAY;
  myWifiSubnet = DEFAULT_WIFI_SUBNET;
  myWifiDns = DEFAULT_WIFI_DNS;
}
//--------------------------------------------------------------------------------


//--------------------------------------------------------------------------------
// Json File
//--------------------------------------------------------------------------------
#ifdef SAVE_AS_JSON
void readConfig()
{
  // ArduinoJson > 6.x
  //DynamicJsonDocument doc(2048);

  // ArduinoJson 5.x
  StaticJsonBuffer <2048> jsonBuffer;

  String strJson = readFile(DEFAULT_CONFIG_FILENAME);

  if (!strJson.isEmpty())
  {
    // ArduinoJson > 6.x
    //DeserializationError error = deserializeJson(doc, jsonFromClient);

    // ArduinoJson 5.x
    JsonObject &doc = jsonBuffer.parseObject(strJson);

    // ArduinoJson 5.x
    bool error = !doc.success();

    if (error)
    {
#ifdef SERIAL_LOG      
      Serial.println("\nJson Error from file!");
      Serial.print("\nJson from file = ");
      Serial.println(strJson);
      Serial.println("\nDefault settings is loaded!\n");
#endif
      loadDefaultConfig();
      return;
    }

    InputLevel = (float)doc["Il"];
    OutputLevel = (float)doc["Ol"];
    Clipper = (float)doc["Clip"];
    
    Compressor = (bool)doc["Comp"];
    BandSync = (bool)doc["Sync"];
    Mute = (bool)doc["Mute"];
    Reserved1 = (bool)doc["Res1"];
    Reserved2 = (bool)doc["Res2"];

    NumBands = (int)doc["Nb"];
    PreEmphasis = (float)doc["PreE"];
    PostEmphasis = (float)doc["PostE"];
    StepBy = (float)doc["Step"];
    Echo = (float)doc["Echo"];
    FiltersQFactor = (float)doc["Fqf"];
    TimeZone = (int)doc["Tz"];

    for (int i = 0; i < ALL_NUM_BANDS; i++)
    {
      if (i < MAX_NUM_BANDS) Equalizer[i] = (float)doc["Eq"][i];
      Protection[i] = (float)doc["Prot"][i];
      Gain[i] = (float)doc["Gn"][i];
      AttackTime[i] = (float)doc["Atk"][i];
      ReleaseTime[i] = (float)doc["Rls"][i];
    }

    myAdminUserName = doc["my_admin_user_name"].as<String>();
    myAdminPassword = doc["my_admin_password"].as<String>();
    myApSsid = doc["my_ap_ssid"].as<String>();
    myApPassword = doc["my_ap_password"].as<String>();
    myWifiSsid = doc["my_wifi_ssid"].as<String>();
    myWifiPassword = doc["my_wifi_password"].as<String>();
    myWifiWithDhcp = (bool)doc["my_wifi_with_dhcp"];
    myWifiIp = doc["my_wifi_ip"].as<String>();
    myWifiGateway = doc["my_wifi_gateway"].as<String>();
    myWifiSubnet = doc["my_wifi_subnet"].as<String>();
    myWifiDns = doc["my_wifi_dns"].as<String>();
  }
  else
  {
    loadDefaultConfig();
  }
}
#endif
//--------------------------------------------------------------------------------


//--------------------------------------------------------------------------------
// Json File
//--------------------------------------------------------------------------------
#ifdef SAVE_AS_JSON
bool saveConfig()
{
  String strJson = "";

  StaticJsonBuffer <2048> jsonBuffer;

  // Create the root object
  JsonObject& root = jsonBuffer.createObject();

  root["Il"] = InputLevel;
  root["Ol"] = OutputLevel;
  root["Clip"] = Clipper;

  root["Comp"] = Compressor;
  root["Sync"] = BandSync;
  root["Mute"] = Mute;
  root["Res1"] = Reserved1;
  root["Res2"] = Reserved2;

  root["Nb"] = NumBands;  
  root["PreE"] = PreEmphasis;
  root["PostE"] = PostEmphasis;
  root["Step"] = StepBy;
  root["Echo"] = Echo;
  
  root["Fqf"] = FiltersQFactor;  
  root["Tz"] = TimeZone;

  JsonArray& Eq = root.createNestedArray("Eq");
  for (int i = 0; i < MAX_NUM_BANDS; i++) Eq.add(Equalizer[i]);

  JsonArray& Prot = root.createNestedArray("Prot");
  for (int i = 0; i < ALL_NUM_BANDS; i++) Prot.add(Protection[i]);

  JsonArray& Gn = root.createNestedArray("Gn");
  for (int i = 0; i < ALL_NUM_BANDS; i++) Gn.add(Gain[i]);

  JsonArray& Atk = root.createNestedArray("Atk");
  for (int i = 0; i < ALL_NUM_BANDS; i++) Atk.add(AttackTime[i]);

  JsonArray& Rls = root.createNestedArray("Rls");
  for (int i = 0; i < ALL_NUM_BANDS; i++) Rls.add(ReleaseTime[i]);

  root["my_admin_user_name"] = myAdminUserName;
  root["my_admin_password"] = myAdminPassword;
  root["my_ap_ssid"] = myApSsid;
  root["my_ap_password"] = myApPassword;
  root["my_wifi_ssid"] = myWifiSsid;
  root["my_wifi_password"] = myWifiPassword;
  root["my_wifi_with_dhcp"] = myWifiWithDhcp;
  root["my_wifi_ip"] = myWifiIp;
  root["my_wifi_gateway"] = myWifiGateway;
  root["my_wifi_subnet"] = myWifiSubnet;
  root["my_wifi_dns"] = myWifiDns;

  root.printTo(strJson);

#ifdef SERIAL_LOG
  Serial.print("\nConstructed json string = ");
  Serial.print(strJson);
  Serial.print(" - len = ");
  Serial.println(strJson.length());
  Serial.println();
#endif  

  if (writeFile(DEFAULT_CONFIG_FILENAME, strJson)) {
    Serial.println("\nJson file successfully saved!\n");
    return true;
  } else {
    Serial.println("\nError: The json file could not be saved!\n");
    return false;
  }
}
#endif
//--------------------------------------------------------------------------------



//--------------------------------------------------------------------------------
// Binary File
//--------------------------------------------------------------------------------
#ifndef SAVE_AS_JSON
void readConfig()
{
  int idx = 0;

  File f;

  if ((f = SPIFFS.open(DEFAULT_CONFIG_FILENAME, "r")) != NULL)
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
    TimeZone = cfgRegs[idx].timezone;

    for (int i = 0; i < ALL_NUM_BANDS; i++)
    {
      if (i < MAX_NUM_BANDS) Equalizer[i] = cfgRegs[idx].Equalizer[i];
      Protection[i] = cfgRegs[idx].Protection[i];
      Gain[i] = cfgRegs[idx].Gain[i];
      AttackTime[i] = cfgRegs[idx].AttackTime[i];
      ReleaseTime[i] = cfgRegs[idx].ReleaseTime[i];
    }

    myAdminUserName = cfgRegs[idx].my_admin_user_name;
    myAdminPassword = cfgRegs[idx].my_admin_password;
    myApSsid = cfgRegs[idx].my_ap_ssid;
    myApPassword = cfgRegs[idx].my_ap_password;
    myWifiSsid = cfgRegs[idx].my_wifi_ssid;
    myWifiPassword = cfgRegs[idx].my_wifi_password;
    myWifiWithDhcp = cfgRegs[idx].my_wifi_with_dhcp;
    myWifiIp = cfgRegs[idx].my_wifi_ip;
    myWifiGateway = cfgRegs[idx].my_wifi_gateway;
    myWifiSubnet = cfgRegs[idx].my_wifi_subnet;
    myWifiDns = cfgRegs[idx].my_wifi_dns;

  }
  else
  {
    loadDefaultConfig();
  }

}
#endif
//--------------------------------------------------------------------------------



//--------------------------------------------------------------------------------
// Binary File
//--------------------------------------------------------------------------------
#ifndef SAVE_AS_JSON
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
  cfgRegs[idx].timezone = TimeZone;

  for (int i = 0; i < ALL_NUM_BANDS; i++)
  {
    if (i < MAX_NUM_BANDS) cfgRegs[idx].Equalizer[i] = Equalizer[i];
    cfgRegs[idx].Protection[i] = Protection[i];
    cfgRegs[idx].Gain[i] = Gain[i];
    cfgRegs[idx].AttackTime[i] = AttackTime[i];
    cfgRegs[idx].ReleaseTime[i] = ReleaseTime[i];
  }

  strlcpy(cfgRegs[idx].my_admin_user_name, myAdminUserName.c_str(), sizeof(cfgRegs[idx].my_admin_user_name));
  strlcpy(cfgRegs[idx].my_admin_password, myAdminPassword.c_str(), sizeof(cfgRegs[idx].my_admin_password));
  strlcpy(cfgRegs[idx].my_ap_ssid, myApSsid.c_str(), sizeof(cfgRegs[idx].my_ap_ssid));
  strlcpy(cfgRegs[idx].my_ap_password, myApPassword.c_str(), sizeof(cfgRegs[idx].my_ap_password));
  strlcpy(cfgRegs[idx].my_wifi_ssid, myWifiSsid.c_str(), sizeof(cfgRegs[idx].my_wifi_ssid));
  strlcpy(cfgRegs[idx].my_wifi_password, myWifiPassword.c_str(), sizeof(cfgRegs[idx].my_wifi_password));
  cfgRegs[idx].my_wifi_with_dhcp = myWifiWithDhcp;
  strlcpy(cfgRegs[idx].my_wifi_ip, myWifiIp.c_str(), sizeof(cfgRegs[idx].my_wifi_ip));
  strlcpy(cfgRegs[idx].my_wifi_gateway, myWifiGateway.c_str(), sizeof(cfgRegs[idx].my_wifi_gateway));
  strlcpy(cfgRegs[idx].my_wifi_subnet, myWifiSubnet.c_str(), sizeof(cfgRegs[idx].my_wifi_subnet));
  strlcpy(cfgRegs[idx].my_wifi_dns, myWifiDns.c_str(), sizeof(cfgRegs[idx].my_wifi_dns));

  //--------------------------------------------------------------------------------
  // Save data to the spiffs file system
  // Supports 10 thousand writes
  //--------------------------------------------------------------------------------
  File f;

  if ((f = SPIFFS.open(DEFAULT_CONFIG_FILENAME, "w")) != NULL)
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
#endif
//--------------------------------------------------------------------------------
