﻿
#include "Header.h"
#include "md5.h"



int main()
{


	AEScipher path("key.txt", "data");
	bool readKey = false;
	std::cout << "Output the keys?\n1 - Yes 0 - No : " << std::endl;
	std::cin >> readKey;
	if (readKey == 1) {
		std::cout<<"Key:" << std::endl;

		path.PrintDataFiles(path.keyss);
	}



	for (int i = 0; i < path.files.size(); i++) {
		path.CheckSumsMD5(path.files[i]);
	}
	std::cout << std::endl;
	
	


	if (path.keyss.size() == path.files.size()) {
		std::cout << "Equal number of keys and files" << std::endl;

		path.StartEncryption();


		std::cout << std::endl << std::endl;

		// new keyss in keyss_shuffer
		path.Shuffer(path.keyss.size());

		std::cout << "0 - The keys go sequentially to the files\n"
			"1 - The order of the keys is unknown: ";

		bool index;

		std::cin >> index;

		std::cout << std::endl;

		if (index == 0) {
			path.StartDecryption();
		}
		else if (index == 1) {
			path.StartDecryptionShuffer();
		}
		else {
			std::cout << "Incorrect data entered" << std::endl;
		}

	}
	else {
		std::cout << "Error: not equal number of keys and files" << std::endl;
		
	}
	


	for (auto& vec : path.keyss) {
		vec.clear(); 
	}
	path.keyss.clear();

	for (auto& vec : path.keyss) {
		vec.clear(); 
	}
	path.keyss.clear();




	return 0;
}