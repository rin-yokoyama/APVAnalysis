#include <iostream>
#include <unistd.h>
#include "APVDecodeManager.h"
#include "APVGrapeDecodeManager.h"

/** prints usage **/
void usage(char *argv0)
{
	std::cout << "[APVDecoderMain]: Usage: "
			  << argv0 << "-t [module type] -i [input file] -o [output file]"
			  << std::endl;
}

int main(int argc, char **argv)
{
	std::string input_file_name;
	std::string module_name = "APV8104";
	std::string output_file_name;

	/** parsing commandline arguments **/
	if (argc < 5)
	{
		usage(argv[0]);
		return 1;
	}
	int opt = 0;
	while ((opt = getopt(argc, argv, "i:o:t:")) != -1)
	{
		switch (opt)
		{
		case 'i':
			input_file_name = optarg;
			break;
		case 'o':
			output_file_name = optarg;
			break;
		case 't':
			module_name = optarg;
			break;
		default:
			usage(argv[0]);
			return 1;
			break;
		}
	}
	if (module_name == "APV8104" || module_name == "APV8508")
	{
		APVDecodeManager manager(input_file_name, output_file_name);
		manager.SetBlockSize(10);
		manager.Init(module_name);
		manager.Loop();
	}
	else if (module_name == "APV7110")
	{
		APVGrapeDecodeManager manager(input_file_name, output_file_name);
		manager.SetBlockSize(1024);
		manager.Init(module_name);
		manager.Loop();
	}

	std::cout << "[APVDecoderMain]: Main loop done." << std::endl;
	return 0;
}
