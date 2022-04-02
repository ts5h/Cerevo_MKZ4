# WIP: ESP32 MKZ4
Customized MKZ4 Firmware for ESP32 (**NOT** ESP8266).  
Forked and customized from `cereve/MKZ4` by ts5h.  
https://github.com/cerevo/MKZ4


## VS Code / c_cpp_properties.json

Add ESP32 libraries path to "includePath" like below:

```.vscode/c_cpp_properties.json
"includePath": [
    "C:\\Users\\at4\\AppData\\Local\\Arduino15\\packages\\esp32\\hardware\\esp32\\2.0.2\\libraries",
    "${workspaceFolder}/**"
],
```


## Licence

[BSD 3-Clause License](https://opensource.org/licenses/BSD-3-Clause)