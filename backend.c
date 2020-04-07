#include <ncurses.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>


void listAllProcessesDirectory()
{
    DIR *pDir;
    struct dirent *entry;
    
    char directoryName[50] = "/proc";
    pDir = opendir(directoryName);
  
    while((entry = readdir(pDir)) != NULL)
    {
      if(isdigit(entry->d_name[0]))
      {
        for (int i = 0; i < strlen(entry->d_name); i++)
        {
          if(isdigit(entry->d_name[i]))
            printf("%c", entry->d_name[i]);
        }
        printf("\n");
      }
    }
}
