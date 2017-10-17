import processing.serial.*;
import cc.arduino.*;

PGraphics layer;
Arduino arduino;
 
int JoystickClickPin = 0;
int YOutJoystickPin = 1; 
int XOutJoystickPin = 2;

int potPin = 3;
int fsrPin = 4;

float XOutRaw;
float YOutRaw;
int clickRaw;
int potRaw;
int fsrRaw;

ArrayList<Particle> pts;
boolean onPressed, showInstruction;
PFont instructionsFont;

void setup() { 
  println(Arduino.list());
  arduino = new Arduino(this, Arduino.list()[1], 57600);
  arduino.pinMode(XOutJoystickPin, Arduino.INPUT);
  arduino.pinMode(YOutJoystickPin, Arduino.INPUT);
  arduino.pinMode(JoystickClickPin, Arduino.INPUT);
  arduino.pinMode(potPin, Arduino.INPUT);
  arduino.pinMode(fsrPin, Arduino.INPUT);

  size(1280, 800, P2D);
  smooth();
  frameRate(30);
  colorMode(HSB);
  rectMode(CENTER);
  pts = new ArrayList<Particle>();
  showInstruction = true;
  instructionsFont = createFont("Helvetica", 18, true);
  background(255);
}

void draw() {
  XOutRaw = arduino.analogRead(XOutJoystickPin) * 2;
  YOutRaw = arduino.analogRead(YOutJoystickPin) * 1.5;
  clickRaw = arduino.analogRead(JoystickClickPin);
  potRaw = arduino.analogRead(potPin);
  fsrRaw = arduino.analogRead(fsrPin);
  
  print("XOutRaw");
  println(XOutRaw);
  print("\n");
  
  print("YOutRaw");
  println(YOutRaw);
  print("\n");
  
  print("ClickRaw");
  println(clickRaw);
  print("\n");
  
  print("potRaw");
  println(potRaw);
  print("\n");
  
  print("fsrRaw");
  println(fsrRaw);
  print("\n");
  
  //ellipse(XOutRaw, YOutRaw, 5.0, 5.0);

  if (showInstruction) {
    background(255);
    fill(128);
    textAlign(CENTER, CENTER);
    textFont(instructionsFont);
    text("drag and draw" + "\n" +
      "Press the joystick to clear the canvas." + "\n"
      , width/2, height/2);
  }
  
  if (fsrRaw != 0){
    onPressed = true;
    if (showInstruction) {
      background(255);
      showInstruction = false;
    }
  } else {
    onPressed = false;
  }
    
   if (clickRaw == 0) {
    for (int i=pts.size()-1; i>-1; i--) {
      Particle p = pts.get(i);
      pts.remove(i);
    }
    background(255);
  }

  if (onPressed || fsrRaw > 0) {
    for (int i=0;i<10;i++) {
      Particle newP = new Particle(XOutRaw, YOutRaw, i+pts.size(), i+pts.size());
      //Particle newP = new Particle(mouseX, mouseY, i+pts.size(), i+pts.size());
      pts.add(newP);
    }
  }
 
  for (int i=0; i<pts.size(); i++) {
    Particle p = pts.get(i);
    p.update();
    p.display();
  }
 
  for (int i=pts.size()-1; i>-1; i--) {
    Particle p = pts.get(i);
    if (p.dead) {
      pts.remove(i);
    }
  }  
}

 //hide instructions if mouse pressed
void mousePressed() {
  onPressed = true;
  if (showInstruction) {
    background(255);
    showInstruction = false;
  }
}
 
 //when the mouse is released change mousePressed to false
void mouseReleased() {
  onPressed = false;
}
 
//when the 'c' key is pressed clear the screen
void keyPressed() {
  if (key == 'c') {
    for (int i=pts.size()-1; i>-1; i--) {
      Particle p = pts.get(i);
      pts.remove(i);
    }
    background(255);
  }
}
 
//Particle object
class Particle{
  PVector loc, vel, acc;
  int lifeSpan, timeLiving;
  boolean dead;
  float alpha, weight, weightRange, decay, xOffset, yOffset;
  color c;
   
  Particle(float x, float y, float xOffset, float yOffset){
    loc = new PVector(x,y); 
    
    //** Adjust these variables **
    lifeSpan = int(random(30, 90)); //how much time the particle lives
    if (fsrRaw > 0) {
      weightRange = random(3, fsrRaw/5); //how big can each circle be
    } else {
      weightRange = random(3, 50); //how big can each circle be
    }
    color cmix = color(potRaw, potRaw, potRaw); //color definition
    print(potRaw);
    c = generateRandomColor(cmix); //generates pseudo-random colors within a same palette based on the value of cmix
    
    float randomDegrees = random(360);
    vel = new PVector(cos(radians(randomDegrees)), sin(radians(randomDegrees))); 
    vel.mult(random(5));
    acc = new PVector(0,0);
    decay = random(0.75, 0.9);    
    this.xOffset = xOffset;
    this.yOffset = yOffset;
  }
   
  void update(){
    if(timeLiving>=lifeSpan){
      dead = true;
    }else{
      timeLiving++;
    }
     
    alpha = float(lifeSpan-timeLiving)/lifeSpan * 70+50;
    weight = float(lifeSpan-timeLiving)/lifeSpan * weightRange;
     
    acc.set(0,0);    
    float rn = (noise((loc.x+frameCount+xOffset)/100, (loc.y+frameCount+yOffset)/100)-0.5)*4*PI;
    float magnitude = noise((loc.y+frameCount)*0.01, (loc.x+frameCount)*0.01);
    PVector direction = new PVector(cos(rn),sin(rn));
    acc.mult(magnitude);
    acc.add(direction);
    float randomDegrees = random(360);
    PVector randomVector = new PVector(cos(radians(randomDegrees)), sin(radians(randomDegrees)));
    randomVector.mult(0.5);
    acc.add(randomVector);    
    vel.add(acc);
    vel.mult(decay);
    vel.limit(3);
    loc.add(vel);
  }
   
  void display(){
    strokeWeight(weight+1.5);
    point(loc.x, loc.y);
    stroke(c);
    point(loc.x, loc.y);
  }
}
 
/* function that returns a random color:
    we average RGB values of random colors with those of a constant color (mix) in order to generate
    an aesthetically pleasent color palette
 */
 // https://stackoverflow.com/questions/43044/algorithm-to-randomly-generate-an-aesthetically-pleasing-color-palette
color generateRandomColor(color mix) {
    int red = int(random(100,250));
    int green = int(random(100,250));
    int blue = int(random(100,250));

    // mixing the color (averaging)
    red = int((red + red(mix)) / 2);
    green = int((green + green(mix)) / 2);
    blue = int((blue + blue(mix)) / 2);
 
    color c= color(red, green, blue);
    return c;
}