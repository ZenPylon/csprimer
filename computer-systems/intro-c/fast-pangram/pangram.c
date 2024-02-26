#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define NUM_CHARS 26

int a_int = (int)'a';

bool ispangram(char *s)
{
  char *c = s;
  bool char_usage_map[NUM_CHARS] = {0};
  while (*c)
  {
    // printf("%c", *c);
    int char_index = tolower((int)*c) - a_int;
    if (char_index >= 0 && char_index < NUM_CHARS)
      char_usage_map[char_index] = true;

    c++;
  }

  for (int i = 0; i < NUM_CHARS; i++)
  {
    if (!char_usage_map[i])
      return false;
  }
  return true;
}

int main()
{
  size_t len;
  ssize_t read;
  char *line = NULL;
  while ((read = getline(&line, &len, stdin)) != -1)
  {
    if (ispangram(line))
      printf("%s", line);
  }

  if (ferror(stdin))
    fprintf(stderr, "Error reading from stdin");

  free(line);
  fprintf(stderr, "ok\n");
}
