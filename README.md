# The complete smart home solution for single family houses and apartment buildings
This project covers a complete (Home Assistant) smart home solution for single family houses and apartment buildings facing the primarily requirement of replacing the existing high voltage switching system by an I2C-binary sensor system using the 70 existing controlling units (like light switches, momentary switches, ..) in order to control:

#
![esp32-02_02](https://github.com/user-attachments/assets/468ebcf7-a131-4bcf-a22b-907f2c339e6a)
     _16-channel i2c-relay-board, i2c mosfet dimmer, MCP23017, esp32UE, XL4015, APV-35_
#

_- 41 power circuits_ (mostly light, also power outlets, garage door opener, ventilator, heating system) with “4/8/16 Channel I2C Electromagnetic Relay Modules” from Krida Electronics. 8 light circuits can be dimmed. Therefore I use I2C mosfet Trailing Edge AC Dimmer from Krida. In case of dimming the relays work as power supplies for the dimmer (as you see in yaml files). There is no custom component or other integration for this dimmer in esphome or hassio. You find the software under components/i2c_dimmer. Dimming by the amount of time pressing the associated momentary switch or by hassio app. As the dimming process control takes place on the esp where dimmer is connected to the binary_sensors on other esp use a central dimming script (dimm_script0x) which is activated by mqtt payloads “pressed” and “released” to access the monochromatic_light on dimmer espx. Generally communication for light switching among the esps works with mqtt (since mqtt has native support for toggle, turn_on, press ..), you can also use esphome-API. In my case one light circuit contains from 2 to 8 light bulbs, sometimes spread over big area, then these 8 dimming AC lines produce electrical noise which can influence the binary_sensors.

_- 21 covers/blinders_ 13 from Selve, 8 from Warema. For Selve you need the Selve RF-USB gateway (866 MHz) which will be plugged into your hassio server usb port (there are also other variants). Integration you find on github or HACS. For warema you need the 433 Mhz RF-Link Gateway and find it on nodo-shop.nl. Apart from Warema you can use more or less every 433MHz device  (except garage-door opener) with that gateway, even sensors like TFA Dostmann climate sensors. Hassio integration available. The tubular motors of covers in operation also cause terrible electrical interference. Use interference filters or 100nF caps as close as possible to the AC connection of motor.

_- 4 media player_ 2 NAD C338, 1 NAD T758, 1 Pioneer LX304. Hassio integrations (BluOS, Google Chromecast or DLNA renderer) available, best results with BluOS devices which also support playlists out of Home Assistant, and more features like media_next_track (e.g. from esphome). Since flac streaming from radioparadise.com (and others) breaks after 4-5 min (probably due to my internet connection as I'm living in the middle of nowhere) I need to re-transcode the stream and offer it again in my lan with stream_main.h which gets started by stream_main.service on pi402. This stream is used on esp32-05 and works perfectly. In case you want to record some audios you can use the ffmpeg command in rec_rp_main.h, this way flac files will be produced with 1 h duration. According to output of "journalctl -b -u stream_main" you get the complete titlelist and can extract your desired audio files e.g. with Audacity, use of "journalctl -b -u stream_main > titlelist.txt" writes this output in the textfile.

_- 1 garage door opener_ esphome button with id: garage_tor on esp32-02 will be pressed for 0.3s which opens/closes the garage door. This button uses GPIO output platform switching i2c-relay05/pin0. As the door is opened the reed relay (image → reed01, reed02) mounted on garage door will be closed and indicates this state in kitchen using the state LED with id: led_02 on esp32-05.


Due to several, unavoidable electrical interference the big challenge is the wiring of binary_sensors (=existing light/momentary switches), relay-boards, sensors (in my case 18 DHT22, 56 DS18B20 (for heating system), PIR ...
binary_sensors are connected to I2C I/O expansion boards MCP23017 which offer 16 in/outputs, 
I2C Relay-boards use PCF8574/75 for I2C communication, both connected to I2c master (=esp32).
MCP23017 can easily be installed in the existing electrical sockets of light-switches (you see on images es01 - es05). The 8channel I2C board, I2C mosfet dimmer, DC-DC converter and MCP23017 fits into the commonly used distribution box with 150x150mm (image).

MCP23017 can also be used for output purpose, as already mentioned the state LED of garage door or heating system, etc.
bin_sens.txt gives overview of binary_sensor connection and location, relays.txt of I2C boards. Wiring cables (GOOBAY 94217 – PIMF) I used 2 twisted wires for 1 connection cable and soldered the ends to the connector, finally crimping and shrink tubing. Power supply (MeanWell APV-35) has 12V output which I laid from room to room and so on. The devices will then be powered by DC-DC converter XL4015 (has filter to eliminate power peaks, e.g. from covers, blinders, venti, heavy duty machines) which are connected to APV-35. This way you have less voltage-drop and can cover larger distances. In my upper floor there are 5 rooms on one I2C-master (esp32-06) with 2 relay-boards and 3 mcp23017, approx. 30 meters. Especially if I2C-wiring uses same electrified hose as 240V wires you should connect the cable shield to GND, at the end it’s opened or connected with 10nF cap. If you did a good wiring job the delayed_on_off function in esphome is not necessary ;-) otherwise the binary sensors show a bouncing and jittering behaviour caused by electrical interferences. In this case the voltage peaks of foreign AC wires (blinder) trigger state changes even if you did not press the assigned bin_sens, this can happen very frequently and cause smoking relay-boards.

Anyway, the delayed_on_off function in esphome should be used, you never know which kind of engines or interferences the AC lines in your house/flat are exposed to.
You will need some knowledge base about wiring, good tools (soldering iron, solder) and a very clean, accurate work style. Don’t use cheap junk from china, you will regret it.

The big goal of binary_sensors is the capability to handle more or less all available entities in your home. In /yaml you find the config-files of all esp devices (esp01 - esp04, esp32-01 - esp32-06, esp05 is dsmr to get smartmeter data, energy consumption). With one binary_sensor you can handle at least 8 operations, good example is bin_sens_85 on esp32-05: 3 operations for blinder handling, 2 operations for light control and dimming, 2 operations for streaming service on media_player.nad_kueche (stream_main.h, stream_rock.h). bin_sens_70 shows the way how to open garage from outside (modified 😁)

#
#

further requirements:

_- getting underfloor heating system from Fa. Mochart smart with individual room control_

Heating system, fed by 30 underfloor heating circuits, should be controlled manually in Home Assistant App (e.g. with automations, sliders, etc), with binary_sensors or automatically by thermostat (like Versatile Thermostat or the simple hassio thermostat card) with Triacs, so you can set a various number of open states between 0 – 1, not only open/close. This makes handling especially with underfloor heating much easier and in case of 30 circuits the switching noise takes little get used to. Generally it makes no difference which kind of heating system you use since they all (more or less) will be handled by valves, except infrared heating, radiant heater ... and of course tiled stoves 😄

After heating action the triacs should be turned off automatically and, of course, turned on when starting – not only because of energy reason but much more of security. Don’t touch triac boards during operation! The main heat switch is triggered by an Electromagnetic Relay (switch.heat_main) instead of the old thermostat, then the automation “drive_enable” will be triggered and sets the defined value of open-state of valves. If the relay turns main_heat off the automation drive_disable is triggered and sets all values to 0 and finally a second relay (switch.triac_eg) breaks AC lines to the triac boards. When using thermostat function everything works automatically.
For individual room control, according to the number of rooms that are handled by heating system same number of temperature sensors needs to be installed, each in one room, they are the predominantly basis for the temp-control. I suggest DHT22 sensors. You can also use AHT20 sensors with i2c interface, but their i2c-address (0x38) cannot be changed.

In order to get a really accurate solution I also suggest to install temp-sensors on each return-water-pipe, covered all around with some kind of thermal insulation (the blue covers around the water-pipe on the images are from security pads of a trampoline), so you can see definitely whats really happening in the pipe. The sensor itself I fixed with cable ties. Every 4th flow-water circuit I also installed a sensor. DS18B20 are ideal sensors for that purpose, with OneWire-I2C board DS2482-800 you get 8 OneWire-channels, so at least 80 sensors possible (with good wiring)
The heating valves are handled by wax engines “Möhlenhoff Alpha-5” which in turn are controlled by the triacs. They are defined as servos which use the slow_pwm output platform of esphome. Servos get value from a template, which can also be a group of several servos (e.g. one room, one floor). Additionally in dashboard you can use sliders to set the open state of each valve or valve group. I use these triac boards (both high quality products available on tindie.com):

- 1 x “ESP32 Floor Heating Valve Controller” (Voltlog) → esp32-01
- 3 x “8CH AC LED Light Dimmer Module Controller Board” (Krida) → esp32-04, esp32-06
	(images → heating control)
#

_- media handling (NVR surveillance system, media streaming, media hosting, file server)_

raspberry pi5 (pi402) is a data and media server with a 512GB SD card and Google coral TPU for 

- hosting audio data (ogg, flac, wav tracks) with Gerbera DLNA media server
- NFS file server
- Frigate NVR surveillance system with IMX219 pan-tilt-hat control cameras on pi zero2

Since Frigate - runs in docker mode - is a ONVIF compliant Real Time Object Detection NVR software I decided to use the rpos ONVIF server (github) together with mediamtx (github) in order to turn the arducam IMX219 on a raspberry zero2 with pan-tilt-hat into an ONVIF cam, so pan-tilt-hat control works also in frigate. Already mentioned in the rpos-repo the waveshare pan-tilt-hat needs a lot of mechanical modifications because only few parts fit, some parts are too long … so the Pimoroni model is recommend. Often surveillance cams are installed on exposed areas with poor wlan connectivity, so another recommendation is to use external antenna on pi zero2. Online you find instructions how to solder an ipex antenna connector on the pi board and break the connections to build-in antenna.
