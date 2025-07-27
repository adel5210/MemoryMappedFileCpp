#include <windows.h>
#include <iostream>

const char* FILE_PATH = "D:/shared.dat";
const size_t FILE_SIZE = sizeof(long);

int main()
{
    HANDLE hFile = CreateFileA(
        FILE_PATH,
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (INVALID_HANDLE_VALUE == hFile) {
		std::cerr << "Failed to create file: " << GetLastError() << std::endl;  
        return 1;
    }

    HANDLE hMapFile = CreateFileMappingA(
        hFile,
        NULL,
        PAGE_READWRITE,
        0,
        FILE_SIZE,
        NULL
    );

    if (NULL == hMapFile) {
		std::cerr << "Failed to create file mapping: " << GetLastError() << std::endl;
		CloseHandle(hFile);
        return 1;
    }

    long* shared_long = (long*)MapViewOfFile(
        hMapFile, FILE_MAP_ALL_ACCESS, 0,0,FILE_SIZE
    );

    if (NULL == shared_long) {
		std::cerr << "Failed to map view of file: " << GetLastError() << std::endl;
		CloseHandle(hMapFile);  
        CloseHandle(hFile);
		return 1;   
    }

	*shared_long = 42; // Write to the shared memory    
	std::cout << "C++ process wrote: " << *shared_long << std::endl;    
	std::cout << "C++ process is waiting for input..." << std::endl;
	
    getchar(); // Wait for user input

    UnmapViewOfFile(shared_long);
    CloseHandle(hMapFile);
    CloseHandle(hFile);

	std::cout << "C++ process finished." << std::endl;  
	return 0;
}
