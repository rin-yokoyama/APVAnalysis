#include <iostream>
#include "APV7110Decoder.h"

APVGrapeData *APV7110Decoder::DecodeAnEvent(char *buff)
{
	ULong64_t data = (unsigned short)buff[0];
	Int_t crate_id = data & 0b1111;
	Int_t board_id = (data & 0b10000000000000) >> 14;
	data = (unsigned short)buff[2];
	Double_t sum_let = data * kCh2ns;
	data = (unsigned short)buff[4];
	Double_t sum_abs = data << 32;
	data = (unsigned short)buff[6];
	sum_abs += data << 16;
	data = (unsigned short)buff[8];
	sum_abs += data;
	sum_abs = sum_abs * kCh2ns;
	Double_t sum_pha = (unsigned short)buff[10];

	auto grape_data = new APVGrapeData();
	grape_data->SetCrateId(crate_id);
	grape_data->SetBoardId(board_id);
	grape_data->SetSumABS(sum_abs);
	grape_data->SetSumLET(sum_let);
	grape_data->SetSumPHA(sum_pha);
	for (int i = 0; i < APVGrapeData::kNSEG; ++i)
	{
		Double_t abs = (unsigned short)buff[12 + i * 2];
		grape_data->SetABS(i, abs);
		Double_t pha = ((unsigned short)buff[30 + i * 2]) & 0b1111111111111;
		grape_data->SetPHA(i, pha);
	}
	return grape_data;
}
