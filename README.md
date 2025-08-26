ESP32 WiFi Robot Controller
This repository contains an Arduino sketch for an ESP32 microcontroller that enables wireless control of a two-wheeled robot.
The ESP32 acts as a WiFi Access Point (AP), allowing any device (like a smartphone or computer) to connect to it directly.
Once connected, you can access a simple web page hosted by the ESP32 to send commands to the robot.

FEATURES:
* WiFi Access Point: The ESP32 creates its own network and exist as AccessPoint.
* Web Server: A lightweight web server runs on the ESP32, serving an HTML page with control buttons.
* Robot Control: The code provides functions for basic robot movements:
Forward
Backward
Left Turn (pivots on the left wheel)
Right Turn (pivots on the right wheel)
Stop
* Duration Control: Commands for movement (forward, backward, left, right) prompt the user for a duration in milliseconds, allowing for precise control.
