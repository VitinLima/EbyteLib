#include "Arduino.h"
#include "definitions.h"

const int M0 = 8;
const int M1 = 7;
const int AUX = 2;

unsigned long int timeout = 200;

bool auxHighFlag = false;
bool auxLowFlag = false;

// extern const char transmission_success_result[] = "Success";
// extern const char transmission_timeout_result[] = "Timeout";

// extern const char UART_parity_bit_0[] = ;
// extern const char UART_parity_bit_1[] = ;
// extern const char UART_parity_bit_2[] = ;
// extern const char UART_parity_bit_3[] = ;

// TTL_UART_baud_rate current_TTL_UART_baud_rate;
// AirDataRate current_air_data_rate;
OperationMode current_operation_mode;

Configuration configuration;