#include "functions.h"

int main() {

    char* inputFILE = "files/input.txt"; //�������� ����
    char* encryptFILE = "files/encrypt.bin"; //������������� ����
    char* decryptFILE = "files/decrypt.txt"; //�������������� ����

    int blocks_number = 0; //����� ������
    int incomplete_blocks_number = 0; //����� �������� ������  
    AES_block* inputBlocks = new AES_block; // �����
    BYTE iv[16]; //������ �������������

    /* ----- ���� ----- */

    //���� ��������� ���� �� �����, �� ������������� ������� getKey() ��� �� ������ ����������� ����� ���������
    char* keyLine = "0123456789abcdef";

    /* ----- ���������� ----- */

    readBlocksFromFile(inputFILE, inputBlocks, blocks_number, incomplete_blocks_number);
    AES_block* encryptedBlocks = AES_Encrypt(keyLine, inputBlocks, blocks_number, iv,incomplete_blocks_number);
    writeBlocksToFile(encryptFILE, encryptedBlocks, blocks_number, incomplete_blocks_number);
    
    /* ----- ������������� ----- */

    readBlocksFromFile(encryptFILE, inputBlocks, blocks_number, incomplete_blocks_number);
    AES_block* decryptedBlocks = AES_Decrypt(keyLine, inputBlocks, blocks_number, iv, incomplete_blocks_number);
    writeBlocksToFile(decryptFILE, decryptedBlocks, blocks_number, incomplete_blocks_number);

}