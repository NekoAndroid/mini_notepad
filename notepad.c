/* Mini notepad, a no fuss, single header notepad written in C
MIT License

Copyright (c) 2025 NekoAndroid

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined _WIN32 || defined _WIN64
#define CLEAR_SCREEN "cls"
#else
#define CLEAR_SCREEN "clear"
#endif

const int BUFFER_LIMIT = 65536; // Limit of characters in file
const int MINI_BUFFER_LIMIT = 2048; //Max number of chars in a single line (that's two kibibytes)
const int FILENAME_LIMIT = 256;
//Declare function prototypes just for good practice
int clear_screen(void);
int load_file(char *target_pointer); //Same as save_file(), but loads to target_pointer instead.
int save_file(char *input_pointer); //Saves input_pointer's contents to a file name (or path) specified by the user
int clean_string(char *target_string); //Removes the newline character of *target_string, then returns.

// End function prototypes and begin main()
int main(void) {
    // Begin declaration of buffers:
    char *text_buffer = calloc(BUFFER_LIMIT, sizeof(char));
    char *input_buffer = calloc(MINI_BUFFER_LIMIT, sizeof(char));
    // End declaration, begin error checking:
    if (text_buffer == NULL) {
        fprintf(stderr, "\nErr: Failed to allocate \"text_buffer\"\n64KiB, heap, main");
        free(input_buffer);
        return EXIT_FAILURE;
    }

    if (input_buffer == NULL) {
        fprintf(stderr, "\nErr: Failed to allocate \"input_buffer\"\n2KiB, heap, main");
        free(text_buffer);
        return EXIT_FAILURE;
    }

    // If pointers doesn't point to NULL, proceed with execution
    clear_screen();
    printf("Ready.\n") //Used so the user can know that the program's ready to take inputs
    while (1) {
        fgets(input_buffer, MINI_BUFFER_LIMIT, stdin);
        if (strncmp(input_buffer, "</exit>", 7) == 0) {
            // Check for known commands
            break; // Break out of the loop and begin cleanup
        }

        if (strncmp(input_buffer, "</load>", 7) == 0) {
            clear_screen();
            load_file(text_buffer);
        }

        if (strncmp(input_buffer, "</save>", 7) == 0) {
            clear_screen();
            save_file(text_buffer);
        }
        else {
            strcat(text_buffer, input_buffer); //If no command is idenified, interpret as input and save it to the text buffer
        }
    }
    //Begin clean up if "</exit>" is executed...
    free(text_buffer);
    free(input_buffer);
    clear_screen();
    return EXIT_SUCCESS; // Return SUCCESS to the OS (aka 0)
}

// Begin other functions:

int clear_screen(void) {
    system(CLEAR_SCREEN); // Not recommended, I KNOW.
    return EXIT_SUCCESS;
}

int load_file(char *target_pointer) {
    // Loads a file with the name of target_filename. Returns 1 if it fails.
    char *target_filename = malloc(FILENAME_LIMIT * sizeof(char));
    if (target_filename == NULL) {
        fprintf(stderr, "\nErr: Failed to allocate \"target_filename\"\n256B, heap, load_file");
        return EXIT_FAILURE; //Return failure. (not like it's gonna be checked anyway... Oh well.)
    }
    printf("Load:");
    fgets(target_filename, FILENAME_LIMIT, stdin);
    clean_string(target_filename); // Strip newline so it doesn't kill the program

    FILE *load_ptr;
    load_ptr = fopen(target_filename, "r"); 
    /*Attempt to open a file, then check for errors. If it's ok, proceed and read
    it's contents */
    if (load_ptr == NULL) {
        // Check for failure at fopen()
        fprintf(stderr, "Err: Unable to open file.\nFile doesn't exist.");
        free(target_filename); //Free it so it doesn't leak
        return EXIT_FAILURE;
    }

    // Proceed with loading file, and free target_filename
    free(target_filename);
    fread(target_pointer, sizeof(char), BUFFER_LIMIT, load_ptr); // Read the file
    fclose(load_ptr); // Close file (or stream). We don't need it anymore anyway.
    printf("%s", target_pointer); // Print contents so the content can ACTUALLY BE READ BY THE USER
    return EXIT_SUCCESS; // Return. Self-explanatory
}

int save_file(char *input_pointer) {
    char *target_filename = malloc(FILENAME_LIMIT * sizeof(char));
    if (target_filename == NULL) {
        fprintf(stderr, "\nErr: Failed to allocate \"target_filename\"\n256B, heap, save_file");
        return EXIT_FAILURE;
    }

    printf("File name to save: ");
    fgets(target_filename, FILENAME_LIMIT, stdin);
    clean_string(target_filename);

    FILE *save_pointer = fopen(target_filename, "w");
    if (save_pointer == NULL) {
        fprintf(stderr, "\nErr: Unable to open file.\n\"save_ptr\" points to NULL\n");
        free(target_filename);
        return EXIT_FAILURE;
    }

    size_t written = fwrite(input_pointer, sizeof(char), strlen(input_pointer), save_pointer);
    if (written < strlen(input_pointer)) {
        fprintf(stderr, "\nErr: Failed to write to %s.\n", target_filename);
        free(target_filename);
        fclose(save_pointer);
        return EXIT_FAILURE;
    }

    free(target_filename);
    fclose(save_pointer);
    printf("File saved successfully.\n");
    return EXIT_SUCCESS;
}

int clean_string(char *target_string) {
    for (int counter = 0; target[counter] != '\0'; counter++) {
        if (target[counter] == '\n') {
            target[counter] = '\0';
            break;
        }
    }
    // After reaching what we want, return
    // Btw, the return will be instant on an empty string
    return EXIT_SUCCESS;
}
