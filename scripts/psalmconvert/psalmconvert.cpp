//
// vim: ts=4 et
// text splitter utility for VGA display
// Copyright (c) 2001 Petr Vanek, petr@fotoventus.cz
//
/// @file   psalmconvert.cpp
/// @author Petr Vanek
///

#include <stdio.h>
#include <string.h>


// presets VGA - TXT
#define CHARS_PER_LINE      14//12
#define MAX_ROWS_PER_PAGE   7 //8
#define MAX_CHARS_PER_PAGE  (CHARS_PER_LINE * MAX_ROWS_PER_PAGE)
#define MAX_CHARS_BUFFER    2048
#define MAX_PATH 280


bool isDelim(unsigned char inchar)
{
    if (inchar == '.'
        || inchar == '!'
        || inchar == ' '
        || inchar == ','
        ||inchar == ';'
        ||inchar == '?') return true;
    return false;
}

void trimBegin(char *line)
{
    size_t len =  strlen(line);

    while(len > 1) {
        if (isDelim(line[0]) == true) {
            memcpy(&line[0],&line[1], len); //shift leftwith last zero
        } else {
            break;
        }
        len =  strlen(line);
    }


}

size_t countWordChars(char *restLine)
{
    size_t size = 0;
    size_t len = strlen(restLine);
    for (size_t i = 0; i < len; i++) {
        if (isDelim(restLine[i]) == false) {
            size ++;
        } else {
            break;
        }
    }
    return size;
}

void fillUpSpace(size_t index, char * line)
{
    //fill up spaces
    for (size_t c = index; c < (CHARS_PER_LINE + 1); c++) {
        line [c] = ' ';
    }
}

void stringFormater(char *line)
{
   
    size_t len = 0;
    char destination [MAX_CHARS_BUFFER];
    size_t rowPosition = 0;
    size_t index = 0;
    int rest = 0;
    char row [10 * MAX_ROWS_PER_PAGE] [MAX_CHARS_BUFFER];

    memset(destination,0,sizeof(destination));

    for (size_t i = 0; i < MAX_ROWS_PER_PAGE; i++)
    {
        memset(row[i],0,MAX_CHARS_BUFFER);
    }

    trimBegin(line);
    len = strlen(line);

    memset(row,0,sizeof(row));
    rest = CHARS_PER_LINE;
    for (size_t i = 0; i < len; i++) {
        if (isDelim(line[i]) == false) {
            if (rest > 0) {
                row [rowPosition] [index] = line[i];
                rest --;
            }
            index ++;
        } else {
            if ((i+1) < len) {
                if (rest > countWordChars(&line[i+1])) {

                    if (rest == CHARS_PER_LINE && isDelim(line[i])==true) {
                        continue;
                    }
                    

                    row [rowPosition] [index] = line[i];
                    rest --;
                    index ++;
                    continue;
                } else {
                  fillUpSpace(index, row [rowPosition] );
                  rowPosition ++;
                  index = 0;
                  rest = CHARS_PER_LINE;
                  continue;
                }
            }
        }

    }


    for (size_t i = 0; i < MAX_ROWS_PER_PAGE; i++)
    {
        if (strlen(row[i]) < CHARS_PER_LINE)  fillUpSpace(strlen(row[i]), row[i]) ;
        else row[i] [CHARS_PER_LINE]= '\0';
    }


    for (size_t i = 0; i < MAX_ROWS_PER_PAGE; i++)
    {
        if (i == 0) strcpy(destination,row[i]);
        else strcat(destination,row[i]);
    }

    /*
    //finalize  trim
    for (size_t i = 0; i < len; i++) {
        int ab = destcnt % (CHARS_PER_LINE + 1);
        if (ab == 0) {
            if (isDelim(line[i]) == true ) continue;
        }
        destination[destcnt] = line[i];
        destcnt ++;
    }
    */

    //final cut
    len =  strlen(destination);
    if (len > MAX_CHARS_PER_PAGE) {
        destination[MAX_CHARS_PER_PAGE] = '\0';
        len = MAX_CHARS_PER_PAGE - 1;
    }

    //remove spaces on the end
    for (size_t p =len; p > 0; p--  ) {
       if (destination[p] == ' ')  {
         destination[p] = '\0';
       } else if (destination[p] == '\r') {
         destination[p] = '\0';
       } else if (destination[p] == '\n') {
         destination[p] = '\0';
       } else {
         break;
       }
    }

    strcpy(line,destination);
}

void writeLine(const  char* pszDir, int counter, const char *szLine)
{

    FILE *       hFile = NULL;
    char file[MAX_PATH];
    char path[MAX_PATH+1];
    memset(path,0,sizeof(path));
    strncpy(path,pszDir,MAX_PATH);
    if (pszDir[strlen(pszDir)-1] != '/') strncat(path,"/",MAX_PATH);
    sprintf(file,"%03d.txt",counter);
    strncat(path,file,MAX_PATH);

    hFile = fopen(path, "wt");
    if (hFile) {
        fputs(szLine,hFile);
    }

    if (hFile) fclose(hFile);
    hFile = NULL;
}

int main (int argc, const char * argv[])
{
    FILE *hFile = NULL;
    int  counter  = 1;
    char szLine[MAX_CHARS_BUFFER];    //number chars per line
    const char*      pszFile = NULL;
    const char*      pszDir  = NULL;

    do {
        printf("---file convertor---\n");
        printf("Note: ASCII only! Central European (Windows Latin2) encding required !\n");
        
        if (argc != 3) {
            printf("invalid params, required: \n\npsalmconvert <input_file.txt>  <output_directory_path>   \n");
            break;
        }
        
        pszFile = argv[1];
        pszDir = argv[2];
        
        hFile = fopen(pszFile, "rt");
        if (!hFile) {
            printf("invalid path\n");
            break;
        }
        
        while (!feof(hFile)) {
            memset(szLine,0,sizeof(szLine));
            if (fgets(szLine, sizeof(szLine), hFile) == NULL) {
                if (counter == 0) printf("read fault\n");
                else printf("finished\n");
                break;
            }
            stringFormater(szLine);
            writeLine(pszDir, counter,szLine);
            counter++;
        }
    } while (0);
    if (hFile) fclose(hFile);
    hFile = NULL;
    return 0;
}

