# ESP-32 Prometheus to LED Matrix
This project demonstrates how a ESP-32 and LED matrix can display Prometheus metrics!

## Hardware
There are two necessary pieces of hardware for this device: an ESP-32 and one or two 32x8 WS2811 (Neopixel) LED arrays. You will need to power the LED array separately from the ESP-32. 

Wiring should be like the following
- Power + forked, with one going to the ESP-32 and one going to the LED array. 
- Power - forked, with one going to the ESP-32 and one going to the LED array. You will also want to connect a GND pin on the ESP-32 to the GND on the LED array
- Data wire from the LED array to ESP-32 on pin 4. If using two LED arrays, the second should go to pin 5.

If your ESP-32 is having trouble connecting to Wifi, you may need to connect an antenna.

## Setup
This project uses the [PlatformIO](https://platformio.org/) IDE to manage dependencies and flash the hardware. For more information on how to get PlatformIO set up, please see their [getting started page](https://docs.platformio.org/en/latest/integration/ide/pioide.html). 

### Config options
First, you will want to edit `platformio.ini` to include your [board type](https://docs.platformio.org/en/latest/projectconf/sections/env/options/platform/board.html) and your [upload port](https://docs.platformio.org/en/latest/projectconf/sections/env/options/upload/upload_port.html). For other options, see [PlatformIO's documentation](https://docs.platformio.org/en/latest/projectconf/sections/env/index.html) about their configuration.

You will need to create a file named `secrets.h` inside `/include`. The file should look like the following:

```
#define WIFI_SSID     ""
#define WIFI_PASSWORD ""

#define GC_URL ""
#define GC_PATH ""
#define GC_PORT 443
#define GC_USER ""
#define GC_PASS ""
#define NTP_SERVER "pool.ntp.org"
```

The WIFI variables will be to connect to your Wifi. 

The GC_ variables will correspond to your Grafana Cloud instance that has a prometheus endpoint. See the [sending metrics](https://github.com/grafana/diy-iot/blob/main/README.md#sending-metrics) documentation on how to get these values. 

Once these values are all set up, you should be able to flash your firmware to your ESP-32 and get your metric data!

## How to use
Once everything is flashed, go to [http://esp32matrix.local](http://esp32matrix.local). You should see a webpage pull up. If your ESP-32 successfully flashed but you don't see a webpage, check your settings. If you have an older network setup or know your network doesn't support multicast DNS, you will need to go to your ESP-32's IP address directly instead. The ESP-32 outputs its IP address to serial when it is booted up. 

The values set on this page are persisted even if the ESP-32 loses power.

The various fields should be self explanitory, the one field you should be sure to fill in being "Query" which will be what query gets sent to prometheus to get a metric back. 

If you enter a URL on the webpage, it will override what is defined in the config. If removed, it will fall back to the config URL.

For the color values, the numbers should be between 0-255 and will affect the brightness of the color specified. We recommend setting a color to "20" as it is plenty bright.