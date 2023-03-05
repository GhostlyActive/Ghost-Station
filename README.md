# Ghost-Station
Ghost Station is a project that allows you to play classic games by swapping out cartridges, just like the old game consoles of the past. The console uses an Arduino board to run games, and it comes with a variety of built-in features like a color display, sound, and joystick controls.
<br>
<br> 
1.) To play the game, we require an SD card module that is connected via SPI on an Arduino, two Arduino boards for making bootloader changes, and a screen to display the game.
2.) Use an ISP (in-system programmer) to burn the bootloader. You can use the avr_boot tool available at [avr_boot](https://github.com/zevero/avr_boot).
3.) The SD card needs to have the compiled code of the game project. To build a game, use the Arduino Editor, rename the .hex file to FIRMWARE.BIN, and copy it to the SD card.
4.) After copying the game to the SD card, restart the Arduino, and you should be able to play the game.
<br>
<br>
<p align="center">
  <img src="/Media/SD_Game_Play.gif" width = "802" alt="Light">


</p>
