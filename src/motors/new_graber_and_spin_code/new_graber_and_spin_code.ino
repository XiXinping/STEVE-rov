int rgmp = 3; //right grab motor positive
int rgmn = 4; //right grab motor negative
int rsmp = 11; //right spin motor positive
int rsmn = 10;
int MPWM = 5; //motor power;
bool in1=false; //grab toggle
bool in2=false; //spin right
bool in3=false; //spin left



void setup() {
  // put your setup code here, to run once:
  pinMode(rgmp, OUTPUT);
  pinMode(rgmn, OUTPUT);
  pinMode(rsmp, OUTPUT);
  pinMode(rsmn, OUTPUT);
  pinMode(MPWM, OUTPUT);
  analogWrite(MPWM, 40);
  

}
void grab(){
  if (in1){
    digitalWrite(rgmp, HIGH);
    digitalWrite(rgmn, LOW);
  }
  else {
    digitalWrite(rgmp, LOW);
    digitalWrite(rgmn, HIGH);
  }
}
void spin(){
  if (in2){
    digitalWrite(rsmp, HIGH);
    digitalWrite(rsmn, LOW);
  }
  else if (in3){
    digitalWrite(rsmp, LOW);
    digitalWrite(rsmn, HIGH);
  }
  else{
    digitalWrite(rsmp, LOW);
    digitalWrite(rsmn, LOW);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(5000);
  in1=true;
  grab();
  spin();
  delay(5000);
  in1=false;
  grab();
  spin();
}
