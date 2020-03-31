# barracuda-ppi
## Features
Control the network ports and LED's on the chassis of Barracuda Web Filter with parallel port interface chassis management, with PFSense installed or other software.
## Installation
The c file has to be compiled on a Free BSD system that is the same version as the PFSense router is using ( PFSENSE 2.4.5 -> Free-BSD 11.3 ). Then place the binary in /usr/sbin/.

Add the two lines to the /conf/config.xml in the <system> section so the ports are connected to the router at bootup
```
  <system>
    <earlyshellcmd>barracuda-ppi -a 0 -p 0 -y 1</earlyshellcmd>
    <shellcmd>barracuda-ppi -y 0 -g 1</shellcmd>
    ...
  </system>
```
To get the LED's to be the corresponding color as the gateway status monitor add the gatewaystatuslights to the /etc/phpshellsessions/ folder and add this line via crontab -e to update the lights every minute.
```* * * * * pfSsh.php playback gatewaystatuslight```
Note: You might have to change WAN_DHCP in gatewaystatuslight to match your WAN port name.
