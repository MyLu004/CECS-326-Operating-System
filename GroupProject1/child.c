/*
 * child.c
 *
 * This program reads the contents of the pipe and writes it to a file.
 */

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define BUFFER_SIZE 10 // Size of buffer for reading from the pipe

int main(int argc, char *argv[])
{
    // Ensure that the destination file is passed as an argument
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <destination file>\n", argv[0]);
        return 1;
    }

    // Open the destination file for writing
    FILE *destFile = fopen(argv[1], "wb");
    if (destFile == NULL)
    {
        fprintf(stderr, "Error: Could not open destination file.\n");
        return 1;
    }

    // Buffer to store data read from the pipe
    char buffer[BUFFER_SIZE];
    DWORD bytesRead;

    // Read from the pipe (stdin, which is redirected from the parent process)
    while (1)
    {
        // Read from the standard input (pipe)
        if (!ReadFile(GetStdHandle(STD_INPUT_HANDLE), buffer, BUFFER_SIZE, &bytesRead, NULL))
        {
            fprintf(stderr, "Error: Failed to read from pipe.\n");
            fclose(destFile);
            return 1;
        }

        // If no more data (EOF)
        if (bytesRead == 0)
        {
            break;
        }

        // Write the data read from the pipe to the destination file
        size_t bytesWritten = fwrite(buffer, 1, bytesRead, destFile);
        if (bytesWritten != bytesRead)
        {
            fprintf(stderr, "Error: Failed to write to destination file.\n");
            fclose(destFile);
            return 1;
        }
    }

    // Close the destination file after copying is done
    fclose(destFile);

    printf("Child: File copied successfully to %s.\n", argv[1]);
    return 0;
}
