//Header 
const char HEAD_page[] PROGMEM = R"=====(
<!DOCTYPE html><html><head><title>Zengtronic LED</title>
<meta name="HandheldFriendly" content="True"><meta name="MobileOptimized" content="480"><meta name="viewport" content="width=device-width, initial-scale=1">
<link rel="shortcut icon" href="data:image/vnd.microsoft.icon;base64,AAABAAEAEBAAAAEAIABoBAAAFgAAACgAAAAQAAAAIAAAAAEAIAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAXAAAATgAAAEwAAABMAAAATAAAAEwAAABMAAAATAAAAEwAAABMAAAATAAAAEwAAABMAAAATAAAAE4AAAAXAAAAHwAAAP8AAADHAAAApgAAAKUAAAClAAAApQAAAKUAAAClAAAApQAAAKUAAAClAAAApgAAAMcAAAD/AAAAHwAAAAEAAAAkAAAA/wAAACEAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACEAAAD/AAAAJAAAAAEAAAAAAAAAAQAAACQAAAD/AAAAJAAAAAEAAAAAAAAAAAAAAAAAAAAAAAAAAQAAACQAAAD/AAAAJAAAAAEAAAAAAAAAAAAAAAAAAAABAAAAJAAAAP8AAAAkAAAAAQAAAAAAAAAAAAAAAQAAACQAAAD/AAAAJAAAAAEAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAEAAAAkAAAA/wAAACQAAAABAAAAAQAAACQAAAD/AAAAJAAAAAEAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAQAAACQAAAD/AAAAJQAAACUAAAD/AAAAJAAAAAEAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABAAAAJQAAAPcAAAD3AAAAJQAAAAEAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAQAAACUAAAD3AAAA9wAAACUAAAABAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAQAAACQAAAD/AAAAJQAAACUAAAD/AAAAJAAAAAEAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAQAAACQAAAD/AAAAJAAAAAEAAAABAAAAJAAAAP8AAAAkAAAAAQAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAQAAACQAAAD/AAAAJAAAAAEAAAAAAAAAAAAAAAEAAAAkAAAA/wAAACQAAAABAAAAAAAAAAAAAAAAAAAAAQAAACQAAAD/AAAAJAAAAAEAAAAAAAAAAAAAAAAAAAAAAAAAAQAAACQAAAD/AAAAJAAAAAEAAAAAAAAAAQAAACQAAAD/AAAAIQAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAIQAAAP8AAAAkAAAAAQAAAB8AAAD/AAAAxwAAAKYAAAClAAAApQAAAKUAAAClAAAApQAAAKUAAAClAAAApQAAAKYAAADHAAAA/wAAAB8AAAAXAAAATgAAAEwAAABMAAAATAAAAEwAAABMAAAATAAAAEwAAABMAAAATAAAAEwAAABMAAAATAAAAE4AAAAX//8AAIABAADf+wAA7/cAAPfvAAD73wAA/b8AAP5/AAD+fwAA/b8AAPvfAAD37wAA7/cAAN/7AACAAQAA//8AAA==" />
<script>
function ajax(url,id) {
console.log("REQUEST TO : " + url); //DEBUG - DELETE LATER
var xhttp = new XMLHttpRequest();
xhttp.open("GET", url, true);
xhttp.onload = function() {
  if (xhttp.status === 200) {
    x = xhttp.responseText;
    console.log("HTTP 200 OK : " + x); //DEBUG - DELETE LATER
    document.getElementById(id).innerHTML = x; 
    }
   }
xhttp.send();
}
function cajax(url, id, value){
  let mapfactor = 366/255;
  document.getElementById('ss').style.background =  "linear-gradient(to right, #FFFFFF 0%, hsl(" + (mapfactor*value) + ",100%,50%))"
  ajax(url, id);
}

</script>
<style>
body{background-color: #151515;font-family:"Arial";color:white;}
button,.button{width: 100%;border: 1px solid #000000;border-radius: 0.3rem;background-color: #6f6f6f;color: #fff;line-height: 2.4rem;font-size: 1.2rem;display: block;text-decoration:none;text-align: center;transition:background 0.3s ease;}
button:hover,button:focus,.button:hover,.button:focus{background-color: #424242;}
button{cursor: pointer;}
select{height: 2.5rem;text-align-last: center;}
option{background-color: white;color: black;}
hr{border: 0;height: 1px;background-image: linear-gradient(to right, rgba(0, 0, 0, 0), rgba(255, 255, 255, 0.75), rgba(0, 0, 0, 0));}
.sl {-webkit-appearance: none;width: 100%;height: 15px;border-radius: 10px;background: #777777;outline: none;}
.sl::-webkit-slider-thumb {-webkit-appearance: none;appearance: none;width: 25px;height: 25px;border: 1px solid #000000;border-radius: 50%;background: #6f6f6f;cursor: pointer;}
.sl::-webkit-slider-thumb:hover{background-color: #424242;}
.cbwrap{text-align: left;}
.cbwrap label{display: inline-block; padding-left: 5px;}
.sc{text-align: left;}
.w50{width: 49%;}
.fl{float:left;}
.fr{float:right;}
.cf{clear:both;}
.info{color: #525252;}
label{display:block;text-align:left;}
#cs{background: linear-gradient(to right, #ff0000 0%, #ffff00 17%, #00ff00 33%, #00ffff 50%, #0000ff 67%, #ff00ff 83%, #ff0000 100%);}
#bs{background: linear-gradient(to right, #000000 0%, #ffffff 100%);}
#ss{background: linear-gradient(to right, #FFFFFF 0%, #ff0000 100%);}
input[type="file"] {display: none;}
input[type="submit"]{cursor: pointer;}
</style></head><body><center>
<div style="display: block;  max-width: 480px;">
<h1>Zengtronic LED</h1>
)=====";

//Index Footer
const char FOOTER_page[] = R"=====(
<hr>
<button class="w50 fl" onclick="ajax('save')" >save*</button>
<a class="button w50 fr" href="settings">settings</a><br class="cf"><br>
</div>
<div class="info"> * autosaving current config every 5 minutes.</div>
</center>
</body></html>
)=====";

//Settings Overview Footer
const char FOOTER_SETTINGS_O_page[] = R"=====(
<hr>
<a class="button" href="/">back</a><br>
</div>
</body></html>
)=====";


//Settings Footer
const char FOOTER_SETTINGS_page[] = R"=====(
<hr>
<a class="button" href="/settings">back</a><br>
</div>
</body></html>
)=====";


//Settings Footer
const char MAIN_SETTINGS_page[] = R"=====(
<a class="button" href="/ssys">System Configuration</a><br>
<a class="button" href="/ssound">Sound Configuration</a><br>
<a class="button" href="/slight">Light Configuration</a><br>
<a class="button" href="/swifi">WiFi Configuration</a><br>
<a class="button" href="/restart">Restart</a>
)=====";

// <a class="button" href="/firmware">Firmware Upgrade</a><br> //Update currently not avaiable


//Update Body
const char UPDATE_page[] = R"=====(
<form method='POST' action='update' enctype='multipart/form-data'>
<label for="fui" class="button">Choose Firmware File (*.bin)</label>
<input type='file' class="button" name='update' id='fui'><br>
<input type='submit' class="button" value='Update'>
</form>
)=====";

//Infopanel with actual state
String getInfoPanel(){
  String p = power ? "on":"off";
  String s = (String)"power: <span id=\"pr\">" + p + "</span>, mode: <span id=\"m\">" + (String)mode + "</span>, pal: <span id=\"p\">" + (String)palette + "</span>, bright:<span id=\"b\"> " + (String)FastLED.getBrightness() + "</span>, col: <span id=\"c\">" +color+ "</span>, sat: <span id=\"s\">" + saturation + "</span>, speed: <span id=\"sp\">" + speed + "</span>, extra: <span id=\"ex\">" + extraparam + "</span>"; 
  return s;
}

//Index Body
String getIndexMenu(){
  String s = "";
  #ifdef DEBUG
    s+ ="<div class=\"info\" id=\"info\">";
    s+=getInfoPanel();
    s+="</div>";
    s+="<script>setInterval(function(){var e=new XMLHttpRequest;e.open(\"GET\",\"info\",!0),e.onload=function(){200===e.status&&(x=e.responseText,document.getElementById(\"info\").innerHTML=x)},e.send()},5e3);</script>";
  #endif
  s+="<br><button onclick=\"ajax('power','pr')\" >on/off</button><br>";
  s+="<select id=\"mdd\" class=\"button\" onchange=\"ajax('/mode?dir=set&value='+this.value,'m')\">";
  for(int i=0; i<modeCount;i++){
    s+="<option value=\"" + (String)i + "\" ";
    if(i== mode){
      s+="selected";
    }
    s+=">" + modes[i];
  }
  s+="</select><br>";

  s+="<select id=\"pdd\" class=\"button\" onchange=\"ajax('/palette?dir=set&value='+this.value,'p')\">";
  for(int i=0; i<paletteCount;i++){
    s+="<option value=\"" + (String)i + "\" ";
    if(i== palette){
      s+="selected";
    }
    s+=">" + palettes[i];
  }
  s+="</select><br>";
  
  s+=" <div class=\"sc\"><label for=\"bs\">brightness:</label>";
  s+="<input type=\"range\" min=\"0\" max=\""+ (String)max_brightness  +"\" value=\""+ (String)FastLED.getBrightness() +"\" class=\"sl\" id=\"bs\" onchange=\"ajax('/brightness?dir=set&value='+this.value,'b')\"><br><br>";
  s+="<label for=\"cs\">Color:</label>";
  s+="<input type=\"range\" min=\"0\" max=\"255\" value=\""+ (String)color +"\" class=\"sl\" id=\"cs\" onchange=\"cajax('/color?dir=set&value='+this.value,'c', this.value)\" ><br><br>";
  s+="<label for=\"ss\">Saturation:</label>";
  s+="<input type=\"range\" min=\"0\" max=\"255\" value=\""+ (String)saturation +"\" class=\"sl\" id=\"ss\" style=\"background: linear-gradient(to right, #FFFFFF 0%, hsl(" + (String)(1.40625*(float)color) + ",100%,50%))\" onchange=\"ajax('/saturation?dir=set&value='+this.value,'s')\"><br><br>";
  s+="<label for=\"sp\">Effect Speed:</label>";
  s+="<input type=\"range\" min=\"1\" max=\"100\" value=\""+ (String)speed +"\" class=\"sl\" id=\"sp\" onchange=\"ajax('/speed?dir=set&value='+this.value,'sp')\"><br><br>";
  s+="<label for=\"ex\">Effect Intensity:</label>";
  s+="<input type=\"range\" min=\"0\" max=\"100\" value=\""+ (String)extraparam +"\" class=\"sl\" id=\"ex\" onchange=\"ajax('/extra?dir=set&value='+this.value,'ex')\"><br>";
  
  return s;
}

String getSystemSettingsMenu(){
  String s=(String)"<form action=\"sss\" method=\"post\">";
  s+=(String)"<label for=\"numleds\">Number of LEDs:</label>";
  s+=(String)"<input type=\"number\" class=\"button\" name=\"numleds\" min=\"0\" max=\"1024\" id=\"numleds\" value=\""+num_leds+"\"><br>";
  // s+=(String)"<label for=\"numleds\">Size of Array (min. Number of LEDs):</label>";
  // s+=(String)"<input type=\"number\" class=\"button\" id=\"arrayleds\" value=\""+ array_leds +"\"><br>";
  //s+=(String)"<label for=\"numleds\">Frames per second:</label>";
  //s+=(String)"<input type=\"number\" class=\"button\" id=\"fps\" name=\"fps\" value=\""+ frames_per_second +"\"><br>"; // not used anymore 
  s+=(String)"<label for=\"numleds\">Max. Brightness:</label>";
  s+=(String)"<input type=\"number\" min=\"10\" max=\"255\" class=\"button\" name=\"max\" id=\"max\" value=\""+ max_brightness +"\"><br>";
  s+=(String)"<div class=\"cbwrap\"><input type=\"checkbox\" id=\"powermngmt\" name=\"pmngmt\" value=\"power\"";
  if(power_mngmt){s+= (String)" checked";}
  s+=(String)"><label for=\"powermngmt\">Power Management</label></div><br>";
  s+=(String)"<label for=\"max_current\">Max. Current (mA):</label>";
  s+=(String)"<input type=\"number\" min=\"100\" max=\"65535\" class=\"button\" name=\"mc\" id=\"max_current\" value=\""+ max_current +"\"><br>";
  s+=(String)"<input type=\"submit\" class=\"button\" value=\"save\"></form>";
  return s;
}

String getLightSettingsMenu(){

  String s = (String)"<label for=\"t\">Color Temperature:</label>";
  s+=(String)"<select id=\"ct\" class=\"button\" name=\"ct\" onchange=\"ajax('/temperature?dir=set&value='+this.value,'o')\">";
  for(int i=0; i<colorTempCount;i++){
   s+=(String)"<option value=\"" + i + "\" ";
   if(i== currentTemp){
     s+=(String)"selected";
   }
   s+=(String)">" + colorTempStrings[i];
  }
  s+=(String)"</select><br>";

  s+=(String)"<div class=\"info\">This settings were set on change.<br>To save for next restart click on the button below.</div><br>";
  s+=(String)"<input type=\"button\" class=\"button\" value=\"save\" onclick=\"ajax('save')\">";
  return s;
}

String getWifiSettingsMenu(){
  String s = (String)"<form action=\"sss\" method=\"post\"><label for=\"wm\">Wifi Mode:</label>";
  
   s+=(String)"<select id=\"wm\" name=\"wm\" class=\"button\">";
  for(int i=0; i<wifiModeCount;i++){
    s+=(String)"<option value=\"" + (String)i + "\" ";
    if(i== wifimode){
      s+=(String)"selected";
    }
    s+=">" + wifiModesStrings[i] + "</option>";
  }
  s+=(String)"</select><br>";
  s+=(String)"<label for=\"ws1\">SSID 1*:</label>";
  s+=(String)"<input type=\"text\" name=\"ws1\" class=\"button\" id=\"ws1\" maxlength=\"32\" value=\"";
  s+=ssid1;
  s+=(String)"\"><br>";
  s+=(String)"<label for=\"wp1\">Password 1*:</label>";
  s+=(String)"<input type=\"text\" name=\"wp1\" class=\"button\" id=\"wp1\" maxlength=\"32\" value=\"";
  s+=password1;
  s+=(String)"\"><br>";
  
  s+=(String)"<label for=\"aws\">Access-Point SSID *:</label>";
  s+=(String)"<input type=\"text\" class=\"button\" name=\"aws\" id=\"aws\" maxlength=\"32\" value=\"";
  s+=apssid;
  s+=(String)"\"><br>";
  s+=(String)"<label for=\"awp\">Access-Point Password*:</label>";
  s+=(String)"<input type=\"text\" class=\"button\" name=\"awp\" id=\"awp\" maxlength=\"32\" value=\"";
  s+=appassword;
  s+=(String)"\"><br>";
  s+=(String)"<div class=\"info\"> * max 32 chars.</div><br><input type=\"submit\" class=\"button\" value=\"save\">";
  s+=(String)"</form>";
  return s;
}

String getSoundSettingsMenu(){
  String s = (String)"<label for=\"aina\">Max. input soundlevel (0% - 100%):</label>";
  s+=(String)"<input type=\"range\" min=\"10\" max=\"100\" value=\""+ (String)ain_adjustment +"\" class=\"sl\" id=\"aina\" onchange=\"ajax('/aina?dir=set&value='+this.value,'aina')\"><br><br>";
  s+=(String)"<label for=\"ainn\">Noise floor:</label>";
  s+=(String)"<input type=\"range\" min=\"0\" max=\"" + ADC_RESOLUTION/4 + "\" value=\""+ (String)ain_noise_floor +"\" class=\"sl\" id=\"ainn\" onchange=\"ajax('/ainn?dir=set&value='+this.value,'ainn')\"><br><br>";
  s+=(String)"<div class=\"cbwrap\"><input type=\"checkbox\" id=\"ainr\" value=\"rev\" onchange=\"ajax('/ainr?dir=set&value='+document.getElementById('ainr').checked,'ainr')\"";
  if(ain_reverse){s+= (String)" checked";}
  s+=(String)"><label for=\"ainr\">Mirror Input</label></div><br>";
  s+=(String)"<div class=\"info\">This settings were set on change.<br>To save for next restart click on the button below.</div><br>";
  s+=(String)"<input type=\"button\" class=\"button\" value=\"save\" onclick=\"ajax('save')\">";
  return s;
}


//Server Handles

void handleRoot() {
 Serial.println("You called root page");
 httpServer.send(200, "text/html", HEAD_page + getIndexMenu() +  FOOTER_page); //Send web page
}

void handleFirmwareUpdate() {
 Serial.println("You called firmware update page");
 httpServer.send(200, "text/html", (String)"" + HEAD_page + UPDATE_page + FOOTER_SETTINGS_page); //Send web page

}

void handleSettings(){
 Serial.println("You called settings page");
 httpServer.send(200, "text/html", (String)"" + HEAD_page + MAIN_SETTINGS_page + FOOTER_SETTINGS_O_page); //Send web page
}

void handleSettingsSystem(){
 Serial.println("You called settings page");
 httpServer.send(200, "text/html", HEAD_page + getSystemSettingsMenu() + FOOTER_SETTINGS_page); //Send web page
}

void handleSettingsLight(){
 Serial.println("You called settings page");
 httpServer.send(200, "text/html", HEAD_page + getLightSettingsMenu() + FOOTER_SETTINGS_page); //Send web page
}

void handleSettingsWifi(){
 Serial.println("You called WiFi settings page");
 httpServer.send(200, "text/html", HEAD_page + getWifiSettingsMenu() + FOOTER_SETTINGS_page); //Send web page
}

void handleSettingsSound(){
 Serial.println("You called settings page");
 httpServer.send(200, "text/html", HEAD_page + getSoundSettingsMenu() + FOOTER_SETTINGS_page); //Send web page
}

void handleSettingsSave(){
 if((httpServer.method() == HTTP_POST)){
   //Wifi Settings save
   if(httpServer.hasArg("wp1") && httpServer.hasArg("ws1") && httpServer.hasArg("aws") && httpServer.hasArg("awp") && httpServer.hasArg("wm")){
    t_ssid1 = httpServer.arg("ws1");
    t_password1 = httpServer.arg("wp1");
    t_apssid = httpServer.arg("aws");
    t_appassword = httpServer.arg("awp");
    t_wifimode = httpServer.arg("wm").toInt();
    if(t_wifimode > (wifiModeCount-1)){t_wifimode = 2;}
   }
 
   //System Settings save
   if(httpServer.hasArg("numleds")  && httpServer.hasArg("max") && httpServer.hasArg("mc")){ //&& httpServer.hasArg("fps")
    //frames_per_second = httpServer.arg("fps").toInt();
    int maxb = httpServer.arg("max").toInt();
    if(maxb > 255){
      max_brightness = 255;
    }else if(maxb < 10){
      max_brightness = 10;
    }else{
      max_brightness = maxb;
    }
    num_leds = httpServer.arg("numleds").toInt();
    max_current = httpServer.arg("mc").toInt();
    if(httpServer.hasArg("pmngmt")){
      power_mngmt = true;
    }else{
      power_mngmt = false;
    }
   }
 }
 
 httpServer.send(200, "text/html",(String)"" + HEAD_page + "Saved Settings. Restarting Device...<br>You can go back in 10 seconds." + FOOTER_SETTINGS_O_page); //Send web page
 safeRestart();
}

void handleInfo() {
 Serial.println("You called info page");
 httpServer.send(200, "text/html", getInfoPanel()); //Send web page

}

void handlePower() { 
 switchPower();
 String p = power ? "on":"off";
 httpServer.send(200, "text/html", p);
 Serial.println("Power on/off");
}

void handleMode() { 
   if (!(httpServer.arg("dir")== "")){
    int value = 1;
    if(httpServer.arg("value")== ""){
      value = 1;
    }else{
      value = httpServer.arg("value").toInt();
    }
    if(httpServer.arg("dir")== "up"){
       setMode(mode+value); 
    }
    if(httpServer.arg("dir")== "down"){
       setMode(mode-value);
     }
     if(httpServer.arg("dir")== "set"){
      setMode(value);
     }
  }
 httpServer.send(200, "text/html", (String)mode);
}

void handleColor() { 
   if (!(httpServer.arg("dir")== "")){
    int value = 1;
    if(httpServer.arg("value")== ""){
      value = 1;
    }else{
      value = httpServer.arg("value").toInt();
    }
    if(httpServer.arg("dir")== "up"){
       setColor(color+value); 
    }
    if(httpServer.arg("dir")== "down"){
       setColor(color-value);
     }
     if(httpServer.arg("dir")== "set"){
      setColor(value);
     }
  }
  
 httpServer.send(200, "text/html", (String)color);
}

void handleSpeed() { 
   if (!(httpServer.arg("dir")== "")){
    int value = 1;
    if(httpServer.arg("value")== ""){
      value = 1;
    }else{
      value = httpServer.arg("value").toInt();
    }
    if(httpServer.arg("dir")== "up"){
       setSpeed(speed+value); 
    }
    if(httpServer.arg("dir")== "down"){
       setSpeed(speed-value);
     }
     if(httpServer.arg("dir")== "set"){
      setSpeed(value);
     }
  }
 httpServer.send(200, "text/html", (String)speed);
}

void handleExtra() { 
   if (!(httpServer.arg("dir")== "")){
    int value = 1;
    if(httpServer.arg("value")== ""){
      value = 1;
    }else{
      value = httpServer.arg("value").toInt();
    }
    if(httpServer.arg("dir")== "up"){
       setExtra(extraparam+value); 
    }
    if(httpServer.arg("dir")== "down"){
       setExtra(extraparam-value);
     }
     if(httpServer.arg("dir")== "set"){
      setExtra(value);
     }
  }
  
 httpServer.send(200, "text/html", (String)extraparam);
}

void handleSaturation() { 
   if (!(httpServer.arg("dir")== "")){
    int value = 1;
    if(httpServer.arg("value")== ""){
      value = 1;
    }else{
      value = httpServer.arg("value").toInt();
    }
    if(httpServer.arg("dir")== "up"){
       setSaturation(saturation+value); 
    }
    if(httpServer.arg("dir")== "down"){
       setSaturation(saturation-value);
     }
     if(httpServer.arg("dir")== "set"){
      setSaturation(value);
     }
  }
 httpServer.send(200, "text/html", (String)saturation);
}

void handleTemperature() { 
   if (!(httpServer.arg("dir")== "")){
    int value = 0;
    if(httpServer.arg("value")== ""){
      value = 0;
    }else{
      value = httpServer.arg("value").toInt();
    }
    if(httpServer.arg("dir")== "set"){
      if(value > colorTempCount){value = 0;}
      FastLED.setTemperature(colorTemp[value]);
      currentTemp = value;
     }
  }
 httpServer.send(200, "text/html", (String)mode);
}

void handlePalette() { 
    if (!(httpServer.arg("dir")== "")){
    int value = 1;
    if(httpServer.arg("value")== ""){
      value = 1;
    }else{
      value = httpServer.arg("value").toInt();
    }
    if(httpServer.arg("dir")== "up"){
       setPalette(palette+value); 
    }
    if(httpServer.arg("dir")== "down"){
       setPalette(palette-value);
     }
     if(httpServer.arg("dir")== "set"){
      setPalette(value);
     }
  }
 httpServer.send(200, "text/html", (String)palette);
}

void handleBrightness() { 
    if (!(httpServer.arg("dir")== "")){
    int value = 1;
    if(httpServer.arg("value")== ""){
      value = 1;
    }else{
      value = httpServer.arg("value").toInt();
    }
    if(httpServer.arg("dir")== "up"){
       setHelligkeit(FastLED.getBrightness()+value); 
    }
    if(httpServer.arg("dir")== "down"){
       setHelligkeit(FastLED.getBrightness()-value);
     }
     if(httpServer.arg("dir")== "set"){
      setHelligkeit(value);
     }
  }
 httpServer.send(200, "text/html", (String)FastLED.getBrightness());
}

void handleAINadjustment() { 
   if (!(httpServer.arg("dir")== "")){
    int value = 1;
    if(httpServer.arg("value")== ""){
      value = ain_adjustment;
    }else{
      value = httpServer.arg("value").toInt();
    }
    if(httpServer.arg("dir")== "up"){
       setAINadjustment(ain_adjustment+value); 
    }
    if(httpServer.arg("dir")== "down"){
       setAINadjustment(ain_adjustment-value);
     }
     if(httpServer.arg("dir")== "set"){
      setAINadjustment(value);
     }
  }
 httpServer.send(200, "text/html", (String)ain_adjustment);
}

void handleAINnoise() { 
   if (!(httpServer.arg("dir")== "")){
    int value = 1;
    if(httpServer.arg("value")== ""){
      value = ain_noise_floor;
    }else{
      value = httpServer.arg("value").toInt();
    }
    if(httpServer.arg("dir")== "up"){
       setAINnoise(ain_noise_floor+value); 
    }
    if(httpServer.arg("dir")== "down"){
       setAINnoise(ain_noise_floor-value);
     }
     if(httpServer.arg("dir")== "set"){
      setAINnoise(value);
     }
  }
 httpServer.send(200, "text/html", (String)ain_noise_floor);
}

void handleAINmirror() {
  if(httpServer.arg("value")== "true" || httpServer.arg("value")== "1"){
      setAINmirror(true);
    }else if(httpServer.arg("value")== "false" || httpServer.arg("value")== "0"){
      setAINmirror(false);
    }
  httpServer.send(200, "text/html", (String)ain_reverse);
}

void handleSave(){
 Serial.println("--- Manual Save ---");
 saveConfig();
 httpServer.send(200, "text/html", "saved");
}

void handleRestart(){
  httpServer.sendHeader("Location", String("/"), true);
  httpServer.send(302, "text/plain","");
  delay(100);
  ESP.restart();
}

void handleNotFound(){
  String message = "<pre>File not Found (404)\n\n";
  message += "URI: ";
  message += httpServer.uri();
  message += "\nMethod: ";
  message += (httpServer.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += httpServer.args();
  message += "\n";
  for (uint8_t i=0; i<httpServer.args(); i++){
    message += " " + httpServer.argName(i) + ": " + httpServer.arg(i) + "\n";
  }
  message += "</pre>";
  httpServer.send(404, "text/html",HEAD_page + message + FOOTER_SETTINGS_page);
}
