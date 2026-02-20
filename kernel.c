#include "kernel.h"
#include "common.h"

extern char __bss[], __bss_end[], __stack_top[];

struct sbiret
sbi_call(long arg0, long arg1, long arg2, long arg3, long arg4, long arg5, long fid, long eid)
{
  register long a0 __asm__("a0") = arg0;
  register long a1 __asm__("a1") = arg1;
  register long a2 __asm__("a2") = arg2;
  register long a3 __asm__("a3") = arg3;
  register long a4 __asm__("a4") = arg4;
  register long a5 __asm__("a5") = arg5;
  register long a6 __asm__("a6") = fid;
  register long a7 __asm__("a7") = eid;

  __asm__ __volatile__("ecall"
                  : "=r"(a0), "=r"(a1)
                  : "r"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4), "r"(a5), "r"(a6), "r"(a7)
                  : "memory");
  return (struct sbiret) {.error = a0, .value = a1};
}

// Функция 1: putchar - вывод символа в консоль
void
putchar(char ch)
{
  sbi_call(ch, 0, 0, 0, 0, 0, 0, SBI_ECALL_0_1_PUTCHAR);
}

// Функция 2: getchar - получение символа из консоли
int
getchar(void)
{
  struct sbiret ret;
  
  do
  {
    ret = sbi_call(0, 0, 0, 0, 0, 0, 0, SBI_ECALL_0_1_GETCHAR);
  } while (ret.error == SBI_ERR_FAILED);
  
  return (int) ret.error;
}

// Простая функция для демонстрации putchar
void
demo_putchar()
{
  putchar('H');
  putchar('e');
  putchar('l');
  putchar('l');
  putchar('o');
  putchar(' ');
  putchar('w');
  putchar('o');
  putchar('r');
  putchar('l');
  putchar('d');
  putchar('!');
  putchar('\n');
}

// Простая функция для демонстрации getchar
void
demo_getchar()
{
  char buffer[64];
  int i = 0;
  
  printf("\nEnter some text (press Enter to finish): ");
  
  while (i < 63) {
    int c = getchar();
    
    if (c == '\r' || c == '\n') {
      putchar('\n');
      break;
    }
    
    if (c >= 32 && c < 127) {
      putchar(c);  // Echo the character
      buffer[i++] = (char)c;
    }
  }
  
  buffer[i] = '\0';
  
  printf("\nYou entered: ");
  for (int j = 0; j < i; j++) {
    putchar(buffer[j]);
  }
  printf("\n");
}

// Простая функция для тестирования putchar
void
test_putchar()
{
  printf("\n=== Testing putchar function ===\n");
  printf("Output using putchar: ");
  
  // Выводим символы по одному
  putchar('T');
  putchar('e');
  putchar('s');
  putchar('t');
  putchar(' ');
  putchar('1');
  putchar('2');
  putchar('3');
  putchar('!');
  putchar('\n');
  
  // Выводим ASCII таблицу (первые 32 символа)
  printf("\nASCII table (first 32 characters):\n");
  for (int i = 0; i < 32; i++) {
    putchar('[');
    if (i < 10) putchar('0' + i);
    else if (i < 32) {
      putchar('0' + (i/10));
      putchar('0' + (i%10));
    }
    putchar(']');
    putchar(':');
    putchar(' ');
    
    if (i == 0) {
      printf("NUL");
    } else if (i == 7) {
      printf("BEL");
    } else if (i == 8) {
      printf("BS");
    } else if (i == 9) {
      printf("TAB");
    } else if (i == 10) {
      printf("LF");
    } else if (i == 13) {
      printf("CR");
    } else if (i == 27) {
      printf("ESC");
    } else {
      putchar(' ');
      putchar(' ');
      putchar(' ');
    }
    
    printf("\n");
  }
}

// Простая функция для тестирования getchar
void
test_getchar()
{
  printf("\n=== Testing getchar function ===\n");
  
  // Тест 1: Простой ввод символа
  printf("Test 1: Press any key: ");
  int c = getchar();
  printf("\nYou pressed: '");
  putchar(c);
  printf("' (ASCII: %d)\n", c);
  
  // Очистка буфера
  while (getchar() != '\n');
  
  // Тест 2: Ввод строки
  printf("\nTest 2: Enter a word (max 20 chars): ");
  char word[21];
  int i = 0;
  
  while (i < 20) {
    c = getchar();
    if (c == '\r' || c == '\n') {
      putchar('\n');
      break;
    }
    word[i++] = c;
    putchar(c);
  }
  word[i] = '\0';
  
  printf("You entered word: ");
  for (int j = 0; j < i; j++) {
    putchar(word[j]);
  }
  printf("\n");
  
  // Тест 3: Специальные клавиши
  printf("\nTest 3: Try special keys (Ctrl+C to exit test):\n");
  printf("Press keys (ESC to finish this test):\n");
  
  while (1) {
    c = getchar();
    if (c == 27) {  // ESC key
      printf("\nESC pressed - ending test\n");
      break;
    }
    
    printf("Key: ");
    if (c >= 32 && c < 127) {
      putchar('\'');
      putchar(c);
      printf("\' (ASCII: %d)\n", c);
    } else {
      printf("Special (ASCII: %d)\n", c);
    }
    
    if (c == 3) {  // Ctrl+C
      printf("Ctrl+C pressed\n");
    }
  }
}

void
kernel_main(void)
{
  // Clear BSS section
  for(char *p = __bss; p < __bss_end; p++)
  {
    *p = 0;
  }
  
  printf("\n=== Laboratory Work #1 ===\n");
  printf("Principles of I/O Organization without OS\n");
  printf("ITMO University\n\n");
  
  printf("Implementation of putchar and getchar functions\n\n");
  
  // Демонстрация putchar
  demo_putchar();
  
  // Тестирование putchar
  test_putchar();
  
  // Тестирование getchar
  test_getchar();
  
  // Интерактивная демонстрация
  printf("\n=== Interactive Demo ===\n");
  printf("You can now type anything. Press Ctrl+C to exit.\n");
  printf("Type your message: ");
  
  while (1) {
    int ch = getchar();
    if (ch == 3) {  // Ctrl+C
      printf("\nExiting...\n");
      break;
    }
    putchar(ch);  // Echo back
  }
  
  printf("\nProgram completed. Entering infinite loop.\n");
  
  for(;;)
  {
    __asm__ __volatile__("wfi");
  }
}

__attribute__((section(".text.boot")))
__attribute__((naked))
void
boot(void)
{
  __asm__ __volatile__(
          "mv sp, %[stack_top]\n"
          "j kernel_main\n"
          :
          : [stack_top] "r" (__stack_top)
  );
}
