Overview
=====
This is my Switch port of SuperMarioWar, based on the Vita port by @Rinnegatamante.

Super Mario War is a Super Mario multiplayer game. The goal is to stomp as many other Marios as possible to win the game.

![](screenshots/smw_1.jpg)

Homepage
======
Official Super Mario War homepage:  
http://supermariowar.supersanctuary.net

Detailed instructions, written by @HerbFagus from the RetroPie team:  
https://github.com/RetroPie/RetroPie-Setup/wiki/Super-Mario-War

Thanks
======
Thanks to my supporters on Patreon: Andyways, CountDuckula, Greg Gibson, Jesse Harlin, Özgür Karter, Matthew Machnee, and RadicalR.

Thanks to the many developers of the original game, such as Michael Schaffer, Florian Hufsky, Two52 and many more contributors.

Installation Instructions
=====
Switch:
- Extract the contents of SuperMarioWar_Switch.zip into the `switch` folder on your SD card, so that you have a folder `switch/SuperMarioWar` with `SuperMarioWar.nro` and more folders and files inside.

Switch-exclusive features
=====
- Split Joycon support: To toggle between split and combined Joycons, hold R and press L.

Controls (controls updated with version 1.08)
=====
Left analog stick / Dpad = Move the character  
B = Jump  
Y = Run/Use projectiles  
A = Use objects  

Build Instructions
=====
````
make -f Makefile_Switch
make SuperMarioWar_Switch.zip -j12
````

Changelog
=====

- (WIP) working on Switch port, currently it crashes after title screen