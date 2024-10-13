/*
 * FileCopy.c
 *
 * This program opens a file and writes its contents to a pipe.
 * The child process reads from the pipe and writes to the destination file.
 */

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define BUFFER_SIZE 25
#define STRING_SIZE 25

int main(int argc, char *argv[])
{
   if (argc != 3)
   {
      fprintf(stderr, "Usage: %s <source file> <destination file>\n", argv[0]);
      return 1;
   }

   // File names
   char *sourceFile = argv[1];
   char *destinationFile = argv[2];

   // Pipe handles
   HANDLE hReadPipe, hWritePipe;
   SECURITY_ATTRIBUTES sa = {sizeof(SECURITY_ATTRIBUTES), NULL, TRUE};

   // Create the pipe
   if (!CreatePipe(&hReadPipe, &hWritePipe, &sa, 0))
   {
      fprintf(stderr, "Error: CreatePipe failed.\n");
      return 1;
   }

   // Create the child process (the process that will write to the destination file)
   PROCESS_INFORMATION pi;
   STARTUPINFO si;
   ZeroMemory(&si, sizeof(si));
   si.cb = sizeof(si);
   si.hStdInput = hReadPipe; // Child process will read from the pipe
   si.dwFlags |= STARTF_USESTDHANDLES;

   ZeroMemory(&pi, sizeof(pi));

   // Command to execute the child process (using cmd to redirect input from pipe)
   char command[256];
   snprintf(command, sizeof(command), "child.exe %s", destinationFile);

   // Create the child process
   if (!CreateProcess(NULL, command, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi))
   {
      fprintf(stderr, "Error: CreateProcess failed.\n");
      CloseHandle(hWritePipe);
      CloseHandle(hReadPipe);
      return 1;
   }

   // Close the read end of the pipe (parent will only write)
   CloseHandle(hReadPipe);

   // Open the source file for reading
   FILE *srcFile = fopen(sourceFile, "rb");
   if (srcFile == NULL)
   {
      fprintf(stderr, "Error: Could not open source file.\n");
      CloseHandle(hWritePipe);
      return 1;
   }

   // Read from the source file and write to the pipe
   char buffer[BUFFER_SIZE];
   size_t bytesRead;

   while ((bytesRead = fread(buffer, 1, BUFFER_SIZE, srcFile)) > 0)
   {
      DWORD bytesWritten;
      if (!WriteFile(hWritePipe, buffer, bytesRead, &bytesWritten, NULL) || bytesWritten != bytesRead)
      {
         fprintf(stderr, "Error: Write to pipe failed.\n");
         fclose(srcFile);
         CloseHandle(hWritePipe);
         return 1;
      }
   }

   // Close the write end of the pipe after writing the data
   CloseHandle(hWritePipe);

   // Close the source file
   fclose(srcFile);

   // Wait for the child process to finish
   WaitForSingleObject(pi.hProcess, INFINITE);

   // Close process and thread handles
   CloseHandle(pi.hProcess);
   CloseHandle(pi.hThread);

   printf("File copy completed successfully.\n");
   return 0;
}
