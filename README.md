# Controlling_Pixel LED_via_Wifi

![pixel LEDs](/img/pixelLEDs.jpg) ![pixel LEDs](/img/NodeMCU_board.jpg)

## How to make

- Open the code in Arduino IDE
- Put your router SSID and Password to following lines. (You can see those lines on top of the Controlling_PixelLED_via_wifi.ino file.)
  ![ssid and password](/img/ssid_password.JPG)
- and put these details as well.
  - LED_PIN :- The NodeMCU pin you use to connect pixel LED data wire.
  - NUM_LEDS :- Number of LEDs you use.
  - LED_TYPE :- LEDs Type (ex :- WS2811, WS2812B ... )
  - COLOR_ORDER :- No need to change necessarily.
    ![main config](/img/mainConfig.JPG)
- chnage the gateway ip address. put your router ip address here.
  ```
  IPAddress gateway(192, 168, 8, 1);
  ```
- change the local_IP address if you want.
  ```
  IPAddress local_IP(192, 168, 8, 107);
  ```
- Then upload the code to the NodeMCU board and after uploding done connect to the LEDs.
- Connect your device (Smart Phone or PC) which you use to control the NodeMCU to the same router.
- Then open a web browser, enter the IP which you above configured as local_ip to URL and press enter.
- Then you will be able to control your LEDs.
  ![controling window](/img/htmlPage.png)

  - Pressing the play button you can see how that particular pattern run.
  - Put the tick mark to patterns you like. Then click the upload button. The patterns you ticked. runs one after one repeatedly.

## Additional

- You can change pattern's name by changing the patternNames array as follow.

```
String patternNames[] = {
  "Running",
  "Rainbow",
  ...
};
```

- If you don't have a router, you can use mobile hotspot.
- You can change the running time of a particular pattern.

  - There are separate blocks for each pattern in 'void loop' by separating like follow.

  ```
  //pattern 1  ----------------------------------------------------
    if ((patternCount == 1 && px[0] == 1 && quickPlay == 0 && (millis() - timecount < 60000)) || quickPlay == 1) {
       ..........
       ..........
       ..........
    }

    delay(1);
    if ((patternCount == 1 && quickPlay == 0 && (millis() - timecount > 60000)) || (patternCount == 1 && px[0] == 0)) {
      timecount = millis();
      patternCount = 2;
      Serial.println("Pattern 2");
    }

  ```

  - you can see first and second if condition have '(millis() - timecount < 60000)' and '(millis() - timecount > 60000))'
  - change that value as you want (in here 60000). same value for both
  - That value indicates the running time in milliseconds
  - ex :- If you want to increase the running time, put a larger value.

- You can add patterns as you wish.

  - Increase numOfPatterns

  ```
  const int numOfPatterns = 9; // number of patterns
  ```

  - Add a pattern name to the patternNames array

  ```
  String patternNames[] = {
  "Pattern 1",
  "Pattern 2"
  ....

  };
  ```

  - go to the void loop. I added some template pattern blocks by commenting.
  - Remove the comments symbols of the first one of those.

  ```
  if ((patternCount == 10 && px[9] == 1 && quickPlay == 0 && (millis() - timecount < 30000)) || quickPlay == 10) {


    }
    if ((patternCount == 10 && quickPlay == 0 && (millis() - timecount > 30000))  || (patternCount == 10 && px[9] == 0)) {
      timecount = millis();
      patternCount = 11;
      Serial.println("Pattern 11");
    }
  ```

  - Write your code inside the first if condition. but there are restrictions for that.
    - You shouldn't use 'delay' function or any loop that the execution sequence hold down within that block.
    - It is ok for a maximum delay of 1 second or 2 seconds.
    - The reason for that, if it has a delay. while running that pattern, you would not able to operate the NodeMCU by using webApp.

![About Me](/img/about_me.jpg)
