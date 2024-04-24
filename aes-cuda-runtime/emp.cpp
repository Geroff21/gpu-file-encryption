#include "functions.h"

int main() {

    char* inputFILE = "input.txt"; //�������� ����
    char* encryptFILE = "encrypt.bin"; //������������� ����
    char* decryptFILE = "decrypt.txt"; //�������������� ����

    int blocks_number; //����� ������
    int incomplete_blocks_number; //����� �������� ������  
    AES_block* inputBlocks = new AES_block; // �����

    /* ----- ���� ----- */

    //���� ��������� ���� �� �����, �� ������������� ������� getKey() ��� �� ������ ����������� ����� ���������
    char* keyLine = "0123456789abcdef";

    /* ----- ���������� ----- */

    readBlocksFromFile(inputFILE, inputBlocks, blocks_number, incomplete_blocks_number);
    AES_block* encryptedBlocks = AES_Encrypt(keyLine, inputBlocks, blocks_number, incomplete_blocks_number);
    writeBlocksToFile(encryptFILE, encryptedBlocks, blocks_number, incomplete_blocks_number);
    
    /* ----- ������������� ----- */

    readBlocksFromFile(encryptFILE, inputBlocks, blocks_number, incomplete_blocks_number);
    AES_block* decryptedBlocks = AES_Decrypt(keyLine, inputBlocks, blocks_number, incomplete_blocks_number);
    writeBlocksToFile(decryptFILE, decryptedBlocks, blocks_number, incomplete_blocks_number);

}