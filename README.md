# Esp32 Serial Multiband Digital Audio Processor

## Features

* Serial interface for control and configuration.
* Adjustment of the number of frequency bands in real time from 1 to 10.
* Graphic equalizer.
* Audio echo.
* Synchronized adjustments for each audio compressor.
* Audio balance adjustment.
* Audio Pre-emphasis and Post-emphasis by software.
* Output audio peak protection system.
* Settings saving system
* Circuit power supply directly from the USB port.
* Simplified scheme with a low number of electronic components.

<br/>
<h2>Schematic (Digital Usb Input)</h2> 
<img src="docs/schema-digital-usb-input.png" />

<br/>
<h2>PCB Layout (Digital Usb Input)</h2> 
<img src="docs/pcb-layout-digital-usb-input.png" />

<br/>
<h2>Prototype (Digital Usb Input)</h2> 
<img src="docs/prototype-digital-usb-input.jpg" />

<br/>
<h2>Schematic (PCM1802 ADC Input)</h2> 
<img src="docs/schema-pcm1802-adc-input.png" />

## Recommended compilation settings

* Microprocessor: ESP32 WROOM Devkit
* Rom: 4MB, Minimal SPIFFS large Apps with OTA
* Ram: Internal ~ 350 KBytes
* Ide: Arduino v1.8.19
* Board Version Support: Esp32 v1.0.1 to v1.0.3

## Serial Interface

<pre>
---------------------------------
DIGITAL AUDIO PROCESSOR
VERSION: 1.0.0.0 2025/06/23
---------------------------------
Please choose an option:
---------------------------------
 1. INPUT LEVEL.........15.00dB
 2. OUTPUT LEVEL........-30.00dB
 3. BALANCE.............0.0%
 4. CLIPPER.............0.00dB
 5. COMPRESSOR..........Enabled
 6. MUTE................Disabled
 7. RESERVED1...........Disabled
 8. RESERVED2...........Enabled
 9. NUM BANDS...........8
10. PRE EMPHASIS........0.00dB
11. POST EMPHASIS.......-3.00dB
12. STEP................-10.0dB
13. ECHO................0.00
14. PROTECTION..........10.00dB
15. GAIN................80.00dB
16. ATTACK TIME......... 5ms
17. RELEASE TIME........30ms
18. FILTERS Q FACTOR....1.000
19. SAVE
20. RESET TO DEFAULT
21. EQ BAND(8)  17000Hz to  18000Hz 0.00dB
22. EQ BAND(7)  16000Hz to  17000Hz 0.00dB
23. EQ BAND(6)  14000Hz to  16000Hz 0.00dB
24. EQ BAND(5)   6000Hz to  14000Hz 0.00dB
25. EQ BAND(4)   2000Hz to   6000Hz 0.00dB
26. EQ BAND(3)    500Hz to   2000Hz 0.00dB
27. EQ BAND(2)    200Hz to    500Hz 2.00dB
28. EQ BAND(1)     20Hz to    200Hz 0.00dB
</pre>

## Tips

* The recommended input level is 15dB for PC volume control at 50%.
* You may have to hold down the BOOT button on the Esp32 and click the reset button to be able to write the firmware with the Arduino IDE, due to the incompatibility of board support 1.0.0.x with USB 3.0 ports!

## Dependencies

* https://github.com/junon10/STM32F411_USB_AUDIO_DAC
* https://github.com/junon10/yummyDSP

## Hardware

* 1 ESP32 WROOM DevKIT
* 1 STM32F411 BlackPill
* 1 PCM5102A DAC board

## Author

- **Junon M.**  
  Contact: [junon10@tutamail.com](mailto:junon10@tutamail.com)

## Contributing

Contributions are welcome! Please fork the repository and send a pull request.

## Repository

- [https://github.com/junon10/esp32-multiband-digital-audio-processor-minimal](https://github.com/junon10/esp32-multiband-digital-audio-processor-minimal)

## Changelog

- **v1.0.0.0 (2025/06/23)**: Initial commit.

## License

- GPLv3

## Notes

* This dynamic audio compression library is not perfect due to the insufficient speed of esp32, which makes it impossible to use real-time logarithm functions for more than two simultaneous frequency bands. In practice with this lib you get a good effect, but with a not very wide input dynamic range.
