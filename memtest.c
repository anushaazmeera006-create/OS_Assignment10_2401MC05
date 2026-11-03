#include "types.h"
#include "stat.h"
#include "user.h"

int
main(void)
{
  char *memory;
  int i;

  printf(1, "\nQ1: Physical Memory Allocation Test\n");

  // Display memory statistics before allocation.
  printf(1, "\nBefore allocation:\n");
  memstats();

  // Request five pages from the process address space.
  printf(1, "\nAllocating 5 pages...\n");

  memory = sbrk(5 * 4096);

  if(memory == (char*)-1){
    printf(1, "Memory allocation failed\n");
    exit();
  }

  // Access each page to force physical allocation.
  for(i = 0; i < 5; i++)
    memory[i * 4096] = 'A';

  // Display statistics after allocation.
  printf(1, "\nAfter allocation:\n");
  memstats();

  // Release the five pages.
  printf(1, "\nReleasing 5 pages...\n");

  if(sbrk(-5 * 4096) == (char*)-1){
    printf(1, "Memory release failed\n");
    exit();
  }

  // Display statistics after releasing memory.
  printf(1, "\nAfter releasing memory:\n");
  memstats();

  exit();
}
