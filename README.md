# Smart Home System with Fall Detection and PIR-based Automatic Lighting 🏠💡

## Overview
This project involves creating a **smart home system** that utilizes sensors for **fall detection** and **automatic lighting control**. The system is powered by an `ESP32` microcontroller, and the data from the sensors is sent to `ThingSpeak` for real-time analysis.

## Features
- **Automatic Lighting** (PIR Sensor) 🚶‍♂️💡
- **Fall Detection** (MPU 6050 Sensor) 🏃‍♂️💥
- **Data Logging and Visualization** on `ThingSpeak` 📊
- **Dashboard** for Real-time Monitoring 📱

## System Components
- **PIR Sensor**: Used for detecting motion and controlling lighting. 🚶‍♀️
- **MPU 6050 Sensor**: Used for detecting falls by analyzing acceleration and angular velocity. 🏃‍♂️

## Directory Structure 🗂️
The project has the following directory structure:

```bash
IoT Project/
├── data.png
├── data.txt
├── Fall/
├── IoT PPT.pptx
├── Proposal_8.docx
├── Proposal_8.pdf
├── Run/
├── sample.csv
├── Smart-lighting-groupProject/
├── Walk/
├── Down/
├── Up/
├── fallDetection/
└── latestcodeforbestcodetrying/
```

### Subdirectories and their contents:

- **Fall/**: Contains data and code related to fall detection. 🏃‍♂️💥
  - *Currently empty.*

- **Run/**: Includes images, data files, and code related to the "running" scenario. 🏃‍♂️
  - `runFinal.png` - Final result of the run scenario 🖼️
  - `run.png` - Initial results/plot for the run scenario 🖼️
  - `run.py` - Python code for analysis 🐍
  - `running final.csv` - Data for the running scenario 📊

  <!-- ![Run Final Image](path/to/Run/runFinal.png) -->

- **Walk/**: Contains images, data files, and code related to the "walking" scenario. 🚶‍♂️
  - `finalWalk.png` - Final result of the walk scenario 🖼️
  - `walk.png` - Initial results/plot for the walk scenario 🖼️
  - `walk.py` - Python code for analysis 🐍
  - `Walk_final.csv` - Data for the walking scenario 📊

  <!-- ![Walk Final Image](path/to/Walk/finalWalk.png) -->

- **Down/**: Includes data and code related to the "downward movement" scenario (e.g., descending stairs). ⬇️
  - `down_final.png` - Final result of the down scenario 🖼️
  - `down.png` - Initial results/plot for the down scenario 🖼️
  - `down.py` - Python code for analysis 🐍
  - `stair_down final.csv` - Data for the downward movement scenario 📊

  <!-- ![Down Final Image](path/to/Down/down_final.png) -->

- **Up/**: Contains data and code related to the "upward movement" scenario (e.g., ascending stairs). ⬆️
  - `up_final.png` - Final result of the up scenario 🖼️
  - `up.png` - Initial results/plot for the up scenario 🖼️
  - `up.py` - Python code for analysis 🐍
  - `stir_up final.csv` - Data for the upward movement scenario 📊

  <!-- ![Up Final Image](path/to/Up/up_final.png) -->

- **fallDetection/**: Contains the main `fallDetection.ino` file for the fall detection logic. ⚠️
  - `fallDetection.ino` - Code for fall detection based on `MPU 6050` sensor data. 🔧

- **latestcodeforbestcodetrying/**: Contains an `ino` file for testing and refining the fall detection code. 🔧
  - `latestcodeforbestcodetrying.ino` - Refined fall detection code for better performance. ⚙️

- **Smart-lighting-groupProject/**: This directory might contain collaborative documents or code for the lighting system. 💡

## Calibration and Implementation 🔧

### PIR Sensor (Automatic Lighting) 💡
1. **Calibration**: 
   - The `PIR sensor` was calibrated under different scenarios to determine the ideal cool-down time. A `2-second cool-down` was deemed ideal. ⏳
   - Issues with false data due to external lighting were resolved by installing side plates on the sensor to prevent interference. 🌞

2. **Data Collection**:
   - Data from the `PIR sensor` was initially collected and analyzed on `ThingSpeak`, adjusting the cool-down time accordingly.
   - Once calibrated, the sensor readings were replaced by `LED status data`, sent to `ThingSpeak` for display. 💡📊


### Fall Detection (MPU 6050) 🏃‍♂️💥
1. **Sensor Setup**: 
   - The `MPU 6050` measures acceleration and angular velocity.
   - Four different scenarios were defined to identify when a fall should **not** be considered:
     - **State 0**: No heavy activity (idle, acceleration is almost null). 🛋️
     - **State 1**: Heavy activity (e.g., walking, running). 🏃‍♂️
     - **State 2**: Ascending/descending stairs. 🏢
     - **State 3**: Turning or bending over. 🤸‍♂️

2. **Data Collection**:
   - Data from different scenarios were used to calculate the jerk value and thresholds. 📊
   - `20,000 readings` were captured with the `ESP32` to set the threshold values for fall detection. 🧠

   <!-- ![Fall Detection Data](path/to/fall_detection_plot.png) -->

3. **Logic**:
   - Based on the sensor data, thresholds for jerk, acceleration, and angular velocity were set. 📏
   - The system uses a **state machine** that transitions between states based on sensor data, detecting falls if certain thresholds are exceeded. ⚠️

## Research Papers 📝
As part of our research process, we **read relevant research papers** to understand fall detection and sensor calibration techniques. We then implemented one of these methodologies into our code base and further refined it based on our specific requirements and test results. 📚

## Issues and Challenges ⚠️
1. **Hardcoding Thresholds**: The system uses fixed thresholds, which require manual updates whenever new data is collected. 🔧
2. **Cool-down Time**: The system has a `1-second delay`, which can miss falls occurring within that time. ⏱️
3. **Scalability**: The number of scenarios is finite, and handling infinite variations in activity makes the system non-scalable. 🔄
4. **Fall Detection Limitations**: A fall while sitting is not detected by the current system. 🛋️

## Future Improvements 🌟
1. **Feedback System**: A feedback mechanism will be implemented to improve detection accuracy. 🗣️
2. **Mobile Integration**: `Blynk` will be used to create a mobile app for remote monitoring and control. 📱
3. **Dashboard Enhancements**: Additional features will be added to the `ThingSpeak` dashboard. 📈

## Course Information 🎓
This project was a part of our **IoT course** at **IIIT Hyderabad**, where we explored various IoT concepts and implemented this system as part of our learning process. 🖥️

Here's the updated README with a **Results** section that includes all relevant PNG files rendered directly:

---

# Results 📊📈

This section showcases the results from different scenarios and sensors, highlighting the processed data and visualizations generated during the project.

---


### 1. **Fall Detection Results** 🏃‍♂️💥

The following images depict the results for different scenarios processed using the **MPU 6050 sensor** for fall detection:

#### a. **Running Scenario** 🏃‍♂️

- **Run Plot**: This plot illustrates the acceleration and angular velocity data collected during the running scenario.

  ![Run Data](./IoT%20Project/Run/run.png)

---

#### b. **Walking Scenario** 🚶‍♂️

- **Walk Plot**: Visualizing the sensor data during a walking scenario.

  ![Walk Data](./IoT%20Project/Walk/walk.png)

---

#### c. **Upward Movement (Stairs Up)** ⬆️

- **Up Plot**: The sensor readings captured while ascending stairs.

  ![Up Data](./IoT%20Project/Up/up.png)

---

#### d. **Downward Movement (Stairs Down)** ⬇️

- **Down Plot**: The processed data during the descent of stairs.

  ![Down Data](./IoT%20Project/Down/down.png)

---

### 2. **Final Processed Results for Scenarios** ✅

Below are the **final processed results** after refining the data for each scenario.

- **Final Walk Plot**:

  ![Final Walk Data](./IoT%20Project/Walk/finalWalk.png)

- **Final Run Plot**:

  ![Final Run Data](./IoT%20Project/Run/runFinal.png)

- **Final Down Plot**:

  ![Final Down Data](./IoT%20Project/Down/down_final.png)

- **Final Up Plot**:

  ![Final Up Data](./IoT%20Project/Up/up_final.png)

---

This **Results** section provides a comprehensive overview of the sensor data processing and analysis conducted for both **fall detection** and **PIR-based automatic lighting**. Each visualization highlights how sensor data was used to achieve the project goals effectively.


![Data](./IoT%20Project/data.png)


## Contribution Table 🤝
- **Sanyam**: Contributed to the overall design and development of the system. 🛠️
- **Kushal**: Implemented the logic in both code and hardware, played a significant role in data collection, and worked on the `ThingSpeak` integration for fall detection. 🖥️🔧
- **Shreyas**: Created the dashboard, defined the scenarios, wrote Python scripts for data analysis, and was instrumental in data collection and `ThingSpeak` integration for `PIR`. 📊💻
- **Inesh**: Worked on `PIR hardware`, wrote scripts, tested the entire system, and contributed to the `ThingSpeak` integration for the `PIR` sensor. 🔌🖥️

