#include "loopTasks.h"

extern Errors errors;
extern Queue queue;
extern DataStruct dataStruct;
extern ImuAPI IMU;
extern FlightTimer flightTimer;
extern SDCard sdCard;

void imuCalcuationsTask(void *arg)
{
    float maxAltitude = 0;
    float currentAltitude = 0;
    uint64_t apogeeTimer = 0;
    const uint64_t apogeeConfirmTime = 1000; //ms
    const uint64_t igniterSafeTime = 9000;
    const uint64_t timeout = 30000; //ms
    const uint64_t apogeeDetectStart = 5000; //ms
    char log[80];
    bool apogeeAltitudeConfirm = false;
    bool apogeeAccZConfirm = false;

    while(1){
        currentAltitude = dataStruct.imuData.altitude;

        if (maxAltitude < currentAltitude)
        {
            maxAltitude = currentAltitude;
            apogeeTimer = millis();
        }
        if((dataStruct.imuData.az < 0.40) && (flightTimer.getFlightTime() > apogeeDetectStart)){
            apogeeAccZConfirm = true;
        }

        if((millis() - apogeeTimer > apogeeConfirmTime) && (flightTimer.getFlightTime() > apogeeDetectStart)){
            apogeeAltitudeConfirm = true;
        }

        if((apogeeAltitudeConfirm && apogeeAccZConfirm) && (flightTimer.getFlightTime() > igniterSafeTime)){
            dataStruct.apogeeDetect = true;
            snprintf(log, sizeof(log), "Apogee detected : %f; %d; %d; %d;", 
                    maxAltitude, (int)flightTimer.getFlightTime(), apogeeAltitudeConfirm, apogeeAccZConfirm);
            
            sdCard.write("/Break_logs.txt", String(log));
            vTaskDelete(NULL);
        
        }else if(flightTimer.getFlightTime() > timeout){
            snprintf(log, sizeof(log), "Apogee not detected : %f; %d; %d; %d;", 
                    maxAltitude, (int)flightTimer.getFlightTime(), apogeeAltitudeConfirm, apogeeAccZConfirm);
            
            sdCard.write("/Break_logs.txt", String(log));
            vTaskDelete(NULL);
        }

        vTaskDelay(40/portTICK_PERIOD_MS);

    }
}

//Simple error handling
void errorTask(void *arg)
{
    uint8_t buzzerPin = 4;
    pinMode(buzzerPin, OUTPUT);
    errors.imu_error = IMU_NOERROR;
    errors.sd_error = SD_NOERROR;

    digitalWrite(buzzerPin, HIGH);
    vTaskDelay(175);
    digitalWrite(buzzerPin, LOW);

    while (1)
    {
        if (errors.imu_error || errors.sd_error || errors.rocketError)
        {
            digitalWrite(buzzerPin, HIGH);
        }
        else
        {
            digitalWrite(buzzerPin, LOW);
        }

        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

/**
 * Czy wyłączyć airBrake jeżeli igniter się nie odpali?
 * bo 30s
 *  
 */
void stateTask(void *arg)
{
    if (!dataStruct.rss.successfulInit)
    {
        errors.rocketError = ROCKET_INIT_ERROR;
        while (1)
        {
            vTaskDelay(100 / portTICK_PERIOD_MS);
        }
    }

    if (dataStruct.rocketState != LAUNCHPAD)
    {
        errors.rocketError = ROCKET_STATE_ERROR;
        while (1)
        {
            vTaskDelay(100 / portTICK_PERIOD_MS);
        }
    }

    while (1)
    {
        if ((dataStruct.rocketState == LAUNCHPAD) && ((digitalRead(liftOffDetector) == 0) || dataStruct.imuData.altitude > 100))
        {
            flightTimer.startTimer();
            dataStruct.rocketState = FLIGHT;
            xTaskCreate(flightControlTask,  "flight control task",    16384,  NULL, 1, NULL); 
            xTaskCreate(imuCalcuationsTask, "imuC calculations task", 32768,  NULL, 1, NULL);
            xTaskCreate(simulationTask,     "simulation Task",        65536, NULL, 2, NULL);
            Serial.println("FLIGHT");

        }
        else if (dataStruct.rocketState == FLIGHT && dataStruct.rss.airBrakeEjection != 0)
        {
            dataStruct.rocketState = AIRBRAKEON;
            Serial.println("AIRBRAKE");
        }
        else if (dataStruct.rocketState == AIRBRAKEON && dataStruct.rss.igniterState != 0)
        {
            dataStruct.rocketState = LANDING;
            Serial.println("LAnDING");
        }
        else if (dataStruct.rocketState == LANDING && dataStruct.imuData.altitude < 100.0)
        {
            dataStruct.rocketState = ONGROUND;
            Serial.println("On ground");
        }
        vTaskDelay(2 / portTICK_PERIOD_MS);
    }
}

void simulationTask(void *arg)
{
    float simStart = 0.0, thrustEndTime = 5.5, simHeight[2] = {IMU.getAltitude(), IMU.getAltitude()}, rocketMass = 3.5, propellantMass = 0.8; // for setup
    StateStruct stateAtStart(simHeight, 0.1, simStart);
    float Cd[301] = {
        0.40720440856436707,
        0.437889003609151,
        0.45182539967372065,
        0.45322313055929303,
        0.44629173006708467,
        0.43524073199831204,
        0.42427967015419205,
        0.4166578097647942,
        0.41178334177560066,
        0.408104188560946,
        0.4043751047340193,
        0.40057817386342387,
        0.39700231175661793,
        0.3938662177419134,
        0.3911077252310404,
        0.38859445115658386,
        0.38621763838107115,
        0.3839630334868,
        0.381840008986011,
        0.3798520238617094,
        0.3779788829799593,
        0.37619447767758996,
        0.37448108862219825,
        0.3728545538044531,
        0.37133910054579106,
        0.3699509972580979,
        0.3686746767150559,
        0.3674866127807961,
        0.3663653969945796,
        0.36529809159618537,
        0.36427387650052195,
        0.3632836779536817,
        0.362325407526492,
        0.3613987231209641,
        0.3605029543043776,
        0.3596361173050867,
        0.358795900016714,
        0.35798017675833443,
        0.35718756755083203,
        0.35641687884054296,
        0.35566692867254907,
        0.3549365814869148,
        0.3542247133224501,
        0.3535302727556481,
        0.3528524985137344,
        0.352190701861618,
        0.35154411504866484,
        0.35091165426206916,
        0.35029215667348207,
        0.34968488287408056,
        0.3490907871331451,
        0.348511247139482,
        0.34794611817525745,
        0.3473891658960755,
        0.34683263355090005,
        0.346274542949405,
        0.3457360301441038,
        0.34524400974822006,
        0.3448038781617264,
        0.3443349589315927,
        0.343735057391538,
        0.3429377448098404,
        0.342019656193015,
        0.34109319248213604,
        0.3402491901694462,
        0.3394922279518602,
        0.3388053200774611,
        0.338169664110759,
        0.3375591908819726,
        0.3369460145377481,
        0.336313486539208,
        0.33568990760538236,
        0.335114815769778,
        0.3346163311910443,
        0.33417690252840104,
        0.33376756056621093,
        0.3333628763570476,
        0.33295158202632813,
        0.33252594996768103,
        0.33208757455962545,
        0.3316753381202439,
        0.33133744495251005,
        0.33112819449963865,
        0.3311262667658107,
        0.33141643689544803,
        0.3321156856334876,
        0.33346981612692367,
        0.33575683712326504,
        0.33920091446002043,
        0.3438108423346971,
        0.3495415720348021,
        0.35635345846345745,
        0.36422847098624445,
        0.3731539825843591,
        0.3831356408901049,
        0.39425219214021484,
        0.4066006572225291,
        0.4201995967287766,
        0.43475373006624063,
        0.4498893163460931,
        0.46551281986365495,
        0.48265152565084196,
        0.502612923923719,
        0.5256855028377391,
        0.548081742305906,
        0.564995120180612,
        0.5732460958637103,
        0.5761630549548982,
        0.5787013646033339,
        0.5845014427330579,
        0.5919439103676388,
        0.598094439305526,
        0.6007922796757327,
        0.6009709949295231,
        0.600337726848725,
        0.6002384300079001,
        0.6005743101525495,
        0.600885385820909,
        0.6008376136136356,
        0.600600702381071,
        0.600470299035978,
        0.6006565373582631,
        0.6010274985964048,
        0.601365750866025,
        0.6015018780082757,
        0.6014585267664305,
        0.601306359609293,
        0.6010997021537372,
        0.6008275326089156,
        0.6004624923320511,
        0.5999865512666916,
        0.5994189937016875,
        0.598788432512214,
        0.5981203434924182,
        0.5974276541123319,
        0.596720154760958,
        0.5959957052445652,
        0.5952044430384849,
        0.594284575035314,
        0.5931953442255542,
        0.5919801379913289,
        0.590703379812667,
        0.5894181235956641,
        0.5881319449506865,
        0.5868410499141671,
        0.5855361691689427,
        0.5841861319834613,
        0.5827542922725739,
        0.5812202751375551,
        0.5796287904253703,
        0.5780408191694079,
        0.5765024352824829,
        0.5750000841951134,
        0.5735053042172439,
        0.5719922417732621,
        0.5704454757453288,
        0.5688521931300471,
        0.5672164311721929,
        0.5656096281092282,
        0.564120072426787,
        0.5627978468565215,
        0.5615402111141572,
        0.5602062191614381,
        0.5586939196219817,
        0.5570573397669015,
        0.555389501529184,
        0.5537638018029234,
        0.552175137326643,
        0.550598779799974,
        0.5490152553067085,
        0.5474261074672856,
        0.545838134286306,
        0.5442567221646968,
        0.5426816110886891,
        0.54111112944084,
        0.5395439805343665,
        0.5379803674051199,
        0.536420868019611,
        0.5348659569224411,
        0.5333156949705726,
        0.5317700395990589,
        0.5302289733069591,
        0.5286925788493564,
        0.52716096404534,
        0.5256342276297291,
        0.5241124320002598,
        0.522595630470398,
        0.5210838766698636,
        0.5195772254933927,
        0.5180757321519761,
        0.5165794498786622,
        0.5150884239947344,
        0.513602697843534,
        0.51212231360672,
        0.5106473088192216,
        0.509177719854286,
        0.5077135818855826,
        0.5062549252884709,
        0.504801779238733,
        0.503354171885004,
        0.5019121272673309,
        0.500475668398614,
        0.4990448173640693,
        0.4976195925381783,
        0.4962000113677381,
        0.49478609047599337,
        0.4933778431919777,
        0.491975282021172,
        0.4905784187350432,
        0.48918726216900493,
        0.4878018204244571,
        0.4864221009502727,
        0.48504810858521774,
        0.483679847515531,
        0.4823173213478383,
        0.48096053137031386,
        0.47960947829151895,
        0.47826416230598445,
        0.4769245815521185,
        0.4755907336542989,
        0.474262615781776,
        0.472940223283292,
        0.47162355105246195,
        0.4703125935807055,
        0.4690073437506609,
        0.46770779404277096,
        0.46641393658284264,
        0.46512576207813977,
        0.463843260881291,
        0.4625664230330221,
        0.4612952373264519,
        0.4600296922427971,
        0.4587697759897675,
        0.4575154756810446,
        0.45626677815680305,
        0.45502367001820293,
        0.4537861369103497,
        0.4525541642393341,
        0.4513277372032163,
        0.4501068401679318,
        0.448891457291385,
        0.4476815725512747,
        0.4464771692044794,
        0.4452782303276721,
        0.44408473884229693,
        0.44289667704888214,
        0.441714027092727,
        0.44053677098634386,
        0.43936489021109953,
        0.43819836611557395,
        0.43703717993560004,
        0.4358813124560225,
        0.434730744348938,
        0.4335854561920257,
        0.43244542818529086,
        0.43131064043432094,
        0.43018107296537567,
        0.42905670548740626,
        0.42793751763003707,
        0.42682348896207034,
        0.4257145988090225,
        0.42461082643558795,
        0.4235121510435898,
        0.4224185515833673,
        0.4213300069423881,
        0.4202464960128699,
        0.4191679977060286,
        0.41809449093783,
        0.4170259544302491,
        0.41596236612930026,
        0.41490370378700714,
        0.41384994575101064,
        0.4128010727514196,
        0.41175706611395985,
        0.41071790485585874,
        0.40968355876035045,
        0.40865399530217106,
        0.4076291905239486,
        0.4066091547398801,
        0.405593906832055,
        0.4045834336931011,
        0.40357759425780027,
        0.402576215471473,
        0.40158015956377896,
        0.4005944299017384,
        0.39962506513671103,
        0.398675831500317,
        0.3977414055452185,
        0.3968141914043379,
        0.3958881932141867,
        0.394963815125634,
        0.39404306129313804,
        0.3931275092434161,
        0.3922170299922215,
        0.391311067927566,
        0.39040917415014,
        0.38951132661134746,
        0.38861760997527095,
        0.38772810890599296,
        0.3868429080675958,
        0.385962092124162};
    RocketStruct rocket(rocketMass, propellantMass, thrustEndTime, stateAtStart, Cd);
    float tempVel;
    float tempLastHeight = IMU.getAltitude(), tempNewHeight = IMU.getAltitude();
    while (1)
    {
        tempVel = (tempNewHeight - tempLastHeight)/0.07;
        simStart += 0.07;
        tempLastHeight = tempNewHeight;
        tempNewHeight = IMU.getAltitude();
        dataStruct.simulationApogee =  rocket.apogeeSimulation(tempNewHeight, tempLastHeight, tempVel, simStart);
        vTaskDelay(1 / portTICK_PERIOD_MS);
    }
}