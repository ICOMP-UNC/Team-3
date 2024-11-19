## Version 1.0.0
### Release Date
[13/11/2024]

### Summary
Version 1.0.0 marks the initial release of the embedded system based on the STM32F103T8C microcontroller. This system integrates multiple modules to handle sensors, motors, and displays, providing core functionalities for measuring distances, controlling motors, and synchronizing display updates.

### Implemented Features
1. **Main Program:**
   - Serves as the system’s entry point.
   - Configures and initializes the microcontroller and peripherals.
   - Coordinates communication between modules such as the ultrasonic sensor, motor, LCD, and speedometer.
   - Implements periodic update logic using interrupts (MVP) or FreeRTOS tasks (best-case scenario).

2. **LCD Module:**
   - Initializes and controls a 16x2 LCD display via I2C using a PCF8574 pin expander.
   - Provides functions to display strings, clear the screen, and adjust the cursor position.

3. **HC-SR04 Module:**
   - Manages the ultrasonic sensor to measure distances using TRIG and ECHO pins.
   - Implements functions to configure the sensor, send pulses, and calculate distance based on echo flight time.

4. **Motor Control Module:**
   - Controls a motor using PWM signals.
   - Allows setting motor power, enabling/disabling the motor, and adjusting the PWM duty cycle.

5. **PID Controller:**
   - Adjusts motor power based on a measured value (e.g., speed or position) and a reference value (setpoint).
   - Provides functions to initialize the controller, calculate the output, and set the reference value.

6. **Speedometer:**
   - Measures motor speed in RPM using a pulse counter or dedicated sensor.

7. **Setpoint Module:**
   - Enables adjusting reference values (setpoints) or PID parameters via a potentiometer connected to the ADC with DMA.

8. **Button Management:**
   - Configures interrupts for START/STOP buttons to activate/deactivate the motor and handle object detection.
   - Includes debounce handling and communication via flags.

9. **Update Logic:**
   - Manages periodic operations like data acquisition, PID adjustments, and display updates using the SysTick interrupt handler.

### How It Was Implemented
- **Project Structure:**
  - Organized into independent modules to ensure maintainability and scalability.
  - Each module encapsulates its functionality (LCD, motor, sensor, etc.) with a clear interface to the main program.

- **Languages and Tools Used:**
  - C language for firmware development.
  - STM32F103T8C peripherals: GPIO, ADC, I2C, and Timers.

- **Methodology:**
  - Designed with a modular approach to decouple functionalities and facilitate unit testing.
  - Utilized interrupts for critical events such as buttons and distance measurement.

- **Key Decisions:**
  - I2C was chosen for LCD control to minimize required pins.
  - Initial MVP functionality relies on interrupts before migrating to FreeRTOS in future versions.
  - SysTick was included for periodic tasks with adjustable priorities.

### Why It Was Done This Way
- **Modularity:** Separating functionalities into modules allows adding or modifying features without impacting other components.
- **Compatibility:** Leveraging the STM32F103T8C’s integrated peripherals ensures an efficient and hardware-compatible design.
- **Flexibility:** Design choices like using interrupts for the MVP ensure basic functionality while allowing future expansion to FreeRTOS.
- **Scalability:** The modular design and use of standards like I2C and PWM make it easier to integrate new sensors or peripherals without redesigning the base system.


## Version 1.1.0
### Release Date
[19/11/2024]

### Summary
This update introduces UART communication to the system, enabling real-time configuration of PID parameters and setpoints through serial commands.

### Added Features
1. **UART Initialization:**
   - Configured USART1 for data transmission and reception.
   - Supports standard UART settings (9600 baud rate, 8 data bits, 1 stop bit).

2. **Interrupt-Driven Communication:**
   - Implemented an ISR for handling received data.
   - Stores incoming characters in a buffer until a complete command is detected.

3. **Command Processing:**
   - Added functionality to parse and execute commands for adjusting PID parameters:
     - Proportional gain (KP)
     - Integral gain (KI)
     - Derivative gain (KD)
     - Setpoint (SP)
   - Provides feedback via UART on command status.

4. **String Transmission:**
   - Enabled UART-based string sending for user feedback.

### Benefits
- Allows dynamic tuning of the PID controller without recompiling the firmware.
- Simplifies system interaction and configuration during runtime.
