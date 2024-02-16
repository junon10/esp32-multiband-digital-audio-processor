
const int SEP_COUNT = 60;

#ifdef PTBR_LANG
const char TEXT_MUTE[] = "Mudo";
const char TEXT_COMPRESSOR[] = "Compressor";
const char TEXT_UNKNOWN_CMD[] = "Erro: Comando desconhecido!";
const char TEXT_SAVED[] = "Ajustes salvos com sucesso!";
const char TEXT_ERROR_SAVING_FILE[] = "Erro ao salvar arquivo!";
const char TEXT_PLS_CHOOSE_AN_OPTION[] = "Por favor escolha uma opção:";
const char TEXT_RESPONSE[] = "Resposta";
const char TEXT_ENABLED[] = "Ligado";
const char TEXT_DISABLED[] = "Desligado";
#endif

#ifdef ENG_LANG
const char TEXT_MUTE[] = "Mute";
const char TEXT_COMPRESSOR[] = "Compressor";
const char TEXT_UNKNOWN_CMD[] = "Error: Unknown Command!";
const char TEXT_SAVED[] = "Settings saved successfully!";
const char TEXT_ERROR_SAVING_FILE[] = "Error saving file!";
const char TEXT_PLS_CHOOSE_AN_OPTION[] = "Please choose an option:";
const char TEXT_RESPONSE[] = "Response";
const char TEXT_ENABLED[] = "Enabled";
const char TEXT_DISABLED[] = "Disabled";
#endif



enum {
  IDX_INPUT_LEVEL = 1,
  IDX_OUTPUT_LEVEL,
  IDX_BALANCE,
  IDX_CLIPPER,

  IDX_COMPRESSOR,
  IDX_MUTE,
  IDX_RESERVED1,
  IDX_RESERVED2,

  IDX_NUM_BANDS,
  IDX_PRE_EMPHASIS,
  IDX_POST_EMPHASIS,
  IDX_STEP_BY,
  IDX_ECHO,

  IDX_PROTECTION,
  IDX_GAIN,
  IDX_ATTACK_TIME,
  IDX_RELEASE_TIME,

  IDX_FILTERS_Q_FACTOR,

  IDX_SAVE,
  IDX_RESET_TO_DEFAULT,
  IDX_EQ_BAND
};

const char LB_INPUT_LEVEL[] = "INPUT LEVEL";
const char LB_OUTPUT_LEVEL[] = "OUTPUT LEVEL";
const char LB_BALANCE[] = "BALANCE";
const char LB_CLIPPER[] = "CLIPPER";

const char LB_PROTECTION[] = "PROTECTION";
const char LB_GAIN[] = "GAIN";
const char LB_ATTACK_TIME[] = "ATTACK TIME";
const char LB_RELEASE_TIME[] = "RELEASE TIME";

const char LB_COMPRESSOR[] = "COMPRESSOR";
const char LB_MUTE[] = "MUTE";
const char LB_RESERVED1[] = "RESERVED1";
const char LB_RESERVED2[] = "RESERVED2";

const char LB_NUM_BANDS[] = "NUM BANDS";
const char LB_PRE_EMPHASIS[] = "PRE EMPHASIS";
const char LB_POST_EMPHASIS[] = "POST EMPHASIS";
const char LB_STEP_BY[] = "STEP";
const char LB_ECHO[] = "ECHO";

const char LB_FILTERS_Q_FACTOR[] = "FILTERS Q FACTOR";

const char LB_SAVE[] = "SAVE";
const char LB_RESET_TO_DEFAULT[] = "RESET TO DEFAULT";
const char LB_EQ_BAND[] = "EQ BAND";


String Separator(int len)
{
  String s = "\n";
  for (int i = 0; i < len; i++) s += "-";
  s += "\n";
  return s;
}


void printFilterFrequencies()
{
  Serial.println(Separator(SEP_COUNT));

  Serial.println("Filter Frequencies:\n");

  for (int i = NumBands - 1; i >= 0; i--)
    Serial.println("BAND EQ" + getStrFilterFreq(i));
  
  Serial.println(Separator(SEP_COUNT));
}


String getBalance(float from)
{
  float P = abs(50.0 - from) * 2.0;
  if (from < 50.0) {
    return ("L + " + String(P, 1) + "%");
  }else if (from > 50.0){ 
    return ("R + " + String(P, 1) + "%");
  }else{
    return (String(P, 1) + "%");
  }
}


String getCommands()
{
  String msg = "";
  const int spaces = 20;
  int m = MAX_NUM_BANDS;

  msg += Separator(SEP_COUNT);

  msg += "DIGITAL AUDIO PROCESSOR\nVERSION: " + String(VERSION);

  msg += Separator(SEP_COUNT);

  msg += TEXT_PLS_CHOOSE_AN_OPTION;
  
  msg += Separator(SEP_COUNT);

  msg += left(String(IDX_INPUT_LEVEL), 2) + ". " + right(LB_INPUT_LEVEL, spaces) + String(InputLevel, 2)  + "dB\n";
  msg += left(String(IDX_OUTPUT_LEVEL), 2) + ". " + right(LB_OUTPUT_LEVEL, spaces) + String(OutputLevel, 2)  + "dB\n";
  msg += left(String(IDX_BALANCE), 2) + ". " + right(LB_BALANCE, spaces) + getBalance(Balance) + "\n";
  msg += left(String(IDX_CLIPPER), 2) + ". " + right(LB_CLIPPER, spaces) + String(Clipper, 2)  + "dB\n";
  msg += left(String(IDX_COMPRESSOR), 2) + ". " + right(LB_COMPRESSOR, spaces);
  if (Compressor) msg += TEXT_ENABLED; else msg += TEXT_DISABLED;
  msg += "\n";

  msg += left(String(IDX_MUTE), 2) + ". " + right(LB_MUTE, spaces);
  if (Mute) msg += TEXT_ENABLED; else msg += TEXT_DISABLED;
  msg += "\n";

  msg += left(String(IDX_RESERVED1), 2) + ". " + right(LB_RESERVED1, spaces);
  if (Reserved1) msg += TEXT_ENABLED; else msg += TEXT_DISABLED;
  msg += "\n";

  msg += left(String(IDX_RESERVED2), 2) + ". " + right(LB_RESERVED2, spaces);
  if (Reserved2) msg += TEXT_ENABLED; else msg += TEXT_DISABLED;
  msg += "\n";

  msg += left(String(IDX_NUM_BANDS), 2) + ". " + right(LB_NUM_BANDS, spaces) + String(NumBands)  + "\n";
  msg += left(String(IDX_PRE_EMPHASIS), 2) + ". " + right(LB_PRE_EMPHASIS, spaces) + String(PreEmphasis, 2)  + "dB\n";
  msg += left(String(IDX_POST_EMPHASIS), 2) + ". " + right(LB_POST_EMPHASIS, spaces) + String(PostEmphasis, 2)  + "dB\n";
  msg += left(String(IDX_STEP_BY), 2) + ". " + right(LB_STEP_BY, spaces) + String(StepBy, 1)  + "dB\n";
  msg += left(String(IDX_ECHO), 2) + ". " + right(LB_ECHO, spaces) + String(Echo, 2)  + "\n";

  // The index m is reflected in all bands when the web server is turned off!
  msg += left(String(IDX_PROTECTION), 2) + ". " + right(LB_PROTECTION, spaces) + String(Protection[m], 2)  + "dB\n";
  msg += left(String(IDX_GAIN), 2) + ". " + right(LB_GAIN, spaces) + String(Gain[m], 2)  + "dB\n";
  msg += left(String(IDX_ATTACK_TIME), 2) + ". " + right(LB_ATTACK_TIME, spaces) + String(AttackTime[m], 0)  + "ms\n";
  msg += left(String(IDX_RELEASE_TIME), 2) + ". " + right(LB_RELEASE_TIME, spaces) + String(ReleaseTime[m], 0)  + "ms\n";

  msg += left(String(IDX_FILTERS_Q_FACTOR), 2) + ". " + right(LB_FILTERS_Q_FACTOR, spaces) + String(FiltersQFactor, 3)  + "\n";

  msg += left(String(IDX_SAVE), 2) + ". " + String(LB_SAVE) + "\n";
  msg += left(String(IDX_RESET_TO_DEFAULT), 2) + ". " + String(LB_RESET_TO_DEFAULT) + "\n";

  for (int i = 0; i < NumBands; i++)
  {
    int j = NumBands - i - 1;
    msg += String(IDX_EQ_BAND + i) + ". " + 
    String(LB_EQ_BAND) + "(" + String(j + 1) + ") " + 
    getStrFilterFreq(j) + " " + 
    String(Equalizer[j], 2)  + "dB\n";
  }
  
  return msg;
}



void changeParam(String &returned_text, const String menu_label, const int menu_index, String text, float &value, const float min_value, const float max_value,  const String unit)
{
  static int pos = 0;
  String S = "";

  if (pos == 0)
  {
    returned_text = String(menu_index);
    S += "\n";

#ifdef PTBR_LANG
    S += "Digite o valor para " + menu_label + " entre " + String(min_value, 6) + unit + " e " + String(max_value, 6) + unit;
#endif

#ifdef ENG_LANG
    S += "Type the value for " + menu_label + " between " + String(min_value, 6) + unit + " and " + String(max_value, 6) + unit;
#endif

    Serial.println(S);
    pos++;
  }
  else
  {
    float number = text.toFloat();

    if ((number >= min_value) && (number <= max_value))
    {
      value = number;
      commitConfig();
      pos = 0;
      returned_text = "";

#ifdef PTBR_LANG
      S += "Resposta = " + String(number, 6) + unit;
#endif

#ifdef ENG_LANG
      S += "Response = " + String(number, 6) + unit;
#endif

      S += getCommands();
      Serial.println(S);
    }
    else
    {

#ifdef PTBR_LANG
      S = "Erro, o valor " + String(number, 6) + unit + " esta fora do intervalo, tente novamente!";
#endif

#ifdef ENG_LANG
      S = "Error, value " + String(number, 6) + unit + " is out of range, try again!!";
#endif
      S += "\n";
      Serial.println(S);
    }
  }

}



void changeIntParam(String &returned_text, const String menu_label, const int menu_index, String text, int &value, const int min_value, const int max_value,  const String unit)
{
  static int pos = 0;
  String S = "";

  if (pos == 0)
  {
    returned_text = String(menu_index);
    S += "\n";

#ifdef PTBR_LANG
    S += "Digite o valor para " + menu_label + " entre " + String(min_value) + unit + " e " + String(max_value) + unit;
#endif

#ifdef ENG_LANG
    S += "Type the value for " + menu_label + " between " + String(min_value) + unit + " and " + String(max_value) + unit;
#endif

    Serial.println(S);
    pos++;
  }
  else
  {
    int number = text.toInt();

    if ((number >= min_value) && (number <= max_value))
    {
      value = number;
      commitConfig();
      pos = 0;
      returned_text = "";
      S += TEXT_RESPONSE + String(number) + unit + "\n";
      S += getCommands();
      Serial.println(S);
    }
    else
    {

#ifdef PTBR_LANG
      S = "Erro, o valor " + String(number) + unit + " está fora do intervalo, tente novamente!";
#endif

#ifdef ENG_LANG
      S = "Error, value " + String(number) + unit + " is out of range, try again!!";
#endif
      S += "\n";
      Serial.println(S);
    }
  }
}



void commandInterpreter()
{
  static String lastS = "";
  String S = "";
  int idx = 0;
  int m = MAX_NUM_BANDS;

  if (Serial.available())
  {
    String text = Serial.readStringUntil('\n');

    S = text;

    if (!lastS.equals("")) S = lastS;

    idx = S.toInt();

    if (idx == IDX_INPUT_LEVEL)
    {
      changeParam(lastS, LB_INPUT_LEVEL, IDX_INPUT_LEVEL, text, InputLevel, MIN_INPUT_LEVEL, MAX_INPUT_LEVEL, "dB");
    }
    else if (idx == IDX_OUTPUT_LEVEL)
    {
      changeParam(lastS, LB_OUTPUT_LEVEL, IDX_OUTPUT_LEVEL, text, OutputLevel, MIN_OUTPUT_LEVEL, MAX_OUTPUT_LEVEL, "dB");
    }
    else if (idx == IDX_BALANCE)
    {
      changeParam(lastS, LB_BALANCE, IDX_BALANCE, text, Balance, MIN_BALANCE, MAX_BALANCE, "%");
    }
    else if (idx == IDX_CLIPPER)
    {
      changeParam(lastS, LB_CLIPPER, IDX_CLIPPER, text, Clipper, MIN_CLIPPER, MAX_CLIPPER, "dB");
    }


    else if (idx == IDX_COMPRESSOR)
    {
      Compressor = !Compressor;
      Serial.println(getCommands());
      if (Compressor) Serial.println(TEXT_COMPRESSOR + String(" ") + TEXT_ENABLED); else Serial.println(TEXT_COMPRESSOR + String(" ") + TEXT_DISABLED);
    }
    else if (idx == IDX_MUTE)
    {
      Mute = !Mute;
      if (Mute) Serial.println(TEXT_MUTE + String(" ") + TEXT_ENABLED); else Serial.println(TEXT_MUTE + String(" ") + TEXT_DISABLED);
    }
    else if (idx == IDX_RESERVED1)
    {
      Reserved1 = !Reserved1;
      Serial.println(getCommands());
      if (Reserved1) Serial.println(LB_RESERVED1 + String(" ") + TEXT_ENABLED); else Serial.println(LB_RESERVED1 + String(" ") + TEXT_DISABLED);
    }
    else if (idx == IDX_RESERVED2)
    {
      Reserved2 = !Reserved2;
      Serial.println(getCommands());
      if (Reserved2) Serial.println(LB_RESERVED2 + String(" ") + TEXT_ENABLED); else Serial.println(LB_RESERVED2 + String(" ") + TEXT_DISABLED);
    }
    else if (idx == IDX_NUM_BANDS)
    {
      changeIntParam(lastS, LB_NUM_BANDS, IDX_NUM_BANDS, text, NumBands, MIN_NUM_BANDS, MAX_NUM_BANDS, "");
    }
    else if (idx == IDX_PRE_EMPHASIS)
    {
      changeParam(lastS, LB_PRE_EMPHASIS, IDX_PRE_EMPHASIS, text, PreEmphasis, MIN_PRE_EMPHASIS, MAX_PRE_EMPHASIS, "dB");
    }
    else if (idx == IDX_POST_EMPHASIS)
    {
      changeParam(lastS, LB_POST_EMPHASIS, IDX_POST_EMPHASIS, text, PostEmphasis, MIN_POST_EMPHASIS, MAX_POST_EMPHASIS, "dB");
    }
    else if (idx == IDX_STEP_BY)
    {
      changeParam(lastS, LB_STEP_BY, IDX_STEP_BY, text, StepBy, MIN_STEP_BY, MAX_STEP_BY, "dB");
    }
    else if (idx == IDX_ECHO)
    {
      changeParam(lastS, LB_ECHO, IDX_ECHO, text, Echo, MIN_ECHO, MAX_ECHO, "");
    }

        
    else if (idx == IDX_PROTECTION)
    {
      changeParam(lastS, LB_PROTECTION, IDX_PROTECTION, text, Protection[m], MIN_PROTECTION, MAX_PROTECTION, "dB");
    }
    else if (idx == IDX_GAIN)
    {
      changeParam(lastS, LB_GAIN, IDX_GAIN, text, Gain[m], MIN_GAIN, MAX_GAIN, "dB");
    }
    else if (idx == IDX_ATTACK_TIME)
    {
      changeParam(lastS, LB_ATTACK_TIME, IDX_ATTACK_TIME, text, AttackTime[m], MIN_ATTACK_TIME, MAX_ATTACK_TIME, "ms");
    }
    else if (idx == IDX_RELEASE_TIME)
    {
      changeParam(lastS, LB_RELEASE_TIME, IDX_RELEASE_TIME, text, ReleaseTime[m], MIN_RELEASE_TIME, MAX_RELEASE_TIME, "ms");
    }


    else if (idx == IDX_FILTERS_Q_FACTOR)
    {
      changeParam(lastS, LB_FILTERS_Q_FACTOR, IDX_FILTERS_Q_FACTOR, text, FiltersQFactor, MIN_FILTERS_Q_FACTOR, MAX_FILTERS_Q_FACTOR, "");
    }
    else if (idx == IDX_SAVE)
    {
      if (saveConfig())
      {
        Serial.println(TEXT_SAVED);
      } else {
        Serial.println(TEXT_ERROR_SAVING_FILE);  
      }
    }
    else if (idx == IDX_RESET_TO_DEFAULT)
    {
      loadDefaultConfig();
      commitConfig();
      Serial.println(getCommands());
      Serial.println("Default settings have been loaded!\nNote: Still not saved!");
    }
    else if ((idx >= IDX_EQ_BAND) && (idx < IDX_EQ_BAND + NumBands))
    {
      int i = idx - IDX_EQ_BAND;
      i = NumBands - i - 1;
      String lbl = LB_EQ_BAND + String("[") + String(i + 1) + String("]");
      changeParam(lastS, lbl, idx, text, Equalizer[i], MIN_EQ_BAND, MAX_EQ_BAND, "dB");
    }
    else
    {
      Serial.println(TEXT_UNKNOWN_CMD);
      Serial.println(getCommands());
    }
  }
}
