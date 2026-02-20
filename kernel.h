#pragma once

struct sbiret {
  long error;
  long value;
};

#define SBI_SUCCESS           0
#define SBI_ERR_FAILED        -1
#define SBI_ECALL_0_1_PUTCHAR 0x01
#define SBI_ECALL_0_1_GETCHAR 0x02
