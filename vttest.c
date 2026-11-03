#include "types.h"
#include "stat.h"
#include "user.h"

int
main(void)
{
  int value;
  int physical_address;
  uint virtual_address;

  value = 100;

  virtual_address = (uint)&value;

  printf(1, "\nQ2: Virtual-to-Physical Address Translation Test\n");

  printf(1, "Variable value: %d\n", value);
  printf(1, "Virtual address: 0x%x\n", virtual_address);

  physical_address = vtop(virtual_address);

  if(physical_address == -1) {
    printf(1, "Translation failed: address is not mapped\n");
  } else {
    printf(1, "Physical address: 0x%x\n", physical_address);
  }

  exit();
}
