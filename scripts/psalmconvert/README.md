# psalmconvert

The old utility converts psalm texts that are stored in the source directory and each line represents one psalm. The utility converts them to a set of files that have a number in the name that is appropriate to the number of the line. Because the display has a fixed grid for displaying text (it does not support proportional font), the utility tries to split the text using spaces so that it fits on the screen and the words are not split between the individual lines. 

The utility is written in C and needs to be translated or the line and column count definitions modified. The default setting is 


#define CHARS_PER_LINE      14
#define MAX_ROWS_PER_PAGE   7 


psalmconvert <input_file.txt>  <output_directory_path>



