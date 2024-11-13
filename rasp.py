import cv2
import serial
import numpy as np
import time

# Setup serial communication with Arduino
arduino = serial.Serial('/dev/ttyUSB0', 9600, timeout=1)

# Set up DroidCam video capture
cap = cv2.VideoCapture('http://localhost:4747/video')

# Load the stop sign cascade classifier (use your own file path if needed)
stop_cascade = cv2.CascadeClassifier('stop_sign_cascade.xml')

# Function to send command to Arduino
def send_command_to_arduino(command):
    print(f"Sending command: {command}")
    arduino.write((command + '\n').encode())  # Send command to Arduino via serial

# Function to detect stop sign and estimate distance (using Haar Cascade)
def detect_stop_sign(frame):
    # Convert the frame to grayscale
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    # Detect stop signs using the cascade classifier
    stop_signs = stop_cascade.detectMultiScale(gray, scaleFactor=1.1, minNeighbors=5, minSize=(30, 30))

    for (x, y, w, h) in stop_signs:
        cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 4)  # Draw rectangle around the stop sign
        
        # Estimate the distance to the stop sign (simplified model: distance inversely proportional to width of detected sign)
        distance = (500 * 24) / w  # Example formula: distance = (constant * real-world size) / detected width
        # Assuming a real-world size of 24 inches for the stop sign, adjust for your setup
        
        print(f"Stop sign detected at distance: {distance} cm")
        
        # If the distance is less than a threshold (e.g., 15 cm), stop the robot
        if distance < 15:
            send_command_to_arduino("STOP")
            return True  # Stop sign detected and action taken

    return False  # No stop sign detected

# Function to analyze frame and decide action
def analyze_frame(frame):
    # Example for movement detection based on object color (e.g., red object for forward)
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    
    # Detect red color (example to move forward)
    lower_red = (0, 120, 70)
    upper_red = (10, 255, 255)
    mask_red = cv2.inRange(hsv, lower_red, upper_red)

    if cv2.countNonZero(mask_red) > 500:  # If red object detected, move forward
        send_command_to_arduino("MOVE_FORWARD")
    
    # Detect blue color (example to move backward)
    lower_blue = (100, 150, 0)
    upper_blue = (140, 255, 255)
    mask_blue = cv2.inRange(hsv, lower_blue, upper_blue)

    if cv2.countNonZero(mask_blue) > 500:  # If blue object detected, move backward
        send_command_to_arduino("MOVE_BACKWARD")
    
    # Detect stop sign (for stopping)
    if detect_stop_sign(frame):
        return  # Stop sign detected and action taken, no need for further analysis in this frame

    # Example: Move left (detect green color)
    lower_green = (35, 100, 100)
    upper_green = (85, 255, 255)
    mask_green = cv2.inRange(hsv, lower_green, upper_green)

    if cv2.countNonZero(mask_green) > 500:
        send_command_to_arduino("MOVE_LEFT")
    
    # Example: Move right (detect yellow color)
    lower_yellow = (20, 100, 100)
    upper_yellow = (30, 255, 255)
    mask_yellow = cv2.inRange(hsv, lower_yellow, upper_yellow)

    if cv2.countNonZero(mask_yellow) > 500:
        send_command_to_arduino("MOVE_RIGHT")

# Main loop to capture and process frames
while True:
    ret, frame = cap.read()
    if not ret:
        break
    
    analyze_frame(frame)
    
    # Show the frame (for debugging purposes)
    cv2.imshow("Frame", frame)

    # Break the loop when 'q' is pressed
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# Cleanup
cap.release()
cv2.destroyAllWindows()
arduino.close()
