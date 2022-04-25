int rgmp = 13; //right grab motor positive
int rgmn = 12; //right grab motor negative
int rsmp = 11; //right spin motor positive
int rsmn = 10;
int lgmp = 9;
int lgmn = 8;
int lsmp = 7;
int lsmn = 6;


bool RB = false; //Right bumper
bool LB = false; //Left bumper
bool RT = false; //Right trigger
bool LT = false; //Left trigger
bool LA = false; //Left Arrow
bool RA = false; //Right Arrow
bool MSL = false; //button to select left motor;  !!!!! need code to make this a toggle button so when one of these is pressed it sets its corisponding variable to true and sets the other one to false!!!!!
bool MSR = false; //button to select right motor;
void setup() {
  // put your setup code here, to run once:
  pinMode(rgmp, OUTPUT);
  pinMode(rgmn, OUTPUT);
  pinMode(rsmp, OUTPUT);
  pinMode(rsmn, OUTPUT);
  pinMode(lgmp, OUTPUT);
  pinMode(lgmn, OUTPUT);
  pinMode(lsmp, OUTPUT);
  pinMode(lsmn, OUTPUT);
}
void grabi (char Motor, char Direction) { // code to spin an idividual grabbing motor (M:which motor L or R for left or right) (D: direction O or C for open or closed)
  int P;
  int N;
  //set the positive and negative to the corrisponding motor
  if (Motor == 'L') {
    P = lgmp;
    N = lgmn;
  }
  if (Motor == 'R') {
    P = rgmp;
    N = rgmn;
  }
  if (Direction == 'O') {
    digitalWrite(P, HIGH);
    digitalWrite(N, LOW);
  }
  if (Direction == 'C') {
    digitalWrite(P, LOW);
    digitalWrite(N, HIGH);
  }
}
void spini(char Motor, String Direction) {  // L:left R:Right , CW: clockwise CC: counterclockwise
  int P;
  int N;
  //set the positive and negative to the corrisponding motor
  if (Motor == 'L') {
    P = lsmp;
    N = lsmn;
  }
  if (Motor == 'R') {
    P = rsmp;
    N = rsmn;
  }
  if (Direction == "CW") {
    digitalWrite(P, HIGH);
    digitalWrite(N, LOW);
  }
  if (Direction == "CC") {
    digitalWrite(P, LOW);
    digitalWrite(N, HIGH);
  }
}
void Grab () { //the grab function to be put into the loop which controls the grabbers
  if (RT) {
    grabi('R', 'C');
  }
  else if (RB) {
    grabi('R', 'O');
  }
  else {
    digitalWrite(rgmp, LOW);
    digitalWrite(rgmn, LOW);
  }

  if (LT) {
    grabi('L', 'C');
  }
  else if (LB) {
    grabi('L', 'O');
  }
  else {
    digitalWrite(lgmp, LOW);
    digitalWrite(lgmn, LOW);
  }
}

void Spin () {

  if (MSR) {
    if (RA) {
      spini('R', "CW");
    }
    else if (LA) {
      spini('R', "CC");
    }
    else {
      digitalWrite(rsmp, LOW);
      digitalWrite(rsmn, LOW);
    }
  }
  else if (MSL) {
    if (RA) {
      spini('L', "CW");
    }
    else if (LA) {
      spini('L', "CC");
    }
    else {
      digitalWrite(lsmp, LOW);
      digitalWrite(lsmn, LOW);
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
