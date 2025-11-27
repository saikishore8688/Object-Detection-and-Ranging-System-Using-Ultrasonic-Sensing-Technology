
import processing.serial.*;

Serial myPort;
String data = "";
int angle = 0, distance1 = 0, distance2 = 0;

// Radar display settings
int radarSize = 500;
int centerX, centerY;

void setup() {
  size(700, 700);
  smooth();
  centerX = width / 2;
  centerY = height / 2;  // Centered radar for 360°

  // Change "COM3" to your Arduino's COM port
  myPort = new Serial(this, "COM3", 9600);
  myPort.bufferUntil('.');
}

void draw() {
  background(0);
  drawRadar();
  drawSweepLines();
  drawObject(distance1, angle - 5, color(255, 0, 0)); // Sensor 1 (Red), shifted by -5°
  drawObject(distance2, angle + 5, color(0, 0, 255)); // Sensor 2 (Blue), shifted by +5°
}

// Serial Event: Read Data from Arduino
void serialEvent(Serial myPort) {
  data = myPort.readStringUntil('.');
  if (data != null) {
    data = trim(data);
    String[] values = split(data, ",");
    if (values.length == 3) {
      angle = int(values[0]);
      distance1 = int(values[1]);
      distance2 = int(values[2]);
    }
  }
}

// Draw Radar with 360-degree grid
void drawRadar() {
  stroke(0, 255, 0);
  noFill();
  
  ellipse(centerX, centerY, radarSize, radarSize);
  ellipse(centerX, centerY, radarSize * 0.75, radarSize * 0.75);
  ellipse(centerX, centerY, radarSize * 0.5, radarSize * 0.5);
  ellipse(centerX, centerY, radarSize * 0.25, radarSize * 0.25);

  for (int i = 0; i < 360; i += 10) {
    float x = centerX + cos(radians(i)) * radarSize / 2;
    float y = centerY + sin(radians(i)) * radarSize / 2;
    line(centerX, centerY, x, y);
  }
}

// Draw TWO sweeping lines (one for each sensor)
void drawSweepLines() {
  stroke(0, 255, 0); // Green for Sensor 1
  float x1 = centerX + cos(radians(angle - 90)) * radarSize / 2;
  float y1 = centerY + sin(radians(angle - 90)) * radarSize / 2;
  line(centerX, centerY, x1, y1);

  stroke(255, 255, 0); // Yellow for Sensor 2
  float x2 = centerX + cos(radians(angle - 90 + 180)) * radarSize / 2; // Offset 10° for visibility
  float y2 = centerY + sin(radians(angle - 90 + 180)) * radarSize / 2;
  line(centerX, centerY, x2, y2);
}

// Correct object positioning for 360° and different sensors
void drawObject(int distance, int objectAngle, color objColor) {
  if (distance > 0 && distance < 100) {
    float scaledDistance = map(distance, 0, 100, 0, radarSize / 2);
    float x = centerX + cos(radians(objectAngle - 90)) * scaledDistance;
    float y = centerY + sin(radians(objectAngle + 90)) * scaledDistance;
    
    fill(objColor);
    noStroke();
    ellipse(x, y, 10, 10);
  }
}
