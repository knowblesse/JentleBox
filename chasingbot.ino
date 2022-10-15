// Welcome Message
#define MSGSIZE 1
void printWelcomeMsg();

// Command sent to the ANY-MAZE
char letter_cson = 'q';
char letter_csoff = 'w';
char letter_EXStart = 's';
char letter_EXEnd = 'e';

enum ExpMode
{
  CONDITIONING,
  EXTINCTION,
  RETENTION
};

struct ExpParam
{
  double habituation_time;
  double cs_duration;
  double us_onset;
  double us_duration_min;
  double us_duration_max;
  double isi_duration_min;
  double isi_duration_max;
  int num_trial;
};

int mode; // Experiment Mode
PersonalParam pParam; // Personal Parameter
ExpParam param; // Experiment Parameter

void setup()
{
  printWelcomeMsg();
  
  bool invalidInput = true;
  Serial1.println("Who are you?");
  Serial1.println("a. KI&YB   b. BM    c. QuickTestMode");
  while(invalidInput)
  {
    if(Serial1.available())
    {
      switch(char(Serial1.read()))
      {
        /* Change this part for a new parameter   */
        /* The unit for the time parameter is sec */
        /*                BEGIN                   */
        case 'a':
          Serial1.println("Choi lab 2.5nd & Crazy Rabbit");
          
          pParam.cndParam.habituation_time = 300;
          pParam.cndParam.cs_duration = 10;
          pParam.cndParam.us_onset = 7.5; 
          pParam.cndParam.us_duration_min = 2.5;
          pParam.cndParam.us_duration_max = 2.5;
          pParam.cndParam.isi_duration_min = 50;
          pParam.cndParam.isi_duration_max = 50;
          pParam.cndParam.num_trial = 5;

          pParam.extParam.habituation_time = 300;
          pParam.extParam.cs_duration = 10;
          pParam.extParam.isi_duration_min = 50;
          pParam.extParam.isi_duration_max = 50;
          pParam.extParam.num_trial = 30;

          pParam.retParam.habituation_time = 300;
          pParam.retParam.cs_duration = 10;
          pParam.retParam.isi_duration_min = 50;
          pParam.retParam.isi_duration_max = 50;
          pParam.retParam.num_trial = 5;

          invalidInput = false;
          break;


void loop()
{

 BT.write(letter_EXEnd);
}

void printWelcomeMsg()
{
  for(int i = 0; i < MSGSIZE; i++)
  {
    Serial1.print(char(EEPROM.read(i)));
  }
}
