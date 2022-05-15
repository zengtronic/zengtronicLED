// Variables for effects
bool anidir = false; // false - forward, true - backward
int anipos = 0;      // Position in Animation... Mostly LED X of Y
int anistep = 0;     // Step in Animation
int everyX = 1;      // Every X funktion calls
int everyXpos = 0;   // EveryX current position
int animationHue = 0;
int animationSat = 255;
unsigned long animationPreviousMillis = 0; // last timestamp
//Variables for colors
CHSV origin;            // The main color 
CHSV complementary;     // Complementary color if needed
CHSV triad1;            // First triad color if needed
CHSV triad2;            // Second triad color if needed


// This function simply return a value between 0 and 255 and maps overlapping values. e.x.: 300 will return 44
uint8_t Wheel(int hue)
{
    if (hue < 0)
    {
        while (hue < 0)
        {
            hue += 256;
        }
    }
    else if (hue > 255)
    {
        while (hue > 255)
        {
            hue -= 256;
        }
    }
    return (uint8_t)hue;
}

//fade all leds to black
void fadeall()
{
    for (int i = 0; i < num_leds; i++)
    {
        leds[i].nscale8(250);
    }
}

//set all LEDs to one color
void colorSolid(CRGB color)
{
    for (int i = 0; i < num_leds; i++)
    {
        leds[i] = color;
    }
}

//blink all leds in one color. 100hz to 1hz
void colorSolidBlink(CRGB color)
{
    unsigned long currentMillis = millis();
    long toggleinterval = 10 * speed;
    if (currentMillis - animationPreviousMillis >= toggleinterval)
    {
        // save the last time you blinked the LED
        animationPreviousMillis = currentMillis;
        anidir = !anidir;
    }

    if (anidir)
    {
        colorSolid(color);
    }
    else
    {
        colorSolid(CRGB::Black);
    }
}


/* ============ SOUND EFFECTS ============ */
#define soundLevelVariants 5
void soundLevel(bool rainbow = false, bool palette = false)
{
    unsigned long currentMillis = millis();
    uint8_t animationInterval = 16; //~60Hz
    if (currentMillis - animationPreviousMillis >= animationInterval)
    {
        animationPreviousMillis = currentMillis;
        uint8_t variant = extraparam / (100 / soundLevelVariants);
        if (variant == soundLevelVariants)
        {
            variant = soundLevelVariants - 1;
        }
        uint8_t topled = 0;
        if (!rainbow)
        {
            animationHue = color;
            animationSat = saturation;
        }
        else
        {
            animationSat = 255;
        }

        origin = CHSV(Wheel(animationHue), animationSat, 255);
        complementary = CHSV(Wheel(animationHue + 128), animationSat, 255);
        triad1 = CHSV(Wheel(animationHue + 85), animationSat, 255);
        triad2 = CHSV(Wheel(animationHue + 171), animationSat, 255);

        //Variant 0: Color bar, other black
        //Variant 1: Color bar, top leds white, other black
        if (variant == 0 || variant == 1)
        {
            if (variant == 1)
            {
                topled = round(num_leds / 10); //10% of the leds are the top leds;
            }
            else
            {
                topled = 0;
            }
            int val = map(sound_in, 0, ADC_RESOLUTION - 1, 0, num_leds - topled);
            for (int i = 0; i < num_leds; i++)
            {
                if (i < val)
                {
                    leds[i] = origin;
                }
                else if (i < (val + topled))
                {
                    leds[i] = CHSV(0, 0, 255);
                }
                else
                {
                    leds[i] = CHSV(0, 0, 0);
                }
            }

            //Variant 2: Color bar, other complementary
            //Variant 3: Color bar, top leds white, other complementary
        }
        else if (variant == 2 || variant == 3)
        {
            if (variant == 3)
            {
                topled = round(num_leds / 10); //10% of the leds are the top leds;
            }
            else
            {
                topled = 0;
            }
            int val = map(sound_in, 0, ADC_RESOLUTION - 1, 0, num_leds - topled);
            for (int i = 0; i < num_leds; i++)
            {
                if (i < val)
                {
                    leds[i] = origin;
                }
                else if (i < (val + topled))
                {
                    leds[i] = CHSV(0, 0, 255);
                }
                else
                {
                    leds[i] = complementary;
                }
            }
            //Variant 4: Color bar, topled first triad, other second triad
        }
        else if (variant == 4)
        {
            topled = round(num_leds / 10); //10% of the leds are the top leds;
            int val = map(sound_in, 0, ADC_RESOLUTION - 1, 0, num_leds - topled);
            for (int i = 0; i < num_leds; i++)
            {
                if (i < val)
                {
                    leds[i] = origin;
                }
                else if (i < (val + topled))
                {
                    leds[i] = triad1;
                }
                else
                {
                    leds[i] = triad2;
                }
            }
        }

        if (rainbow)
        {
            everyX = (int)(speed / 5);
            everyXpos += 1;
            if (everyXpos > everyX)
            {
                animationHue += 1;
                if (animationHue > 255)
                {
                    animationHue = animationHue - 256;
                }
                everyXpos = 0;
            }
        }
    }
}

#define soundLevelCenteredVariants 7
void soundLevelCentered(bool rainbow = false, bool palette = false)
{
    unsigned long currentMillis = millis();
    uint8_t animationInterval = 16; //~60Hz
    if (currentMillis - animationPreviousMillis >= animationInterval)
    {

        fadeToBlackBy(leds, num_leds, 50 + speed * 2);
        animationPreviousMillis = currentMillis;
        int middle_left = (int)(num_leds / 2); // cast cuts down everything after the , (as we want)
        int middle_right = middle_left;        // Middle is the same

        // even led numbers:
        if (!leds_odd)
        {
            middle_left = (num_leds / 2) - 1; //Bsp: 60 LEDs -> 29 (index)
            middle_right = num_leds / 2;      //Bsp: 60 LEDs -> 30 (index)
        }

        uint8_t variant = extraparam / (100 / soundLevelCenteredVariants);
        if (variant == soundLevelCenteredVariants)
        {
            variant = soundLevelCenteredVariants - 1;
        }
        uint8_t topled = 0;
        if (!rainbow)
        {
            animationHue = color;
            animationSat = saturation;
        }
        else
        {
            animationSat = 255;
        }

        //convert to rgb as fastleds hsv uses the rainbow spectrum
        origin = CHSV(Wheel(animationHue), animationSat, 255);
        complementary = CHSV(Wheel(animationHue + 128), animationSat, 255);
        triad1 = CHSV(Wheel(animationHue + 85), animationSat, 255);
        triad2 = CHSV(Wheel(animationHue + 171), animationSat, 255);

        //Variant 0: Color bar, other black
        //Variant 1: Color bar, top leds white, other black
        if (variant == 0 || variant == 1)
        {
            if (variant == 1)
            {
                topled = round(num_leds / 20); //5% of the leds are the top leds;
            }
            else
            {
                topled = 0;
            }
            int val = map(sound_in, 0, ADC_RESOLUTION - 1, 0, (num_leds / 2) - topled * 2);
            for (int i = 0; i < (num_leds / 2 + 1); i++)
            {
                if (i < val)
                {
                    leds[i + middle_right] = origin;
                    leds[middle_left - i] = origin;
                }
                else if (i < (val + topled))
                {
                    leds[i + middle_right] = CRGB::White;
                    leds[middle_left - i] = CRGB::White;
                }
                else
                {
                    //leds[i + middle_right] = CRGB::Black; REMOVED FOR TEST ONLY
                    //leds[middle_left - i] = CRGB::Black;
                }
            }

            //Variant 2: Color bar, other complementary
            //Variant 3: Color bar, top leds white, other complementary
        }
        else if (variant == 2 || variant == 3)
        {
            if (variant == 3)
            {
                topled = round(num_leds / 20); //5% of the leds are the top leds;
            }
            else
            {
                topled = 0;
            }
            int val = map(sound_in, 0, ADC_RESOLUTION - 1, 0, (num_leds / 2) - topled * 2);
            for (int i = 0; i < (num_leds / 2 + 1); i++)
            { // Note: the + 1 is important for odd led numbers to reach the outermost leds!
                if (i < val)
                {
                    leds[i + middle_right] = origin;
                    leds[middle_left - i] = origin;
                }
                else if (i < (val + topled))
                {
                    leds[i + middle_right] = CRGB::White;
                    leds[middle_left - i] = CRGB::White;
                }
                else
                {
                    leds[i + middle_right] = complementary;
                    leds[middle_left - i] = complementary;
                }
            }
            //Variant 4: Color bar, topled first triad, other second triad
        }
        else if (variant == 4)
        {
            topled = round(num_leds / 20); //10% of the leds are the top leds;
            int val = map(sound_in, 0, ADC_RESOLUTION - 1, 0, (num_leds / 2) - topled * 2);
            for (int i = 0; i < (num_leds / 2 + 1); i++)
            {
                if (i < val)
                {
                    leds[i + middle_right] = origin;
                    leds[middle_left - i] = origin;
                }
                else if (i < (val + topled))
                {
                    leds[i + middle_right] = triad1;
                    leds[middle_left - i] = triad1;
                }
                else
                {
                    leds[i + middle_right] = triad2;
                    leds[middle_left - i] = triad2;
                }
            }
            // Variant 5: Bar black, other colored
            //Variant 6: Bar black, topled white, other colored
        }
        else if (variant == 5 || variant == 6)
        {
            if (variant == 6)
            {
                topled = round(num_leds / 20); //5% of the leds are the top leds;
            }
            else
            {
                topled = 0;
            }
            int val = map(sound_in, 0, ADC_RESOLUTION - 1, 0, (num_leds / 2) - topled * 2);

            for (int i = 0; i < (num_leds / 2 + 1); i++)
            {
                if (i < val)
                {
                    leds[i + middle_right] = CRGB::Black;
                    leds[middle_left - i] = CRGB::Black;
                }
                else if (i < (val + topled))
                {
                    leds[i + middle_right] = CRGB::White;
                    leds[middle_left - i] = CRGB::White;
                }
                else
                {
                    leds[i + middle_right] = origin;
                    leds[middle_left - i] = origin;
                }
            }
        }

        if (rainbow)
        {
            everyX = (int)(speed / 5);
            everyXpos += 1;
            if (everyXpos > everyX)
            {
                animationHue += 1;
                if (animationHue > 255)
                {
                    animationHue = animationHue - 256;
                }
                everyXpos = 0;
            }
        }
    }
}

#define soundSolidVariants 7
void soundSolid(bool rainbow = false, bool palette = false)
{
    unsigned long currentMillis = millis();
    uint8_t animationInterval = 16; //~60Hz
    if (currentMillis - animationPreviousMillis >= animationInterval)
    {
        animationPreviousMillis = currentMillis;
        uint8_t variant = extraparam / (100 / soundSolidVariants);
        if (variant == soundSolidVariants)
        {
            variant = soundSolidVariants - 1;
        }
        if (!rainbow)
        {
            animationHue = color;
            animationSat = saturation;
        }
        else
        {
            animationSat = 255;
        }

        //Variant 0: solid color, brightness 50 to 255
        //Variant 1: solid color, brightness 0 to 255
        if (variant == 0 || variant == 1)
        {
            int val = 0;
            if (variant == 0)
            {
                val = map(sound_in, 0, ADC_RESOLUTION - 1, 50, 255);
            }
            else
            {
                val = map(sound_in, 0, ADC_RESOLUTION - 1, 0, 255);
            }
            fill_solid(leds, num_leds, CHSV(animationHue, animationSat, val));
            //Variant 2: solid color, brightness 255 to 50
            //Variant 3: solid color, brightness 255 to 0
        }
        else if (variant == 2 || variant == 3)
        {
            int val = 0;
            if (variant == 2)
            {
                val = map(sound_in, 0, ADC_RESOLUTION - 1, 255, 50);
            }
            else
            {
                val = map(sound_in, 0, ADC_RESOLUTION - 1, 255, 0);
            }
            fill_solid(leds, num_leds, CHSV(animationHue, animationSat, val));
            //Variant 4:
            //Variant 5:
            //Variant 6:
        }
        else if (variant == 4 || variant == 5 || variant == 6)
        {
            int val = 0;
            if (variant == 4)
            {
                val = map(sound_in, 0, ADC_RESOLUTION - 1, 0, 48);
            }
            else if (variant == 5)
            {
                val = map(sound_in, 0, ADC_RESOLUTION - 1, 0, 64);
            }
            else
            {
                val = map(sound_in, 0, ADC_RESOLUTION - 1, 0, 80);
            }
            fill_solid(leds, num_leds, CHSV(Wheel(animationHue + val), animationSat, 255));
        }
        if (rainbow)
        {
            everyX = (int)(speed / 5);
            everyXpos += 1;
            if (everyXpos > everyX)
            {
                animationHue += 1;
                if (animationHue > 255)
                {
                    animationHue = animationHue - 256;
                }
                everyXpos = 0;
            }
        }
    }
}

#define soundFlowVariants 7
void soundFlow(bool rainbow = false, bool palette = false)
{
    unsigned long currentMillis = millis();
    uint8_t animationInterval = 20; //~20hz
    if (currentMillis - animationPreviousMillis >= animationInterval)
    {
        animationPreviousMillis = currentMillis;
        uint8_t variant = extraparam / (100 / soundFlowVariants);
        if (variant == soundFlowVariants)
        {
            variant = soundFlowVariants - 1;
        }
        if (!rainbow)
        {
            animationHue = color;
            animationSat = saturation;
        }
        else
        {
            animationSat = 255;
        }

        int val = 0;
        //Variant 0: solid color, brightness 50 to 255
        //Variant 1: solid color, brightness 0 to 255
        if (variant == 0 || variant == 1)
        {
            if (variant == 0)
            {
                val = map(sound_in, 0, ADC_RESOLUTION - 1, 50, 255);
            }
            else
            {
                val = map(sound_in, 0, ADC_RESOLUTION - 1, 0, 255);
            }
            for (int i = (num_leds - 1); i >= 0; i--)
            {
                if (i == 0)
                {
                    leds[i] = CHSV(animationHue, animationSat, val);
                }
                else
                {
                    leds[i] = leds[i - 1];
                }
            }
            //Variant 2: solid color, brightness 255 to 50
            //Variant 3: solid color, brightness 255 to 0
        }
        else if (variant == 2 || variant == 3)
        {
            if (variant == 2)
            {
                val = map(sound_in, 0, ADC_RESOLUTION - 1, 255, 50);
            }
            else
            {
                val = map(sound_in, 0, ADC_RESOLUTION - 1, 255, 0);
            }
            for (int i = (num_leds - 1); i >= 0; i--)
            {
                if (i == 0)
                {
                    leds[i] = CHSV(animationHue, animationSat, val);
                }
                else
                {
                    leds[i] = leds[i - 1];
                }
            }
            //Variant 4:
            //Variant 5:
            //Variant 6:
        }
        else if (variant == 4 || variant == 5 || variant == 6)
        {
            if (variant == 4)
            {
                val = map(sound_in, 0, ADC_RESOLUTION - 1, 0, 48);
            }
            else if (variant == 5)
            {
                val = map(sound_in, 0, ADC_RESOLUTION - 1, 0, 64);
            }
            else
            {
                val = map(sound_in, 0, ADC_RESOLUTION - 1, 0, 80);
            }
            for (int i = (num_leds - 1); i >= 0; i--)
            {
                if (i == 0)
                {
                    leds[i] = CHSV(Wheel(animationHue + val), animationSat, 255);
                }
                else
                {
                    leds[i] = leds[i - 1];
                }
            }
        }
        if (rainbow)
        {
            everyX = 5; //(int)(speed / 5);
            everyXpos += 1;
            if (everyXpos > everyX)
            {
                animationHue += 1;
                if (animationHue > 255)
                {
                    animationHue = animationHue - 256;
                }
                everyXpos = 0;
            }
        }
    }
}

#define soundFlowCenteredVariants 7
void soundFlowCentered(bool rainbow = false, bool palette = false)
{
    unsigned long currentMillis = millis();
    uint8_t animationInterval = 20; //~50hz
    if (currentMillis - animationPreviousMillis >= animationInterval)
    {
        animationPreviousMillis = currentMillis;
        uint8_t variant = extraparam / (100 / soundFlowCenteredVariants);
        if (variant == soundFlowCenteredVariants)
        {
            variant = soundFlowCenteredVariants - 1;
        }
        if (!rainbow)
        {
            //set hue and saturation to the selected one if rainbow mode not selected
            animationHue = color;
            animationSat = saturation;
        }
        else
        {
            animationSat = 255;
        }
        int middle_left = (int)(num_leds / 2); // cast cuts down everything after the , (as we want)
        int middle_right = middle_left;        // Middle is the same

        // even led numbers:
        if (!leds_odd)
        {
            middle_left = (num_leds / 2) - 1; //Bsp: 60 LEDs -> 29 (index)
            middle_right = num_leds / 2;      //Bsp: 60 LEDs -> 30 (index)
        }

        int val = 0;
        //Variant 0: solid color, brightness 50 to 255
        //Variant 1: solid color, brightness 0 to 255
        //Variant 2: solid color, brightness 255 to 50
        //Variant 3: solid color, brightness 255 to 0
        if (variant == 0 || variant == 1 || variant == 2 || variant == 3)
        {
            if (variant == 0)
            {
                val = map(sound_in, 0, ADC_RESOLUTION - 1, 50, 255);
            }
            else if (variant == 1)
            {
                val = map(sound_in, 0, ADC_RESOLUTION - 1, 0, 255);
            }
            else if (variant == 2)
            {
                val = map(sound_in, 0, ADC_RESOLUTION - 1, 255, 50);
            }
            else
            {
                val = map(sound_in, 0, ADC_RESOLUTION - 1, 255, 0);
            }
            origin = CHSV(animationHue, animationSat, val);
            //Variant 4: Shift color by up to 64 (of 256) degree
            //Variant 5: Shift color by up to 80 (of 256) degree
            //Variant 6: Shift color by up to 96 (of 256) degree
        }
        else if (variant == 4 || variant == 5 || variant == 6)
        {
            if (variant == 4)
            {
                val = map(sound_in, 0, ADC_RESOLUTION - 1, 0, 64);
            }
            else if (variant == 5)
            {
                val = map(sound_in, 0, ADC_RESOLUTION - 1, 0, 80);
            }
            else
            {
                val = map(sound_in, 0, ADC_RESOLUTION - 1, 0, 96);
            }
            origin = CHSV(Wheel(animationHue + val), animationSat, 255);
        }

        //shift all leds from the right part of the middle to right
        for (int i = (num_leds - 1); i > middle_right; i--)
        {
            leds[i] = leds[i - 1];
        }

        //shift all leds from the left part of the middle to left
        for (int i = 0; i < middle_left; i++)
        {
            leds[i] = leds[i + 1];
        }

        // write new color to the 2 middle points (propably the same position)
        leds[middle_left] = origin;
        leds[middle_right] = origin;

        // cycle through the rainbow colors (if selected)
        if (rainbow)
        {
            everyX = (int)(speed / 5);
            everyXpos += 1;
            if (everyXpos > everyX)
            {
                animationHue = Wheel(animationHue + 1);
                everyXpos = 0;
            }
        }
    }
}

/* ============ FFT SOUND EFFECTS ============ */
/* ============    Coming soon    ============ */


/* ============  STATIC EFFECTS   ============ */
/* 
    KITT
    Moving dot from side to side (like K.I.T.T. in Knight Rider)
    with a fading trail.
*/
void kitt(int color = 0, bool rainbow = false, int changeSpeed = 1, int rate = 75)
{
    unsigned long currentMillis = millis();
    uint8_t animationInterval = 5;
    if (currentMillis - animationPreviousMillis >= animationInterval)
    {
        animationPreviousMillis = currentMillis;
        int fadeRate = map(rate, 0, 100, 10, 100); //map to minimum 10 since ex. 0 doesnt make any sense
        everyX = (int)((speed * 10) / num_leds);
        if (everyXpos == 0)
        {
            everyXpos = everyX;
            fadeToBlackBy(leds, num_leds, fadeRate);
            //animationHue increases if it is in rainbow mode otherwise itsanimationHue
            if (rainbow)
            {
                animationHue += changeSpeed;
                animationSat = 255;
            }
            else
            {
                animationHue = color;
                animationSat = saturation;
            }

            if (anidir == false)
            {
                leds[anipos] = CHSV(animationHue, animationSat, 255);
                anipos++;
                if (anipos > num_leds - 1)
                {
                    anipos = num_leds - 1;
                    anidir = true;
                }
            }
            else
            {
                leds[anipos] = CHSV(animationHue, animationSat, 255);
                anipos--;
                if (anipos < 0)
                {
                    anipos = 0;
                    anidir = false;
                }
            }
        }
        else
        {
            everyXpos--;
        }
    }
}

void playPalette(CRGBPalette256 palette, int startdelay, int stretch, bool smooth = true)
{
    unsigned long currentMillis = millis();
    long animationInterval = startdelay + speed;
    if (currentMillis - animationPreviousMillis >= animationInterval)
    {
        animationPreviousMillis = currentMillis;
        for (int i = 0; i < num_leds; i++)
        {
            if(smooth){
                leds[i] = ColorFromPalette(palette, animationHue, 255, LINEARBLEND);
            }else{
                leds[i] = ColorFromPalette(palette, animationHue, 255, NOBLEND);
            }
        }
        animationHue = Wheel(animationHue + 1);
    }
}

void movingPalette(CRGBPalette256 palette, bool smooth = true)
{
    unsigned long currentMillis = millis();
    long animationInterval = 10 + speed;
    if (currentMillis - animationPreviousMillis >= animationInterval)
    {
        animationPreviousMillis = currentMillis;
        uint8_t pos = animationHue;
        uint8_t stretch = round(extraparam/10);
        if(stretch == 0){
            stretch = 1;
        }
        for (int i = num_leds - 1; i > 0; i--)
        {
            if(smooth){
                leds[i] = ColorFromPalette(palette, pos, 255, LINEARBLEND);
            }else{
                leds[i] = ColorFromPalette(palette, pos, 255, NOBLEND);
            }
            pos = Wheel(pos - stretch);
        }
        animationHue = Wheel(animationHue + 1);
    }
}

/*
    PACIFICA by Kriegsman
    https://gist.github.com/kriegsman/83325f561c033d9adbd31120b573deb9
*/


CRGBPalette16 pacifica_palette_1 = 
    { 0x000507, 0x000409, 0x00030B, 0x00030D, 0x000210, 0x000212, 0x000114, 0x000117, 
      0x000019, 0x00001C, 0x000026, 0x000031, 0x00003B, 0x000046, 0x14554B, 0x28AA50 };
CRGBPalette16 pacifica_palette_2 = 
    { 0x000507, 0x000409, 0x00030B, 0x00030D, 0x000210, 0x000212, 0x000114, 0x000117, 
      0x000019, 0x00001C, 0x000026, 0x000031, 0x00003B, 0x000046, 0x0C5F52, 0x19BE5F };
CRGBPalette16 pacifica_palette_3 = 
    { 0x000208, 0x00030E, 0x000514, 0x00061A, 0x000820, 0x000927, 0x000B2D, 0x000C33, 
      0x000E39, 0x001040, 0x001450, 0x001860, 0x001C70, 0x002080, 0x1040BF, 0x2060FF };


// Add one layer of waves into the led array
void pacifica_one_layer( CRGBPalette16& p, uint16_t cistart, uint16_t wavescale, uint8_t bri, uint16_t ioff)
{
  uint16_t ci = cistart;
  uint16_t waveangle = ioff;
  uint16_t wavescale_half = (wavescale / 2) + 20;
  for( uint16_t i = 0; i < num_leds; i++) {
    waveangle += 250;
    uint16_t s16 = sin16( waveangle ) + 32768;
    uint16_t cs = scale16( s16 , wavescale_half ) + wavescale_half;
    ci += cs;
    uint16_t sindex16 = sin16( ci) + 32768;
    uint8_t sindex8 = scale16( sindex16, 240);
    CRGB c = ColorFromPalette( p, sindex8, bri, LINEARBLEND);
    leds[i] += c;
  }
}

// Add extra 'white' to areas where the four layers of light have lined up brightly
void pacifica_add_whitecaps()
{
  uint8_t basethreshold = beatsin8( 9, 55, 65);
  uint8_t wave = beat8( 7 );
  
  for( uint16_t i = 0; i < num_leds; i++) {
    uint8_t threshold = scale8( sin8( wave), 20) + basethreshold;
    wave += 7;
    uint8_t l = leds[i].getAverageLight();
    if( l > threshold) {
      uint8_t overage = l - threshold;
      uint8_t overage2 = qadd8( overage, overage);
      leds[i] += CRGB( overage, overage2, qadd8( overage2, overage2));
    }
  }
}

// Deepen the blues and greens
void pacifica_deepen_colors()
{
  for( uint16_t i = 0; i < num_leds; i++) {
    leds[i].blue = scale8( leds[i].blue,  145); 
    leds[i].green= scale8( leds[i].green, 200); 
    leds[i] |= CRGB( 2, 5, 7);
  }
}

void pacifica_loop()
{
  // Increment the four "color index start" counters, one for each wave layer.
  // Each is incremented at a different speed, and the speeds vary over time.
  static uint16_t sCIStart1, sCIStart2, sCIStart3, sCIStart4;
  static uint32_t sLastms = 0;
  uint32_t ms = GET_MILLIS();
  uint32_t deltams = ms - sLastms;
  sLastms = ms;
  uint16_t speedfactor1 = beatsin16(3, 179, 269);
  uint16_t speedfactor2 = beatsin16(4, 179, 269);
  uint32_t deltams1 = (deltams * speedfactor1) / 256;
  uint32_t deltams2 = (deltams * speedfactor2) / 256;
  uint32_t deltams21 = (deltams1 + deltams2) / 2;
  sCIStart1 += (deltams1 * beatsin88(1011,10,13));
  sCIStart2 -= (deltams21 * beatsin88(777,8,11));
  sCIStart3 -= (deltams1 * beatsin88(501,5,7));
  sCIStart4 -= (deltams2 * beatsin88(257,4,6));

  // Clear out the LED array to a dim background blue-green
  fill_solid( leds, num_leds, CRGB( 2, 6, 10));

  // Render each of four layers, with different scales and speeds, that vary over time
  pacifica_one_layer( pacifica_palette_1, sCIStart1, beatsin16( 3, 11 * 256, 14 * 256), beatsin8( 10, 70, 130), 0-beat16( 301) );
  pacifica_one_layer( pacifica_palette_2, sCIStart2, beatsin16( 4,  6 * 256,  9 * 256), beatsin8( 17, 40,  80), beat16( 401) );
  pacifica_one_layer( pacifica_palette_3, sCIStart3, 6 * 256, beatsin8( 9, 10,38), 0-beat16(503));
  pacifica_one_layer( pacifica_palette_3, sCIStart4, 5 * 256, beatsin8( 8, 10,28), beat16(601));

  // Add brighter 'whitecaps' where the waves lines up more
  pacifica_add_whitecaps();

  // Deepen the blues and greens a bit
  pacifica_deepen_colors();
}

// Function to call in the effect selection
void pacifica(){

    unsigned long currentMillis = millis();
    uint8_t animationInterval = 20;
    if (currentMillis - animationPreviousMillis >= animationInterval)
    {
        animationPreviousMillis = currentMillis;
        pacifica_loop();
    }
}

/*
    Discostrobe by Kriegsman
    https://gist.github.com/kriegsman/626dca2f9d2189bd82ca
*/
// drawRainbowDashes - draw rainbow-colored 'dashes' of light along the led strip:
//   starting from 'startpos', up to and including 'lastpos'
//   with a given 'period' and 'width'
//   starting from a given hue, which changes for each successive dash by a 'huedelta'
//   at a given saturation and value.
//
//   period = 5, width = 2 would be  _ _ _ X X _ _ _ Y Y _ _ _ Z Z _ _ _ A A _ _ _ 
//                                   \-------/       \-/
//                                   period 5      width 2
//
static void drawRainbowDashes( 
  uint8_t startpos, uint16_t lastpos, uint8_t period, uint8_t width, 
  uint8_t huestart, uint8_t huedelta, uint8_t saturation, uint8_t value)
{
  uint8_t hue = huestart;
  for( uint16_t i = startpos; i <= lastpos; i += period) {
    // Switched from HSV color wheel to color palette
    // Was: CRGB color = CHSV( hue, saturation, value); 
    CRGB color = ColorFromPalette( currentPalette, hue, value, LINEARBLEND);
    
    // draw one dash
    uint16_t pos = i;
    for( uint8_t w = 0; w < width; w++) {
      leds[ pos ] = color;
      pos++;
      if( pos >= num_leds) {
        break;
      }
    }
    
    hue += huedelta;
  }
}

// discoWorker updates the positions of the dashes, and calls the draw function
//
void discoWorker( 
    uint8_t dashperiod, uint8_t dashwidth, int8_t  dashmotionspeed,
    uint8_t stroberepeats,
    uint8_t huedelta)
 {
  static uint8_t sRepeatCounter = 0;
  static int8_t sStartPosition = 0;
  static uint8_t sStartHue = 0;

  // Always keep the hue shifting a little
  sStartHue += 1;

  // Increment the strobe repeat counter, and
  // move the dash starting position when needed.
  sRepeatCounter = sRepeatCounter + 1;
  if( sRepeatCounter>= stroberepeats) {
    sRepeatCounter = 0;
    
    sStartPosition = sStartPosition + dashmotionspeed;
    
    // These adjustments take care of making sure that the
    // starting hue is adjusted to keep the apparent color of 
    // each dash the same, even when the state position wraps around.
    if( sStartPosition >= dashperiod ) {
      while( sStartPosition >= dashperiod) { sStartPosition -= dashperiod; }
      sStartHue  -= huedelta;
    } else if( sStartPosition < 0) {
      while( sStartPosition < 0) { sStartPosition += dashperiod; }
      sStartHue  += huedelta;
    }
  }

  // draw dashes with full brightness (value), and somewhat
  // desaturated (whitened) so that the LEDs actually throw more light.
  const uint8_t kSaturation = 208;
  const uint8_t kValue = 255;

  // call the function that actually just draws the dashes now
  drawRainbowDashes( sStartPosition, num_leds-1, 
                     dashperiod, dashwidth, 
                     sStartHue, huedelta, 
                     kSaturation, kValue);
}



void discostrobe_loop()
{
  // First, we black out all the LEDs
  fill_solid( leds, num_leds, CRGB::Black);

  // To achive the strobe effect, we actually only draw lit pixels
  // every Nth frame (e.g. every 4th frame).  
  // sStrobePhase is a counter that runs from zero to kStrobeCycleLength-1,
  // and then resets to zero.  
  const uint8_t kStrobeCycleLength = 4; // light every Nth frame
  static uint8_t sStrobePhase = 0;
  sStrobePhase = sStrobePhase + 1;
  if( sStrobePhase >= kStrobeCycleLength ) { 
    sStrobePhase = 0; 
  }

  // We only draw lit pixels when we're in strobe phase zero; 
  // in all the other phases we leave the LEDs all black.
  if( sStrobePhase == 0 ) {

    // The dash spacing cycles from 4 to 9 and back, 8x/min (about every 7.5 sec)
    uint8_t dashperiod= beatsin8( 8/*cycles per minute*/, 4,10);
    // The width of the dashes is a fraction of the dashperiod, with a minimum of one pixel
    uint8_t dashwidth = (dashperiod / 4) + 1;
    
    // The distance that the dashes move each cycles varies 
    // between 1 pixel/cycle and half-the-dashperiod/cycle.
    // At the maximum speed, it's impossible to visually distinguish
    // whether the dashes are moving left or right, and the code takes
    // advantage of that moment to reverse the direction of the dashes.
    // So it looks like they're speeding up faster and faster to the
    // right, and then they start slowing down, but as they do it becomes
    // visible that they're no longer moving right; they've been 
    // moving left.  Easier to see than t o explain.
    //
    // The dashes zoom back and forth at a speed that 'goes well' with
    // most dance music, a little faster than 120 Beats Per Minute.  You
    // can adjust this for faster or slower 'zooming' back and forth.
    uint8_t zoomBPM = 64;                                                                                  //128 BPM... evtl parametisieren!
    int8_t  dashmotionspeed = beatsin8( (zoomBPM /2), 1,dashperiod);
    // This is where we reverse the direction under cover of high speed
    // visual aliasing.
    if( dashmotionspeed >= (dashperiod/2)) { 
      dashmotionspeed = 0 - (dashperiod - dashmotionspeed );
    }

    
    // The hueShift controls how much the hue of each dash varies from 
    // the adjacent dash.  If hueShift is zero, all the dashes are the 
    // same color. If hueShift is 128, alterating dashes will be two
    // different colors.  And if hueShift is range of 10..40, the
    // dashes will make rainbows.
    // Initially, I just had hueShift cycle from 0..130 using beatsin8.
    // It looked great with very low values, and with high values, but
    // a bit 'busy' in the middle, which I didnt like.
    //   uint8_t hueShift = beatsin8(2,0,130);
    //
    // So instead I layered in a bunch of 'cubic easings'
    // (see http://easings.net/#easeInOutCubic )
    // so that the resultant wave cycle spends a great deal of time
    // "at the bottom" (solid color dashes), and at the top ("two
    // color stripes"), and makes quick transitions between them.
    uint8_t cycle = beat8(2); // two cycles per minute
    uint8_t easedcycle = ease8InOutCubic( ease8InOutCubic( cycle));
    uint8_t wavecycle = cubicwave8( easedcycle);
    uint8_t hueShift = scale8( wavecycle,130);


    // Each frame of the animation can be repeated multiple times.
    // This slows down the apparent motion, and gives a more static
    // strobe effect.  After experimentation, I set the default to 1.
    uint8_t strobesPerPosition = 1; // try 1..4


    // Now that all the parameters for this frame are calculated,
    // we call the 'worker' function that does the next part of the work.
    discoWorker( dashperiod, dashwidth, dashmotionspeed, strobesPerPosition, hueShift);
  }  
}


//Function to call in the effect selection
void discostrobe(){
    unsigned long currentMillis = millis();
    uint8_t animationInterval = 10; //50 Hz
    if (currentMillis - animationPreviousMillis >= animationInterval)
    {
        animationPreviousMillis = currentMillis;
        discostrobe_loop();
    }
}