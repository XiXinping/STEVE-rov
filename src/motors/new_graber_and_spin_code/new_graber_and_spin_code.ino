int GrabMotorPos = 3;   //blue/in3/out3
int GrabMotorNeg = 4;   //green/in4/out4
int SpinMotorPos = 11;  //in1/red
int SpinMotorNeg = 10;  //in2/black
int GrabCurrentControl = 5; //ENB !needs to be capable of analog
int SpinCurrentControl = 6; //ENA !needs to be capable of analog
bool CloseGrabInput=false; //grab toggle
bool SpinLeftInput=false;   //spin right
bool SpinRightInput=false;  //spin left




void setup() {
  // put your setup code here, to run once:
  pinMode(GrabMotorPos, OUTPUT);
  pinMode(GrabMotorNeg, OUTPUT);
  pinMode(SpinMotorPos, OUTPUT);
  pinMode(SpinMotorNeg, OUTPUT);
  pinMode(GrabCurrentControl, OUTPUT);
  pinMode(SpinCurrentControl, OUTPUT);
  analogWrite(GrabCurrentControl, 55);  //255=5v
  analogWrite(SpinCurrentControl, 255); //255=5v


}
void grab(){
  if (CloseGrabInput){
    digitalWrite(GrabMotorPos, HIGH);
    digitalWrite(GrabMotorNeg, LOW);
  }
  else {
    digitalWrite(GrabMotorPos, LOW);
    digitalWrite(GrabMotorNeg, HIGH);
  }
}
void spin(){
  if (SpinLeftInput){
    digitalWrite(SpinMotorPos, LOW);
    digitalWrite(SpinMotorNeg, HIGH);
  }
  else if (SpinRightInput){
    digitalWrite(SpinMotorPos, HIGH);
    digitalWrite(SpinMotorNeg, LOW);
  }
  else{
    digitalWrite(SpinMotorPos, LOW);
    digitalWrite(SpinMotorNeg, LOW);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  grab();
  spin();
  delay(5000);
  CloseGrabInput=true;
  SpinRightInput=true;
  grab();
  spin();
  delay(5000);
  SpinLeftInput=true;
  grab();
  spin();
  delay(5000);
  SpinRightInput=false;
  CloseGrabInput=false;
  grab();
  spin();
  delay(5000);
  SpinLeftInput=false;
  grab();
  spin();
  
}
