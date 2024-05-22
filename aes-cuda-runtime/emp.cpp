#include "functions.h"
#include "md5.h"
#include "md5.cpp"

namespace fs = std::filesystem;

MD5 md5;


//������ � ������� ������
std::vector<std::string> getFileNames(char* pathToFiles) {

    std::vector<std::string> fileNames;

    for (const auto& entry : fs::directory_iterator(pathToFiles)) {

        std::string inputFILE = entry.path().generic_string();
        std::cout << inputFILE << std::endl;
        fileNames.push_back(inputFILE);

    }

    return fileNames;
}

//������ �� ����� �������
std::vector<std::string > getKeys(char* pathToKeyFile) {

    std::vector<std::string> allKeys;
    std::string key;
    std::ifstream file(pathToKeyFile);

    while (getline(file, key)) {
        allKeys.push_back(key);
    }

    file.close();

    return allKeys;
}

//�������� ����� inputFILE, ��� ���������� �� ����� keyLine, ������ � ���� encryptFILE
std::string process_ENC(char* inputFILE, char* encryptFILE, char* keyLine) {

    int blocks_number = 0; //����� ������
    int incomplete_blocks_number = 0; //����� �������� ������  
    AES_block* inputBlocks = new AES_block; // �����
    BYTE iv[16]; //������ �������������

    /* ----- ���������� ----- */

    readBlocksFromFile(inputFILE, inputBlocks, blocks_number, incomplete_blocks_number);

    std::string hash = md5(inputBlocks, blocks_number);

    AES_block* encryptedBlocks = AES_Encrypt(keyLine, inputBlocks, blocks_number, iv, incomplete_blocks_number);
    writeBlocksToFile(encryptFILE, encryptedBlocks, blocks_number, incomplete_blocks_number);

    return hash;
}

//�������� ����� encryptFILE, ��� ������������� �� ����� keyLine, ������ � ���� decryptFILE
std::string process_DEC(char* encryptFILE, char* decryptFILE, char* keyLine, std::vector<std::string> hashes) {

    int blocks_number = 0; //����� ������
    int incomplete_blocks_number = 0; //����� �������� ������  
    AES_block* inputBlocks = new AES_block; // �����
    BYTE iv[16]; //������ �������������

    /* ----- ������������� ----- */

    readBlocksFromFile(encryptFILE, inputBlocks, blocks_number, incomplete_blocks_number);
    AES_block* decryptedBlocks = AES_Decrypt(keyLine, inputBlocks, blocks_number, iv, incomplete_blocks_number);

    std::string hash = md5(decryptedBlocks, blocks_number);

    //���� ������� �������� 
    if (std::find(hashes.begin(), hashes.end(), hash) != hashes.end()) {
        writeBlocksToFile(decryptFILE, decryptedBlocks, blocks_number, incomplete_blocks_number);
        return hash;
    }
    else {
        return "badhash";
    }
        
}

void shuffleKeys(std::vector<std::string> &keys) {
    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(keys.begin(), keys.end(), g);
}

int main() {

    //���������

    char* pathToFiles = "files";
    char* pathToKeyFile = "keys.txt";

    std::vector<std::string> keys = getKeys(pathToKeyFile);
    std::vector<std::string> fileNames = getFileNames(pathToFiles);
    std::vector<std::string> fileHashes;

    //������
    shuffleKeys(keys); 

    //������� ��� �����.
    for (int i = keys.size() - 1; i >= 0; --i) {

        std::string inputFilePath = fileNames[i]; //�������� ����

        std::string encFilePath = fileNames[i]; //����������� ����
        encFilePath.erase(0, encFilePath.find_first_of("/"));
        encFilePath.erase(encFilePath.find_last_of("."), encFilePath.size() - 1);
        encFilePath = "encrypted" + encFilePath + ".bin";

        char* inputFILE = inputFilePath.data();
        char* encryptFILE = encFilePath.data();

        char* keyLine = reinterpret_cast<char*>(keys[i].data());

        std::string hash_INP = process_ENC(inputFILE, encryptFILE, keyLine);
        std::cout << i << ") HASH_INP: " << hash_INP << std::endl;

        fileHashes.push_back(hash_INP);

    }

    //������
    shuffleKeys(keys);

    //�������� ������������
    for (int i = keys.size() - 1; i >= 0; --i) {

        std::string encFilePath = fileNames[i]; //����������� ����
        encFilePath.erase(0, encFilePath.find_first_of("/"));
        encFilePath.erase(encFilePath.find_last_of("."), encFilePath.size() - 1);
        encFilePath = "encrypted" + encFilePath + ".bin";

        std::string decFilePath = fileNames[i]; //������������� ����
        decFilePath.erase(0, decFilePath.find_first_of("/"));
        decFilePath = "decrypted" + decFilePath;

        char* encryptFILE = encFilePath.data();
        char* decryptFILE = decFilePath.data();

        for (int j = keys.size() - 1; j >= 0; --j) {

            char* keyLine = reinterpret_cast<char*>(keys[j].data());

            std::string hash_DEC = process_DEC(encryptFILE, decryptFILE, keyLine, fileHashes);

            if (hash_DEC != "badhash") {
                std::cout << j << ") HASH_DEC: " << hash_DEC << std::endl;
            }

        }

    }

}
