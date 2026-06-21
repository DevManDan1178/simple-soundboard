#include <nfd.h>
#include <stdio.h>
#include <filesystem>  
#include <iostream>
#include "file_dialog.hpp"

const nfdu8filteritem_t ACCEPTED_FILE_TYPES[1] = {
    { "Audio files", "mp3,wav,flac,aac,ogg,m4a" },
};

// Sampled (and slightly modified) from the original github repository [https://github.com/btzy/nativefiledialog-extended]
std::filesystem::path FileDialog::OpenAudioFile() {     
    NFD_Init();

    
    std::filesystem::path openedPath;

    nfdopendialogu8args_t args = {0};
    args.filterList = ACCEPTED_FILE_TYPES;
    args.filterCount = std::size(ACCEPTED_FILE_TYPES);

    nfdu8char_t *outPath;
    nfdresult_t result = NFD_OpenDialogU8_With(&outPath, &args);

    if (result == NFD_OKAY)
    {
        openedPath = std::filesystem::path(outPath);

        std::cout << "File obtained > Path: " << outPath << "\n File name: " << openedPath.filename().string() << std::endl;
        NFD_FreePathU8(outPath);
    }
    else if (result == NFD_CANCEL)
    {
        std::cout << "No file obtained - user pressed cancel." << std::endl;
    }
    else 
    {
        std::printf("No file obtained - error: %s\n", NFD_GetError());
    }

    NFD_Quit();
    return openedPath;
}
