#ifndef DEFINITIONS_H
#define DEFINITIONS_H

extern const int M0;
extern const int M1;
extern const int AUX;

extern unsigned long int timeout;

extern bool auxHighFlag;
extern bool auxLowFlag;

// extern const char transmission_success_result[];
// extern const char transmission_timeout_result[];

// extern const char UART_parity_bit_0[];
// extern const char UART_parity_bit_1[];
// extern const char UART_parity_bit_2[];
// extern const char UART_parity_bit_3[];

enum HEAD{
  SAVE_ON_POWER_DOWN = 0xC0,
  DONT_SAVE_ON_POWER_DOWN = 0xC2
};

enum UART_PARITY_BIT{
  UART_PARITY_BIT_8N1 = 0b00,
  UART_PARITY_BIT_8o1 = 0b01,
  UART_PARITY_BIT_8E1 = 0b10,
  UART_PARITY_BIT_8N1_11 = 0b11
};

enum TTL_UART_BAUD_RATE{
  TTL_UART_baud_rate_1200 = 0b000,
  TTL_UART_baud_rate_2400 = 0b001,
  TTL_UART_baud_rate_4800 = 0b010,
  TTL_UART_baud_rate_9600 = 0b011,
  TTL_UART_baud_rate_19200 = 0b100,
  TTL_UART_baud_rate_38400 = 0b101,
  TTL_UART_baud_rate_57600 = 0b110,
  TTL_UART_baud_rate_115200 = 0b111
};

enum AIR_DATA_RATE{
  Air_Data_Rate_300 = 0b000,
  Air_Data_Rate_1200 = 0b001,
  Air_Data_Rate_2400 = 0b010,
  Air_Data_Rate_4800 = 0b011,
  Air_Data_Rate_9600 = 0b100,
  Air_Data_Rate_19200 = 0b101
};

enum FIXED_TRANSMISSION_ENABLING_BIT{
  TRANSPARENT_TRANSMISSION_MODE = 0b0,
  FIXED_TRANSMISSION_MODE = 0b1
};

enum TRANSMISSION_POWER{
  TRANSMISSION_POWER_20dBm = 0b00,
  TRANSMISSION_POWER_17dBm = 0b01,
  TRANSMISSION_POWER_14dBm = 0b10,
  TRANSMISSION_POWER_10dBm = 0b11
};

enum OperationMode{
  NORMAL = 0b00,
  WAKE_UP = 0b01,
  POWER_SAVING = 0b10,
  SLEEP = 0b11
};

// enum TRANSMISSION_RESULT{
//   TRANSMISSION_SUCCESS = transmission_success_result,
//   TRANSMISSION_TIMEOUT = transmission_timeout_result
// }

// extern TTL_UART_baud_rate current_TTL_UART_baud_rate;
// extern AirDataRate current_air_data_rate;
extern OperationMode current_operation_mode;

struct SPED_Struct{
  unsigned UART_parity_bit : 2;
  unsigned TTL_UART_baud_rate : 3;
  unsigned Air_data_rate : 3;
};

union SPED_Param{
  uint8_t byte;
  SPED_Struct bits;
};

struct OPTION_Struct{
  unsigned FTEB : 1;
  unsigned IODM : 1;
  unsigned WWUT : 3;
  unsigned FECS : 1;
  unsigned TXPW : 2;
};

union OPTION_Param{
  uint8_t OPTION;
  OPTION_Struct bits;
};

struct Configuration_Struct{
  uint8_t HEAD;
  uint8_t ADDH;
  uint8_t ADDL;
  SPED_Param SPED;
  uint8_t CHAN;
  OPTION_Param OPTION;
};

union Configuration{
  uint8_t bytes[6];
  Configuration_Struct parameters;
};

extern Configuration configuration;

#endif
