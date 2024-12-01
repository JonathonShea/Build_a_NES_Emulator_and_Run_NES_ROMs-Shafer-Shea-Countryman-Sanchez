#include <iostream>
#include "NesRam.h"
#include "Clock.h"
#include <iostream>
#include <fstream>
#include <string> 
#include <shobjidl_core.h>
#include <Cartridge.h>

static constexpr std::array<uint8_t, 4> magicNumbers = { 0x4E, 0x45, 0x53, 0x1A }; // NES<EOF> magic numbers to identify a NES ROM file

std::string OpenFileDialog();

int main(int argc, const char * argv[]){
	Clock clock;
	std::ifstream romFile; 
	std::vector<uint8_t> romData;
	std::string filePath;
    if (argc > 1) {
        filePath = argv[1];
    }
    else{
        filePath = OpenFileDialog();
    }
	romFile.open(filePath, std::ios::binary);
	if (romFile.is_open()) {
		std::cout << "Opened file: " << filePath << std::endl;
		romData = std::vector<uint8_t>(std::istreambuf_iterator<char>(romFile), std::istreambuf_iterator<char>());
	}
	else {
		std::cout << "Failed to open file: " << filePath << std::endl;
		return(0);
	}
	// Check for valid NES ROM file header
	for (int i = 0; i < magicNumbers.size(); i++) {
		if (romData[i] != magicNumbers[i]) {
			std::cout << "Invalid NES ROM file" << std::endl;
			return(0);
		}
	}
	Cartridge cart(romData);
	romFile.close();
  return 0;
}


// Opens a windows file open dialog based on https://learn.microsoft.com/en-us/windows/win32/shell/common-file-dialog
std::string OpenFileDialog() {
    HRESULT hr;
    std::string filePath;
    IFileOpenDialog* pFileOpen;

    // Initialize COM library
    hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    if (SUCCEEDED(hr)) {
        // Create the FileOpenDialog object
        hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));
        if (SUCCEEDED(hr)) {
            // Show the Open dialog box
            hr = pFileOpen->Show(NULL);
            if (SUCCEEDED(hr)) {
                // Get the file name from the dialog box
                IShellItem* pItem;
                hr = pFileOpen->GetResult(&pItem);
                if (SUCCEEDED(hr)) {
                    PWSTR pszFilePath;
                    hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
                    if (SUCCEEDED(hr)) {
                        // Convert the file path to a string
                        std::wstring ws(pszFilePath);
                        filePath = std::string(ws.begin(), ws.end());
                        CoTaskMemFree(pszFilePath);
                    }
                    pItem->Release();
                }
            }
            pFileOpen->Release();
        }
        CoUninitialize();
    }
    return filePath;
}