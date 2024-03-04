#define PIN_LED_BLUE (2) //2번 LED는 파란
#define PIN_LED_RED (3) //3번 LED는 빨강
#define PIN_LED_GREEN (4) //4번 LED는 녹색
#define PIN_BTN_LEFT (5) //5번은 왼쪽 버튼
#define PIN_BTN_RIGHT (6) //6번은 오른쪽 버튼
#define PIN_TRIG (8) // echo sensor의 trigger 출력
#define PIN_ECHO (9) //echo sensor의 echo 입력
#define PIN_BUZZ (10) //Buzzer 출력
#define PIN_PAUSE (325) //325 delay에 사용 (노래 출력시)
#define PORT_RAND (A1) //
#define SERIAL_RATE (9600)//9600 보드레이트
#define SERIAL_DEALY (50) //0.05초 dealy
#define PULSE_WIDTH (30) //in usec 마이크로세컨드 
#define PULSE_TIMEOUT (50000) //in usec 마이크로세컨드 
#define MIN_RANGE (50.)  //50 mm


int nTotalScore = 0; // 합산 스코어
int nPin; //
int n = 0; //player가 지정한 숫자 초기값
int playn = 0; //play한 횟수 초기값
String str = ""; //str 입력 초기값

//////////////
//초기화
//////////////

void init3Led() {
  pinMode(PIN_LED_BLUE, OUTPUT); //파랑 출력
  pinMode(PIN_LED_RED, OUTPUT);//레드 출력
  pinMode(PIN_LED_GREEN, OUTPUT); //녹색 출력
  stop3Led();
}
 
void initRand() {
  int nVolt = analogRead(PORT_RAND);
  randomSeed(nVolt); // 난수를 초기화
}
 
void initbutton() {
  pinMode(PIN_BTN_LEFT, INPUT_PULLUP); //왼쪽 버튼 입력
  pinMode(PIN_BTN_RIGHT, INPUT_PULLUP); // 오른쪽 버튼 입력
}

void initGame() { //모두 초기화
  init3Led();
  initRand();
  initbutton();
  initBuzz();
  initEcho();
  Serial.begin(SERIAL_RATE); //시작
}


//////////////////
////LED METHOD////
//////////////////

void startLed(int nPin) {   /////////////////
  digitalWrite(nPin, HIGH);
  tone(PIN_BUZZ, 392 ,250); //학
  delay(PIN_PAUSE);
  tone(PIN_BUZZ, 392 ,250); //교
  delay(PIN_PAUSE);
  tone(PIN_BUZZ, 440 ,250); //종
  delay(PIN_PAUSE);
  tone(PIN_BUZZ, 440 ,250); //이
  delay(PIN_PAUSE);
  tone(PIN_BUZZ, 392 ,250); //땡
  delay(PIN_PAUSE);
  tone(PIN_BUZZ, 392 ,250); //땡
  delay(PIN_PAUSE);
  tone(PIN_BUZZ, 330 ,550); //땡
  delay(PIN_PAUSE);
}

void stopLed(int nPin) { ///////////////////
  digitalWrite(nPin, LOW);
  tone(PIN_BUZZ, 392 ,250); //기
  delay(PIN_PAUSE);
  tone(PIN_BUZZ, 329 ,250); //다
  delay(PIN_PAUSE);
  tone(PIN_BUZZ, 293 ,250); //리
  delay(PIN_PAUSE);
  tone(PIN_BUZZ, 329 ,250); //신
  delay(PIN_PAUSE);
  tone(PIN_BUZZ, 261 ,750); //다
  delay(PIN_PAUSE);
  noTone(PIN_BUZZ);
}

void stop3Led() {
  digitalWrite(PIN_LED_BLUE, LOW); //LED 파랑 끔
  digitalWrite(PIN_LED_RED, LOW); //LED 빨강 끔
  digitalWrite(PIN_LED_GREEN, LOW); //LED 녹색 끔
}

int getRandLedPin() {
  int nRand = random(3); //0, 1, 2
  return nRand + PIN_LED_BLUE; //0, 1, 2 -> 2, 3, 4
}

//////////////////
///Melody METHOD//
//////////////////

void initBuzz() {
  pinMode(PIN_BUZZ, OUTPUT); //부저 포트 출력부 지정
}

void playMelody(){
  tone(PIN_BUZZ, 100); //시작 멜로디
  delay(500);
  tone(PIN_BUZZ, 300); //시작 멜로디
  delay(500);
}

void stopMelody(){
  noTone(PIN_BUZZ); //엔딩 멜로디
  
}

//////////////////
///Sensor METHOD//
//////////////////
void initEcho(){
  pinMode(PIN_TRIG, OUTPUT);//출력
  pinMode(PIN_ECHO, INPUT); //입력
  digitalWrite(PIN_TRIG, LOW); //출력 0
}

void makeTrigger(){
  digitalWrite(PIN_TRIG, HIGH); 
  delayMicroseconds(PULSE_WIDTH); //정의           
  digitalWrite(PIN_TRIG, LOW); 
}


//in mm
double getEchoRange(){
  long nEchoTime = pulseIn(PIN_ECHO, HIGH);
  double range = nEchoTime*1e-6*340./1e-3/2.; // in mm; 음속: 340 m/s, 1e-6 - 1usec, 1e-3 -1mm
  return range;
}

//////////////////
////BTN METHOD////
//////////////////

bool checkLeftButton(){ //  누르면 TRUE, 평소는 FALSE
  int nInput = digitalRead(PIN_BTN_LEFT); //평소 HIGH 누르면 LOW
  return nInput == LOW;
}

bool checkRightButton(){
  int nInput = digitalRead(PIN_BTN_RIGHT);
  return nInput == LOW;
}

bool checkEcho()
{
  makeTrigger();
  double range = getEchoRange();
  bool bResult = range != 0. && range <= MIN_RANGE;  
  return bResult;
}

long checkBttonEcho(bool& bLeftInput, bool& bRightInput, bool& bCenterInput)
{
  long  nStartTime = millis();  //게임 시작시간
  bLeftInput = bRightInput = bCenterInput = false;
  while (1) {
    bLeftInput = checkLeftButton();
    bRightInput = checkRightButton();
    bCenterInput = checkEcho();
    if (bLeftInput || bRightInput || bCenterInput) break;
  }
  long  nStopTime = millis();  //버튼 누른 시간 측정
  return nStopTime = nStartTime;
} 


void endGame() //게임종료
{
  Serial.println("종료");
  Serial.end();  
  noTone(PIN_BUZZ);
  stopLed(nPin);
}

int evalInput(int nLed,bool bLeftInput,bool bRightInput,bool bCenterInput) {
  if (nLed == PIN_LED_RED) { //만약 led가 red이면 Leftinput이 참
    if (bLeftInput == true && bRightInput == false && bCenterInput ==false)
    return 1; //그리고 return 아니면 0
    else return 0;
  }
  else if (nLed == PIN_LED_BLUE) { //만약 led가 blue이면 Rightinput이 참
    if (bLeftInput == false && bRightInput == true && bCenterInput ==false)
    return 1; //그리고 return 아니면 0
    else return 0;
  }
  else if (nLed == PIN_LED_GREEN) { //만약 led가 Green이면 Centertinput이 참
     if (bLeftInput == false && bRightInput == false && bCenterInput ==true)
    return 1; //그리고 return 아니면 0
    else return 0;
  }
}

int scanInt() //입력부
{
  String str = ""; // 쓰는 글자 들어감
  while (str.length() == 0) //문자 길이가 0일 동안 반복
  {
    str = Serial.readStringUntil('\n'); //Enter 전까지 출력 
  }
  return str.toInt();
}

int startMenu()
{
  Serial.println("     ☆blue and red flag Game!☆      ");
  Serial.println("┌─────────────────────┐");
  Serial.println("│Lv1. Game 1round Play              │");
  Serial.println("│Lv2. Game 2round Play              │");
  Serial.println("│Lv3. Game 3round Play              │");
  Serial.println("│Lv4. Game 4round Play              │");
  Serial.println("│Lv5. Game 5round Play              │");
  Serial.println("│Lv6. Game 6round Play              │");
  Serial.println("│Lv7. Game 7round Play              │");
  Serial.println("└─────────────────────┘");
  Serial.println(" 원하는 스타일 선택.\n");
  Serial.println("");
  int num = scanInt(); //선택 할 Lv를 num으로 입력받아 반환하고

  return num; //num을 반환한다.
}

void setup() {
  // put your setup code here, to run once:
  initGame();
  playMelody();
  stopMelody();
  initEcho();
  n = startMenu(); //전역변수 n에 (입력받은)num값을 저장.
  str = Serial.readStringUntil('\n'); //enter 전까지 읽는다.
}

void loop() {
  // put your main code here, to run repeatedly:
  playn++; // playn을 1 증가
  
  int nRandLed = getRandLedPin(); //랜덤LED에 getRandLedPin을 넣어서 랜덤 색 출력
  bool bLeftInput, bRightInput, bCenterInput; // 버튼 입력여부를 확인하기위한 변수
  startLed(nRandLed); //LED 켜기
  long nTime = checkBttonEcho(bLeftInput, bRightInput, bCenterInput); //입력 확인 함수
  int nScore = evalInput(nRandLed, bLeftInput, bRightInput, bCenterInput); //점수 계산 함수
  nTotalScore += nScore; //총합 점수 계산
  stopLed(nRandLed); //LED를 멈춘다.
  delay(1000);
  Serial.println("left = " + String(bLeftInput) + "right = " + String(bRightInput) + "Center = " + String(bCenterInput)); //왼쪽, 오른쪽, 센터 입력들을 출력한다.
  Serial.println("Time = " + String(nTime * 1e-3) + "sec"); //시간 출력 1000
  Serial.println("the present Score = " + String(nScore)); //현재 스코어(점수)
  Serial.println("total present Score = " + String(nTotalScore));//현재 스코어의 총합
  
  if (playn == n or str == "exit") //playn이 n 혹은 "" 일 때 게임 종료 
  {
    endGame(); //게임 종료
  }
  
  Serial.println(" ===================================="); //잘 나오는지 확인   
}
