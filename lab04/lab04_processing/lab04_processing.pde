
import peasy.*;
import processing.serial.*;


PeasyCam camera;

float state1_cp1x = 210;
float state1_cp1y = 1210;
float state1_x1 = 100;
float state1_y1 = 490;
float state1_cp2x = 200;
float state1_cp2y = 100;
float state1_x2 = 410;
float state1_y2 = 210;

float state2_cp1x = 110;
float state2_cp1y = 1310;
float state2_x1 = 100;
float state2_y1 = 490;
float state2_cp2x = 240;
float state2_cp2y = 250;
float state2_x2 = 410;
float state2_y2 = 510;

float state3_cp1x = 110;
float state3_cp1y = 1410;
float state3_x1 = 100;
float state3_y1 = 490;
float state3_cp2x = 240;
float state3_cp2y = 400;
float state3_x2 = 410;
float state3_y2 = 1210;

float curr_state_cp1x = 210;
float curr_state_cp1y = 1210;
float curr_state_x1 = 100;
float curr_state_y1 = 490;
float curr_state_cp2x = 200;
float curr_state_cp2y = 100;
float curr_state_x2 = 410;
float curr_state_y2 = 210;

float target_state_cp1x = 210;
float target_state_cp1y = 1210;
float target_state_x1 = 100;
float target_state_y1 = 490;
float target_state_cp2x = 200;
float target_state_cp2y = 100;
float target_state_x2 = 410;
float target_state_y2 = 210;

int state;
int pressure = 1;

Serial myPort;
int serialVal = 0;

float fsrRaw = 0;
float flexRaw = 0;

void setup() {
  printArray(Serial.list());
  myPort = new Serial(this, Serial.list()[1], 9600);
  size(500, 700, P3D);
  camera = new PeasyCam(this, 250, 350, 0, 700);
} 

void draw() {
  if ( myPort.available() > 0) {
    String readVal = myPort.readStringUntil('\n');
    //print(readVal);
    if (readVal != null) {
      println(readVal);
      //int[] values = int(split(readVal, ','));
      float[] values = float(split(readVal, ','));
      fsrRaw = values[0];
      flexRaw = values[1];
      print("fsrRaw ");
      println(fsrRaw);
      print("flexRaw ");
      println(flexRaw);      
    }
  }
  if (fsrRaw == 0) {
    pressure = 0;
  } else if (fsrRaw < 50) {
    pressure = 1;
  } else if (fsrRaw < 100) {
    pressure = 2;
  } else {
    pressure = 3;
  }

  if (flexRaw < 155) {
    state = 3;
  } else if (flexRaw < 250) {
    state = 2;
  } else {
    state = 1;
  }
    
  noFill(); 
  background(0);
  lights();
  smooth();  
  
  stroke(255);
  strokeWeight(20);
  
  float interpolation_factor = 0.1;
  
  if (state == 1) {
    target_state_cp1x = state1_cp1x;
    target_state_cp1y = state1_cp1y;
    target_state_x1 = state1_x1;
    target_state_y1 = state1_y1;
    target_state_cp2x = state1_cp2x;
    target_state_cp2y = state1_cp2y;
    target_state_x2 = state1_x2;
    target_state_y2 = state1_y2;
  } else if (state == 2) {
    target_state_cp1x = state2_cp1x;
    target_state_cp1y = state2_cp1y;
    target_state_x1 = state2_x1;
    target_state_y1 = state2_y1;
    target_state_cp2x = state2_cp2x;
    target_state_cp2y = state2_cp2y;
    target_state_x2 = state2_x2;
    target_state_y2 = state2_y2;  
  } else if (state == 3) {
    target_state_cp1x = state3_cp1x;
    target_state_cp1y = state3_cp1y;
    target_state_x1 = state3_x1;
    target_state_y1 = state3_y1;
    target_state_cp2x = state3_cp2x;
    target_state_cp2y = state3_cp2y;
    target_state_x2 = state3_x2;
    target_state_y2 = state3_y2;    
  }
  
  curr_state_cp1x = lerp(curr_state_cp1x, target_state_cp1x, interpolation_factor);
  curr_state_cp1y = lerp(curr_state_cp1y, target_state_cp1y, interpolation_factor);  
  curr_state_x1 = lerp(curr_state_x1, target_state_x1, interpolation_factor);  
  curr_state_y1 = lerp(curr_state_y1, target_state_y1, interpolation_factor);  
  curr_state_cp2x = lerp(curr_state_cp2x, target_state_cp2x, interpolation_factor);  
  curr_state_cp2y = lerp(curr_state_cp2y, target_state_cp2y, interpolation_factor);  
  curr_state_x2 = lerp(curr_state_x2, target_state_x2, interpolation_factor);
  curr_state_y2 = lerp(curr_state_y2, target_state_y2, interpolation_factor);
  
  curve(
    curr_state_cp1x, 
    curr_state_cp1y,
    curr_state_x1,
    curr_state_y1,
    curr_state_cp2x,
    curr_state_cp2y,
    curr_state_x2,
    curr_state_y2
  );

  if (pressure == 0) {
    fill(255, 255, 255);
  }
  
  if (pressure == 1) {
    fill(255, 186, 186);
  }

  if (pressure == 2) {
    fill(255, 114, 114);
  }
  
  if (pressure == 3) {
    fill(255, 0, 0);
  }
  
  noStroke();
   
  segment(curr_state_cp2x, curr_state_cp2y, 20, 15);
}

void keyPressed() {
  if (key == '1'){
    state = 1;
  }
  if (key == '2'){
    state = 2;
  }
  if (key == '3'){
    state = 3;
  }
  if (key == 'q'){
    pressure = 0;
  }
  if (key == 'w'){
    pressure = 1;
  }
  if (key == 'e'){
    pressure = 2;
  }
  if (key == 'r'){
    pressure = 3;
  }  
}

void segment(float x, float y, float a, float r) {
  pushMatrix();
  translate(x,y);
  rotate(a);
  sphere(r);
  popMatrix();
}