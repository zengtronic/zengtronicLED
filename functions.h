

// save config and restart esp
void safeRestart()
{
  saveConfig();
  delay(500);
  #ifdef USE_ESP32
    ESP.restart();
  #else
    ESP.reset();
  #endif
}

//toogle the power state
void switchPower()
{
  power = !power;
  Serial.println((String) "Power: " + power);
}

//set current mode
void setMode(int number)
{
  if (number < 0)
  {
    mode = modeCount - 1 - number;
  }
  else if (number > modeCount)
  {
    mode = number - modeCount - 1;
  }
  else
  {
    mode = number;
  }
  //reset strip color array
  for (int i = 0; i < num_leds; i++)
  {
    fill_solid(leds,num_leds,CRGB::Black);
  }
  Serial.println((String) "Mode: " + mode);
}

//set the current brightness
void setHelligkeit(int number)
{
  if (number < 0)
  {
    FastLED.setBrightness(0);
  }
  else if (number > max_brightness)
  {
    FastLED.setBrightness(max_brightness);
  }
  else
  {
    FastLED.setBrightness(number);
  }
  Serial.println((String) "Brightness: " + FastLED.getBrightness());
}

//set the current color (Hue)
void setColor(int number)
{
  if (number < 0)
  {
    color = 255 + number;
  }
  else if (number > 255)
  {
    color = number - 255;
  }
  else
  {
    color = number;
  }
  Serial.println((String) "Color: " + color);
}

//set the current saturation
void setSaturation(int number)
{
  if (number < 0)
  {
    saturation = 255 + number;
  }
  else if (number > 255)
  {
    saturation = number - 255;
  }
  else
  {
    saturation = number;
  }
  Serial.println((String) "Saturation: " + saturation);
}

//set the current effect speed
void setSpeed(int number)
{
  if (number < 1)
  {
    speed = 0;
  }
  else
  {
    speed = number;
  }
  Serial.println((String) "Speed: " + speed);
}

//set the current exptra parameter
void setExtra(int number)
{
  if (number < 1)
  {
    extraparam = 0;
  }
  else
  {
    extraparam = number;
  }
  Serial.println((String) "Extra: " + extraparam);
}

//set the current palette
void setPalette(int number)
{
  if (number < 0)
  {
    palette = paletteCount - 1 - number;
  }
  else if (number > paletteCount)
  {
    palette = number - paletteCount - 1;
  }
  else
  {
    palette = number;
  }
  if (palette == 17)
  {
    for (int i = 0; i < 16; i++)
    {
      currentPalette[i] = CHSV(random8(), 255, random8());
    }
  }
  Serial.println((String) "Palette: " + palette);
}

//set the analog in adjustment
void setAINadjustment(int number)
{
  if (number < 10)
  {
    ain_adjustment = 10;
  }
  else if(number > 100)
  {
    ain_adjustment = 100;
  }else{
    ain_adjustment = number;
  }
  Serial.println((String) "AIN-Adjustment: " + ain_adjustment);
}

//set the analog in ground floor
void setAINnoise(int number)
{
  if (number < 0)
  {
    ain_noise_floor = 0;
  }
  else if(number > (ADC_RESOLUTION/4))
  {
    ain_noise_floor = ADC_RESOLUTION/4;
  }else{
    ain_noise_floor = number;
  }
  Serial.println((String) "AIN-Adjustment: " + ain_noise_floor);
}

//set the analog in ground floor
void setAINmirror(bool checked)
{
  ain_reverse = checked;
  Serial.println((String) "AIN-Mirrored: " + ain_reverse);
}

/*
 * EFFECTS AND EFFECTS HELPER FUNCTIONS
 * All functions must be programmed non-blocking!
 */




unsigned long soundPreviousMillis = 0;
uint8_t soundInterval = 5; 

void process_sound_in(){
  unsigned long currentMillis = millis();
  if (currentMillis - soundPreviousMillis >= soundInterval)  {
    soundPreviousMillis = currentMillis;
    int ain = analogRead(A0_IN);
    if(ain_reverse){
      ain = map(ain, 0,ADC_RESOLUTION, ADC_RESOLUTION, 0);
    }
    ADCFilter.Filter(ain);
    int filtered_ain = ADCFilter.Current();
    //underflow (negative numbers which occurs in mapping) makes problems, so keep them out. O
    if(filtered_ain < ain_noise_floor){
      filtered_ain = ain_noise_floor;
    }
    sound_in = map(filtered_ain, ain_noise_floor, (int)((ADC_RESOLUTION/100)*ain_adjustment), 0, ADC_RESOLUTION - 1);  
    //Serial.println((String)"IN: " + ain + " OUT: " + sound_in);
  }
}
