#include "Utilities.h"
#include <shobjidl_core.h>

// Opens a windows file open dialog based on https://learn.microsoft.com/en-us/windows/win32/shell/common-file-dialog
std::string Utilities::OpenFileDialog() {
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