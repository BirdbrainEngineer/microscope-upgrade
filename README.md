# Cheap microscope upgrade project

This repository contains the Arduino code as well as shell commands for my cheap, educational microscope upgrade project, for which a [video on YouTube also exists](https://www.youtube.com/watch?v=QV-GfkwQU_8). 

The code in here is essentially just a botched way to control three stepper motors from a control panel (of which, two controls are potentiometers). For this project I did not want to go down the rabbithole of trying to add proper computer-numeric control, but maybe once I get myself a proper research microscope I would look into redoing this project and includign computer-numeric control as well. 

### How to stream 1440p footage from a Raspberry Pi

One nice part of the upgrades was the addition of a 1440p live camera feed from the microscope to my PC, where I can better view the sample, measure distances as well as record what the microscope sees for videos. 

To do that, I used a Raspberry Pi Cam 3, driven by a Raspberry Pi 3. 

First step is to install Raspbian (Raspberry Pi OS) on the Raspberry Pi.

Then make a `stream.sh` file in your home directory on the Raspberry Pi with the following contents.
```
libcamera-vid -t 0 --inline -o udp://239.42.135.169:5555 --mode 2304:1296:8:P --width 2560 --height 1440 --vflip 1 --framerate 10 --codec mjpeg
```

After that, make a systemd unit on the Raspberry Pi.
```
[Unit]
Description=Camera stream
After=multi-user.target

[Service]
Type=idle
ExecStart=/usr/bin/bash /home/user/stream.sh

[Install]
WantedBy=multi-user.target
```

Finally you will need to run the following commands on the Raspberry Pi.

`sudo systemctl daemon-reload`

`sudo systemctl enable stream`

And optionally, if you do not wish to restart the Raspberry Pi for the stream to start, run the following on the Raspberry Pi.

`sudo systemctl start stream`

And that's it! Now, if you run the following command on a device that is connected to the same local network as the Raspberry Pi, you should be able to see the stream.

```
ffplay udp://239.42.135.169:5555 -fflags nobuffer -flags low_delay -framedrop
```