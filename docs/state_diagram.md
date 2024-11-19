
``` mermaid
stateDiagram-v2
    [*] --> Init : Power On

    Init : Initializing System
    Init --> WFO : System Initialization Complete

    WFO : Waiting for Object
    WFO --> Measure : Object Detected (Triggered by CONTROL_BUTTON)

    Measure : Measure Object Height
    Measure --> Avg : Take 10 Height Measurements

    Avg : Calculate Average Height
    Avg --> LCD : Display Average Height on LCD (1st Line)

    LCDPass : Display "PASS" on LCD for 3s
    LCDReject : Display "REJECTED" on LCD (2nd Line)

    LCD : Determine Object Status
    LCD --> LCDPass : Height < Limit
    LCD --> Stop : Height >= Limit

    Stop : Stopping Motor
    Stop --> LCDReject : Display "REJECTED" on LCD 2nd Line, Motor Stopped

    Refresh : Clear LCD and Reset
    LCDPass --> Refresh : Object Passed Sensor
    LCDReject --> Refresh : Object Removed
    Refresh --> WFO : LCD Cleared, Ready for Next Object

    %% Additional Branches for User Input and System Control
    WFO --> Stopped : STOP_BUTTON Pressed
    Measure --> Stopped : STOP_BUTTON Pressed
    LCDPass --> Stopped : STOP_BUTTON Pressed
    Stopped --> WFO : CONTROL_BUTTON Pressed to Restart System

    %% System Update and PID Control
    WFO --> Update : Periodic Update (Triggered by SysTick Every 50ms)
    Update : Update System State
    Update --> upt_pid : Perform PID Calculation for Motor Speed
    upt_pid --> MotorAdjust : Adjust Motor Power via PWM
    MotorAdjust --> WFO : Return to Waiting State
```
