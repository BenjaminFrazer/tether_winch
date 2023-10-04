#include <Arduino.h>
#include <SimpleCLI.h>
#include <DueTimer.h>
#include <Thread.h>
#include <ThreadController.h>
#include <AccelStepper.h>

#define STEPS_P_REV 1600

#define DIR_PIN 25
// Define pins
#define EN_PIN    23 // LOW: Driver e5nabled. HIGH: Driver disabled
#define STEP_PIN  13 // Step on rising edge

// ThreadController that will controll all threads
ThreadController controll = ThreadController();

//My Thread
Thread myThread = Thread();
//His Thread
Thread heartBeatThread = Thread();

//
AccelStepper stepper(AccelStepper::DRIVER, STEP_PIN, DIR_PIN); // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5/

// callback for hisThread
bool val = LOW;
void heartBeatCB(){
    // Serial.println("Tick");
    val = !val;
    digitalWrite(LED_BUILTIN, val);
}

// This is the callback for the Timer
void timerCallback(){
    stepper.run();
}

void thread_timer_cb(){
	controll.run();
}

// Create CLI Object
SimpleCLI cli;

// Commands
Command sum;



// Callback in case of an error
void errorCallback(cmd_error* e) {
    CommandError cmdError(e); // Create wrapper object

    Serial.print("ERROR: ");
    Serial.println(cmdError.toString());

    if (cmdError.hasCommand()) {
        Serial.print("Did you mean \"");
        Serial.print(cmdError.getCommand().toString());
        Serial.println("\"?");
    }
}

// Callback function for sum command
void move_to_cb(cmd* c) {
    Command cmd(c);               // Create wrapper object

    int argNum = cmd.countArgs(); // Get number of arguments
    int sum    = 0;
    Argument arg      = cmd.getArg(0);
    String   argValue = arg.getValue();
    int argIntValue   = argValue.toFloat();
    stepper.moveTo(argIntValue*STEPS_P_REV);
}

void speed_cb(cmd* c){
    Command cmd(c);               // Create wrapper object
    int argNum = cmd.countArgs(); // Get number of arguments
    int sum    = 0;
    Argument arg      = cmd.getArg(0);
    String   argValue = arg.getValue();
    float argFloatValue   = argValue.toFloat();
    Serial.println(argFloatValue);
    stepper.setMaxSpeed(argFloatValue*STEPS_P_REV);
}


void accel_cb(cmd* c){
    Command cmd(c);               // Create wrapper object
    int argNum = cmd.countArgs(); // Get number of arguments
    int sum    = 0;
    Argument arg      = cmd.getArg(0);
    String   argValue = arg.getValue();
    float argFloatValue   = argValue.toFloat();
    Serial.println(argFloatValue);
    stepper.setAcceleration(argFloatValue*STEPS_P_REV);
}

void setup(){
    Serial.begin(9600);
    Serial.println("Setup..");
    // Prepare pins
    pinMode(EN_PIN, OUTPUT);
    pinMode(STEP_PIN, OUTPUT);
    pinMode(DIR_PIN, OUTPUT);
    digitalWrite(EN_PIN, LOW);    // Enable driver in hardware
    digitalWrite(DIR_PIN, HIGH);    // Enable driver in hardware

	// Configure stepThread
	heartBeatThread.onRun(heartBeatCB);
	heartBeatThread.setInterval(500);

	// Adds threads to the controller
	controll.add(&heartBeatThread);

    // setup hardware timer
    Timer1.attachInterrupt(timerCallback).start(10); // 0.050ms
    Timer2.attachInterrupt(thread_timer_cb).start(200); // 200us thread timer for periodic, slow tasks
    // Startup stuff
    delay(1000);

    cli.setOnError(errorCallback); // Set error Callback


    // Create the sum command, accepting infinite number of arguments
    // Each argument is seperated by a space
    // For example: sum 1 2 3
    sum = cli.addSingleArgCmd("move", move_to_cb);
    sum = cli.addSingleArgCmd("speed", speed_cb);
    sum = cli.addSingleArgCmd("accel", accel_cb);


    noInterrupts();
    interrupts();


    // stepper
    stepper.setMaxSpeed(0.2*STEPS_P_REV);
    stepper.setAcceleration(4*STEPS_P_REV);

    Serial.println("Done!");
}


bool mode = false;
void loop(){
    if (Serial.available()) {
            // Read out string from the serial monitor
            String input = Serial.readStringUntil('\n');

            cli.parse(input);
            // Serial.print("# ");
            // Serial.println(input);

            // Parse the user input into the CLI
        }
    if (cli.errored()) {
            CommandError cmdError = cli.getError();

            noInterrupts();
            Serial.print("ERROR: ");
            Serial.println(cmdError.toString());

            if (cmdError.hasCommand()) {
                Serial.print("Did you mean \"");
                Serial.print(cmdError.getCommand().toString());
                Serial.println("\"?");
            }
            interrupts();
        }
}
