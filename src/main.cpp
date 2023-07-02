#include <Arduino.h>
#include <TMC2208Stepper.h>
#include <SimpleCLI.h>

#define LED_PIN 13
#define DIR_PIN 25
// Define pins
#define EN_PIN    23 // LOW: Driver e5nabled. HIGH: Driver disabled
#define STEP_PIN  13 // Step on rising edge
TMC2208Stepper driver = TMC2208Stepper(&Serial1);  // Create driver and use

#define LINE_BUFF_LEN 30
#define MAX_ARG_LEN 10 // maximum length in chars of a given arg
#define MAX_ARG_CNT 4  // maximum number of characters

class CLI
{
    public:
        CLI(UARTClass* Ser){
            ser = Ser;
        };
        /*
        ** Start parsing and handling terminal ouput.
         */
        void start(){

        };
        /* Stop parsing and handling terminal ouput. */
        void stop(){

        };
        /* Enable/disable echo */
        void do_echo(bool val){
            doEcho = val;
        };
        // void (*onTerminal)
        char line_buff[LINE_BUFF_LEN];
        /*
        ** Call this funtion every loop to handle incomming terminal input.
         */
        void tick(){
        };
        UARTClass* ser;
        void default_handler(char* arg){
            for(uint8_t i = 0; i<3; i++){
                ser->print(arg[i]);
                if (arg[i] == '\n'){
                    break;
                }
            }
        };

    private:
        char *argv[MAX_ARG_CNT]; // array of cstring pointers to each argument
        bool enable = false;  // flag to enable parsing and handling of terminal output.
        bool doEcho = false; // echo out incomming data to the user, creating a "terminal"
        enum class error_codes {
            SUCCESS = 0, // succeeded
            PROG_FAILURE = 1, // generic program failure
            CLI_USAGE = 2
        };
        void error(error_codes code){
            char err_buff[30];
            sprintf(err_buff, "ERR: %u\n", code);
            ser->print(err_buff);
        }
        int argc; // argument count
        /* Parse a single line of space deliminated terminal ouput and populate argc and argv. */
        int parseLn(char ln[]){
            char c;
            argc = 0;
            uint8_t arg_len = 0;
            char *ptr = strtok(ln, " ");

            while(ptr != NULL)
                {
                    argc++;
                    if (argc >= MAX_ARG_CNT){
                        ser->println("CLI overflow");
                        error(error_codes::CLI_USAGE);
                        return 0;
                    }
                    argv[argc] = ptr;
                    ptr = strtok(NULL, " ");
                }

            return parse_cmd(argc, argv);
        };
        enum class arg_type{
            VOID = 0,
            INT= 1,
            UINT8 = 2,
            UINT16 = 3,
            FLOAT = 4,
            STR = 5,
        };
        struct kwargs{
            char keyword[MAX_ARG_LEN];
            arg_type type = arg_type::VOID;
        };
        struct parse_instructions {
            char args[MAX_ARG_CNT];
            char kw_args[MAX_ARG_CNT];
        };
        int parse_cmd(uint8_t argc, char *argv[]){
            //
            for (uint8_t i = 0; i<argc; i++){ // loop through each argument
                // each command has it's own parse instructions

            }

        };

};

void setup(){
    pinMode(EN_PIN, OUTPUT);
    digitalWrite(EN_PIN, LOW);    // Enable driver in hardware
    Serial.begin(9600);
    Serial.println("Start...");
    // Prepare pins
    pinMode(STEP_PIN, OUTPUT);
    pinMode(LED_PIN, OUTPUT);
    pinMode(DIR_PIN, OUTPUT);
    digitalWrite(DIR_PIN, HIGH);    // Enable driver in hardware
    auto cli = CLI(&Serial);

}


bool mode = false;
void loop(){
    mode = !mode;
    delayMicroseconds(80);
    digitalWrite(STEP_PIN, !digitalRead(STEP_PIN));
    digitalWrite(LED_PIN, mode);
}


