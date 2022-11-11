#include <iostream>
#include <string>
#include <fstream>

void pickFileMaker()
{
	std::ofstream textFile;
	textFile.open("DPpickFile.txt");
	for(int i=1;i<100;i++) {
		textFile << "/store/user/rramos2/LeptonJetMass0_3/RunIISummer20UL17_NanoAODv2/210802_052537/0000/NanoAODv2_" + std::to_string(i) + ".root" << std::endl; 
	}
	textFile.close();
}
