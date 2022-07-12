/**
 * @file APV7110Decoder.h
 * @author Rin Yokoyama (yokoyama@cns.s.u-tokyo.ac.jp)
 * @brief Decoder for the APV7110/8508 list mode data
 * @version 0.1
 * @date 2022-07-02
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef __APV7110_DECODER_H__
#define __APV7110_DECODER_H__

#include <vector>
#include <algorithm>
#include "APVGrapeData.h"
#include "APVDecoderBase.h"
#include <map>

class APV7110Decoder : public APVDecoderBase<APVGrapeData>
{
public:
	/**
	 * @brief Construct a new APV7110Decoder object
	 *
	 */
	APV7110Decoder() : APVDecoderBase<APVGrapeData>(kEventDataSize) {}
	/**
	 * @brief Destroy the APV7110Decoder object
	 *
	 */
	virtual ~APV7110Decoder() {}
	/**
	 * @brief Decode one event of data
	 *
	 * @param buff pointer to the buffer at the start of the event data
	 * @return APVSingleData* pointer to the data object
	 */
	APVGrapeData *DecodeAnEvent(char *buff) override;

	/**
	 * @brief Select APV Module type. 8104 is selected by default
	 *
	 * @param name  "APV8508" or "APV7110"
	 */
	void SetAPVModuleType(std::string name);

private:
	static const int kEventDataSize = 10; // data size of one event in bytes
	const double kCh2ns = 10;
};

#endif /// __APV7110_DECODER_H__
