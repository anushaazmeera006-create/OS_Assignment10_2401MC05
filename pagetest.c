#include "types.h"
#include "stat.h"
#include "user.h"

int
main(void)
{
  char *p;

  printf(1, "\nQ3: Page Table Inspection and Protection Test\n");

  // Allocate one page.
  p = sbrk(4096);

  if(p == (char*)-1){
    printf(1, "Memory allocation failed\n");
    exit();
  }

  // Touch the page to ensure it is physically allocated.
  p[0] = 'A';

  printf(1, "\nBefore changing permissions:\n");
  pageinfo((uint)p);

  printf(1, "\nChanging page to read-only...\n");

  if(setreadonly((uint)p) < 0){
    printf(1, "Failed to set page read-only\n");
    exit();
  }

  printf(1, "\nAfter changing permissions:\n");
  pageinfo((uint)p);

  printf(1, "\nQ3 test completed successfully.\n");

  exit();
}

