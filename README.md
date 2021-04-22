This is an arduino module used to control a specific mobile robot
which is currently under development using a PS4 controller.

`ps4-wheels-controller.ino` is used with Arduino Mega 2560. The pin setup is documented below.

|    Wheel    | Spin | Break | Reverse | Hall |
|:-----------:|:----:|:-----:|:-------:|:----:|
|  Front Left |   7  |   22  |    23   |  21  |
| Front Right |   6  |   24  |    25   |  20  |
|  Back Left  |   5  |   26  |    27   |  19  |
|  Back Right |   4  |   28  |    29   |  18  |

Note that the front-left wheel spinning direction is opposite to others.
