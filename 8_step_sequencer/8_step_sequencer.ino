// vim: ft=c

/*
 * 8 step sequencer for analog synthesizer
 * for use with an arduino nano, may be used with an uno if the 8 buttons are
 * not needed.
 *
 * rewritten from a project on https://www.lookmumnocomputer.com/projects/#/sequencer-keyboard/
 *
 */

//constants for the Arduino pin values
#define STEP1 2
#define STEP2 3
#define STEP3 4
#define STEP4 5
#define STEP5 6
#define STEP6 7
#define STEP7 8
#define STEP8 9
#define STEPBUTTON1   14
#define STEPBUTTON2   15
#define STEPBUTTON3   16
#define STEPBUTTON4   17
#define STEPBUTTON5   18
#define STEPBUTTON6   19
#define STEPBUTTON7   21
#define STEPBUTTON8   20

#define RESET 10
#define ZERO 11
#define FORWARD 12
#define BACK 13

int old_step = 0; //global to store previous stepper value

/*
 * check pins for runmode and determine step from that
 *
 * params:
 * step (int) - pin value to use for sending voltage to the correct
 *              arduino pin.
 *
 * returns: int value of step
 */
int stepper(int step)
{
    int modes[] = {FORWARD, BACK, ZERO, RESET};
    int current_mode;
    //iterate over the runmodes to determine which is getting HIGH volt
    for (int i = 0; i < (sizeof(modes)/sizeof(int)); ++i) {
        if (digitalRead(modes[i]) == HIGH) {
            switch (modes[i]) {
                case BACK:
                    //decrement to step back
                    step--;
                    current_mode = BACK;
                    break;
                case ZERO:
                    step = 0;
                    current_mode = ZERO;
                    break;
                case RESET:
                    step = 0;
                    current_mode = RESET;
                    break;
                default:
                    //default to stepping forward
                    step++;
                    current_mode = FORWARD;
                    break;
            }
        }
        //bounds checking to make sure we don't loop outside of our STEP values
        if ((step < STEP1 || step > STEP8) && current_mode == FORWARD) {
            return STEP1;
        }
        else if ((step < STEP1 || step > STEP8) && current_mode == BACK) {
            return STEP8;
        }
    }
    return step;
}

/*
 * check input pins for a buttonpush.
 * if a button is pushed, return the corresponding integer
 * to play a sequence on the correct output pin.
 *
 * relationship between STEP and STEPBUTTON is: STEP{n} = STEPBUTTON{n} - 12
 */
int buttonpush()
{
    // check if the digital pins are activated and return the correct value
    for (int i = STEPBUTTON1; i <= STEPBUTTON8; ++i) {
        if (digitalRead(i) == HIGH) {
            return i - 12;
        }
    }
    // return 0 if we don't have any button pushes
    return 0;
}

void setup()
{
    // setup pins for input or output
    pinMode(STEP1, OUTPUT);
    pinMode(STEP2, OUTPUT);
    pinMode(STEP3, OUTPUT);
    pinMode(STEP4, OUTPUT);
    pinMode(STEP5, OUTPUT);
    pinMode(STEP6, OUTPUT);
    pinMode(STEP7, OUTPUT);
    pinMode(STEP8, OUTPUT);
    pinMode(FORWARD, INPUT);
    pinMode(BACK, INPUT);
    pinMode(RESET, INPUT);
    pinMode(ZERO, INPUT);
    pinMode(STEPBUTTON1, INPUT);
    pinMode(STEPBUTTON2, INPUT);
    pinMode(STEPBUTTON3, INPUT);
    pinMode(STEPBUTTON4, INPUT);
    pinMode(STEPBUTTON5, INPUT);
    pinMode(STEPBUTTON6, INPUT);
}


void loop()
{
    // set the value of a buttonpush, if no button is pushed, set the next step
    // in the sequence.
    int current_step = buttonpush();
    if (current_step < STEP1) {
        current_step = stepper(old_step);
    }
    old_step = current_step;
    //write voltage to the correct pin, depending on the led val
    for (int i = STEP1; i <= STEP8; i++) {
        if (i == current_step) {
            digitalWrite(i, HIGH);
        } else {
            digitalWrite(i, LOW);
        }
    }
}
