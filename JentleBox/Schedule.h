struct ExpParam
{
  String name;
  double habituation_time;
  double cs_duration;
  double us_onset;
  double us_duration;
  double iti_duration_min;
  double iti_duration_max;
  int num_trial;
};

int numExp = 5;
ExpParam expParam[5];
// 1. us_onset must be greater than 200ms
// 2. if you don't want to present the US, set us_duration to 0. us_onset will be ignored.
void initializeExpParam()
{
  expParam[0].name = "Ji Hoon";
  expParam[0].habituation_time = 120;
  expParam[0].cs_duration = 10;
  expParam[0].us_onset = 9;
  expParam[0].us_duration = 1;
  expParam[0].iti_duration_min = 60;
  expParam[0].iti_duration_max = 60;
  expParam[0].num_trial = 10;

  expParam[1].name = "Seyoung";
  expParam[1].habituation_time = 120;
  expParam[1].cs_duration = 10;
  expParam[1].us_onset = 8;
  expParam[1].us_duration = 2;
  expParam[1].iti_duration_min = 60;
  expParam[1].iti_duration_max = 60;
  expParam[1].num_trial = 30;

  expParam[2].name = "Cat";
  expParam[2].habituation_time = 120;
  expParam[2].cs_duration = 10;
  expParam[2].us_onset = 8;
  expParam[2].us_duration = 2;
  expParam[2].iti_duration_min = 60;
  expParam[2].iti_duration_max = 60;
  expParam[2].num_trial = 30;

  expParam[3].name = "Hua";
  expParam[3].habituation_time = 50;
  expParam[3].cs_duration = 10;
  expParam[3].us_onset = 8;
  expParam[3].us_duration = 2;
  expParam[3].iti_duration_min = 5;
  expParam[3].iti_duration_max = 5;
  expParam[3].num_trial = 15;

  expParam[4].name = "Debug";
  expParam[4].habituation_time = 10;
  expParam[4].cs_duration = 10;
  expParam[4].us_onset = 8;
  expParam[4].us_duration = 2;
  expParam[4].iti_duration_min = 5;
  expParam[4].iti_duration_max = 5;
  expParam[4].num_trial = 30;
}

