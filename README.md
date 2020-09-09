# Realsense Depth Data Recording + Analysis Application

[![Open in Colab](https://colab.research.google.com/assets/colab-badge.svg)](https://colab.research.google.com/github/MaahirG/DepthDataApplication/blob/master/DepthCNN.ipynb)

## Collection Pipeline Architecture
To make the application easy to use, and start right when the TX2/Xavier is powered on, I used a service file(.service) that runs a script (.sh) that initiates the recording application (./recording.out) upon start up.

### Quick Start
**--> Open the depth analysis CNN based model in COLAB!**

**--> clone repo**

**--> cmake . (generates the makefile w.r.t your computer)**

**--> make (build)**

**--> ./recording.out (to run script)** 

### Tips
NOTE: Do ' nohup ./recording.out & ' if you ssh into the computer and need the terminal to not close once you close your ssh connection. **'Nohup'** is 'no hangup' - meaning the process will run in the background even if terminal/ssh times out - bypass hangup time and keeps the script going even if no return handshake received from ssh client. To see a background process (nohup): do "ps -a" and observe ./recording.out. To kill the process do "kill process_id".

Remember: Always run the script from the right folder inside the external HDD or MicroSD or SD.
--> Also with new hard drive give sudo permissions: sudo chown -R username:username /media/username/nameofdrive

If having issues: delete all CMake/Makefile related files: rm-rf, rm etc  EXCEPT for CMakeLists.txt and then rebuild the makefile: cmake .  then build: make

### Handy Linux Commands
```sh
sudo systemctl enable depth-recording.service	//enables the service (will run @startup)
sudo systemctl status depth-recording.service	//checks the status of the service
sudo systemctl stop depth-recording.service	  //stops the service if it is running
sudo systemctl start depth-recording.service	//starts the service without having to reboot
sudo systemctl daemon-reload			            // always do after any updates to code
```
### Requirements
* [https://developer.nvidia.com/embedded/jetson-xavier-nx-devkit] - NVIDIA Xavier NX Jetpack Download - Includes OpenCV, Python & other dependencies.
*	Realsense SDK - [https://github.com/IntelRealSense/realsense-ros] - METHOD 2.
*	OpenCV (already on Jetpack OS) 
*	CMake (Should be already installed, if not google and use apt-get)
*	Service file (for auto start on upon power up)
*	Linux .sh script
*	Realsense camera plugged in (USB-C to USB-A)
•	SD card with code repository stored on it (256-500gb preferably)


### Setup Process

1. Either change directory (cd) into your external storage device directory (cd /media/ExternalStorageNameOrPATH) and then clone the repo, or just clone the repo in the home directory (type: ‘cd’) ONLY IF you are using a large MicroSD as the OS storage.
2. Once in the correct directory, type: “git clone https://github.com/TopHatRobotics/realsense-interfacing.git && cd realsense-interfacing ” 
3. Type: “ cmake . “ OR “ cmake “   (I can’t remember) → generate the makefile
4. Type: “ make “ OR “ make . “     (I can’t remember) → build the makefile
5. Create the service file to start application at system startup:
```sh
    cd /etc/systemd/system/		               //change directory
    sudo touch depth-recording.service	//create service file

    Paste this in:
    [Unit]
    Description=Depth Camera Pipeline Starting

    [Service]
    Type=simple
    ExecStartPre=/bin/sleep 30
    ExecStart=/bin/bash -c /usr/local/bin/depth-recording.sh

    [Install]
    WantedBy=multi-user.target
```
6. Create a script file to run the application via the service file: 
NOTE: YOU NEED TO CHANGE THE PATH IN ‘Paste this in’ BELOW TO WHEREVER YOU CLONED REALSENSE INTERFACING!
```sh
    cd /usr/local/bin/			         	//change directory
    sudo touch depth-recording.sh			//create script file

    Paste this in:
    #!/bin/bash
    cd /media/nvidia/Seagate\ Backup\ Plus\ Drive/realsense-interfacing/
    ./recording.out

    while[1]
    do
      sleep(300)
    done

    Now type in terminal:
    sudo chmod +x depth-recording.sh		//makes file executable
```
7. Close and reopen terminal OR source ~/.bashrc

8. At this point everything is set up. To test the recording pipeline change directory into realsense-interfacing and type: “ ./recording.out ”. You should see a plethora of timestamps → the recording has started and will take a picture every few seconds. (processing time impacts the specified 3/4/5 second timer in the code). The images should be saved to the data folder in realsense-interfacing  check that and if you see images great!

9. Ctrl + C to stop the recording → Use the desktop GUI if you want and delete the images inside ‘data’ to leave an empty folder. 

10. Now configure the .service file: 

```sh
    sudo systemctl daemon-reload
    sudo systemctl enable depth-recording.service
    sudo systemctl daemon-reload
    sudo reboot (SAVE YOUR WORK IF APPLICABLE - rebooting computer)

    Note: The service takes 30 seconds to actually start (on purpose) because the system takes about 15 seconds to startup all prerequisite relevant processes, so it will not start recording until 30 seconds in. Should be good about 10 seconds after you get into the computer.

    sudo systemctl status depth-recording.service

```

11. If you see ‘active’ in green colouring, it is working. Check again in 1 minute to ensure it is really working.



License
----

MIT

"# DepthDataApplication" 
