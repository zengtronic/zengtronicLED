
template <class T> int EEPROM_writeAnything(int ee, const T& value)
{
    const byte* p = (const byte*)(const void*)&value;
    unsigned int i;
    for (i = 0; i < sizeof(value); i++)
          EEPROM.write(ee++, *p++);
    return i;
}

template <class T> int EEPROM_readAnything(int ee, T& value)
{
    byte* p = (byte*)(void*)&value;
    unsigned int i;
    for (i = 0; i < sizeof(value); i++)
          *p++ = EEPROM.read(ee++);
    return i;
}

// HINT for myself: never place two booleans next to it in the eeprom!
struct config_s
{
    char ssid1[32] = {0};
    char password1[32] = {0};
    char ssid2[32] = {0};
    char password2[32] = {0};
    char apssid[32] = {0};
    char appassword[32] = {0};
    uint8_t wifimode = 2;
    int num_leds = 60;
    uint8_t frames_per_second = 30;
    uint8_t max_brightness = 192;
    boolean power_mngmt = false;
    uint16_t max_current = 5000;
    uint8_t ain_adjustment = 25; 
    bool ain_reverse = false; 
    uint16_t ain_noise_floor = 20; 
};

struct state_s{
    bool power;
    uint8_t brightness;
    uint8_t mode; 
    uint8_t speed;
    uint8_t color;
    uint8_t saturation;
    uint8_t palette;
    uint8_t extraparam;
    uint8_t colorTemperature;
};

typedef struct config_s Configuration;
typedef struct state_s State;


void eepromWriteInt(int adr, int wert) {
  byte low, high;
  low = wert & 0xFF;
  high = (wert >> 8) & 0xFF;
  EEPROM.write(adr, low); // dauert 3,3ms
  EEPROM.write(adr + 1, high);
  return;
}

int eepromReadInt(int adr) {
  byte low, high;
  low = EEPROM.read(adr);
  high = EEPROM.read(adr + 1);
  return low + ((high << 8) & 0xFF00);
}

void eepromUpdateInt(int adr, int wert) {
  if (eepromReadInt(adr) != wert) {
    eepromWriteInt(adr, wert);
    //Serial.print("--- Updated Adress ");
    //Serial.print(adr);
    //Serial.print(" with Int value: ");
    //Serial.println(wert);
  }
}

void eepromDumpInt() {
  int col = 0;
  for(int i=0; i<= EEPROMSIZE; i+=4){
     Serial.print(eepromReadInt(i));
     Serial.print("\t");
     col++;
     if(col > 15){
     col=0;
     Serial.println(" ");
     }
  }
}

void printConfig(){
  Serial.println("---Actual Configuration---");
  Serial.print("WIFI-SSID: ");
  Serial.println(ssid1);
  Serial.print("WIFI-PW: ");
  Serial.println(password1);
  Serial.print("AP-SSID: ");
  Serial.println(apssid);
  Serial.print("AP-PW: ");
  Serial.println(appassword);
  Serial.print("WIFI-Mode: ");
  Serial.println(wifimode);

  
  Serial.print("Num LEDS: ");
  Serial.println(num_leds);
  Serial.print("Max Brightness: ");
  Serial.println(max_brightness);
  Serial.print("FPS: ");
  Serial.println(frames_per_second);
  Serial.print("Power Management: ");
  Serial.println(power_mngmt);
  Serial.print("Max Current: ");
  Serial.println(max_current);
  Serial.print("Analog In Adjustment: ");
  Serial.println(ain_adjustment);
  Serial.print("Analog In Noise Floor: ");
  Serial.println(ain_noise_floor);
  Serial.print("Analog In Mirroring: ");
  Serial.println(ain_reverse);

  Serial.print("Power: ");
  Serial.println(power);
  Serial.print("Brightness: ");
  Serial.println(FastLED.getBrightness());
  Serial.print("Mode: ");
  Serial.println(mode);
  Serial.print("Speed: ");
  Serial.println(speed);
  Serial.print("Color: ");
  Serial.println(color);
  Serial.print("Saturation: ");
  Serial.println(saturation);
  Serial.print("Palette: ");
  Serial.println(palette);
  Serial.print("Extra Parameter: ");
  Serial.println(extraparam);
  Serial.print("Color Temperature: ");
  Serial.println(currentTemp);
  Serial.println("--------------------------");
}

void printIntOfString(char text[],int length){
  Serial.println("Start of String");
for (int i = 0; i < length; ++i){
    Serial.print(i);
    Serial.print(". Char: ");
    Serial.print(text[i]); 
    Serial.print(" Int: ");
    Serial.println((int)text[i]); 
  }
}

void saveConfig(){

  Configuration old_conf;
  State old_st;
  EEPROM.get(EEPROMOFFSET + 4, old_conf);
  EEPROM.get(EEPROMOFFSET + sizeof(old_conf) + 2, old_st);
  //Serial.println(sizeof(old_conf));
  Configuration conf;
  State st;

  t_ssid1.toCharArray(conf.ssid1,32);
  t_password1.toCharArray(conf.password1,32);
  t_ssid2.toCharArray(conf.ssid2,32);
  t_password2.toCharArray(conf.password2,32);
  t_apssid.toCharArray(conf.apssid,32);
  t_appassword.toCharArray(conf.appassword,32);
  conf.wifimode = t_wifimode;
  conf.num_leds = num_leds;
  conf.frames_per_second = frames_per_second;
  conf.power_mngmt = power_mngmt;
  conf.max_current = max_current;
  conf.max_brightness = max_brightness;
  conf.ain_adjustment = ain_adjustment;
  conf.ain_noise_floor = ain_noise_floor;
  conf.ain_reverse = ain_reverse;

  st.power = power;
  st.mode = mode;
  st.color = color;
  st.saturation = saturation;
  st.palette = palette;
  st.extraparam = extraparam;
  st.brightness = FastLED.getBrightness();
  st.colorTemperature = currentTemp;
  st.speed = speed;

  char a[sizeof(old_conf)];
  memcpy(a, &old_conf, sizeof(old_conf));
  char b[sizeof(conf)];
  memcpy(b, &conf, sizeof(conf));
  //printIntOfString(a,sizeof(a));
  //printIntOfString(b,sizeof(b));
  
  bool confsame = (conf.ssid1 == old_conf.ssid1) && (conf.password1 == old_conf.password1) && (conf.ssid2 == old_conf.ssid2) && (conf.password2 == old_conf.password2) && (conf.apssid == old_conf.apssid) && (conf.appassword == old_conf.appassword) && (conf.wifimode == old_conf.wifimode) && (conf.num_leds == old_conf.num_leds) && (conf.frames_per_second == old_conf.frames_per_second) && (conf.max_brightness == old_conf.max_brightness) && (conf.power_mngmt == old_conf.power_mngmt) && (conf.max_current == old_conf.max_current) && (conf.ain_adjustment == old_conf.ain_adjustment) && (conf.ain_noise_floor == old_conf.ain_noise_floor) && (conf.ain_reverse == old_conf.ain_reverse);
  bool statesame =(st.power == old_st.power) && (st.brightness == old_st.brightness) && (st.mode == old_st.mode) && (st.speed == old_st.speed) && (st.color == old_st.color) && (st.saturation == old_st.saturation) && (st.palette == old_st.palette) && (st.extraparam == old_st.extraparam) && (st.colorTemperature == old_st.colorTemperature);
  Serial.println(confsame);
  Serial.println(statesame);
  Serial.println(memcmp(&conf,&old_conf,sizeof(Configuration)));
  Serial.println(memcmp(&st,&old_st,sizeof(State)));
  if(confsame && statesame){
    Serial.println("Saved not the Configuration because its the same as the old one. Same by Comparement.");
  }else if(memcmp(&conf,&old_conf,sizeof(Configuration)) == 0 && memcmp(&st,&old_st,sizeof(State)) == 0){
    Serial.println("Saved not the Configuration because its the same as the old one. Same Memory");
  }else{
    EEPROM.put(EEPROMOFFSET + 4, conf);
    EEPROM.put(EEPROMOFFSET + sizeof(conf) + 2, st);
    EEPROM.commit();
    Serial.println("Saved Configuration");
    printConfig();
  }
  
}

void loadConfig(){
  
  Serial.println("----- Loading Configuration -----");
  Serial.print("Testpattern: ");
  int pattern = eepromReadInt(EEPROMOFFSET);
  Serial.println(pattern);
  if(pattern != IDENTIFIER){
    eepromWriteInt(EEPROMOFFSET,IDENTIFIER);
    saveConfig();
    Serial.println("No Settings found! Saved default values!");
  }
  Configuration conf;
  State st;
  EEPROM.get(EEPROMOFFSET + 4, conf);
  EEPROM.get(EEPROMOFFSET + sizeof(conf) + 2, st);
  ssid1 = conf.ssid1;
  password1 = conf.password1;
  ssid2 = conf.ssid2;
  password2 = conf.password2;
  apssid = conf.apssid;
  appassword = conf.appassword;
  wifimode = conf.wifimode;
  t_ssid1 = conf.ssid1;
  t_password1 = conf.password1;
  t_ssid2 = conf.ssid2;
  t_password2 = conf.password2;
  t_apssid = conf.apssid;
  t_appassword = conf.appassword;
  t_wifimode = conf.wifimode;
  
  num_leds = conf.num_leds;
  frames_per_second = conf.frames_per_second;
  power_mngmt = conf.power_mngmt;
  max_current = conf.max_current;
  max_brightness = conf.max_brightness;
  
  ain_adjustment = conf.ain_adjustment;
  ain_noise_floor = conf.ain_noise_floor;
  ain_reverse = conf.ain_reverse;

  power = st.power;
  mode = st.mode;
  speed = st.speed;
  color = st.color;
  saturation = st.saturation;
  palette = st.palette;
  extraparam = st.extraparam;
  FastLED.setBrightness(st.brightness);
  FastLED.setTemperature(colorTemp[st.colorTemperature]);
  currentTemp = st.colorTemperature;
  printConfig();
  
}

