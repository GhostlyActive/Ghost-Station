# Ghost-Station
Ghost Station is a project that allows you to play classic games by swapping out cartridges, just like the old game consoles of the past. The console uses an Arduino board to run games, and it comes with a variety of built-in features like a color display, sound, and joystick controls.
<br>
<br> 
1.) An SD card module that is wired per SPI on an Arduino is necessary for this project. Additionally, two Arduino boards are required for bootloader changes, and a screen is needed to display the game.<br>
2.) To burn the bootloader, use an ISP (in-system programmer). The avr_boot tool available at [avr_boot](https://github.com/zevero/avr_boot).<br>
3.) The SD card must contain the compiled code of the game project. Build the game using the Arduino Editor and rename the .hex file to FIRMWARE.BIN. Copy the renamed file to the SD card.<br>
4.) Restart the Arduino after copying the game to the SD card. The game should be playable now.
<br>
<br>
<b>Current work: </b>Intro video when switching games (like on the Playstation 2), improve games, more games, improving hardware case . . . 
<br>
<br>
<p align="center">
  <img src="/Media/SD_Game_Play.gif" width = "802" alt="SD_Cards">
  <img src="/Media/Case.jpg" width = "802" alt="Case">
</p>
