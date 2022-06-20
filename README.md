# ESP32 MKZ4
Customized MKZ4 Firmware for ESP32 (**NOT** for ESP8266).  
Based on `cerevo/MKZ4`.  
https://github.com/cerevo/MKZ4

*My machine works with these codes for now, but I don't expect them to work universally for all DIY products, so I recommend customizing them.

![from_right_4_3](https://user-images.githubusercontent.com/1344010/174665008-7c383e6b-36a1-4dbc-9a13-ddf7250322fe.png)


## Schematic
WIP


## ESP32-Arduino / Installing using Arduino IDE

https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html


## Dependencies

- Servo Library for ESP32
https://www.arduino.cc/reference/en/libraries/esp32servo/
https://github.com/madhephaestus/ESP32Servo


## Ex) VS Code / c_cpp_properties.json

Add ESP32 libraries path to "includePath" like below:

```.vscode/c_cpp_properties.json
"includePath": [
    "C:\\Users\\at4\\AppData\\Local\\Arduino15\\packages\\esp32\\hardware\\esp32\\2.0.2\\libraries",
    "${workspaceFolder}/**",
    "..\\libraries"
],
```


## Licence

[BSD 3-Clause License](https://opensource.org/licenses/BSD-3-Clause)
