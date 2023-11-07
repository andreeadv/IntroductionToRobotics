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

* __Code__: [_See my code here_](https://github.com/andreeadv/IntroductionToRobotics/blob/main/hmk3_elevator_simulator/hmk3_elevator_simulator.ino!)

* __Associated video of how it works__: [_YouTube video here_](https://youtube.com/shorts/-f7SUdCCzrI?si=qBz2WyXPWmIR6kdS)

* __Electronic scheme__:![elevator simulator hmk#2](https://github.com/andreeadv/IntroductionToRobotics/assets/91892810/c5d236fe-11b8-473c-892d-f825cc0abc40)


*7segdr: (https://github.com/andreeadv/IntroductionToRobotics/assets/91892810/7532369a-50f6-40d1-9de8-a260ad0b418d)


