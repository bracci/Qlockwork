/******************************************************************************
Words.h
******************************************************************************/

#ifndef WORDS_H
#define WORDS_H

// CH_BE

#define CH_BE_VOR        matrix[2] |= 0b0000000011100000
#define CH_BE_AB       matrix[3] |= 0b1100000000000000
#define CH_BE_ESISCH_BM  0b1101111000000000
#define CH_BE_ESISCH     matrix[0] |=  CH_BE_ESISCH_BM
#define CH_BE_ESISCH_R   matrix[0] &=  ~CH_BE_ESISCH_BM
#define CH_BE_GSI        matrix[9] |= 0b0000000011100000
#define CH_BE_AM       matrix[0] |= 0b1000000000000000
#define CH_BE_PM       matrix[0] |= 0b0100000000000000

#define CH_BE_FUEF       matrix[0] |= 0b0000000011100000
#define CH_BE_ZAEAE      matrix[1] |= 0b0000000011100000
#define CH_BE_VIERTU     matrix[1] |= 0b1111110000000000
#define CH_BE_ZWAENZG      matrix[2] |= 0b1111110000000000
#define CH_BE_HAUBI      matrix[3] |= 0b0001111100000000

#define CH_BE_H_EIS      matrix[4] |= 0b1110000000000000
#define CH_BE_H_ZWOEI      matrix[4] |= 0b0001111000000000
#define CH_BE_H_DRUE     matrix[4] |= 0b0000000011100000
#define CH_BE_H_VIER     matrix[5] |= 0b1111100000000000
#define CH_BE_H_FUEFI      matrix[5] |= 0b0000011110000000
#define CH_BE_H_SAECHSI    matrix[6] |= 0b1111110000000000
#define CH_BE_H_SIEBNI     matrix[6] |= 0b0000001111100000
#define CH_BE_H_ACHTI      matrix[7] |= 0b1111100000000000
#define CH_BE_H_NUENI      matrix[7] |= 0b0000011110000000
#define CH_BE_H_ZAENI      matrix[8] |= 0b1111000000000000
#define CH_BE_H_EUFI     matrix[8] |= 0b0000000111100000
#define CH_BE_H_ZWOEUFI    matrix[9] |= 0b1111110000000000

// CH_ZH

#define CH_ZH_VOR        matrix[2] |= 0b0000000011100000
#define CH_ZH_AB       matrix[3] |= 0b1100000000000000
#define CH_ZH_ESISCH_BM  0b1101111000000000
#define CH_ZH_ESISCH     matrix[0] |=  CH_ZH_ESISCH_BM
#define CH_ZH_ESISCH_R   matrix[0] &=  ~CH_ZH_ESISCH_BM
#define CH_ZH_GSI        matrix[9] |= 0b0000000011100000
#define CH_ZH_AM       matrix[0] |= 0b1000000000000000
#define CH_ZH_PM       matrix[0] |= 0b0100000000000000

#define CH_ZH_FUEF       matrix[1] |= 0b0000000111100000
#define CH_ZH_ZAEAE      matrix[0] |= 0b0000000011100000
#define CH_ZH_VIERTU     matrix[1] |= 0b1111111000000000
#define CH_ZH_ZWAENZG      matrix[2] |= 0b1111110000000000
#define CH_ZH_HAUBI      matrix[3] |= 0b0001111100000000

#define CH_ZH_H_EIS      matrix[4] |= 0b1110000000000000
#define CH_ZH_H_ZWOEI      matrix[4] |= 0b0001111000000000
#define CH_ZH_H_DRUE     matrix[4] |= 0b0000000111100000
#define CH_ZH_H_VIER     matrix[5] |= 0b1111100000000000
#define CH_ZH_H_FUEFI      matrix[5] |= 0b0000001111100000
#define CH_ZH_H_SAECHSI    matrix[6] |= 0b1111110000000000
#define CH_ZH_H_SIEBNI     matrix[7] |= 0b0000011111100000
#define CH_ZH_H_ACHTI      matrix[7] |= 0b1111100000000000
#define CH_ZH_H_NUENI      matrix[6] |= 0b0000001111100000
#define CH_ZH_H_ZAENI      matrix[8] |= 0b1111100000000000
#define CH_ZH_H_EUFI     matrix[8] |= 0b0000000111100000
#define CH_ZH_H_ZWOEUFI    matrix[9] |= 0b1111110000000000

// CH_ZH_2

#define CH_ZH_2_VOR        matrix[2] |= 0b0000000011100000
#define CH_ZH_2_AB       matrix[3] |= 0b1100000000000000
#define CH_ZH_2_ESISCH_BM  0b1101111000000000
#define CH_ZH_2_ESISCH     matrix[0] |=  CH_ZH_2_ESISCH_BM
#define CH_ZH_2_ESISCH_R   matrix[0] &=  ~CH_ZH_2_ESISCH_BM
#define CH_ZH_2_GSI        matrix[9] |= 0b0000000011100000
#define CH_ZH_2_AM       matrix[9] |= 0b0000001100000000
#define CH_ZH_2_PM       matrix[3] |= 0b0000000001100000

#define CH_ZH_2_FUEF       matrix[1] |= 0b0000000111100000
#define CH_ZH_2_ZAEAE      matrix[0] |= 0b0000000011100000
#define CH_ZH_2_VIERTU     matrix[1] |= 0b1111111000000000
#define CH_ZH_2_ZWAENZG      matrix[2] |= 0b1111110000000000
#define CH_ZH_2_HAUBI      matrix[3] |= 0b0001111100000000

#define CH_ZH_2_H_EIS      matrix[4] |= 0b1110000000000000
#define CH_ZH_2_H_ZWOEI      matrix[4] |= 0b0001111000000000
#define CH_ZH_2_H_DRUE     matrix[4] |= 0b0000000111100000
#define CH_ZH_2_H_VIER     matrix[5] |= 0b1111100000000000
#define CH_ZH_2_H_FUEFI      matrix[5] |= 0b0000011111000000
#define CH_ZH_2_H_SAECHSI    matrix[6] |= 0b1111110000000000
#define CH_ZH_2_H_SIEBNI     matrix[7] |= 0b1111110000000000
#define CH_ZH_2_H_ACHTI      matrix[7] |= 0b0000001111100000
#define CH_ZH_2_H_NUENI      matrix[6] |= 0b0000001111100000
#define CH_ZH_2_H_ZAENI      matrix[8] |= 0b1111100000000000
#define CH_ZH_2_H_EUFI     matrix[8] |= 0b0000001111000000
#define CH_ZH_2_H_ZWOEUFI    matrix[9] |= 0b1111110000000000

// CH_AG

#define CH_AG_VOR        matrix[2] |= 0b0000000011100000
#define CH_AG_AB       matrix[3] |= 0b1100000000000000
#define CH_AG_ESISCH_BM  0b1101111000000000
#define CH_AG_ESISCH     matrix[0] |=  CH_AG_ESISCH_BM
#define CH_AG_ESISCH_R   matrix[0] &=  ~CH_AG_ESISCH_BM
#define CH_AG_GSI        matrix[9] |= 0b0000000011100000
#define CH_AG_AM       matrix[0] |= 0b1000000000000000
#define CH_AG_PM       matrix[0] |= 0b0100000000000000

#define CH_AG_FUEF       matrix[1] |= 0b0000000111100000
#define CH_AG_ZAEAE      matrix[0] |= 0b0000000011100000
#define CH_AG_VIERTU     matrix[1] |= 0b1111111000000000
#define CH_AG_ZWAENZG      matrix[2] |= 0b1111110000000000
#define CH_AG_HAUBI      matrix[3] |= 0b0001111100000000

#define CH_AG_H_EIS      matrix[4] |= 0b1110000000000000
#define CH_AG_H_ZWOEI      matrix[4] |= 0b0001111000000000
#define CH_AG_H_DRUE     matrix[4] |= 0b0000000011110000
#define CH_AG_H_VIER     matrix[5] |= 0b1111100000000000
#define CH_AG_H_FUEFI      matrix[5] |= 0b0000001111100000
#define CH_AG_H_SAECHSI    matrix[6] |= 0b1111110000000000
#define CH_AG_H_SIEBNI     matrix[7] |= 0b0000011111100000
#define CH_AG_H_ACHTI      matrix[7] |= 0b1111100000000000
#define CH_AG_H_NUENI      matrix[6] |= 0b0000111110000000
#define CH_AG_H_ZAENI      matrix[8] |= 0b1111100000000000
#define CH_AG_H_EUFI     matrix[8] |= 0b0000000111100000
#define CH_AG_H_ZWOEUFI    matrix[9] |= 0b1111110000000000

// CH_GR

#define CH_GR_VOR        matrix[3] |= 0b1110000000000000
#define CH_GR_AB       matrix[3] |= 0b0001100000000000
#define CH_GR_ESISCH_BM  0b1101111000000000
#define CH_GR_ESISCH     matrix[0] |=  CH_GR_ESISCH_BM
#define CH_GR_ESISCH_R   matrix[0] &=  ~CH_GR_ESISCH_BM
#define CH_GR_GSI        matrix[9] |= 0b0000000011100000
#define CH_GR_AM       matrix[0] |= 0b1000000000000000
#define CH_GR_PM       matrix[0] |= 0b0100000000000000

#define CH_GR_FUEF       matrix[2] |= 0b0000000111100000
#define CH_GR_ZAEAE      matrix[1] |= 0b1111000000000000
#define CH_GR_VIERTU     matrix[2] |= 0b1111111000000000
#define CH_GR_ZWAENZG      matrix[1] |= 0b0000011111100000
#define CH_GR_HAUBI      matrix[3] |= 0b0000001111100000

#define CH_GR_H_EIS      matrix[4] |= 0b0000000111000000
#define CH_GR_H_ZWOEI      matrix[5] |= 0b1111000000000000
#define CH_GR_H_DRUE     matrix[5] |= 0b0000001111000000
#define CH_GR_H_VIER     matrix[6] |= 0b0000001111100000
#define CH_GR_H_FUEFI      matrix[9] |= 0b0000001111100000
#define CH_GR_H_SAECHSI    matrix[6] |= 0b1111110000000000
#define CH_GR_H_SIEBNI     matrix[7] |= 0b0000011111000000
#define CH_GR_H_ACHTI      matrix[7] |= 0b1111100000000000
#define CH_GR_H_NUENI      matrix[9] |= 0b1111100000000000
#define CH_GR_H_ZAENI      matrix[8] |= 0b0000001111100000
#define CH_GR_H_EUFI     matrix[4] |= 0b0011110000000000
#define CH_GR_H_ZWOEUFI    matrix[8] |= 0b1111110000000000

// CH_GR_2

#define CH_GR_2_VOR        matrix[2] |= 0b0000000011100000
#define CH_GR_2_AB       matrix[3] |= 0b0110000000000000
#define CH_GR_2_ESISCH_BM  0b1101111000000000
#define CH_GR_2_ESISCH     matrix[0] |=  CH_GR_2_ESISCH_BM
#define CH_GR_2_ESISCH_R   matrix[0] &=  ~CH_GR_2_ESISCH_BM
#define CH_GR_2_GSI        matrix[9] |= 0b0000000011100000

#define CH_GR_2_FUEF       matrix[0] |= 0b0000000011100000
#define CH_GR_2_ZAEAE      matrix[1] |= 0b0000000111100000
#define CH_GR_2_VIERTU     matrix[1] |= 0b1111111000000000
#define CH_GR_2_ZWAENZG    matrix[2] |= 0b1111111000000000
#define CH_GR_2_HAUBI      matrix[3] |= 0b0000011111000000

#define CH_GR_2_H_EIS      matrix[4] |= 0b1110000000000000
#define CH_GR_2_H_ZWOEI      matrix[4] |= 0b0001111000000000
#define CH_GR_2_H_DRUE     matrix[4] |= 0b0000000111100000
#define CH_GR_2_H_VIER     matrix[5] |= 0b0000001111000000
#define CH_GR_2_H_FUEFI      matrix[5] |= 0b0111000000000000
#define CH_GR_2_H_SAECHSI    matrix[6] |= 0b0000011111000000
#define CH_GR_2_H_SIEBNI     matrix[6] |= 0b1111000000000000
#define CH_GR_2_H_ACHTI      matrix[7] |= 0b0111100000000000
#define CH_GR_2_H_NUENI      matrix[7] |= 0b0000001110000000
#define CH_GR_2_H_ZAENI      matrix[8] |= 0b1111000000000000
#define CH_GR_2_H_EUFI     matrix[8] |= 0b0000001110000000
#define CH_GR_2_H_ZWOEUFI    matrix[9] |= 0b0111110000000000

// CH_VS

#define CH_VS_VOR        matrix[2] |= 0b0000000011100000
#define CH_VS_AB       matrix[3] |= 0b0110000000000000
#define CH_VS_ESISCH_BM  0b1101111000000000
#define CH_VS_ESISCH     matrix[0] |=  CH_VS_ESISCH_BM
#define CH_VS_ESISCH_R   matrix[0] &=  ~CH_VS_ESISCH_BM
#define CH_VS_GSI        matrix[9] |= 0b0000000011100000
#define CH_VS_AM       matrix[0] |= 0b1000000000000000
#define CH_VS_PM       matrix[0] |= 0b0100000000000000

#define CH_VS_FUEF       matrix[1] |= 0b0000000111100000
#define CH_VS_ZAEAE      matrix[0] |= 0b0000000011100000
#define CH_VS_VIERTU     matrix[1] |= 0b1111111000000000
#define CH_VS_ZWAENZG      matrix[2] |= 0b1111110000000000
#define CH_VS_HAUBI      matrix[3] |= 0b0000011111000000

#define CH_VS_H_EIS      matrix[4] |= 0b1110000000000000
#define CH_VS_H_ZWOEI      matrix[4] |= 0b0001111000000000
#define CH_VS_H_DRUE     matrix[4] |= 0b0000000011100000
#define CH_VS_H_VIER     matrix[5] |= 0b0000111110000000
#define CH_VS_H_FUEFI      matrix[5] |= 0b1111000000000000
#define CH_VS_H_SAECHSI    matrix[7] |= 0b0000111111100000
#define CH_VS_H_SIEBNI     matrix[6] |= 0b1111100000000000
#define CH_VS_H_ACHTI      matrix[6] |= 0b0000011111000000
#define CH_VS_H_NUENI      matrix[7] |= 0b1111000000000000
#define CH_VS_H_ZAENI      matrix[8] |= 0b1111100000000000
#define CH_VS_H_EUFI     matrix[8] |= 0b0000001111000000
#define CH_VS_H_ZWOEUFI    matrix[9] |= 0b1111110000000000

// CH_BS

#define CH_BS_VOR        matrix[2] |= 0b0000000011100000
#define CH_BS_AB       matrix[3] |= 0b0110000000000000
#define CH_BS_ESISCH_BM  0b1101111000000000
#define CH_BS_ESISCH     matrix[0] |=  CH_BS_ESISCH_BM
#define CH_BS_ESISCH_R   matrix[0] &=  ~CH_BS_ESISCH_BM
#define CH_BS_GSI        matrix[9] |= 0b0000000011100000
#define CH_BS_AM       matrix[0] |= 0b1000000000000000
#define CH_BS_PM       matrix[0] |= 0b0100000000000000

#define CH_BS_FUEF       matrix[1] |= 0b0000000111100000
#define CH_BS_ZAEAE      matrix[0] |= 0b0000000011100000
#define CH_BS_VIERTU     matrix[1] |= 0b1111111000000000
#define CH_BS_ZWAENZG      matrix[2] |= 0b1111111000000000
#define CH_BS_HAUBI      matrix[3] |= 0b0000011111000000

#define CH_BS_H_EIS      matrix[4] |= 0b1110000000000000
#define CH_BS_H_ZWOEI      matrix[4] |= 0b0001111000000000
#define CH_BS_H_DRUE     matrix[4] |= 0b0000000111100000
#define CH_BS_H_VIER     matrix[5] |= 0b0000001111100000
#define CH_BS_H_FUEFI      matrix[5] |= 0b0111110000000000
#define CH_BS_H_SAECHSI    matrix[6] |= 0b0000011111000000
#define CH_BS_H_SIEBNI     matrix[6] |= 0b1111100000000000
#define CH_BS_H_ACHTI      matrix[7] |= 0b0111110000000000
#define CH_BS_H_NUENI      matrix[7] |= 0b0000001111100000
#define CH_BS_H_ZAENI      matrix[8] |= 0b1111100000000000
#define CH_BS_H_EUFI     matrix[8] |= 0b0000001111000000
#define CH_BS_H_ZWOEUFI    matrix[9] |= 0b1111110000000000

//D3

#define D3_ESISCH_BM  0b1101111000000000
#define D3_ESISCH     matrix[0] |= D3_ESISCH_BM
#define D3_ESISCH_R   matrix[0] &= ~D3_ESISCH_BM
#define D3_VOR        matrix[3] |= 0b0000000011100000
#define D3_NACH       matrix[3] |= 0b1111000000000000
#define D3_AM       matrix[0] |= 0b1000000000000000
#define D3_PM       matrix[0] |= 0b0100000000000000

#define D3_FUENF      matrix[2] |= 0b0000000111100000
#define D3_ZEHN       matrix[2] |= 0b1111000000000000
#define D3_VIERTL     matrix[1] |= 0b0000111111000000
#define D3_HALB       matrix[4] |= 0b1111000000000000
#define D3_DREIVIERTL   matrix[1] |= 0b1111111111000000

#define D3_H_OISE     matrix[5] |= 0b1111000000000000
#define D3_H_ZWOIE      matrix[6] |= 0b1111100000000000
#define D3_H_DREIE      matrix[7] |= 0b1111100000000000
#define D3_H_VIERE      matrix[9] |= 0b0000001111100000
#define D3_H_FUENFE     matrix[4] |= 0b0000011111000000
#define D3_H_SECHSE     matrix[5] |= 0b0011111100000000
#define D3_H_SIEBNE     matrix[9] |= 0b1111110000000000
#define D3_H_ACHTE      matrix[6] |= 0b0000011111000000
#define D3_H_NEUNE      matrix[8] |= 0b0001111100000000
#define D3_H_ZEHNE      matrix[8] |= 0b1111100000000000
#define D3_H_ELFE     matrix[5] |= 0b0000000111100000
#define D3_H_ZWOELFE    matrix[7] |= 0b0000011111100000

// DE

#define DE_VOR        matrix[3] |= 0b1110000000000000
#define DE_NACH       matrix[3] |= 0b0000000111100000
#define DE_ESIST_BM   0b1101110000000000
#define DE_ESIST      matrix[0] |= DE_ESIST_BM
#define DE_ESIST_R    matrix[0] &= ~DE_ESIST_BM
#define DE_UHR        matrix[9] |= 0b0000000011100000
#define DE_AM       matrix[5] |= 0b0000011000000000
#define DE_PM       matrix[6] |= 0b0000110000000000

#define DE_FUENF      matrix[0] |= 0b0000000111100000
#define DE_ZEHN       matrix[1] |= 0b1111000000000000
#define DE_VIERTEL      matrix[2] |= 0b0000111111100000
#define DE_ZWANZIG      matrix[1] |= 0b0000111111100000
#define DE_HALB       matrix[4] |= 0b1111000000000000
#define DE_DREIVIERTEL    matrix[2] |= 0b1111111111100000

#define DE_H_EIN      matrix[5] |= 0b1110000000000000
#define DE_H_EINS     matrix[5] |= 0b1111000000000000
#define DE_H_ZWEI     matrix[5] |= 0b0000000111100000
#define DE_H_DREI     matrix[6] |= 0b1111000000000000
#define DE_H_VIER     matrix[6] |= 0b0000000111100000
#define DE_H_FUENF      matrix[4] |= 0b0000000111100000
#define DE_H_SECHS      matrix[7] |= 0b1111100000000000
#define DE_H_SIEBEN     matrix[8] |= 0b1111110000000000
#define DE_H_ACHT     matrix[7] |= 0b0000000111100000
#define DE_H_NEUN     matrix[9] |= 0b0001111000000000
#define DE_H_ZEHN     matrix[9] |= 0b1111000000000000
#define DE_H_ELF      matrix[4] |= 0b0000011100000000
#define DE_H_ZWOELF     matrix[8] |= 0b0000001111100000

// EN

#define EN_ITIS_BM    0b1101100000000000
#define EN_ITIS       matrix[0] |= EN_ITIS_BM
#define EN_ITIS_R     matrix[0] &= ~EN_ITIS_BM
#define EN_TIME       matrix[0] |= 0b0000000111100000
#define EN_A        matrix[1] |= 0b1000000000000000
#define EN_OCLOCK     matrix[9] |= 0b0000011111100000
#define EN_AM       matrix[0] |= 0b0000000110000000
#define EN_PM       matrix[0] |= 0b0000000001100000

#define EN_QUATER     matrix[1] |= 0b0011111110000000
#define EN_TWENTY     matrix[2] |= 0b1111110000000000
#define EN_FIVE       matrix[2] |= 0b0000001111000000
#define EN_HALF       matrix[3] |= 0b1111000000000000
#define EN_TEN        matrix[3] |= 0b0000011100000000
#define EN_TO       matrix[3] |= 0b0000000001100000
#define EN_PAST       matrix[4] |= 0b1111000000000000

#define EN_H_NINE     matrix[4] |= 0b0000000111100000
#define EN_H_ONE      matrix[5] |= 0b1110000000000000
#define EN_H_SIX      matrix[5] |= 0b0001110000000000
#define EN_H_THREE      matrix[5] |= 0b0000001111100000
#define EN_H_FOUR     matrix[6] |= 0b1111000000000000
#define EN_H_FIVE     matrix[6] |= 0b0000111100000000
#define EN_H_TWO      matrix[6] |= 0b0000000011100000
#define EN_H_EIGHT      matrix[7] |= 0b1111100000000000
#define EN_H_ELEVEN     matrix[7] |= 0b0000011111100000
#define EN_H_SEVEN      matrix[8] |= 0b1111100000000000
#define EN_H_TWELVE     matrix[8] |= 0b0000011111100000
#define EN_H_TEN      matrix[9] |= 0b1110000000000000

// ES

#define ES_SONLAS_BM  0b0111011100000000
#define ES_ESLA_BM    0b1100011000000000
#define ES_SONLAS     matrix[0] |= ES_SONLAS_BM
#define ES_ESLA       matrix[0] |= ES_ESLA_BM
#define ES_SONLAS_R   matrix[0] &= ~ES_SONLAS_BM
#define ES_ESLA_R     matrix[0] &= ~ES_ESLA_BM
#define ES_Y        matrix[6] |= 0b0000010000000000
#define ES_MENOS      matrix[6] |= 0b0000001111100000
#define ES_AM       matrix[0] |= 0b1000000000000000
#define ES_PM       matrix[0] |= 0b0100000000000000

#define ES_CINCO      matrix[8] |= 0b0000001111100000
#define ES_DIEZ       matrix[7] |= 0b0000000111100000
#define ES_CUARTO     matrix[9] |= 0b0000011111100000
#define ES_VEINTE     matrix[7] |= 0b0111111000000000
#define ES_VEINTICINCO    matrix[8] |= 0b1111111111100000
#define ES_MEDIA      matrix[9] |= 0b1111100000000000

#define ES_H_UNA      matrix[0] |= 0b0000000011100000
#define ES_H_DOS      matrix[1] |= 0b1110000000000000
#define ES_H_TRES     matrix[1] |= 0b0000111100000000
#define ES_H_CUATRO     matrix[2] |= 0b1111110000000000
#define ES_H_CINCO      matrix[2] |= 0b0000001111100000
#define ES_H_SEIS     matrix[3] |= 0b1111000000000000
#define ES_H_SIETE      matrix[3] |= 0b0000011111000000
#define ES_H_OCHO     matrix[4] |= 0b1111000000000000
#define ES_H_NUEVE      matrix[4] |= 0b0000111110000000
#define ES_H_DIEZ     matrix[5] |= 0b0011110000000000
#define ES_H_ONCE     matrix[5] |= 0b0000000111100000
#define ES_H_DOCE     matrix[6] |= 0b1111000000000000

// FR

#define FR_TRAIT      matrix[8] |= 0b0000010000000000
#define FR_ET       matrix[7] |= 0b1100000000000000
#define FR_LE       matrix[6] |= 0b0000001100000000
#define FR_MOINS      matrix[6] |= 0b1111100000000000
#define FR_ILEST_BM   0b1101110000000000
#define FR_ILEST      matrix[0] |= FR_ILEST_BM
#define FR_ILEST_R    matrix[0] &= ~FR_ILEST_BM
#define FR_HEURE      matrix[5] |= 0b0000011111000000
#define FR_HEURES     matrix[5] |= 0b0000011111100000
#define FR_AM       matrix[7] |= 0b0000000001100000
#define FR_PM       matrix[9] |= 0b0000000011000000

#define FR_CINQ       matrix[8] |= 0b0000001111000000
#define FR_DIX        matrix[6] |= 0b0000000011100000
#define FR_QUART      matrix[7] |= 0b0001111100000000
#define FR_VINGT      matrix[8] |= 0b1111100000000000
#define FR_DEMIE      matrix[9] |= 0b0001111100000000

#define FR_H_UNE      matrix[2] |= 0b0000111000000000
#define FR_H_DEUX     matrix[0] |= 0b0000000111100000
#define FR_H_TROIS      matrix[1] |= 0b0000001111100000
#define FR_H_QUATRE     matrix[1] |= 0b1111110000000000
#define FR_H_CINQ     matrix[3] |= 0b0000000111100000
#define FR_H_SIX      matrix[3] |= 0b0000111000000000
#define FR_H_SEPT     matrix[2] |= 0b0000000111100000
#define FR_H_HUIT     matrix[3] |= 0b1111000000000000
#define FR_H_NEUF     matrix[2] |= 0b1111000000000000
#define FR_H_DIX      matrix[4] |= 0b0011100000000000
#define FR_H_ONZE     matrix[5] |= 0b1111000000000000
#define FR_H_MIDI     matrix[4] |= 0b1111000000000000
#define FR_H_MINUIT     matrix[4] |= 0b0000011111100000

// IT

#define IT_SONOLE_BM  0b1111011000000000
#define IT_SONOLE     matrix[0] |= IT_SONOLE_BM
#define IT_SONOLE_R   matrix[0] &= ~IT_SONOLE_BM
#define IT_LE       matrix[0] |= 0b0000011000000000
#define IT_ORE        matrix[0] |= 0b0000000011100000
#define IT_E_BM       0b1000000000000000
#define IT_E        matrix[1] |= IT_E_BM
#define IT_E_R      matrix[1] &= ~IT_E_BM
#define IT_AM       matrix[0] |= 0b1000000000000000
#define IT_PM       matrix[0] |= 0b0100000000000000

#define IT_H_LUNA     matrix[1] |= 0b0011110000000000
#define IT_H_DUE      matrix[1] |= 0b0000000111000000
#define IT_H_TRE      matrix[2] |= 0b1110000000000000
#define IT_H_OTTO     matrix[2] |= 0b0001111000000000
#define IT_H_NOVE     matrix[2] |= 0b0000000111100000
#define IT_H_DIECI      matrix[3] |= 0b1111100000000000
#define IT_H_UNDICI     matrix[3] |= 0b0000011111100000
#define IT_H_DODICI     matrix[4] |= 0b1111110000000000
#define IT_H_SETTE      matrix[4] |= 0b0000001111100000
#define IT_H_QUATTRO    matrix[5] |= 0b1111111000000000
#define IT_H_SEI      matrix[5] |= 0b0000000011100000
#define IT_H_CINQUE     matrix[6] |= 0b1111110000000000

#define IT_MENO       matrix[6] |= 0b0000000111100000
#define IT_E2       matrix[7] |= 0b1000000000000000
#define IT_UN       matrix[7] |= 0b0011000000000000
#define IT_QUARTO     matrix[7] |= 0b0000011111100000
#define IT_VENTI      matrix[8] |= 0b1111100000000000
#define IT_CINQUE     matrix[8] |= 0b0000011111100000
#define IT_DIECI      matrix[9] |= 0b1111100000000000
#define IT_MEZZA      matrix[9] |= 0b0000001111100000

// SD

#define SD_SUNUSAS_BM  0b1111000011100000
#define SD_SUNUSAS     matrix[0] |= SD_SUNUSAS_BM
#define SD_SUNUSAS_R   matrix[0] &= ~SD_SUNUSAS_BM
#define SD_ESA_BM       0b0000100011000000
#define SD_ESA        matrix[0] |= SD_ESA_BM
#define SD_ESA_R      matrix[0] &= ~SD_ESA_BM
#define SD_AM       matrix[0] |= 0b1000000000000000
#define SD_PM       matrix[0] |= 0b0100000000000000

#define SD_H_UNA     matrix[1] |= 0b1110000000000000
#define SD_H_DUAS      matrix[1] |= 0b0001111000000000
#define SD_H_TRES      matrix[2] |= 0b1111000000000000
#define SD_H_OTTO     matrix[3] |= 0b0000000111100000
#define SD_H_NOE     matrix[5] |= 0b0000000011100000
#define SD_H_DEGHE      matrix[6] |= 0b1111100000000000
#define SD_H_UNDIGHI     matrix[3] |= 0b1111111000000000
#define SD_H_DOIGHI     matrix[4] |= 0b1111110000000000
#define SD_H_SETTE      matrix[1] |= 0b0000001111100000
#define SD_H_BATTORO    matrix[5] |= 0b1111111000000000
#define SD_H_SESE      matrix[4] |= 0b0000000111100000
#define SD_H_CHIMBE     matrix[2] |= 0b0000011111100000

#define SD_MANCU       matrix[6] |= 0b0000001111100000
#define SD_E       matrix[7] |= 0b1000000000000000
#define SD_UNU       matrix[7] |= 0b0011100000000000
#define SD_QUASTU     matrix[9] |= 0b1111110000000000
#define SD_VINTI      matrix[8] |= 0b1111100000000000
#define SD_CHIMBE     matrix[8] |= 0b0000011111100000
#define SD_DEGHE      matrix[7] |= 0b0000001111100000
#define SD_MESA      matrix[9] |= 0b0000000111100000

// NL

#define NL_VOOR       matrix[1] |= 0b0000000111100000
#define NL_OVER       matrix[2] |= 0b1111000000000000
#define NL_VOOR2      matrix[4] |= 0b1111000000000000
#define NL_OVER2      matrix[3] |= 0b0000000111100000
#define NL_HETIS_BM   0b1110110000000000
#define NL_HETIS      matrix[0] |= NL_HETIS_BM
#define NL_HETIS_R    matrix[0] &= ~NL_HETIS_BM
#define NL_UUR        matrix[9] |= 0b0000000011100000
#define NL_AM       matrix[0] |= 0b1000000000000000
#define NL_PM       matrix[0] |= 0b0100000000000000

#define NL_VIJF       matrix[0] |= 0b0000000111100000
#define NL_TIEN       matrix[1] |= 0b1111000000000000
#define NL_KWART      matrix[2] |= 0b0000001111100000
#define NL_ZWANZIG      matrix[1] |= 0b0000111111100000
#define NL_HALF       matrix[3] |= 0b1111000000000000

#define NL_H_EEN      matrix[4] |= 0b0000000111000000
#define NL_H_EENS     matrix[4] |= 0b0000000111100000
#define NL_H_TWEE     matrix[5] |= 0b1111000000000000
#define NL_H_DRIE     matrix[5] |= 0b0000000111100000
#define NL_H_VIER     matrix[6] |= 0b1111000000000000
#define NL_H_VIJF     matrix[6] |= 0b0000111100000000
#define NL_H_ZES      matrix[6] |= 0b0000000011100000
#define NL_H_ZEVEN      matrix[7] |= 0b1111100000000000
#define NL_H_ACHT     matrix[8] |= 0b1111000000000000
#define NL_H_NEGEN      matrix[7] |= 0b0000001111100000
#define NL_H_TIEN     matrix[8] |= 0b0000111100000000
#define NL_H_ELF      matrix[8] |= 0b0000000011100000
#define NL_H_TWAALF     matrix[9] |= 0b1111110000000000

// SW

#define SW_I1                  matrix[1] |= 0b0000100000000000
#define SW_I2                  matrix[3] |= 0b0000001000000000
#define SW_I3                  matrix[2] |= 0b0000001000000000
#define SW_I4                  matrix[1] |= 0b0000000000100000

#define SW_OEVER               matrix[4] |= 0b1111000000000000
#define SW_KLOCKANAER_BM       0b1111111011000000
#define SW_KLOCKANAER          matrix[0] |= SW_KLOCKANAER_BM
#define SW_KLOCKANAER_R        matrix[0] &= ~SW_KLOCKANAER_BM
#define SW_AM                  matrix[4] |= 0b0000011000000000
#define SW_PM                  matrix[3] |= 0b0000000001100000

#define SW_FEM                 matrix[1] |= 0b1110000000000000
#define SW_TIO                 matrix[1] |= 0b0000001110000000
#define SW_KVART               matrix[2] |= 0b1111100000000000
#define SW_TJUGO               matrix[3] |= 0b1111100000000000
#define SW_HALV                matrix[4] |= 0b0000000111100000
#define SW_KVART               matrix[2] |= 0b1111100000000000

#define SW_H_ETT               matrix[5] |= 0b1110000000000000
#define SW_H_ETT               matrix[5] |= 0b1110000000000000
#define SW_H_TVA               matrix[5] |= 0b0000000011100000
#define SW_H_TRE               matrix[6] |= 0b1110000000000000
#define SW_H_FYRA              matrix[6] |= 0b0000000111100000
#define SW_H_FEM               matrix[7] |= 0b1110000000000000
#define SW_H_SEX               matrix[7] |= 0b0000000011100000
#define SW_H_SJU               matrix[8] |= 0b1110000000000000
#define SW_H_ATTA              matrix[8] |= 0b0001111000000000
#define SW_H_NIO               matrix[8] |= 0b0000000011100000
#define SW_H_TIO               matrix[9] |= 0b1110000000000000
#define SW_H_ELVA              matrix[9] |= 0b0001111000000000
#define SW_H_TOLV              matrix[9] |= 0b0000000111100000

#endif
