int GrabMotorPos = 3; //right grab motor positive
int GrabMotorNeg = 4; //right grab motor negative
int SpinMotorPos = 11; //right spin motor positive
int SpinMotorNeg = 10;
int MPWM = 5; //motor power;
bool CloseGrabInput=false; //grab toggle
bool SpinRightInput=false; //spin right
bool SpinLeftInput=false; //spin left



void setup() {
  // put your setup code here, to run once:
  pinMode(GrabMotorPos, OUTPUT);
  pinMode(GrabMotorNeg, OUTPUT);
  pinMode(SpinMotorPos, OUTPUT);
  pinMode(SpinMotorNeg, OUTPUT);
  pinMode(MPWM, OUTPUT);
  analogWrite(MPWM, 40);
  

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
  if (SpinRightInput){
    digitalWrite(SpinMotorPos, HIGH);
    digitalWrite(SpinMotorNeg, LOW);
  }
  else if (SpinLeftInput){
    digitalWrite(SpinMotorPos, LOW);
    digitalWrite(SpinMotorNeg, HIGH);
  }
  else{
    digitalWrite(SpinMotorPos, LOW);
    digitalWrite(SpinMotorNeg, LOW);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(5000);
  CloseGrabInput=true;
  grab();
  spin();
  delay(5000);
  CloseGrabInput=false;
  grab();
  spin();
  
}
