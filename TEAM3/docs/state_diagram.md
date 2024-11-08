
``` mermaid
stateDiagram-v2
    [*] --> Init : Power On

    WFO : Waiting for Object
    Init --> WFO : System Initialization Complete

    Measure : Measure Object
    WFO --> Measure : Object Detected

    Avg : Get Average Height
    Measure --> Avg : Measured Height 10 times

    LCD : Display Height in LCD 1st Line 
    Avg --> LCD : Computed Average Height

    LCDPass : Display "PASS" for 3s on LCD 2nd Line
    LCD --> LCDPass : Height < Limit

    Stop : Stopping Motor
    LCDReject : Display "REJECTED" on LCD 2nd Line
    LCD --> Stop : Height >= Limit
    Stop --> LCDReject : Motor Stopped

    Refresh : Clear 1st Line LCD
    LCDPass --> Refresh : Object Advanced past sensor
    LCDReject --> Refresh : Object Removed
    Refresh --> WFO : 1st Line cleared
```