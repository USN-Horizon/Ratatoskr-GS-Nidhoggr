# Horizon Groundstation

The Horizon Groundstation is an application for visualizing real-time sensor data from a rocket, including telemetry and gyro data, through a sophisticated graphical interface. Developed using C++ for data processing and QML for the front-end, this project was created to provide a reliable, real-time monitoring tool as part of the University of South-Eastern Norway’s student company, USN Horizon.

## Table of Contents

- [Introduction](#introduction)
- [Project Structure](#project-structure)
- [Features](#features)
- [Signal & Slot Mechanism](#signal--slot-mechanism)
- [Testing](#testing)
- [Future Improvements](#future-improvements)
- [Authors](#authors)

## Introduction

The Horizon Groundstation application processes sensor data from a rocket in real-time, converting it into graphical forms such as line charts and gyro-based animations. This allows users to monitor parameters like humidity and orientation dynamically. This documentation provides a comprehensive view of the technical details and functionality for future development and maintenance.

## Project Structure

The application is designed to separate concerns, with each component having a designated purpose:

- **controllers/**: Contains `DataController` classes for data processing.
- **models/**: Holds data models, such as `HumidityModel`, which are structured for sensor data.
- **graph/**: Manages graphical representation, particularly the `GraphView` responsible for line charts.
- **utils/**: Contains helper functions, including CSV parsing for test data loading.
- **qml/**: Includes QML files for the user interface, defining the main window, graphs, and layout components.

This modular design facilitates ease of development and testing, as well as integration with real sensors.

## Features

1. **Real-time Graph Visualization**: The application continuously updates graphs with sensor data (currently simulated), providing an up-to-the-second display.
2. **Gyro Animation**: A rotating rocket animation reflects real-time changes in pitch, yaw, and roll.
3. **Data Averaging and Current Value Calculation**: Real-time averages and current values are displayed, giving users detailed insights into telemetry data.

## Signal & Slot Mechanism

The application employs Qt’s signal-slot architecture to ensure fast data flow between the back-end (C++) and the front-end (QML):

- **Data Update**: When new data is added to a model (e.g., `HumidityModel`), signals like `modelChanged()` and `averageChanged()` are emitted.
- **Front-End Update**: The QML interface listens to these signals and triggers immediate updates to display the latest data visually.
- **Graph Refresh**: Each signal triggers the `GraphView` component to redraw graphs, ensuring the graphical representation remains accurate.

## Testing

The application supports two main testing methods:

1. **Random Data Generation**: Simulated real-time data using `QRandomGenerator` for testing continuous data handling and updating.
2. **CSV Data Import**: Historical data can be loaded from CSV files, enabling developers to validate performance with specific data sets. CSV testing helps to verify the signal-slot system and ensure data integrity across sessions.

## Future Improvements

1. **Sensor Integration**: Connect real sensors for real-world applications.
2. **Enhanced GUI**: Expand the interface to include advanced controls and customization options.
3. **Gyro Visualization Expansion**: Add visual indicators for additional data points, such as acceleration in multiple directions.

## Authors

- **Hamsa Hashi**
- **Emory Eugene Høiensahl**

