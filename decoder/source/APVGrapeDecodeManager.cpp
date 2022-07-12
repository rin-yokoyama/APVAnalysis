#include "APVGrapeDecodeManager.h"
#include <iostream>

const unsigned int APVGrapeDecodeManager::kPrintFreq = 1000000;
const unsigned int APVGrapeDecodeManager::kNWorkers = 10;

APVGrapeDecodeManager::~APVGrapeDecodeManager()
{
	fin_.close();
	rootfile_.Close();
	if (buffer_)
		delete[] buffer_;
	if (decoder_)
		delete decoder_;
	if (prog_printer_)
		delete prog_printer_;
}

void APVGrapeDecodeManager::Init(std::string module_type)
{
	/// check if files are open
	if (!fin_)
	{
		std::cout << "[APVGrapeDecodeManager]: Input file is not open." << std::endl;
		return;
	}
	if (!rootfile_.IsOpen())
	{
		std::cout << "[APVGrapeDecodeManager]: Output root file is not open." << std::endl;
		return;
	}

	fin_.seekg(0, fin_.end);
	prog_printer_ = new ProgressPrinter(0, (double)fin_.tellg(), kPrintFreq);
	fin_.seekg(0, fin_.beg);

	/// Create a root tree and add a branch for the APVSingleData
	output_tree_ = new TTree("APVTree", "APVTree");
	output_tree_->Branch("APVGrapeData", "APVGrapeData", &data_);

	/// Allocate buffer with the size of block_size_
	buffer_ = new char[block_size_ * kNWorkers];

	/// Create kNWorkers decoder instances
	if (module_type == "APV7110")
	{
		for (int i = 0; i < kNWorkers; ++i)
		{
			auto decoder = new APV7110Decoder();
			decoders_.emplace_back(*decoder);
		}
		loop_ready_ = true;
	}
	else
	{
		std::cout << "[APVGrapeDecodeManager]: Invalid module type " << module_type << std::endl;
	}
}

void APVGrapeDecodeManager::Loop()
{
	if (!loop_ready_)
	{
		std::cout << "[APVGrapeDecodeManager]: The loop is not ready. Exiting..." << std::endl;
		return;
	}
	std::cout << "[APVGrapeDecodeManager]: Starting the main loop..." << std::endl;
	double ts_max = 0;
	std::vector<APVGrapeData> last_block;

	auto root_fill = [&last_block, this]()
	{
		for (const auto &block_elem : last_block)
		{
			data_ = block_elem;
			output_tree_->Fill();
		}
	};

	/// main loop: read untill eof
	while (!fin_.read(buffer_, sizeof(char) * kNWorkers * block_size_).eof())
	{
		std::vector<std::thread> threads;
		auto func = [this](int i)
		{ decoders_[i].Decode(buffer_ + i * block_size_, block_size_); };
		for (int i = 0; i < kNWorkers; i++)
		{
			// threads.emplace_back(func, i);
			func(i);
		}

		// for (auto &th : threads)
		//{
		//	th.join();
		// }
		/// combine vectors from each thread
		std::vector<APVGrapeData> decoded;
		for (auto &decoder : decoders_)
		{
			decoded.insert(decoded.end(), decoder.GetDataVec().begin(), decoder.GetDataVec().end());
		}

		std::sort(decoded.begin(), decoded.end(), [](const APVGrapeData &x, const APVGrapeData &y)
				  { return x.GetSumABS() < y.GetSumABS(); });

		/// A function to insert elements that have smaller time stamps
		/// than the last element of the previous block
		auto moveElem = [&last_block](const APVGrapeData &x)
		{
			last_block.insert(
				std::find_if(last_block.begin(), last_block.end(), [x](const APVGrapeData &a)
							 { return a.GetSumABS() > x.GetSumABS(); }),
				// last_block.end(),
				x);
		};

		/// The iterator to the first element with a timestamp grater than ts_max
		auto first_elem = std::find_if(decoded.begin(), decoded.end(), [&ts_max](const APVGrapeData &x)
									   { return x.GetSumABS() > ts_max; });

		if (first_elem == decoded.begin())
		{
			if (!last_block.empty())
			{
				root_fill();
			}
			if (!decoded.empty())
			{
				last_block = decoded;
				ts_max = last_block.back().GetSumABS();
			}
		}
		else
		{

			// std::for_each(decoded.begin(), first_elem - 1, moveElem);
			last_block.insert(last_block.end(), decoded.begin(), first_elem);
			std::sort(last_block.begin(), last_block.end(), [](const APVGrapeData &x, const APVGrapeData &y)
					  { return x.GetSumABS() < y.GetSumABS(); });

			// for(const auto& dec: last_block){
			//	std::cout << std::fixed << std::setprecision(4) << dec->GetTiming() << std::endl;
			// }
			if (!last_block.empty())
			{
				root_fill();
			}
			// std::cout << last_block.size() << std::endl;
			last_block = std::vector<APVGrapeData>(first_elem, decoded.end());
			if (!last_block.empty())
			{
				ts_max = last_block.back().GetSumABS();
			}
		}

		prog_printer_->PrintProgress(fin_.tellg());
	}
	if (!last_block.empty())
	{
		root_fill();
	}
	output_tree_->Write();
}
