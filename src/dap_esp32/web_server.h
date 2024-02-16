
#ifdef WEBSERVER_EN
WebServer server(80);

const char msgCookie0[] = "ESPSESSIONID=0";
const char msgCookie1[] = "ESPSESSIONID=1";


//--------------------------------------------------------------------------------
void makeStaticIp()
{
  if ((myWifiIp != "") &&
      (myWifiGateway != "") &&
      (myWifiSubnet != "") &&
      (myWifiDns != ""))
  {
    if (!myWifiWithDhcp)
    {
      IPAddress wifi_ip;
      IPAddress wifi_gateway;
      IPAddress wifi_subnet;
      IPAddress wifi_dns;

      wifi_ip.fromString(myWifiIp.c_str());
      wifi_gateway.fromString(myWifiGateway.c_str());
      wifi_subnet.fromString(myWifiSubnet.c_str());
      wifi_dns.fromString(myWifiDns.c_str());

      Serial.println(F("Configuring network for static IP"));
      Serial.print(F("IP.............")); Serial.println(myWifiIp);
      Serial.print(F("Gateway........")); Serial.println(myWifiGateway);
      Serial.print(F("Subnet Mask....")); Serial.println(myWifiSubnet);
      Serial.print(F("DNS:...........")); Serial.println(myWifiDns);

      WiFi.config(wifi_ip, wifi_gateway, wifi_subnet, wifi_dns);
    }
    else
    {
      Serial.println(F("Configuring network with DHCP"));
    }
  }
}
//--------------------------------------------------------------------------------




//---------------------------------------------------------------------------------------------------------
const char SYSTEM_RESTALL[] PROGMEM = R"=====(
<!DOCTYPE HTML>
<html lang="pt-br">
<head>
<meta charset="utf-8"/>
<title>#TITLE#</title>
<meta http-equiv="cache-control" content="no-cache">
<meta name="viewport" content="width=device-width, initial-scale=1, user-scalable=no"/>
</head>
<body>
<br>
<h3 id="msg1">Select the files, click send and wait</h3><br>
<form id="file-catcher">
    <input id="file-input" type="file" multiple/><br><br>
    <button type="submit" class="btn"/>
    Submit
    </button>
    <br><br>
    <progress id="progressbar" value="0" max="100"></progress>
</form>
</body>
<script type="text/javascript">

var Msg1 = document.getElementById('msg1');
var ProgressBar1 = document.getElementById('progressbar');

function functionHandlesError(event) {
  Msg1.innerHTML = "Error uploading files!";
}

function functionCompleted(event) {
  Msg1.innerHTML = "Successfully concluded!";
}

function functionCancel(event) {
  Msg1.innerHTML = "Uploads have been cancelled!";
}


(function () { 
//defining filecatcher 
var fileCatcher = document.getElementById('file-catcher'); 

//defining fileinput 
var fileInput = document.getElementById('file-input'); 

//defining File list display 
var fileListDisplay = document.getElementById('file-list-display'); 

//declaring file list 
var fileList = []; 
var renderFileList, sendFile;
 

fileCatcher.addEventListener('submit', function (evnt) {
  evnt.preventDefault();
    fileList.forEach(function (file) {
    sendFile(file);
    });
  });

 
fileInput.addEventListener('change', function (evnt) {
  fileList = [];
  for (var i = 0; i < fileInput.files.length; i++) {
    fileList.push(fileInput.files[i]);
    }
    renderFileList();
  });
 
  renderFileList = function () {
  fileListDisplay.innerHTML = "";
    fileList.forEach(function (file, index) {
    var fileDisplayEl = document.createElement("p");
      fileDisplayEl.innerHTML = (index + 1) + ": " + file.name;
      fileListDisplay.appendChild(fileDisplayEl);
    });
  };
 
  sendFile = function (file) {
    var formData = new FormData();
    var request = new XMLHttpRequest();

    request.upload.addEventListener("progress", uploadProgress, false);
    request.upload.addEventListener("error", functionHandlesError, false); 
    request.upload.addEventListener("load", functionCompleted, false);
    request.upload.addEventListener("abort", functionCancel, false);  

    formData.set("file", file);
    request.open("POST", "/upload");
    request.send(formData);
  };
})();


function uploadProgress(event) {
  if (event.lengthComputable) {
    var percent = Math.round(event.loaded * 100 / event.total); // simple percentage calculation.
    ProgressBar1.value = percent; // updates the value of the progress bar.
    Msg1.innerHTML = "Progress: " + percent + "%";
  } else {
    Msg1.innerHTML = "Unable to compute progress!";
  }
}

</script>
</html>
)=====";
//---------------------------------------------------------------------------------------------------------



//--------------------------------------------------------------------------------
//Check if header is present and correct
bool is_authentified() {
  //Serial.println(F("Enter is_authentified"));
  if (server.hasHeader(F("Cookie"))) {
    //Serial.print(F("Found cookie: "));
    String cookie = server.header(F("Cookie"));
    //Serial.println(cookie);
    if (cookie.indexOf(msgCookie1) != -1) {
      //Serial.println(F("Authentification Successful"));
      return true;
    }
  }
  Serial.println(F("Authentification Failed"));
  return false;
}
//--------------------------------------------------------------------------------



//--------------------------------------------------------------------------------
String getContentType(String filename) {
  if (filename.endsWith(".htm")) return "text/html";
  else if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".png")) return "image/png";
  else if (filename.endsWith(".gif")) return "image/gif";
  else if (filename.endsWith(".jpg")) return "image/jpeg";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  else if (filename.endsWith(".xml")) return "text/xml";
  else if (filename.endsWith(".pdf")) return "application/x-pdf";
  else if (filename.endsWith(".zip")) return "application/x-zip";
  else if (filename.endsWith(".gz")) return "application/x-gzip";
  else if (filename.endsWith(".bin")) return "application/octet-stream";
  return "text/plain";
}
//--------------------------------------------------------------------------------




//--------------------------------------------------------------------------------
bool handleFileRead(String path)
{
  // send the right file to the client (if it exists)

  //Serial.println("handleFileRead: " + path);

  if (path.endsWith("/")) path += "index.html";         // If a folder is requested, send the index file

  String contentType = getContentType(path);            // Get the MIME type

  if (SPIFFS.exists(path)) {                            // If the file exists
    File file = SPIFFS.open(path, "r");                 // Open it
    size_t sent = server.streamFile(file, contentType); // And send it to the client
    file.close();                                       // Then close the file again
    return true;
  }

  //Serial.println("\tFile Not Found");
  return false;                                         // If the file doesn't exist, return false
}
//--------------------------------------------------------------------------------


//--------------------------------------------------------------------------------
void insertStylesheet(String &page)
{
  String s = "<style>"; s += readFile(F("/stylesheet.css")); s += "</style>";
  page.replace(F("<link rel=\"stylesheet\" type=\"text/css\" href=\"stylesheet.css\" media=\"screen\"/>"), s);
}
//--------------------------------------------------------------------------------


//--------------------------------------------------------------------------------
void insertFavicon(String &page)
{
  String s = "";
  s = readFile(F("/favicon.base64"));
  page.replace(F("YourBase64StringHere"), s);
}
//--------------------------------------------------------------------------------

/*
  //--------------------------------------------------------------------------------
  // Remove N occurrences of a substring in a string
  // str = text to search
  // substr = text to be removed
  // idx = occurrence number to be removed
  //--------------------------------------------------------------------------------
  void strRemove(String &str, String substr, int idx)
  {
  String s = str;

  int len = substr.length();

  int j = 0;
  for (int i = 0; i < idx; i++)
  {
    j = s.indexOf(substr, j + len);
    if (j == -1) break;
  }

  if (j != -1)
  {
    s.remove(j, len);
    str = s;
  }
  }
  //--------------------------------------------------------------------------------
*/

//--------------------------------------------------------------------------------
/*
  void insertCompressorBands(String &page) {
  String s1 = readFile(F("/compressor-band.html"));
  String s2 = "", s3 = "";

  // Common to all bands
  s2 = s1;
  s2.replace("#I#", String(ALL_NUM_BANDS));
  s3 += s2;

  // Individual for each band
  for (int i = 0; i < MAX_NUM_BANDS; i++) {
    s2 = s1;
    s2.replace("#I#", String(i + 1));
    s3 += s2;
  }

  page.replace("#COMPRESSORS#", s3);
  }
*/
//--------------------------------------------------------------------------------


//--------------------------------------------------------------------------------
/*
  void insertEqBands(String &page) {
  String s1 = readFile(F("/eq-band.html"));
  String s2 = "", s3 = "";

  for (int i = 0; i < MAX_NUM_BANDS; i++) {
    s2 = s1;
    s2.replace("#I#", String(i + 1));
    s3 += s2;
  }
  page.replace("#EQUALIZER#", s3);
  }
*/
//--------------------------------------------------------------------------------


//--------------------------------------------------------------------------------
void handleFirmwareUploadMsgs()
{
  String Msg = "";

  if (server.hasArg(F("MSGS")))
  {
    String s = server.arg(F("MSGS"));

    if (s.equals("OK"))
    {
      Msg = "Successfully updated to version: " + VERSION + " Chip id: " + CHIP_ID + "<br>";
    }
    server.send(200, F("text/html"), Msg);
  }
}
//--------------------------------------------------------------------------------




//--------------------------------------------------------------------------------
void handleRegister()
{
  String s = readFile(F("/register.html"));

  if (!isGoodFileSystem){
    server.sendHeader(F("Location"), F("/file-upload"));
    server.sendHeader(F("Cache-Control"), F("no-cache"));
    server.send(301);
    return;    
  }

  if (server.hasArg(F("SERIAL_NUMBER")))
  {
    String sn = server.arg(F("SERIAL_NUMBER"));

    //if (!sn.equalsIgnoreCase(antiPiracy.getSerial()))
    //{
    //  antiPiracy.saveSerial(sn);
    //  isLicensed = antiPiracy.licenseIsValid(CHIP_ID);
    //}
  }

  if (isLicensed)
  {
    s.replace(F("#MSG#"), "CHIP ID: " + CHIP_ID + " - Product Activated!");
  }
  else
  {
    s.replace(F("#MSG#"), "CHIP ID: " + CHIP_ID + " - Error: Invalid code!");
  }

  //s.replace(F("#SN#"), antiPiracy.getSerial());
  s.replace(F("#TITLE#"), APPLICATION_TITLE);
  s.replace(F("#TITLE2#"), PRODUCT_ACTIVATION_PAGE_TITLE);
  insertStylesheet(s);
  insertFavicon(s);
  s.replace(F("#MENU_HTML#"), readFile(F("/menu.html")));
  s.replace(F("<b>"), "");
  s.replace(F("</b>"), "");
  server.send(200, F("text/html"), s);
}
//--------------------------------------------------------------------------------






//--------------------------------------------------------------------------------
bool isValidClient()
{

#ifdef IN_DEVELOPMENT
  return true;
#endif

  if (!is_authentified()) {
    server.sendHeader(F("Location"), F("/login"));
    server.sendHeader(F("Cache-Control"), F("no-cache"));
    server.send(301);
    return false;
  }

  if (!isLicensed)
  {
    server.sendHeader(F("Location"), F("/register"));
    server.sendHeader(F("Cache-Control"), F("no-cache"));
    server.send(301);
    return false;
  }

  return true;
}
//--------------------------------------------------------------------------------




//--------------------------------------------------------------------------------
void handleFileDelete()
{
  if (!isValidClient()) return;

  String s = "";
  String page = readFile(F("/file-delete.html"));

  page.replace(F("#TITLE#"), APPLICATION_TITLE);
  page.replace(F("#TITLE2#"), FILE_DELETE_PAGE_TITLE);

  File dir = SPIFFS.open("/");
  File file = dir.openNextFile();

  s += "<b>File System:</b><br><br>";
  s += "Total: " + String(SPIFFS.totalBytes()) + " B<br>";
  s += "Used: " + String(SPIFFS.usedBytes()) + " B<br>";
  s += "Free: " + String(SPIFFS.totalBytes() - SPIFFS.usedBytes()) + " B<br><br>";

  if (server.hasArg(F("FILE-TO-DELETE")))
  {
    String fileToDelete = server.arg(F("FILE-TO-DELETE"));

    if (!fileToDelete.equals(""))
    {
      if (!fileToDelete.startsWith("/")) fileToDelete = "/" + fileToDelete;
      
      if (SPIFFS.exists(fileToDelete.c_str()))
      {
        deleteFile(SPIFFS, fileToDelete.c_str());
        Serial.println("Deleted file -> " + fileToDelete);
      } else {
        Serial.println("Error: File [" + fileToDelete + "] does not exist!");
      }
    }
  }

  s += "<select name='FILE-TO-DELETE' id='FILE-TO-DELETE'>";
  s += "<option value=''>File not selected</option>";

  while (file)
  {
    s += "<option value='" + String(file.name()) + "'>" + String(file.name()) + " " + String(file.size()) + " B</option>";
    file = dir.openNextFile();
  }

  s += "</select><br>";

  insertStylesheet(page);
  insertFavicon(page);
  page.replace(F("#SELECT#"), s);
  page.replace(F("#MENU_HTML#"), readFile(F("/menu.html")));
  server.send(200, F("text/html"), page);
}
//--------------------------------------------------------------------------------





//--------------------------------------------------------------------------------
//login page, also called for disconnect
void handleLogin() {
  String msg;
  if (server.hasHeader(F("Cookie"))) {
    //Serial.print("Found cookie: ");
    String cookie = server.header(F("Cookie"));
    //Serial.println(cookie);
  }
  if (server.hasArg(F("DISCONNECT"))) {
    //Serial.println(F("Disconnection"));
    server.sendHeader(F("Location"), F("/login"));
    server.sendHeader(F("Cache-Control"), F("no-cache"));
    server.sendHeader(F("Set-Cookie"), msgCookie0);
    server.send(301);
    return;
  }
  if (server.hasArg(F("USERNAME")) && server.hasArg(F("PASSWORD"))) {
    if (server.arg(F("USERNAME")) == myAdminUserName &&  server.arg(F("PASSWORD")) == myAdminPassword) {
      server.sendHeader(F("Location"), "/");
      server.sendHeader(F("Cache-Control"), F("no-cache"));
      server.sendHeader(F("Set-Cookie"), msgCookie1);
      server.send(301);
      //Serial.println("Log in Successful");
      return;
    }
    msg = F("Wrong username/password! try again.");
    //Serial.println(F("Log in Failed"));
  }

  String content = readFile(F("/login.html"));
  content.replace(F("#TITLE#"), APPLICATION_TITLE);
  content.replace(F("#TITLE2#"), LOGIN_PAGE_TITLE);
  insertStylesheet(content);
  insertFavicon(content);
  content.replace(F("#MSG#"), msg);
  content.replace(F("#MENU_HTML#"), "");
  server.send(200, F("text/html"), content);
}
//--------------------------------------------------------------------------------



//--------------------------------------------------------------------------------
void handleFileUpload()
{
  if (isGoodFileSystem){
    if (!isValidClient()) return;
  }
  
  // upload a new file to the SPIFFS
  HTTPUpload& upload = server.upload();

  if (upload.status == UPLOAD_FILE_START)
  {
    String filename = upload.filename;

    if (!filename.startsWith("/")) filename = "/" + filename;

    Serial.print("handleFileUpload Name: "); Serial.println(filename);

    fsUploadFile = SPIFFS.open(filename, "w");   // Open the file for writing in SPIFFS (create if it doesn't exist)
    filename = String();
  }
  else if (upload.status == UPLOAD_FILE_WRITE)
  {
    // Write the received bytes to the file SPIFFS
    if (fsUploadFile) fsUploadFile.write(upload.buf, upload.currentSize);
  }
  else if (upload.status == UPLOAD_FILE_END)
  {
    if (fsUploadFile)
    {
      // If the file was successfully created
      fsUploadFile.close();    // Close the file again
      Serial.print("handleFileUpload Size: "); Serial.println(upload.totalSize);
    }
    else
    {
      server.send(500, "text/plain", "500: couldn't create file");
    }
  }
}
//--------------------------------------------------------------------------------



//--------------------------------------------------------------------------------
//root page can be accessed only if authentification is ok
void handleRoot()
{

  if (!isValidClient()) return;

  String s = readFile(F("/index.html"));
  String lb = "";

  s.replace(F("#TITLE#"), APPLICATION_TITLE);
  s.replace(F("#TITLE2#"), ROOT_PAGE_TITLE);
  insertStylesheet(s);
  insertFavicon(s);

  s.replace(F("#MENU_HTML#"), readFile(F("/menu.html")));

  // Audio Level {
  s.replace("#Il#", String(InputLevel * 10.0f, 1));
  s.replace("#MinIl#", String(MIN_INPUT_LEVEL * 10.0f, 1));
  s.replace("#MaxIl#", String(MAX_INPUT_LEVEL * 10.0f, 1));

  s.replace("#Ol#", String(OutputLevel * 10.0f, 1));
  s.replace("#MinOl#", String(MIN_OUTPUT_LEVEL * 10.0f, 1));
  s.replace("#MaxOl#", String(MAX_OUTPUT_LEVEL * 10.0f, 1));

  s.replace("#Bal#", String(Balance * 10.0f, 1));
  s.replace("#MinBal#", String(MIN_BALANCE * 10.0f, 1));
  s.replace("#MaxBal#", String(MAX_BALANCE * 10.0f, 1));

  s.replace("#Clip#", String(Clipper * 10.0f, 1));
  s.replace("#MinClip#", String(MIN_CLIPPER * 10.0f, 1));
  s.replace("#MaxClip#", String(MAX_CLIPPER * 10.0f, 1));
  // } Audio Level

  // Main {
  if (Compressor) {
    s.replace("#CkComp#", "checked");
  } else {
    s.replace("#CkComp#", "");
  }

  if (BandSync) {
    s.replace("#CkSync#", "checked");
  } else {
    s.replace("#CkSync#", "");
  }

  if (Mute) {
    s.replace("#CkMute#", "checked");
  } else {
    s.replace("#CkMute#", "");
  }

  if (Reserved1) {
    s.replace("#CkRes1#", "checked");
  } else {
    s.replace("#CkRes1#", "");
  }

  if (Reserved2) {
    s.replace("#CkRes2#", "checked");
  } else {
    s.replace("#CkRes2#", "");
  }

  s.replace("#Nb#", String(NumBands));
  s.replace("#MinNb#", String(MIN_NUM_BANDS));
  s.replace("#MaxNb#", String(MAX_NUM_BANDS));

  s.replace("#PreE#", String(PreEmphasis * 10.0f, 1));
  s.replace("#MinPreE#", String(MIN_PRE_EMPHASIS * 10.0f, 1));
  s.replace("#MaxPreE#", String(MAX_PRE_EMPHASIS * 10.0f, 1));

  s.replace("#PostE#", String(PostEmphasis * 10.0f, 1));
  s.replace("#MinPostE#", String(MIN_POST_EMPHASIS * 10.0f, 1));
  s.replace("#MaxPostE#", String(MAX_POST_EMPHASIS * 10.0f, 1));

  s.replace("#Step#", String(StepBy * 10.0f));
  s.replace("#MinStep#", String(MIN_STEP_BY * 10.0f, 1));
  s.replace("#MaxStep#", String(MAX_STEP_BY * 10.0f, 1));

  s.replace("#Echo#", String(Echo * 100.0f, 1));
  s.replace("#MinEcho#", String(MIN_ECHO * 100.0f, 1));
  s.replace("#MaxEcho#", String(MAX_ECHO * 100.0f, 1));
  // } Main

  // Eq {
  s.replace("#MinEq#", String(MIN_EQ_BAND * 10.0f, 1));
  s.replace("#MaxEq#", String(MAX_EQ_BAND * 10.0f, 1));
  // } Eq

  // Compressor Bands {
  s.replace("#MinProt#", String(MIN_PROTECTION * 10.0f, 1));
  s.replace("#MaxProt#", String(MAX_PROTECTION * 10.0f, 1));

  s.replace("#MinGn#", String(MIN_GAIN, 1));
  s.replace("#MaxGn#", String(MAX_GAIN, 1));

  s.replace("#MinAtk#", String(MIN_ATTACK_TIME));
  s.replace("#MaxAtk#", String(MAX_ATTACK_TIME));

  s.replace("#MinRls#", String(MIN_RELEASE_TIME));
  s.replace("#MaxRls#", String(MAX_RELEASE_TIME));
  // } Compressor bands

  //Serial.println("Debug page = " + s);

  server.send(200, F("text/html"), s);
}
//--------------------------------------------------------------------------------


//--------------------------------------------------------------------------------
void handleJson()
{

  // ArduinoJson > 6.x
  //DynamicJsonDocument doc(2048);

  // ArduinoJson 5.x
  StaticJsonBuffer <2048> jsonBufferFromClient, jsonBufferToClient;

  String jsonToClient = "", msgs = "";
  bool Save = false, ReadData = false, WriteData = false;

  if (!isValidClient()) return;

  // Receives settings from the client
  if (server.hasArg("jsonToServer"))
  {
    String jsonFromClient = server.arg("jsonToServer");

    // ArduinoJson > 6.x
    //DeserializationError error = deserializeJson(doc, jsonFromClient);

    // ArduinoJson 5.x
    JsonObject &doc = jsonBufferFromClient.parseObject(jsonFromClient);

    // ArduinoJson 5.x
    bool error = !doc.success();

    if (error)
    {
#ifdef SERIAL_LOG
      Serial.println("\nJson Error from Client!");
      Serial.println(jsonFromClient);
#endif
      return;
    }

    ReadData = (bool)doc["ReadData"];
    WriteData = (bool)doc["WriteData"];

    if (WriteData)
    {
      InputLevel = (float)doc["Il"] / 10.0f;
      OutputLevel = (float)doc["Ol"] / 10.0f;
      Balance = (float)doc["Bal"] / 10.0f;
      Clipper = (float)doc["Clip"] / 10.0f;

      Compressor = (bool)doc["Comp"];
      BandSync = (bool)doc["Sync"];
      Mute = (bool)doc["Mute"];
      Reserved1 = (bool)doc["Res1"];
      Reserved2 = (bool)doc["Res2"];

      NumBands = (int)doc["Nb"];
      PreEmphasis = (float)doc["PreE"] / 10.0f;
      PostEmphasis = (float)doc["PostE"] / 10.0f;
      StepBy = (float)doc["Step"] / 10.0f;
      Echo = (float)doc["Echo"] / 100.0f;

      for (int i = 0; i < ALL_NUM_BANDS; i++)
      {
        if (i < MAX_NUM_BANDS) Equalizer[i] = (float)doc["Eq"][i] / 10.0f;
        Protection[i] = (float)doc["Prot"][i] / 10.0f;
        Gain[i] = (float)doc["Gn"][i];
        AttackTime[i] = (float)doc["Atk"][i];
        ReleaseTime[i] = (float)doc["Rls"][i];
      }

      Save = (bool)doc["Save"];

      commitConfig();

      if (Save)
      {
        saveConfig();
        msgs = "Settings saved successfully!";
      } else {
        msgs = "";
      }
    } // if (WriteData)


    // Sends information to the client
    //
    // Create the root object
    JsonObject& root = jsonBufferToClient.createObject();

    JsonArray& Freq = root.createNestedArray("Freq");
    for (int i = 0; i < ALL_NUM_BANDS; i++) Freq.add(getFilterFreq(i));

    root["msgs"] = msgs;
    root["ReadData"] = ReadData;
    root["WriteData"] = WriteData;

    if (ReadData)
    {
      //teste
      root["Il"] = InputLevel * 10.0f;
      root["Ol"] = OutputLevel * 10.0f;
      root["Bal"] = Balance * 10.0f;
      root["Clip"] = Clipper * 10.0f;

      root["Comp"] = Compressor;
      root["Sync"] = BandSync;
      root["Mute"] = Mute;
      root["Res1"] = Reserved1;
      root["Res2"] = Reserved2;

      root["Nb"] = NumBands;
      root["PreE"] = PreEmphasis * 10.0f;
      root["PostE"] = PostEmphasis * 10.0f;
      root["Step"] = StepBy * 10.0f;
      root["Echo"] = Echo * 100.0f;
      //teste

      JsonArray& Eq = root.createNestedArray("Eq");
      for (int i = 0; i < MAX_NUM_BANDS; i++) Eq.add(Equalizer[i] * 10.0);

      JsonArray& Prot = root.createNestedArray("Prot");
      for (int i = 0; i < ALL_NUM_BANDS; i++) Prot.add(Protection[i] * 10);

      JsonArray& Gn = root.createNestedArray("Gn");
      for (int i = 0; i < ALL_NUM_BANDS; i++) Gn.add(Gain[i]);

      JsonArray& Atk = root.createNestedArray("Atk");
      for (int i = 0; i < ALL_NUM_BANDS; i++) Atk.add(AttackTime[i]);

      JsonArray& Rls = root.createNestedArray("Rls");
      for (int i = 0; i < ALL_NUM_BANDS; i++) Rls.add(ReleaseTime[i]);
    }


    JsonArray& Vu = root.createNestedArray("Vu");
    for (int i = 0; i < MAX_NUM_BANDS; i++)
    {
      Vu.add((int)linear_2_decibel(VuArrValue[i]));
      VuArrValue[i] = 0.0;
    }

    root.printTo(jsonToClient);

#ifdef SERIAL_LOG
    Serial.print("jsonToClient = ");
    Serial.println(jsonToClient);
#endif

    server.send(200, F("text/json"), jsonToClient);
  }
}
//--------------------------------------------------------------------------------


//--------------------------------------------------------------------------------
void handleAllSettings()
{
  if (!isValidClient()) return;
  String s = readFile(F("/all-settings.html"));
  s.replace(F("#TITLE#"), APPLICATION_TITLE);
  s.replace(F("#TITLE2#"), ALL_SETTINGS_PAGE_TITLE);
  insertStylesheet(s);
  insertFavicon(s);
  s.replace(F("#MENU_HTML#"), readFile(F("/menu.html")));
  server.send(200, F("text/html"), s);
}
//--------------------------------------------------------------------------------



//--------------------------------------------------------------------------------
void handleNetworkSettings()
{
  if (!isValidClient()) return;

  String s = readFile(F("/network-settings.html"));

  s.replace(F("#TITLE#"), APPLICATION_TITLE);
  s.replace(F("#TITLE2#"), NETWORK_PAGE_TITLE);

  if ((server.hasArg(F("myAdminUserName"))) &&
      (server.hasArg(F("myAdminPassword"))) &&
      (server.hasArg(F("myApSsid"))) &&
      (server.hasArg(F("myApPassword"))) &&
      (server.hasArg(F("myWifiSsid"))) &&
      (server.hasArg(F("myWifiPassword"))) &&
      (server.hasArg(F("myWifiIp"))) &&
      (server.hasArg(F("myWifiGateway"))) &&
      (server.hasArg(F("myWifiSubnet"))) &&
      (server.hasArg(F("myWifiDns"))))
  {
    myAdminUserName = server.arg(F("myAdminUserName"));
    myAdminPassword = server.arg(F("myAdminPassword"));

    myApSsid = server.arg(F("myApSsid"));
    myApPassword = server.arg(F("myApPassword"));

    myWifiSsid = server.arg(F("myWifiSsid"));
    myWifiPassword = server.arg(F("myWifiPassword"));

    if (server.hasArg(F("opt")))
    {
      if (server.arg(F("opt")) == "dhcp")
        myWifiWithDhcp = true; else myWifiWithDhcp = false;
    }

    myWifiIp = server.arg(F("myWifiIp"));
    myWifiGateway = server.arg(F("myWifiGateway"));
    myWifiSubnet = server.arg(F("myWifiSubnet"));
    myWifiDns = server.arg(F("myWifiDns"));

    saveConfig();
  }

  // displays the values saved in the eeprom memory in the config page
  s.replace(F("#myAdminUserName#"), myAdminUserName);
  s.replace(F("#myAdminPassword#"), myAdminPassword);

  s.replace(F("#myApSsid#"), myApSsid);
  s.replace(F("#myApPassword#"), myApPassword);

  s.replace(F("#myWifiSsid#"), myWifiSsid);
  s.replace(F("#myWifiPassword#"), myWifiPassword);

  if (myWifiWithDhcp)
  {
    s.replace(F("#DHCP_CHECKED#"), "checked");
    s.replace(F("#STATIC_IP_CHECKED#"), "");
  }
  else
  {
    s.replace(F("#DHCP_CHECKED#"), "");
    s.replace(F("#STATIC_IP_CHECKED#"), "checked");
  }

  s.replace(F("#myWifiIp#"), myWifiIp);
  s.replace(F("#myWifiGateway#"), myWifiGateway);
  s.replace(F("#myWifiSubnet#"), myWifiSubnet);
  s.replace(F("#myWifiDns#"), myWifiDns);

  insertStylesheet(s);
  insertFavicon(s);
  s.replace(F("#MENU_HTML#"), readFile(F("/menu.html")));
  s.replace(F("<b>"), "");
  s.replace(F("</b>"), "");

  server.send(200, F("text/html"), s);
}
//--------------------------------------------------------------------------------




//--------------------------------------------------------------------------------
// Function that will be executed when a request is made
// on a route that has not been declared
//--------------------------------------------------------------------------------
void onNotFound()
{
  server.send(404, F("text/html"), readFile(F("/not-found.html")));
}
//--------------------------------------------------------------------------------





//--------------------------------------------------------------------------------
void init_webserver()
{
  Serial.println(F("HTTP server starting..."));

  // if the client posts to the upload page
  // Send status 200 (OK) to tell the client we are ready to receive
  // Receive and save the file
  server.on("/upload", HTTP_POST, []() {
    server.send(200);
  }, handleFileUpload);



  server.on(F("/firmware-upgrade"), []() {
    if (!isValidClient()) return;
    String s = readFile(F("/firmware-upgrade.html"));
    s.replace(F("#TITLE#"), APPLICATION_TITLE);
    s.replace(F("#TITLE2#"), FIRMWARE_UPDATE_PAGE_TITLE);
    insertStylesheet(s);
    insertFavicon(s);
    s.replace(F("#MENU_HTML#"), readFile(F("/menu.html")));
    s.replace(F("#MSG#"), "Version: " + VERSION + " Chip id: " + CHIP_ID + "<br>");
    server.send(200, F("text/html"), s);
  });



  /*handling uploading firmware file */
  server.on(F("/update"), HTTP_POST, []() {

    server.sendHeader(F("Connection"), F("close"));
    server.send(200);

    Serial.println(F("The system will reboot in a few seconds, wait..."));

    RestartTimeX100ms = DEFAULT_RESTART_TIME;
    espRestart = true;

    ForcedMute = true;
    MuteTimeX100ms = DEFAULT_FORCED_MUTE_TIME;

  }, []() {

    //if (is_authentified())
    //{
    //  Compressor = false;
            
      MuteTimeX100ms = DEFAULT_FORCED_MUTE_TIME;

      HTTPUpload& upload = server.upload();

      if (upload.status == UPLOAD_FILE_START) {
        Serial.printf("Update: %s\n", upload.filename.c_str());

        ForcedMute = true;
        MuteTimeX100ms = DEFAULT_FORCED_MUTE_TIME;

        if (!Update.begin(UPDATE_SIZE_UNKNOWN)) { //start with max available size
          Update.printError(Serial);
        }
      } else if (upload.status == UPLOAD_FILE_WRITE) {

        ForcedMute = true;
        MuteTimeX100ms = DEFAULT_FORCED_MUTE_TIME;

        /* flashing firmware to ESP*/
        if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
          Update.printError(Serial);
        }
      } else if (upload.status == UPLOAD_FILE_END) {
        if (Update.end(true)) { //true to set the size to the current progress
          Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
          server.sendHeader("Location", "/firmware-upgrade");
          server.sendHeader(F("Cache-Control"), F("no-cache"));
          server.send(303);
        } else {
          Update.printError(Serial);
        }
      }
    //}
  });



  server.on(F("/backup-and-restall"), []() {

    if (!isValidClient()) return;

    String s = readFile(F("/backup-and-restall.html"));
    s.replace(F("#TITLE#"), APPLICATION_TITLE);
    s.replace(F("#TITLE2#"), BACKUP_AND_RESTALL_PAGE_TITLE);
    insertStylesheet(s);
    insertFavicon(s);
    s.replace("#FILENAME#", DEFAULT_CONFIG_FILENAME);
    s.replace(F("#MENU_HTML#"), readFile(F("/menu.html")));
    server.send(200, F("text/html"), s);
  });



  server.on(F("/file-upload"), []() {
    
    String s = "";

    if (!isGoodFileSystem){
      s = SYSTEM_RESTALL;
    } else {

      s = readFile(F("/file-upload.html"));
    
      if (!isValidClient()) return;
    
      s.replace(F("#TITLE#"), APPLICATION_TITLE);
      s.replace(F("#TITLE2#"), FILE_UPLOAD_PAGE_TITLE);
      insertStylesheet(s);
      insertFavicon(s);
      s.replace(F("#MENU_HTML#"), readFile(F("/menu.html")));
    }
    
    server.send(200, F("text/html"), s);
  });



  server.on(DEFAULT_CONFIG_FILENAME, HTTP_GET, []() {

    if (!isValidClient()) return;

    if (!handleFileRead(DEFAULT_CONFIG_FILENAME))
    {
      server.send(404, F("text/html"), readFile(F("/not-found.html")));
    }

  });


  // if the client posts to the upload page
  // Send status 200 (OK) to tell the client we are ready to receive
  // Receive and save the file
  server.on("/upload", HTTP_POST, []() {
    server.send(200);
  }, handleFileUpload);



  server.on(F("/"), handleRoot);

  //  server.on(F("/status-msgs"), HTTP_POST, handleStatusMsgs);
  //  server.on(F("/login-msgs"), HTTP_POST, handleLoginMsgs);
  server.on(F("/firmware-upload-msgs"), HTTP_POST, handleFirmwareUploadMsgs);

  server.on(F("/all-settings"), handleAllSettings);

  server.on(F("/network-settings"), handleNetworkSettings);
  server.on(F("/network-settings"), HTTP_POST, handleNetworkSettings);

  server.on(F("/register"), handleRegister);
  server.on(F("/register"), HTTP_POST, handleRegister);

  server.on(F("/set"), HTTP_POST, handleJson);

  server.on(F("/login"), handleLogin);

  server.on(F("/file-delete"), handleFileDelete);
  server.on(F("/file-delete"), HTTP_POST, handleFileDelete);

  server.onNotFound(onNotFound);

  //here the list of headers to be recorded
  const char * headerkeys[] = {"User-Agent", "Cookie"} ;
  size_t headerkeyssize = sizeof(headerkeys) / sizeof(char*);
  //ask server to track these headers
  server.collectHeaders(headerkeys, headerkeyssize);

  server.begin();
}
//--------------------------------------------------------------------------------


//--------------------------------------------------------------------------------
bool isValidWifiNetwork()
{
  if (myWifiSsid.isEmpty() && myWifiPassword.isEmpty())
    return false;
  else
    return true;
}
//--------------------------------------------------------------------------------


//--------------------------------------------------------------------------------
void configureWiFi()
{
  WiFi.mode(WIFI_AP_STA);

  if (isValidWifiNetwork()) WiFi.begin(myWifiSsid.c_str(), myWifiPassword.c_str());

  WiFi.softAP(myApSsid.c_str(), myApPassword.c_str());

  init_webserver();

  //ftpSrv.begin(myAdminUserName.c_str(), myAdminPassword.c_str());
}
//--------------------------------------------------------------------------------


//--------------------------------------------------------------------------------
void wifiConnectionTask(void * pvParameters)
{
  /* Block for 1000ms. */
  const TickType_t xDelay = 1000 / portTICK_PERIOD_MS;

  esp_task_wdt_add(NULL);

  while (true)
  {
    // If WiFi is not connected
    if ((WiFi.status() != WL_CONNECTED) && (isValidWifiNetwork()))
    {
      Serial.println(F("WiFi: Connecting..."));

      int t = 1;

      // Wait while not connected
      while (WiFi.status() != WL_CONNECTED)
      {
        Serial.print(String(t) + ", ");
        vTaskDelay(xDelay);

        if (t == 10)
        {
          WiFi.reconnect();
        }

        if (t == 20)
        {
          t = 0;
          feedTheDog();
          Serial.println();
          Serial.println();
          Serial.println(F("Incorrect SSID/PASSWORD, go to http://192.168.4.1 and change your network settings!"));
          Serial.println(F("Resetting WiFi connection after 20 seconds, and trying again..."));
          Serial.println();
        }

        t++;
      }

      Serial.println();
      Serial.print(F("WiFi: Connected to "));
      Serial.println(WiFi.SSID() + ", IP:" + WiFi.localIP().toString() + "\n");
      Serial.println();

      isWifiConnected = true;
      makeStaticIp();
      //ntp_ds1307.set_wifi_status(isWifiConnected);
      //ntp_ds1307.setup_ntp();
    }
    else
    {
      if (!isWifiConnected)
      {
        Serial.println();
        Serial.println(F("SSID/PASSWORD not configured, go to http://192.168.4.1 and change your network settings!"));
        Serial.println();
      }
    }

    feedTheDog();
    vTaskDelay(xDelay);
  }
}
//--------------------------------------------------------------------------------


//--------------------------------------------------------------------------------
void webserverTask(void * pvParameters)
{
  // Block for 1ms.
  const TickType_t xDelay = 1 / portTICK_PERIOD_MS;

  esp_task_wdt_add(NULL);

  while (true)
  {
    server.handleClient();
    feedTheDog();
    vTaskDelay(xDelay);
  }
}
//--------------------------------------------------------------------------------

#endif // #ifdef WEBSERVER_EN
