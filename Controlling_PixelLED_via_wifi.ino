#include <ESP8266WiFi.h>
#include <EEPROM.h>
#include <FastLED.h>

const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

WiFiServer server(80);

IPAddress local_IP(192, 168, 8, 107);
// Set your Gateway IP address
IPAddress gateway(192, 168, 8, 1);
//
IPAddress subnet(255, 255, 255, 0);
//IPAddress primaryDNS(8, 8, 8, 8);   //optional
//IPAddress secondaryDNS(8, 8, 4, 4); //optional

const int numOfPatterns = 9; // number of patterns
byte px[numOfPatterns];
String patternNames[] = { // change the pattern names if you want
  "Pattern 1",
  "Pattern 2",
  "Pattern 3",
  "Pattern 4",
  "Pattern 5",
  "Pattern 6",
  "Pattern 7",
  "Pattern 8",
  "Pattern 9"
};

int patternCount = 1;
int quickPlay = 0;


//for pattern 5
int counter1_p5 = 0;
int selector_p5 = -10;
long delayTimmer = millis();

//for pattern 6
int counter1_p6 = 0;


//for pattern 7
int counter1_p7 = 0;
int counter2_p7 = 0;

//for pattern 8
int Position = 0;
int counter1_p8 = 0;
long delayTimmer2 = millis();
int colorCounter = 0;

//for pattern 9
boolean selector_p9 = 0;




#define LED_PIN     3
#define NUM_LEDS    200
int BRIGHTNESS = 200;
#define LED_TYPE    WS2811
#define COLOR_ORDER RGB

CRGB leds[NUM_LEDS];
#define UPDATES_PER_SECOND 100
CRGBPalette16 currentPalette;
TBlendType    currentBlending;
extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;


#define TEMPERATURE_1 Tungsten100W
#define TEMPERATURE_2 OvercastSky
#define DISPLAYTIME 20
#define BLACKTIME   3
long timecount;



int FRAMES_PER_SECOND = 120;
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))


CRGBPalette16 gPal;
bool gReverseDirection = false;
#define COOLING  55
#define SPARKING 120


CRGBArray<NUM_LEDS> ledss;
byte colors[5][3] = {  // create buddhist five color
  {0, 0, 255},
  {255, 255, 0},
  {255, 0, 0},
  {255, 255, 255},
  {252, 136, 3}
};
byte led[NUM_LEDS];

char color[] = {HUE_RED, HUE_BLUE, HUE_GREEN, HUE_YELLOW, HUE_PINK, HUE_PURPLE, HUE_ORANGE};


void setup() {
  Serial.begin(115200);
  delay(10);

  //connect to WiFi network
  Serial.println();
  Serial.println();

  // Configures static IP address
  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("STA Failed to configure");
  }

  
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin (ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  { delay(500);
    Serial.print(".");
  }


  Serial.println("");
  Serial.println("WiFi connected");
  // Start the server
  server.begin();
  Serial.println("Server started");

  //Print the IP Address

  Serial.print("Use this url to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");



  EEPROM.begin(512);

  for (int i = 0; i < numOfPatterns; i++) {
    px[i] = EEPROM.read(i);
  }
  for (int i = 0; i < numOfPatterns; i++) {
    Serial.print("p" + String(i + 1) + "=" + String(px[i]) + " ");
  }



  delay( 3000 ); // power-up safety delay
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );

  currentPalette = RainbowColors_p;
  currentBlending = LINEARBLEND;
  timecount = millis();

  createFiveColorLEDArray( );  // create buddhist five color led array by calling the function

}

typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { rainbow, rainbowWithGlitter, confetti, juggle, bpm };
//SimplePatternList gPatterns = { rainbow, rainbowWithGlitter, confetti, sinelon, juggle, bpm };

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns



void loop()
{
  // Check if a client has connected
  WiFiClient client = server.available();

  Serial.println("new client");
  while (!client || !client.available()) {
    //if (!client) {
    client = server.available();
    //}


    //pattern 1  ----------------------------------------------------
    if ((patternCount == 1 && px[0] == 1 && quickPlay == 0 && (millis() - timecount < 60000)) || quickPlay == 1) {
      ChangePalettePeriodically();
      static uint8_t startIndex = 0;
      startIndex = startIndex + 1; // motion speed
      FillLEDsFromPaletteColors( startIndex);
      FastLED.show();
      FastLED.delay(1000 / UPDATES_PER_SECOND);

    }

    delay(1);
    if ((patternCount == 1 && quickPlay == 0 && (millis() - timecount > 60000)) || (patternCount == 1 && px[0] == 0)) {
      timecount = millis();
      patternCount = 2;
      Serial.println("Pattern 2");
    }


    //pattern 2 -------------------------------------------------
    if ((patternCount == 2 && px[1] == 1 && quickPlay == 0 && (millis() - timecount < 30000)) || quickPlay == 2) {
      // draw a generic, no-name rainbow
      static uint8_t starthue = 0;
      fill_rainbow( leds , NUM_LEDS, --starthue, 20);

      // Choose which 'color temperature' profile to enable.
      uint8_t secs = (millis() / 1000) % (DISPLAYTIME * 2);
      if ( secs < DISPLAYTIME) {
        FastLED.setTemperature( TEMPERATURE_1 ); // first temperature
        leds[0] = TEMPERATURE_1; // show indicator pixel
      } else {
        FastLED.setTemperature( TEMPERATURE_2 ); // second temperature
        leds[0] = TEMPERATURE_2; // show indicator pixel
      }

      // Black out the LEDs for a few secnds between color changes
      // to let the eyes and brains adjust
      //if ( (secs % DISPLAYTIME) < BLACKTIME) {
      // memset8( leds, 0, NUM_LEDS * sizeof(CRGB));
      //}
      FastLED.show();
      FastLED.delay(8);

    }

    if ((patternCount == 2 && quickPlay == 0 && (millis() - timecount > 30000))  || (patternCount == 2 && px[1] == 0)) {
      timecount = millis();
      patternCount = 3;
      Serial.println("Pattern 3");
    }

    //pattern 3 -------------------------------------------------
    if ((patternCount == 3 && px[2] == 1 && quickPlay == 0 && (millis() - timecount < 50000)) || quickPlay == 3) {
      // Call the current pattern function once, updating the 'leds' array
      gPatterns[gCurrentPatternNumber]();

      // send the 'leds' array out to the actual LED strip
      FastLED.show();
      // insert a delay to keep the framerate modest
      FastLED.delay(1000 / FRAMES_PER_SECOND);

      // do some periodic updates
      EVERY_N_MILLISECONDS( 20 ) {
        gHue++;  // slowly cycle the "base color" through the rainbow
      }
      EVERY_N_SECONDS( 10 ) {
        nextPattern();  // change patterns periodically
      }
    }
    if ((patternCount == 3 && quickPlay == 0 && (millis() - timecount > 50000))  || (patternCount == 3 && px[2] == 0)) {
      timecount = millis();
      patternCount = 4;
      Serial.println("Pattern 4");
    }


    //pattern 4 ------------------------------------------------- this pattern has a considerable delay

    if ((patternCount == 4 && px[3] == 1 && quickPlay == 0 && (millis() - timecount < 40000)) || quickPlay == 4) {
      FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(ledss, NUM_LEDS).setCorrection( TypicalLEDStrip );
      BRIGHTNESS = 255;

      static uint8_t hue;
      for (int i = 0; i < NUM_LEDS / 2; i++) {
        // fade everything out
        ledss.fadeToBlackBy(40);

        // let's set an led value
        ledss[i] = CHSV(hue++, 255, 255);

        // now, let's first 20 leds to the top 20 leds,
        ledss(NUM_LEDS / 2, NUM_LEDS - 1) = ledss(NUM_LEDS / 2 - 1 , 0);
        FastLED.delay(33);
      }


      FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
      BRIGHTNESS = 200;
    }
    if ((patternCount == 4 && quickPlay == 0 && (millis() - timecount > 40000))  || (patternCount == 4 && px[3] == 0)) {
      timecount = millis();
      patternCount = 5;
      Serial.println("Pattern 5");
    }






    //pattern 5 -------------------------------------------------
    if ((patternCount == 5 && px[4] == 1 && quickPlay == 0 && (millis() - timecount < 30000)) || quickPlay == 5) {
      BRIGHTNESS = 255;

      if (millis() - delayTimmer > 30) {
        delayTimmer = millis();

        if (selector_p5 < 0) {
          for (int i = 0; i < NUM_LEDS; i++) {
            if (i + counter1_p5 >= NUM_LEDS) {
              leds[i + counter1_p5 - NUM_LEDS] = CHSV(i * 10 + 5, 255, 255);
            }
            else {
              leds[i + counter1_p5] = CHSV(i * 10 + 5, 255, 255);
            }
          }
          FastLED.show();
          counter1_p5++;
          if (counter1_p5 == NUM_LEDS) {
            selector_p5++;
            counter1_p5 = 0;
          }
          if (selector_p5 == 0) {
            selector_p5 = 10;
            counter1_p5 = NUM_LEDS - 1;
          }

        }
        else {
          for (int i = 0; i < NUM_LEDS; i++) {
            if (i + counter1_p5 >= NUM_LEDS) {
              leds[i + counter1_p5 - NUM_LEDS] = CHSV(i * 10 + 5, 255, 255);
            }
            else {
              leds[i + counter1_p5] = CHSV(i * 10 + 5, 255, 255);
            }
          }
          FastLED.show();

          counter1_p5--;
          if (counter1_p5 == -1) {
            selector_p5--;
            counter1_p5 = NUM_LEDS - 1;
          }
          if (selector_p5 == 0) {
            selector_p5 = -10;
            counter1_p5 = 0;
          }
        }

        if ((counter1_p5 == NUM_LEDS - 1 && selector_p5 == 10) || (counter1_p5 == 0 && selector_p5 == -10)) {
          for (int u = 0; u < 3; u++) {
            for (int j = 255; j >= 100; j -= 6) {
              for (int i = 0; i < NUM_LEDS; i++) {
                leds[i] = CHSV(i * 10 + 5, 255, j);
              }
              FastLED.show();
            }
            for (int j = 100; j < 256; j += 6) {
              for (int i = 0; i < NUM_LEDS; i++) {
                leds[i] = CHSV(i * 10 + 5, 255, j);
              }
              FastLED.show();
            }
          }
        }
      }
    }
    if ((patternCount == 5 && quickPlay == 0 && (millis() - timecount > 30000))  || (patternCount == 5 && px[4] == 0)) {
      timecount = millis();
      patternCount = 6;
      Serial.println("Pattern 6");
    }





    //pattern 6 -------------------------------------------------

    if ((patternCount == 6 && px[5] == 1 && quickPlay == 0 && (millis() - timecount < 30000)) || quickPlay == 6) {

      if (millis() - delayTimmer > 30) {
        delayTimmer = millis();

        for (int i = 0; i < NUM_LEDS; i++) {
          leds[i] = CHSV(counter1_p5, 255, 255);
        }
        FastLED.show();
        counter1_p5++;
        if (counter1_p5 == 256) {
          counter1_p5 = 0;
        }
      }
    }
    if ((patternCount == 6 && quickPlay == 0 && (millis() - timecount > 30000))  || (patternCount == 6 && px[5] == 0)) {
      timecount = millis();
      patternCount = 7;
      Serial.println("Pattern 7");
    }




    //pattern 7 -------------------------------------------------

    if ((patternCount == 7 && px[6] == 1 && quickPlay == 0 && (millis() - timecount < 30000)) || quickPlay == 7) {
      if (millis() - delayTimmer > 20) {
        delayTimmer = millis();

        leds[counter2_p7] = CHSV(color[counter1_p7], 255, 255);
        FastLED.show();

        counter2_p7++;
        if (counter2_p7 == NUM_LEDS) {
          counter2_p7 = 0;
          counter1_p7++;
        }
        if (counter1_p7 == 7) {
          counter1_p7 = 0;
        }
      }
    }
    if ((patternCount == 7 && quickPlay == 0 && (millis() - timecount > 30000))  || (patternCount == 7 && px[6] == 0)) {
      timecount = millis();
      patternCount = 8;
      Serial.println("Pattern 8");
    }



    //pattern 8 -------------------------------------------------

    if ((patternCount == 8 && px[7] == 1 && quickPlay == 0 && (millis() - timecount < 40000)) || quickPlay == 8) {

      for (int i = 0 ; i < 5 ; i++ ) {
        if (millis() - delayTimmer2 < 5000 && colorCounter == i) {
          RunningLights(colors[i][0], colors[i][1], colors[i][2], 50);
        }
      }

      if (millis() - delayTimmer2 > 5000) {
        delayTimmer2 = millis();
        colorCounter++;
        if (colorCounter == 5) {
          colorCounter = 0;
        }
      }

    }
    if ((patternCount == 8 && quickPlay == 0 && (millis() - timecount > 40000))  || (patternCount == 8 && px[7] == 0)) {
      timecount = millis();
      patternCount = 9;
      Serial.println("Pattern 9");
    }



    //pattern 9 -------------------------------------------------
    if ((patternCount == 9 && px[8] == 1 && quickPlay == 0 && (millis() - timecount < 30000)) || quickPlay == 9) {

      if (millis() - delayTimmer < 10000 && selector_p9 == 0) {
        if (millis() - delayTimmer2 > 100) {
          delayTimmer2 = millis();
          for (int i = 0; i < NUM_LEDS; i++) {
            leds[i] = CRGB(colors[led[i]][0], colors[led[i]][1], colors[led[i]][2]);
          }
          FastLED.show();
          moveForwardOneStep();
        }
      }

      if (millis() - delayTimmer < 10000 && selector_p9 == 1) {
        if (millis() - delayTimmer2 > 100) {
          delayTimmer2 = millis();
          for (int i = 0; i < NUM_LEDS; i++) {
            leds[i] = CRGB(colors[led[i]][0], colors[led[i]][1], colors[led[i]][2]);
          }
          FastLED.show();
          moveBackwardOneStep();
        }
      }

      if (millis() - delayTimmer > 10000) {
        delayTimmer = millis();
        selector_p9 = !selector_p9;
      }
    }
    if ((patternCount == 9 && quickPlay == 0 && (millis() - timecount > 30000))  || (patternCount == 9 && px[8] == 0)) {
      timecount = millis();
      patternCount = 1;
      Serial.println("Pattern 1"); //back to first
    }



    //pattern 10 -------------------------------------------------
    //    if ((patternCount == 10 && px[9] == 1 && quickPlay == 0 && (millis() - timecount < 30000)) || quickPlay == 10) {
    //
    //
    //    }
    //    if ((patternCount == 10 && quickPlay == 0 && (millis() - timecount > 30000))  || (patternCount == 10 && px[9] == 0)) {
    //      timecount = millis();
    //      patternCount = 11;
    //      Serial.println("Pattern 11");
    //    }


    //pattern 11 -------------------------------------------------
    //    if ((patternCount == 11 && px[10] == 1 && quickPlay == 0 && (millis() - timecount < 30000)) || quickPlay == 10) {
    //
    //
    //    }
    //    if ((patternCount == 11 && quickPlay == 0 && (millis() - timecount > 30000))  || (patternCount == 11 && px[10] == 0)) {
    //      timecount = millis();
    //      patternCount = 12;
    //      Serial.println("Pattern 12");
    //    }


    //pattern 12 -------------------------------------------------
    //    if ((patternCount == 12 && px[11] == 1 && quickPlay == 0 && (millis() - timecount < 30000)) || quickPlay == 12) {
    //
    //
    //    }
    //    if ((patternCount == 12 && quickPlay == 0 && (millis() - timecount > 30000))  || (patternCount == 12 && px[11] == 0)) {
    //      timecount = millis();
    //      patternCount = 13;
    //      Serial.println("Pattern 13");
    //    }


    //pattern 13 ------------------------------------------------
    //    if ((patternCount == 13 && px[12] == 1 && quickPlay == 0 && (millis() - timecount < 30000)) || quickPlay == 13) {
    //
    //
    //    }
    //    if ((patternCount == 13 && quickPlay == 0 && (millis() - timecount > 30000))  || (patternCount == 13 && px[12] == 0)) {
    //      timecount = millis();
    //      patternCount = 14;
    //      Serial.println("Pattern 14");
    //    }


    //pattern 14 -------------------------------------------------
    //    if ((patternCount == 14 && px[13] == 1 && quickPlay == 0 && (millis() - timecount < 30000)) || quickPlay == 14) {
    //
    //
    //    }
    //    if ((patternCount == 14 && quickPlay == 0 && (millis() - timecount > 30000))  || (patternCount == 14 && px[13] == 0)) {
    //      timecount = millis();
    //      patternCount = 115;
    //      Serial.println("Pattern 15");
    //    }


    //pattern 15 -------------------------------------------------
    //    if ((patternCount == 15 && px[14] == 1 && quickPlay == 0 && (millis() - timecount < 30000)) || quickPlay == 15) {
    //
    //
    //    }
    //    if ((patternCount == 15 && quickPlay == 0 && (millis() - timecount > 30000))  || (patternCount == 15 && px[14] == 0)) {
    //      timecount = millis();
    //      patternCount = 16;
    //      Serial.println("Pattern 16");
    //    }
  }

  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
  //Match the request

  if (request.indexOf("quickPlay") != -1) {
    for (int i = 0; i < numOfPatterns; i++) {
      if (request.indexOf("quickPlay=" + String(i + 1)) != -1) {
        if (quickPlay == i + 1) {
          quickPlay = 0;
        }
        else {
          quickPlay = i + 1;
        }
        break;
      }
    }
  }
  else if (request.indexOf("p") != -1 && request.indexOf("=") != -1) {
    quickPlay = 0;


    for (int i = 0; i < numOfPatterns; i++) {
      if (request.indexOf("p" + String(i + 1) + "=1") != -1) {
        px[i] = 1 ;
        EEPROM.write(i, 1);
      }
      else {
        px[i] = 0 ;
        EEPROM.write(i, 0);
      }
    }
    EEPROM.commit();
  }


  for (int i = 0; i < numOfPatterns; i++) {
    Serial.print("p" + String(i + 1) + "=" + String(px[i]) + " ");
  }
  Serial.println("quickPlay=" + String(quickPlay));


  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");// do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html lang='en'>");
  client.println("<head>");
  client.println("<meta charset='UTF-8'>");
  client.println("<meta http-equiv='X-UA-Compatible' content='IE=edge'>");
  client.println("<meta name='viewport' content='width=device-width, initial-scale=1.0'>");
  client.println("<link href='https://cdn.jsdelivr.net/npm/bootstrap@5.0.1/dist/css/bootstrap.min.css' rel='stylesheet' integrity='sha384-+0n0xVW2eSR5OomGNYDnhzAbDsOXxcvSN1TPprVMTNDbiYZCxYbOOl7+AMvyTG2x' crossorigin='anonymous'>");
  client.println("<link rel='icon' href='https://lh3.googleusercontent.com/proxy/FX0DL0_4VyCoE4PQVeID8UrRpl1jjUPANYhTOH-8qRCHbGclZ6ZqiC7a2uzFydsMJt7lZhfQVRLepgNsQ2AOMdgVFAeh-UKz' type='image/x-icon'>");
  client.println("<link rel='stylesheet' href='https://cdn.jsdelivr.net/npm/bootstrap-icons@1.5.0/font/bootstrap-icons.css'>");
  client.println("<title>Pixel Demo</title>");
  client.println("</head>");

  client.println("<body>");

  client.println("<div class='container'>");
  client.println("<form method='GET'>");
  client.println("<ul class='list-group mt-2'>");


  for (int i = 0; i < numOfPatterns; i++) {
    client.println("<li class='list-group-item'>");
    if (px[i]) {
      client.println("<input id='p" + String(i + 1) + "' name='p" + String(i + 1) + "' class='form-check-input me-1' type='checkbox' value='1' checked>");
    }
    else {
      client.println("<input id='p" + String(i + 1) + "' name='p" + String(i + 1) + "' class='form-check-input me-1' type='checkbox' value='1'>");
    }
    client.println("<span>" + patternNames[i] + "</span>");

    if (quickPlay == i + 1) {
      client.println("<a class='btn btn-primary btn-sm mx-3' href='?quickPlay=" + String(i + 1) + "' role='button'>");
      client.println("<span>Play</span>");
      client.println("<span class='badge rounded-pill bg-light text-dark mx-1'>▶️ </span>");
      client.println("</a>");
    }
    else {
      client.println("<a class='btn btn-primary btn-sm mx-3' href='?quickPlay=" + String(i + 1) + "' role='button'>Play</a>");
    }
    client.println("</li>");
  }

  client.println("</ul>");
  client.println("<button id='submitBtn' type='submit' class='btn btn-success mt-3'>Upload</button>");
  client.println("</form>");
  client.println("<small class='row'><small class='col text-center'><b type='button' data-bs-toggle='modal' data-bs-target='#aboutMe'>About Me</b> </small></small>");
  client.println("</div>");


  client.println("<div class='modal fade' id='aboutMe' tabindex='-1' aria-labelledby='exampleModalLabel' aria-hidden='true'>");
  client.println("<div class='modal-dialog'>");
  client.println("<div class='modal-content'>");
  client.println("<div class='modal-header'><h5 class='modal-title' id='exampleModalLabel'>About Me</h5><button type='button' class='btn-close' data-bs-dismiss='modal' aria-label='Close'></button></div>");
  client.println("<div class='modal-body'>");
  client.println("<span><b>Developed by Dushan Pramod</b></span><br><span>dushanpramod@gmail.com</span><br>");
  client.println("<div class='mt-2'>");
  client.println("<span><a class='btn btn-outline-info' href='https://github.com/DushanPramod/Controlling_PixelLED_via_wifi' role='button'>Source code <i class='bi bi-github'></i></a></span>");
  client.println("<span><a class='btn btn-outline-primary' href='https://www.facebook.com/dushan.pramod' role='button'><i class='bi bi-facebook'></i></a></span>");
  client.println("<span><a class='btn btn-outline-primary' href='https://twitter.com/dushanpramod' role='button'><i class='bi bi-twitter'></i></a></span>");
  client.println("</div>");
  client.println("</div>");
  client.println("<div class='modal-footer'><button type='button' class='btn btn-success' data-bs-dismiss='modal'>Close</button></div>");
  client.println("</div>");
  client.println("</div>");
  client.println("</div>");



  client.println("</body>");

  client.println("<script src='https://cdn.jsdelivr.net/npm/bootstrap@5.0.1/dist/js/bootstrap.bundle.min.js' integrity='sha384-gtEjrD/SeCtmISkJkNUaaKMoLD0//ElJ19smozuHV6z3Iehds+3Ulb9Bn9Plx0x4' crossorigin='anonymous'></script>");
  client.println("<script src='https://cdn.jsdelivr.net/npm/@popperjs/core@2.9.2/dist/umd/popper.min.js' integrity='sha384-IQsoLXl5PILFhosVNubq5LC7Qb9DXgDA9i+tQ8Zj3iwWAwPtgFTxbJ8NT4GN1R8p' crossorigin='anonymous'></script>");
  client.println("<script src='https://cdn.jsdelivr.net/npm/bootstrap@5.0.1/dist/js/bootstrap.min.js' integrity='sha384-Atwg2Pkwv9vp0ygtn1JAojH0nYbwNJLPhwyoVbhoPwBhjQPR5VtM2+xf0Uwh9KtT' crossorigin='anonymous'></script>");

  //  client.println("");
  //  client.println("");

  client.println("</html>");


  delay(1);

  Serial.println("Client disconnected");
  Serial.println("");
}
