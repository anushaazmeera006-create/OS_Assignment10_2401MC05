
#include "types.h"
#include "stat.h"
#include "user.h"

#define MAXREF 20
#define MAXFRAMES 5

int pages[MAXREF] = {
  7, 0, 1, 2, 0, 3, 0, 4, 2, 3,
  0, 3, 2, 1, 2, 0, 1, 7, 0, 1
};

struct Result {
  int faults;
  int hits;
};

void
displayframes(int frames[], int n)
{
  int i;

  printf(1, "[ ");
  for(i = 0; i < n; i++) {
    if(frames[i] == -1)
      printf(1, "- ");
    else
      printf(1, "%d ", frames[i]);
  }
  printf(1, "]");
}

int
findpage(int frames[], int n, int page)
{
  int i;

  for(i = 0; i < n; i++) {
    if(frames[i] == page)
      return i;
  }
  return -1;
}

void
initializeframes(int frames[], int n)
{
  int i;

  for(i = 0; i < n; i++)
    frames[i] = -1;
}

struct Result
fifo(int n, int verbose)
{
  int frames[MAXFRAMES];
  int next = 0;
  int i, pos;
  struct Result result;

  result.faults = 0;
  result.hits = 0;
  initializeframes(frames, n);

  if(verbose)
    printf(1, "\n========== FIFO (%d FRAMES) ==========\n", n);

  for(i = 0; i < MAXREF; i++) {
    pos = findpage(frames, n, pages[i]);

    if(pos != -1) {
      result.hits++;
      if(verbose) {
        printf(1, "Page %d: ", pages[i]);
        displayframes(frames, n);
        printf(1, " HIT\n");
      }
    } else {
      result.faults++;
      frames[next] = pages[i];
      next = (next + 1) % n;

      if(verbose) {
        printf(1, "Page %d: ", pages[i]);
        displayframes(frames, n);
        printf(1, " FAULT\n");
      }
    }
  }

  if(verbose) {
    printf(1, "FIFO Total Faults: %d\n", result.faults);
    printf(1, "FIFO Total Hits: %d\n", result.hits);
  }

  return result;
}

struct Result
lru(int n, int verbose)
{
  int frames[MAXFRAMES];
  int lastused[MAXFRAMES];
  int i, j, pos, victim, oldest;
  struct Result result;

  result.faults = 0;
  result.hits = 0;
  initializeframes(frames, n);

  for(i = 0; i < n; i++)
    lastused[i] = -1;

  if(verbose)
    printf(1, "\n========== LRU (%d FRAMES) ==========\n", n);

  for(i = 0; i < MAXREF; i++) {
    pos = findpage(frames, n, pages[i]);

    if(pos != -1) {
      result.hits++;
      lastused[pos] = i;

      if(verbose) {
        printf(1, "Page %d: ", pages[i]);
        displayframes(frames, n);
        printf(1, " HIT\n");
      }
    } else {
      result.faults++;

      victim = -1;

      for(j = 0; j < n; j++) {
        if(frames[j] == -1) {
          victim = j;
          break;
        }
      }

      if(victim == -1) {
        oldest = lastused[0];
        victim = 0;

        for(j = 1; j < n; j++) {
          if(lastused[j] < oldest) {
            oldest = lastused[j];
            victim = j;
          }
        }
      }

      frames[victim] = pages[i];
      lastused[victim] = i;

      if(verbose) {
        printf(1, "Page %d: ", pages[i]);
        displayframes(frames, n);
        printf(1, " FAULT\n");
      }
    }
  }

  if(verbose) {
    printf(1, "LRU Total Faults: %d\n", result.faults);
    printf(1, "LRU Total Hits: %d\n", result.hits);
  }

  return result;
}

int
optimalvictim(int frames[], int n, int current)
{
  int i, j;
  int farthest = -1;
  int victim = 0;

  for(i = 0; i < n; i++) {
    for(j = current + 1; j < MAXREF; j++) {
      if(frames[i] == pages[j])
        break;
    }

    if(j == MAXREF)
      return i;

    if(j > farthest) {
      farthest = j;
      victim = i;
    }
  }

  return victim;
}

struct Result
optimal(int n, int verbose)
{
  int frames[MAXFRAMES];
  int i, pos, victim;
  struct Result result;

  result.faults = 0;
  result.hits = 0;
  initializeframes(frames, n);

  if(verbose)
    printf(1, "\n========== OPTIMAL (%d FRAMES) ==========\n", n);

  for(i = 0; i < MAXREF; i++) {
    pos = findpage(frames, n, pages[i]);

    if(pos != -1) {
      result.hits++;

      if(verbose) {
        printf(1, "Page %d: ", pages[i]);
        displayframes(frames, n);
        printf(1, " HIT\n");
      }
    } else {
      result.faults++;

      victim = -1;

      for(pos = 0; pos < n; pos++) {
        if(frames[pos] == -1) {
          victim = pos;
          break;
        }
      }

      if(victim == -1)
        victim = optimalvictim(frames, n, i);

      frames[victim] = pages[i];

      if(verbose) {
        printf(1, "Page %d: ", pages[i]);
        displayframes(frames, n);
        printf(1, " FAULT\n");
      }
    }
  }

  if(verbose) {
    printf(1, "Optimal Total Faults: %d\n", result.faults);
    printf(1, "Optimal Total Hits: %d\n", result.hits);
  }

  return result;
}

void
comparison(void)
{
  int n;
  struct Result f, l, o;

  printf(1, "\n========== PAGE REPLACEMENT COMPARISON ==========\n");
  printf(1, "Frames\tAlgorithm\tFaults\tHits\tHit Ratio\tFault Ratio\n");

  for(n = 3; n <= 5; n++) {
    f = fifo(n, 0);
    l = lru(n, 0);
    o = optimal(n, 0);

    printf(1, "%d\tFIFO\t\t%d\t%d\t%d/20\t\t%d/20\n",
           n, f.faults, f.hits, f.hits, f.faults);

    printf(1, "%d\tLRU\t\t%d\t%d\t%d/20\t\t%d/20\n",
           n, l.faults, l.hits, l.hits, l.faults);

    printf(1, "%d\tOptimal\t\t%d\t%d\t%d/20\t\t%d/20\n",
           n, o.faults, o.hits, o.hits, o.faults);
  }
}

int
main(void)
{
  printf(1, "\nQ4: Page Replacement Algorithms\n");
  printf(1, "Reference String: ");

  for(int i = 0; i < MAXREF; i++)
    printf(1, "%d ", pages[i]);

  printf(1, "\n");

  fifo(3, 1);
  lru(3, 1);
  optimal(3, 1);

  comparison();

  exit();
}
