This is an arduino module used to control a specific mobile robot
which is currently under development using a PS4 controller.

`ps4-wheels-controller.ino` uses the Arduino mega pin setup documented below.

|    Wheel    | Spin | Break | Reverse | Hall |
|:-----------:|:----:|:-----:|:-------:|:----:|
|  Front Left |   7  |   25  |    29   |  21  |
| Front Right |   6  |   24  |    28   |  20  |
|  Back Left  |   5  |   23  |    27   |  19  |
|  Back Right |   4  |   22  |    26   |  18  |

Note that the front left wheel spinning direction is opposite to others.