# IntroductionToRobotics (2023 - 2024)

* This repository hosts the laboratory assignments for the __Introduction to Robotics__ course, taken during the third year at the __Faculty of Mathematics and Computer Science__, University of Bucharest.
* Each assignment in this repository comes with clear instructions for each homework assignment, implementation details, setup pictures, and associated videos.

# Homeworks
<details>
  
<summary><h2>Homework #2: Controlling RGB LED with 3 potentiometers</h2> </summary>

* __Technical Details__: Use a separate potentiometer for controlling each color of the RGB LED: Red, Green, and Blue.  This control must leverage digital electronics.
  * Components:
      * RGB LED (At least 1)
      * Potentiometers (At least 3)
      * Resistors and wires as needed

* __Code__: [_See my code here_](https://github.com/andreeadv/IntroductionToRobotics/blob/main/hmk2_controlling_RGB_with_potentiometers/hmk2_controlling_RGB_with_potentiometers.ino)

* __Associated video of how it works__: [_YouTube video here_](https://youtube.com/shorts/eV0GxsFZj0g?feature=share)

* __Electronic scheme__:

![controlling_RGB_with_potentiometers_hmk#1](https://github.com/andreeadv/IntroductionToRobotics/assets/91892810/79a0a355-0c8b-4573-a4f4-9993281ebd44)

</details>

<details>
  
<summary> <h2>Homework #3: Elevator simulator</h2></summary>

__This assignment involves simulating a 3-floor elevator control system using LEDs, buttons, and a buzzer with Arduino.__

* __Technical Details__: Each of the 3 LEDs should represent one of the 3 floors. The LED corresponding to the current floor should light up. Additionally,
another LED should represent the elevator’s operational state. It should blink when the elevator is moving and remain static when stationary. Implement 3 buttons that represent the call buttons from the 3 floors. When pressed, the elevator should simulate movement towards the floor after a short interval (2-3 seconds).
The buzzer should sound briefly during the following scenarios:
– Elevator arriving at the desired floor (something resembling a ”cling”).  
– Elevator doors closing and movement.
If the elevator is already at the desired floor, pressing the button for that floor should have no effect. Otherwise, after a button press, the elevator should ”wait for the doors to close” and then ”move” to the corresponding floor. If the elevator is in movement, it should either do nothing or it should stack its decision (get to the first
programmed floor, open the doors, wait, close them and then go to the next desired floor).
  * Components:
      * LEDs (At least 4: 3 for the floors and 1 for the elevator’s operational state)
      * Buttons (At least 3 for floor calls)
      * Buzzer (1)
      * Resistors and wires as needed

* __Code__: [_See my code here_](https://github.com/andreeadv/IntroductionToRobotics/blob/main/hmk3_elevator_simulator/hmk3_elevator_simulator.ino)

* __Associated video of how it works__: [_YouTube video here_](https://youtube.com/shorts/-f7SUdCCzrI?si=qBz2WyXPWmIR6kdS)

* __Electronic scheme__:![elevator simulator hmk#2](https://github.com/andreeadv/IntroductionToRobotics/assets/91892810/c5d236fe-11b8-473c-892d-f825cc0abc40)

</details>

<details> 
   <summary><h2>Homework #4: 7 segment display drawing</h2></summary>
  
* __General description__: The joystick should be used to control the position of the segment and ”draw” on the display.  The movement between segments should be natural, meaning they should jump from the current positiononly to neighbors, but without passing through ”walls”.
  * Components:
      * 1  7-segment  display
      * 1  joystick
      * resistors  and  wires  (per logic)

* __Technical Details__: The  initial  position  should  be  on  the  DP.  The  current position always blinks (irrespective of the fact that the segment is on or off).  Use the joystick to move from one position to neighbors. Short pressing the button toggles the segment state  from  ON  to  OFF  or  from  OFF  to  ON.  Long  pressing  the  button resets the entire display by turning all the segments OFF and moving the current position to the decimal point.
    
* __Code__: [_See my code here_](https://github.com/andreeadv/IntroductionToRobotics/blob/main/hmk4_7_segment_display_drawing/hmk4_7_segment_display_drawing.ino)
* __video of how it works__: [_YouTube video here_](https://youtube.com/shorts/J3yzabjXu8U)
* __Electronic scheme__:![hmk4_7_segment_display_drawing](https://github.com/andreeadv/IntroductionToRobotics/assets/91892810/7532369a-50f6-40d1-9de8-a260ad0b418d)



</details>


<details> 
   <summary><h2>Homework #5: Stopwatch timer </h2></summary>
  
* __General description__: The display will indicate "000.0". Initiating the timer is accomplished by pressing the Start button.
While the timer is active, pressing the lap button will store the current timer value in memory (non-persistent). Up to 4 laps can be saved, with the fifth press overriding the first. Resetting the timer has no effect, and pausing the timer stops its progression.
In Pause Mode, the lap flag button becomes inactive. Pressing the reset button in this mode resets the timer to "000.0".
After a reset, the flag buttons can be used to cycle through the saved lap times. Each press of the flag button navigates to the next saved lap. Continuous pressing cycles through the laps continuously. Resetting in this state clears all flags and resets the timer to "000.0".
  * Components:
      * 1 7-segment display
      * 3 buttons
      * resistors and wires (perlogic)
    
* __Code__: [_See my code here_](https://github.com/andreeadv/IntroductionToRobotics/blob/main/hmk5_stopwatch_timer/hmk5_stopwatch_timer.ino)
* __video of how it works__: [_YouTube video here_](https://youtube.com/shorts/WxoeM28Ryb4?feature=share)
* __Electronic scheme__: ![hmk#5_stopwatch_timer](https://github.com/andreeadv/IntroductionToRobotics/assets/91892810/f8ccf736-a16c-4dbf-b1cb-553fd2a6e56a)


</details>


<details> 
   <summary><h2>Homework #6: Pseudo-Smart Environment Monitor and Logger </h2></summary>
  
* __General description__: This Arduino-based project monitors environmental conditions using an Ultrasonic Sensor and an LDR (Light-Dependent Resistor). The system includes an RGB LED for visual alerts and offers various settings accessible through a user-friendly menu structure.
  
  * Components:
      * Arduino Uno Board
      * Ultrasonic Sensor(HC-SR04)
      * Light-Depended Resistor
      * RGB LED
      * Resistors as needed
   
*__Menu structure__: Menu Structure:

->Sensor Settings
1.1 Sensors Sampling Interval: Set sampling rate (1-10 seconds).<br>
1.2 Ultrasonic Alert Threshold: Define the threshold for ultrasonic sensor alerts.<br>
1.3 LDR Alert Threshold: Set the threshold for LDR sensor alerts.<br>
1.4 Back: Return to the main menu.<br>
->Reset Logger Data<br>
2.1 Yes<br>
2.2 No<br>
->System Status<br>
3.1 Current Sensor Readings: Continuously display sensor readings at the set sampling rate.<br>
3.2 Current Sensor Settings: Display sampling rate and threshold values for all sensors.<br>
3.3 Display Logged Data: Show the last 10 sensor readings.<br>
3.4 Back: Return to the main menu.<br>
->RGB LED Control<br>
4.1 Manual Color Control: Set RGB colors manually.<br>
4.2 LED: Toggle Automatic ON/OFF.<br>
4.3 Back: Return to the main menu.<br>
    
* __Code__: [_See my code here_](https://github.com/andreeadv/IntroductionToRobotics/blob/main/hmk6_Pseudo_Smart_Environment_Monitor_and_Logger/hmk6_Pseudo_Smart_Environment_Monitor_and_Logger.ino)
* __video of how it works__: [_YouTube video here_](https://youtu.be/QiVJDpO03bA)
* __Electronic scheme__: ![menu_sensors](https://github.com/andreeadv/IntroductionToRobotics/assets/91892810/70825269-9a07-419d-b5a8-f6c65058105f)


</details>



