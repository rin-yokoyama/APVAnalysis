/**
 * @file APVGrapeDecodeManager.h
 * @author Rin Yokoyama (yokoyama@cns.s.u-tokyo.ac.jp)
 * @brief Manage running the apv decoder
 * @version 0.1
 * @date 2022-07-12
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef __APV_GRAPE_DECODE_MANAGER_H__
#define __APV_GRAPE_DECODE_MANAGER_H__

#include <fstream>
#include <string>
#include <thread>
#include <TTree.h>
#include <TFile.h>
#include "APV7110Decoder.h"
#include "ProgressPrinter.h"

class APVGrapeDecodeManager
{
public:
	/**
	 * @brief Construct a new APVGrapeDecodeManager object
	 *
	 * @param ifname input binary file name
	 * @param ofname output root file name
	 */
	APVGrapeDecodeManager(std::string ifname, std::string ofname) : fin_(ifname, std::ios::in | std::ios::binary), rootfile_(ofname.c_str(), "RECREATE"), block_size_(10)
	{
	}
	/**
	 * @brief Destroy the APVGrapeDecodeManager object
	 *
	 */
	virtual ~APVGrapeDecodeManager();

	/**
	 * @brief Initialize this manager before the main loop
	 *
	 * @param module_type
	 */
	void Init(std::string module_type);
	/**
	 * @brief Set the Block Size object in bytes
	 *
	 * @param size
	 */
	void SetBlockSize(ULong64_t size) { block_size_ = size; }

	/**
	 * @brief Main analysis loop
	 *
	 */
	void Loop();

private:
	const static unsigned int kPrintFreq; /// Print frequency of the progress
	const static unsigned int kNWorkers;  /// Number of workers
	TFile rootfile_;
	std::ifstream fin_;
	ULong64_t block_size_;
	char *buffer_ = nullptr; /// A pointer to the data buffer
	APVDecoderBase<APVGrapeData> *decoder_ = nullptr;
	std::vector<APV7110Decoder> decoders_;
	bool loop_ready_ = false; /// A flag to show if the main loop is ready
	TTree *output_tree_ = nullptr;
	APVGrapeData data_;
	ProgressPrinter *prog_printer_ = nullptr; /// A pointer to the ProgressPrinter object
};

#endif ///__APV__DECODE_MANAGER_H__