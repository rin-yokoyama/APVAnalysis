/**
 * @file APVGrapeData.h
 * @author Rin Yokoyama (yokoyama@cns.s.u-tokyo.ac.jp)
 * @brief Data container for a single hit data from APV modules
 * @version 0.1
 * @date 2022-07-11
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef __APVGrapeData_H__
#define __APVGrapeData_H__
#include "TObject.h"
#include <iostream>
#include <algorithm>
#include <iterator>

class APVGrapeData : public TObject
{
public:
	/**
	 * @brief Default constructor of an APVGrapeData object
	 *
	 */
	APVGrapeData() : fRunNumber(0), fEventId(0), fCrateId(-1), fBoardId(-1), fSumLET(0), fSumABS(0), fSumPHA(0), fABSVec(kNSEG), fPHAVec(kNSEG)
	// APVGrapeData()
	{
	}

	/**
	 * @brief Copy constructor of an APVGrapeData object
	 *
	 * @param obj
	 */

	APVGrapeData(const APVGrapeData &obj) : TObject(obj)
	{
		fRunNumber = obj.GetRunNumber();
		fEventId = obj.GetEventId();
		fCrateId = obj.GetCrateId();
		fBoardId = obj.GetBoardId();
		fSumLET = obj.GetSumLET();
		fSumABS = obj.GetSumABS();
		fSumPHA = obj.GetSumPHA();
		std::copy(obj.GetABSVec().begin(), obj.GetABSVec().end(), std::back_inserter(fABSVec));
		std::copy(obj.GetPHAVec().begin(), obj.GetPHAVec().end(), std::back_inserter(fPHAVec));
	}
	/**
	 * @brief Destroy the APVGrapeData object
	 *
	 */
	virtual ~APVGrapeData() {}

	/**
	 * @brief Get the Run Number
	 *
	 * @return Int_t
	 */
	Int_t GetRunNumber() const { return fRunNumber; }
	/**
	 * @brief Get the Event Id
	 *
	 * @return ULong64_t
	 */
	ULong64_t GetEventId() const { return fEventId; }
	/**
	 * @brief Get the Crate ID
	 *
	 * @return Int_t
	 */
	Int_t GetCrateId() const { return fCrateId; }

	/**
	 * @brief Get the Board I D object
	 *
	 * @return Int_t
	 */
	Int_t GetBoardId() const { return fBoardId; }

	Double_t GetSumLET() const { return fSumLET; }
	Double_t GetSumABS() const { return fSumABS; }
	Double_t GetSumPHA() const { return fSumPHA; }
	Double_t GetTiming() const { return fSumABS; }
	const std::vector<Double_t> &GetABSVec() const { return fABSVec; }
	const std::vector<Double_t> &GetPHAVec() const { return fPHAVec; }

	void SetRunNumber(Int_t n) { fRunNumber = n; }
	void SetEventId(const ULong64_t &n) { fEventId = n; }
	void SetCrateId(Int_t n) { fCrateId = n; }
	void SetBoardId(Int_t n) { fBoardId = n; }
	void SetSumLET(Double_t val) { fSumLET = val; }
	void SetSumABS(Double_t val) { fSumABS = val; }
	void SetSumPHA(Double_t val) { fSumPHA = val; }

	void SetABS(Int_t n, Double_t val)
	{
		if (CheckChId(n))
			fABSVec[n] = val;
	}
	void SetPHA(Int_t n, Double_t val)
	{
		if (CheckChId(n))
			fPHAVec[n] = val;
	}

	static const int kNSEG = 9;

private:
	Int_t fRunNumber;
	ULong64_t fEventId;
	Int_t fCrateId;
	Int_t fBoardId;
	Double_t fSumLET;
	Double_t fSumABS;
	Double_t fSumPHA;
	std::vector<Double_t> fABSVec;
	std::vector<Double_t> fPHAVec;

	bool CheckChId(Int_t ch)
	{
		if (ch < 0 || ch >= kNSEG)
		{
			std::cout << "[APVGrapeData]: Invalid ch id " << ch << std::endl;
			return false;
		}
		else
		{
			return true;
		}
	}

	ClassDef(APVGrapeData, 1)
};

#endif // __APVGrapeData_H__