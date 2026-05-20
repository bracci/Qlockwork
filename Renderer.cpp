/******************************************************************************
Renderer.cpp
******************************************************************************/

#include "Renderer.h"

Renderer::Renderer()
{
}

// Write time to screenbuffer.
void Renderer::setTime(uint8_t hours, uint8_t minutes, uint8_t language, bool chGsi, uint16_t matrix[])
{
  switch (language)
  {
  case FRONTCOVER_DE_DE:
  case FRONTCOVER_DE_SW:
  case FRONTCOVER_DE_BA:
  case FRONTCOVER_DE_SA:
    DE_ESIST;
    switch (minutes / 5)
    {
    case 0:
      // full hour
      setHours(hours, true, language, matrix);
      break;
    case 1:
      // 5 nach
      DE_FUENF;
      DE_NACH;
      setHours(hours, false, language, matrix);
      break;
    case 2:
      // 10 nach
      DE_ZEHN;
      DE_NACH;
      setHours(hours, false, language, matrix);
      break;
    case 3:
      // viertel nach
      if ((language == FRONTCOVER_DE_SW) || (language == FRONTCOVER_DE_SA))
      {
        DE_VIERTEL;
        setHours(hours + 1, false, language, matrix);
      }
      else
      {
        DE_VIERTEL;
        DE_NACH;
        setHours(hours, false, language, matrix);
      }
      break;
    case 4:
      // 20 nach
      if (language == FRONTCOVER_DE_SA)
      {
        DE_ZEHN;
        DE_VOR;
        DE_HALB;
        setHours(hours + 1, false, language, matrix);
      }
      else
      {
        DE_ZWANZIG;
        DE_NACH;
        setHours(hours, false, language, matrix);
      }
      break;
    case 5:
      // 5 vor halb
      DE_FUENF;
      DE_VOR;
      DE_HALB;
      setHours(hours + 1, false, language, matrix);
      break;
    case 6:
      // halb
      DE_HALB;
      setHours(hours + 1, false, language, matrix);
      break;
    case 7:
      // 5 nach halb
      DE_FUENF;
      DE_NACH;
      DE_HALB;
      setHours(hours + 1, false, language, matrix);
      break;
    case 8:
      // 20 vor
      if (language == FRONTCOVER_DE_SA)
      {
        DE_ZEHN;
        DE_NACH;
        DE_HALB;
        setHours(hours + 1, false, language, matrix);
      }
      else
      {
        DE_ZWANZIG;
        DE_VOR;
        setHours(hours + 1, false, language, matrix);
      }
      break;
    case 9:
      // viertel vor
      if ((language == FRONTCOVER_DE_SW) || (language == FRONTCOVER_DE_BA) || (language == FRONTCOVER_DE_SA))
      {
        DE_DREIVIERTEL;
        setHours(hours + 1, false, language, matrix);
      }
      else
      {
        DE_VIERTEL;
        DE_VOR;
        setHours(hours + 1, false, language, matrix);
      }
      break;
    case 10:
      // 10 vor
      DE_ZEHN;
      DE_VOR;
      setHours(hours + 1, false, language, matrix);
      break;
    case 11:
      // 5 vor
      DE_FUENF;
      DE_VOR;
      setHours(hours + 1, false, language, matrix);
      break;
    default:
      ;
    }
    break;
  case FRONTCOVER_D3:
    D3_ESISCH;
    switch (minutes / 5)
    {
    case 0:
      // glatte Stunde
      setHours(hours, true, language, matrix);
      break;
    case 1:
      // 5 nach
      D3_FUENF;
      D3_NACH;
      setHours(hours, false, language, matrix);
      break;
    case 2:
      // 10 nach
      D3_ZEHN;
      D3_NACH;
      setHours(hours, false, language, matrix);
      break;
    case 3:
      // viertl nach
      D3_VIERTL;
      setHours(hours + 1, false, language, matrix);
      break;
    case 4:
      // 10 vor halb
      D3_ZEHN;
      D3_VOR;
      D3_HALB;
      setHours(hours + 1, false, language, matrix);
      break;
    case 5:
      // 5 vor halb
      D3_FUENF;
      D3_VOR;
      D3_HALB;
      setHours(hours + 1, false, language, matrix);
      break;
    case 6:
      // halb
      D3_HALB;
      setHours(hours + 1, false, language, matrix);
      break;
    case 7:
      // 5 nach halb
      D3_FUENF;
      D3_NACH;
      D3_HALB;
      setHours(hours + 1, false, language, matrix);
      break;
    case 8:
      // 10 nach halb
      D3_ZEHN;
      D3_NACH;
      D3_HALB;
      setHours(hours + 1, false, language, matrix);
      break;
    case 9:
      // viertel vor
      D3_DREIVIERTL;
      setHours(hours + 1, false, language, matrix);
      break;
    case 10:
      // 10 vor
      D3_ZEHN;
      D3_VOR;
      setHours(hours + 1, false, language, matrix);
      break;
    case 11:
      // 5 vor
      D3_FUENF;
      D3_VOR;
      setHours(hours + 1, false, language, matrix);
      break;
    }
    break;
  case FRONTCOVER_CH_BE:
    CH_BE_ESISCH;
    switch (minutes / 5)
    {
    case 0:
      // glatte Stunde
      setHours(hours, true, language, matrix);
      break;
    case 1:
      // 5 ab
      CH_BE_FUEF;
      CH_BE_AB;
      setHours(hours, false, language, matrix);
      break;
    case 2:
      // 10 ab
      CH_BE_ZAEAE;
      CH_BE_AB;
      setHours(hours, false, language, matrix);
      break;
    case 3:
      // viertel ab
      CH_BE_VIERTU;
      CH_BE_AB;
      setHours(hours, false, language, matrix);
      break;
    case 4:
      // 20 ab
      CH_BE_ZWAENZG;
      CH_BE_AB;
      setHours(hours, false, language, matrix);
      break;
    case 5:
      // 5 vor halb
      CH_BE_FUEF;
      CH_BE_VOR;
      CH_BE_HAUBI;
      setHours(hours + 1, false, language, matrix);
      break;
    case 6:
      // halb
      CH_BE_HAUBI;
      setHours(hours + 1, false, language, matrix);
      break;
    case 7:
      // 5 ab halb
      CH_BE_FUEF;
      CH_BE_AB;
      CH_BE_HAUBI;
      setHours(hours + 1, false, language, matrix);
      break;
    case 8:
      // 20 vor
      CH_BE_ZWAENZG;
      CH_BE_VOR;
      setHours(hours + 1, false, language, matrix);
      break;
    case 9:
      // viertel vor
      CH_BE_VIERTU;
      CH_BE_VOR;
      setHours(hours + 1, false, language, matrix);
      break;
    case 10:
      // 10 vor
      CH_BE_ZAEAE;
      CH_BE_VOR;
      setHours(hours + 1, false, language, matrix);
      break;
    case 11:
      // 5 vor
      CH_BE_FUEF;
      CH_BE_VOR;
      setHours(hours + 1, false, language, matrix);
      break;
    default:
      ;
    }
    if (chGsi && minutes % 5) CH_BE_GSI;
    break;
  case FRONTCOVER_CH_ZH:
    CH_ZH_ESISCH;
    switch (minutes / 5)
    {
    case 0:
      // glatte Stunde
      setHours(hours, true, language, matrix);
      break;
    case 1:
      // 5 ab
      CH_ZH_FUEF;
      CH_ZH_AB;
      setHours(hours, false, language, matrix);
      break;
    case 2:
      // 10 ab
      CH_ZH_ZAEAE;
      CH_ZH_AB;
      setHours(hours, false, language, matrix);
      break;
    case 3:
      // viertel ab
      CH_ZH_VIERTU;
      CH_ZH_AB;
      setHours(hours, false, language, matrix);
      break;
    case 4:
      // 20 ab
      CH_ZH_ZWAENZG;
      CH_ZH_AB;
      setHours(hours, false, language, matrix);
      break;
    case 5:
      // 5 vor halb
      CH_ZH_FUEF;
      CH_ZH_VOR;
      CH_ZH_HAUBI;
      setHours(hours + 1, false, language, matrix);
      break;
    case 6:
      // halb
      CH_ZH_HAUBI;
      setHours(hours + 1, false, language, matrix);
      break;
    case 7:
      // 5 ab halb
      CH_ZH_FUEF;
      CH_ZH_AB;
      CH_ZH_HAUBI;
      setHours(hours + 1, false, language, matrix);
      break;
    case 8:
      // 20 vor
      CH_ZH_ZWAENZG;
      CH_ZH_VOR;
      setHours(hours + 1, false, language, matrix);
      break;
    case 9:
      // viertel vor
      CH_ZH_VIERTU;
      CH_ZH_VOR;
      setHours(hours + 1, false, language, matrix);
      break;
    case 10:
      // 10 vor
      CH_ZH_ZAEAE;
      CH_ZH_VOR;
      setHours(hours + 1, false, language, matrix);
      break;
    case 11:
      // 5 vor
      CH_ZH_FUEF;
      CH_ZH_VOR;
      setHours(hours + 1, false, language, matrix);
      break;
    default:
      ;
    }
    if (chGsi && minutes % 5) CH_ZH_GSI;
    break;
  case FRONTCOVER_CH_ZH_2:
    CH_ZH_2_ESISCH;
    switch (minutes / 5)
    {
    case 0:
      // glatte Stunde
      setHours(hours, true, language, matrix);
      break;
    case 1:
      // 5 ab
      CH_ZH_2_FUEF;
      CH_ZH_2_AB;
      setHours(hours, false, language, matrix);
      break;
    case 2:
      // 10 ab
      CH_ZH_2_ZAEAE;
      CH_ZH_2_AB;
      setHours(hours, false, language, matrix);
      break;
    case 3:
      // viertel ab
      CH_ZH_2_VIERTU;
      CH_ZH_2_AB;
      setHours(hours, false, language, matrix);
      break;
    case 4:
      // 20 ab
      CH_ZH_2_ZWAENZG;
      CH_ZH_2_AB;
      setHours(hours, false, language, matrix);
      break;
    case 5:
      // 5 vor halb
      CH_ZH_2_FUEF;
      CH_ZH_2_VOR;
      CH_ZH_2_HAUBI;
      setHours(hours + 1, false, language, matrix);
      break;
    case 6:
      // halb
      CH_ZH_2_HAUBI;
      setHours(hours + 1, false, language, matrix);
      break;
    case 7:
      // 5 ab halb
      CH_ZH_2_FUEF;
      CH_ZH_2_AB;
      CH_ZH_2_HAUBI;
      setHours(hours + 1, false, language, matrix);
      break;
    case 8:
      // 20 vor
      CH_ZH_2_ZWAENZG;
      CH_ZH_2_VOR;
      setHours(hours + 1, false, language, matrix);
      break;
    case 9:
      // viertel vor
      CH_ZH_2_VIERTU;
      CH_ZH_2_VOR;
      setHours(hours + 1, false, language, matrix);
      break;
    case 10:
      // 10 vor
      CH_ZH_2_ZAEAE;
      CH_ZH_2_VOR;
      setHours(hours + 1, false, language, matrix);
      break;
    case 11:
      // 5 vor
      CH_ZH_2_FUEF;
      CH_ZH_2_VOR;
      setHours(hours + 1, false, language, matrix);
      break;
    default:
      ;
    }
    if (chGsi && minutes % 5) CH_ZH_2_GSI;
    break;
  case FRONTCOVER_CH_AG:
    CH_AG_ESISCH;
    switch (minutes / 5)
    {
    case 0:
      // glatte Stunde
      setHours(hours, true, language, matrix);
      break;
    case 1:
      // 5 ab
      CH_AG_FUEF;
      CH_AG_AB;
      setHours(hours, false, language, matrix);
      break;
    case 2:
      // 10 ab
      CH_AG_ZAEAE;
      CH_AG_AB;
      setHours(hours, false, language, matrix);
      break;
    case 3:
      // viertel ab
      CH_AG_VIERTU;
      CH_AG_AB;
      setHours(hours, false, language, matrix);
      break;
    case 4:
      // 20 ab
      CH_AG_ZWAENZG;
      CH_AG_AB;
      setHours(hours, false, language, matrix);
      break;
    case 5:
      // 5 vor halb
      CH_AG_FUEF;
      CH_AG_VOR;
      CH_AG_HAUBI;
      setHours(hours + 1, false, language, matrix);
      break;
    case 6:
      // halb
      CH_AG_HAUBI;
      setHours(hours + 1, false, language, matrix);
      break;
    case 7:
      // 5 ab halb
      CH_AG_FUEF;
      CH_AG_AB;
      CH_AG_HAUBI;
      setHours(hours + 1, false, language, matrix);
      break;
    case 8:
      // 20 vor
      CH_AG_ZWAENZG;
      CH_AG_VOR;
      setHours(hours + 1, false, language, matrix);
      break;
    case 9:
      // viertel vor
      CH_AG_VIERTU;
      CH_AG_VOR;
      setHours(hours + 1, false, language, matrix);
      break;
    case 10:
      // 10 vor
      CH_AG_ZAEAE;
      CH_AG_VOR;
      setHours(hours + 1, false, language, matrix);
      break;
    case 11:
      // 5 vor
      CH_AG_FUEF;
      CH_AG_VOR;
      setHours(hours + 1, false, language, matrix);
      break;
    default:
      ;
    }
    if (chGsi && minutes % 5) CH_AG_GSI;
    break;
  case FRONTCOVER_CH_GR:
    CH_GR_ESISCH;
    switch (minutes / 5)
    {
    case 0:
      // glatte Stunde
      setHours(hours, true, language, matrix);
      break;
    case 1:
      // 5 ab
      CH_GR_FUEF;
      CH_GR_AB;
      setHours(hours, false, language, matrix);
      break;
    case 2:
      // 10 ab
      CH_GR_ZAEAE;
      CH_GR_AB;
      setHours(hours, false, language, matrix);
      break;
    case 3:
      // viertel ab
      CH_GR_VIERTU;
      CH_GR_AB;
      setHours(hours, false, language, matrix);
      break;
    case 4:
      // 20 ab
      CH_GR_ZWAENZG;
      CH_GR_AB;
      setHours(hours, false, language, matrix);
      break;
    case 5:
      // 5 vor halb
      CH_GR_FUEF;
      CH_GR_VOR;
      CH_GR_HAUBI;
      setHours(hours + 1, false, language, matrix);
      break;
    case 6:
      // halb
      CH_GR_HAUBI;
      setHours(hours + 1, false, language, matrix);
      break;
    case 7:
      // 5 ab halb
      CH_GR_FUEF;
      CH_GR_AB;
      CH_GR_HAUBI;
      setHours(hours + 1, false, language, matrix);
      break;
    case 8:
      // 20 vor
      CH_GR_ZWAENZG;
      CH_GR_VOR;
      setHours(hours + 1, false, language, matrix);
      break;
    case 9:
      // viertel vor
      CH_GR_VIERTU;
      CH_GR_VOR;
      setHours(hours + 1, false, language, matrix);
      break;
    case 10:
      // 10 vor
      CH_GR_ZAEAE;
      CH_GR_VOR;
      setHours(hours + 1, false, language, matrix);
      break;
    case 11:
      // 5 vor
      CH_GR_FUEF;
      CH_GR_VOR;
      setHours(hours + 1, false, language, matrix);
      break;
    default:
      ;
    }
    if (chGsi && minutes % 5) CH_GR_GSI;
    break;
  case FRONTCOVER_CH_GR_2:
    CH_GR_2_ESISCH;
    switch (minutes / 5)
    {
    case 0:
      // glatte Stunde
      setHours(hours, true, language, matrix);
      break;
    case 1:
      // 5 ab
      CH_GR_2_FUEF;
      CH_GR_2_AB;
      setHours(hours, false, language, matrix);
      break;
    case 2:
      // 10 ab
      CH_GR_2_ZAEAE;
      CH_GR_2_AB;
      setHours(hours, false, language, matrix);
      break;
    case 3:
      // viertel ab
      CH_GR_2_VIERTU;
      CH_GR_2_AB;
      setHours(hours, false, language, matrix);
      break;
    case 4:
      // 20 ab
      CH_GR_2_ZWAENZG;
      CH_GR_2_AB;
      setHours(hours, false, language, matrix);
      break;
    case 5:
      // 5 vor halb
      CH_GR_2_FUEF;
      CH_GR_2_VOR;
      CH_GR_2_HAUBI;
      setHours(hours + 1, false, language, matrix);
      break;
    case 6:
      // halb
      CH_GR_2_HAUBI;
      setHours(hours + 1, false, language, matrix);
      break;
    case 7:
      // 5 ab halb
      CH_GR_2_FUEF;
      CH_GR_2_AB;
      CH_GR_2_HAUBI;
      setHours(hours + 1, false, language, matrix);
      break;
    case 8:
      // 20 vor
      CH_GR_2_ZWAENZG;
      CH_GR_2_VOR;
      setHours(hours + 1, false, language, matrix);
      break;
    case 9:
      // viertel vor
      CH_GR_2_VIERTU;
      CH_GR_2_VOR;
      setHours(hours + 1, false, language, matrix);
      break;
    case 10:
      // 10 vor
      CH_GR_2_ZAEAE;
      CH_GR_2_VOR;
      setHours(hours + 1, false, language, matrix);
      break;
    case 11:
      // 5 vor
      CH_GR_2_FUEF;
      CH_GR_2_VOR;
      setHours(hours + 1, false, language, matrix);
      break;
    default:
      ;
    }
    if (chGsi && minutes % 5) CH_GR_2_GSI;
    break; 
  case FRONTCOVER_CH_VS:
    CH_VS_ESISCH;
    switch (minutes / 5)
    {
    case 0:
      // glatte Stunde
      setHours(hours, true, language, matrix);
      break;
    case 1:
      // 5 ab
      CH_VS_FUEF;
      CH_VS_AB;
      setHours(hours, false, language, matrix);
      break;
    case 2:
      // 10 ab
      CH_VS_ZAEAE;
      CH_VS_AB;
      setHours(hours, false, language, matrix);
      break;
    case 3:
      // viertel ab
      CH_VS_VIERTU;
      CH_VS_AB;
      setHours(hours, false, language, matrix);
      break;
    case 4:
      // 20 ab
      CH_VS_ZWAENZG;
      CH_VS_AB;
      setHours(hours, false, language, matrix);
      break;
    case 5:
      // 5 vor halb
      CH_VS_FUEF;
      CH_VS_VOR;
      CH_VS_HAUBI;
      setHours(hours + 1, false, language, matrix);
      break;
    case 6:
      // halb
      CH_VS_HAUBI;
      setHours(hours + 1, false, language, matrix);
      break;
    case 7:
      // 5 ab halb
      CH_VS_FUEF;
      CH_VS_AB;
      CH_VS_HAUBI;
      setHours(hours + 1, false, language, matrix);
      break;
    case 8:
      // 20 vor
      CH_VS_ZWAENZG;
      CH_VS_VOR;
      setHours(hours + 1, false, language, matrix);
      break;
    case 9:
      // viertel vor
      CH_VS_VIERTU;
      CH_VS_VOR;
      setHours(hours + 1, false, language, matrix);
      break;
    case 10:
      // 10 vor
      CH_VS_ZAEAE;
      CH_VS_VOR;
      setHours(hours + 1, false, language, matrix);
      break;
    case 11:
      // 5 vor
      CH_VS_FUEF;
      CH_VS_VOR;
      setHours(hours + 1, false, language, matrix);
      break;
    default:
      ;
    }
    if (chGsi && minutes % 5) CH_VS_GSI;
    break; 
  case FRONTCOVER_CH_BS:
    CH_BS_ESISCH;
    switch (minutes / 5)
    {
    case 0:
      // glatte Stunde
      setHours(hours, true, language, matrix);
      break;
    case 1:
      // 5 ab
      CH_BS_FUEF;
      CH_BS_AB;
      setHours(hours, false, language, matrix);
      break;
    case 2:
      // 10 ab
      CH_BS_ZAEAE;
      CH_BS_AB;
      setHours(hours, false, language, matrix);
      break;
    case 3:
      // viertel ab
      CH_BS_VIERTU;
      CH_BS_AB;
      setHours(hours, false, language, matrix);
      break;
    case 4:
      // 20 ab
      CH_BS_ZWAENZG;
      CH_BS_AB;
      setHours(hours, false, language, matrix);
      break;
    case 5:
      // 5 vor halb
      CH_BS_FUEF;
      CH_BS_VOR;
      CH_BS_HAUBI;
      setHours(hours + 1, false, language, matrix);
      break;
    case 6:
      // halb
      CH_BS_HAUBI;
      setHours(hours + 1, false, language, matrix);
      break;
    case 7:
      // 5 ab halb
      CH_BS_FUEF;
      CH_BS_AB;
      CH_BS_HAUBI;
      setHours(hours + 1, false, language, matrix);
      break;
    case 8:
      // 20 vor
      CH_BS_ZWAENZG;
      CH_BS_VOR;
      setHours(hours + 1, false, language, matrix);
      break;
    case 9:
      // viertel vor
      CH_BS_VIERTU;
      CH_BS_VOR;
      setHours(hours + 1, false, language, matrix);
      break;
    case 10:
      // 10 vor
      CH_BS_ZAEAE;
      CH_BS_VOR;
      setHours(hours + 1, false, language, matrix);
      break;
    case 11:
      // 5 vor
      CH_BS_FUEF;
      CH_BS_VOR;
      setHours(hours + 1, false, language, matrix);
      break;
    default:
      ;
    }
    if (chGsi && minutes % 5) CH_BS_GSI;
    break; 
  case FRONTCOVER_EN:
    EN_ITIS;
    switch (minutes / 5)
    {
    case 0:
      // glatte Stunde
      setHours(hours, true, language, matrix);
      break;
    case 1:
      // 5 nach
      EN_FIVE;
      EN_PAST;
      setHours(hours, false, language, matrix);
      break;
    case 2:
      // 10 nach
      EN_TEN;
      EN_PAST;
      setHours(hours, false, language, matrix);
      break;
    case 3:
      // viertel nach
      EN_A;
      EN_QUATER;
      EN_PAST;
      setHours(hours, false, language, matrix);
      break;
    case 4:
      // 20 nach
      EN_TWENTY;
      EN_PAST;
      setHours(hours, false, language, matrix);
      break;
    case 5:
      // 5 vor halb
      EN_TWENTY;
      EN_FIVE;
      EN_PAST;
      setHours(hours, false, language, matrix);
      break;
    case 6:
      // halb
      EN_HALF;
      EN_PAST;
      setHours(hours, false, language, matrix);
      break;
    case 7:
      // 5 nach halb
      EN_TWENTY;
      EN_FIVE;
      EN_TO;
      setHours(hours + 1, false, language, matrix);
      break;
    case 8:
      // 20 vor
      EN_TWENTY;
      EN_TO;
      setHours(hours + 1, false, language, matrix);
      break;
    case 9:
      // viertel vor
      EN_A;
      EN_QUATER;
      EN_TO;
      setHours(hours + 1, false, language, matrix);
      break;
    case 10:
      // 10 vor
      EN_TEN;
      EN_TO;
      setHours(hours + 1, false, language, matrix);
      break;
    case 11:
      // 5 vor
      EN_FIVE;
      EN_TO;
      setHours(hours + 1, false, language, matrix);
      break;
    }
    break;
  case FRONTCOVER_ES:
    switch (minutes / 5)
    {
    case 0:
      // glatte Stunde
      ES_hours(hours, matrix);
      setHours(hours, false, language, matrix);
      break;
    case 1:
      // 5 nach
      ES_Y;
      ES_CINCO;
      ES_hours(hours, matrix);
      setHours(hours, false, language, matrix);
      break;
    case 2:
      // 10 nach
      ES_Y;
      ES_DIEZ;
      ES_hours(hours, matrix);
      setHours(hours, false, language, matrix);
      break;
    case 3:
      // viertel nach
      ES_Y;
      ES_CUARTO;
      ES_hours(hours, matrix);
      setHours(hours, false, language, matrix);
      break;
    case 4:
      // 20 nach
      ES_Y;
      ES_VEINTE;
      ES_hours(hours, matrix);
      setHours(hours, false, language, matrix);
      break;
    case 5:
      // 5 vor halb
      ES_Y;
      ES_VEINTICINCO;
      ES_hours(hours, matrix);
      setHours(hours, false, language, matrix);
      break;
    case 6:
      // halb
      ES_Y;
      ES_MEDIA;
      ES_hours(hours, matrix);
      setHours(hours, false, language, matrix);
      break;
    case 7:
      // 5 nach halb
      ES_MENOS;
      ES_VEINTICINCO;
      ES_hours(hours + 1, matrix);
      setHours(hours + 1, false, language, matrix);
      break;
    case 8:
      // 20 vor
      ES_MENOS;
      ES_VEINTE;
      ES_hours(hours + 1, matrix);
      setHours(hours + 1, false, language, matrix);
      break;
    case 9:
      // viertel vor
      ES_MENOS;
      ES_CUARTO;
      ES_hours(hours + 1, matrix);
      setHours(hours + 1, false, language, matrix);
      break;
    case 10:
      // 10 vor
      ES_MENOS;
      ES_DIEZ;
      ES_hours(hours + 1, matrix);
      setHours(hours + 1, false, language, matrix);
      break;
    case 11:
      // 5 vor
      ES_MENOS;
      ES_CINCO;
      ES_hours(hours + 1, matrix);
      setHours(hours + 1, false, language, matrix);
      break;
    }
    break;
  case FRONTCOVER_FR:
    FR_ILEST;
    switch (minutes / 5)
    {
    case 0:
      // glatte Stunde
      setHours(hours, true, language, matrix);
      FR_hours(hours, matrix);
      break;
    case 1:
      // 5 nach
      setHours(hours, false, language, matrix);
      FR_hours(hours, matrix);
      FR_CINQ;
      break;
    case 2:
      // 10 nach
      setHours(hours, false, language, matrix);
      FR_hours(hours, matrix);
      FR_DIX;
      break;
    case 3:
      // viertel nach
      setHours(hours, false, language, matrix);
      FR_hours(hours, matrix);
      FR_ET;
      FR_QUART;
      break;
    case 4:
      // 20 nach
      setHours(hours, false, language, matrix);
      FR_hours(hours, matrix);
      FR_VINGT;
      break;
    case 5:
      // 5 vor halb
      setHours(hours, false, language, matrix);
      FR_hours(hours, matrix);
      FR_VINGT;
      FR_TRAIT;
      FR_CINQ;
      break;
    case 6:
      // halb
      setHours(hours, false, language, matrix);
      FR_hours(hours, matrix);
      FR_ET;
      FR_DEMIE;
      break;
    case 7:
      // 5 nach halb
      setHours(hours + 1, false, language, matrix);
      FR_hours(hours + 1, matrix);
      FR_MOINS;
      FR_VINGT;
      FR_TRAIT;
      FR_CINQ;
      break;
    case 8:
      // 20 vor
      setHours(hours + 1, false, language, matrix);
      FR_hours(hours + 1, matrix);
      FR_MOINS;
      FR_VINGT;
      break;
    case 9:
      // viertel vor
      setHours(hours + 1, false, language, matrix);
      FR_hours(hours + 1, matrix);
      FR_MOINS;
      FR_LE;
      FR_QUART;
      break;
    case 10:
      // 10 vor
      setHours(hours + 1, false, language, matrix);
      FR_hours(hours + 1, matrix);
      FR_MOINS;
      FR_DIX;
      break;
    case 11:
      // 5 vor
      setHours(hours + 1, false, language, matrix);
      FR_hours(hours + 1, matrix);
      FR_MOINS;
      FR_CINQ;
      break;
    }
    break;
  case FRONTCOVER_IT:
    switch (minutes / 5)
    {
    case 0:
      // glatte Stunde
      setHours(hours, true, language, matrix);
      IT_hours(hours, matrix);
      break;
    case 1:
      // 5 nach
      IT_E2;
      IT_CINQUE;
      setHours(hours, false, language, matrix);
      IT_hours(hours, matrix);
      break;
    case 2:
      // 10 nach
      IT_E2;
      IT_DIECI;
      setHours(hours, false, language, matrix);
      IT_hours(hours, matrix);
      break;
    case 3:
      // viertel nach
      IT_E2;
      IT_UN;
      IT_QUARTO;
      setHours(hours, false, language, matrix);
      IT_hours(hours, matrix);
      break;
    case 4:
      // 20 nach
      IT_E2;
      IT_VENTI;
      setHours(hours, false, language, matrix);
      IT_hours(hours, matrix);
      break;
    case 5:
      // 5 vor halb
      IT_E2;
      IT_VENTI;
      IT_CINQUE;
      setHours(hours, false, language, matrix);
      IT_hours(hours, matrix);
      break;
    case 6:
      // halb
      IT_E2;
      IT_MEZZA;
      setHours(hours, false, language, matrix);
      IT_hours(hours, matrix);
      break;
    case 7:
      // 5 nach halb
      IT_MENO;
      IT_VENTI;
      IT_CINQUE;
      setHours(hours + 1, false, language, matrix);
      IT_hours(hours + 1, matrix);
      break;
    case 8:
      // 20 vor
      IT_MENO;
      IT_VENTI;
      setHours(hours + 1, false, language, matrix);
      IT_hours(hours + 1, matrix);
      break;
    case 9:
      // viertel vor
      IT_MENO;
      IT_UN;
      IT_QUARTO;
      setHours(hours + 1, false, language, matrix);
      IT_hours(hours + 1, matrix);
      break;
    case 10:
      // 10 vor
      IT_MENO;
      IT_DIECI;
      setHours(hours + 1, false, language, matrix);
      IT_hours(hours + 1, matrix);
      break;
    case 11:
      // 5 vor
      IT_MENO;
      IT_CINQUE;
      setHours(hours + 1, false, language, matrix);
      IT_hours(hours + 1, matrix);
      break;
    }
    break;
  case FRONTCOVER_SD:
    switch (minutes / 5)
    {
    case 0:
      // glatte Stunde
      setHours(hours, true, language, matrix);
      SD_hours(hours, matrix);
      break;
    case 1:
      // 5 nach
      SD_E;
      SD_CHIMBE;
      setHours(hours, false, language, matrix);
      SD_hours(hours, matrix);
      break;
    case 2:
      // 10 nach
      SD_E;
      SD_DEGHE;
      setHours(hours, false, language, matrix);
      SD_hours(hours, matrix);
      break;
    case 3:
      // viertel nach
      SD_E;
      SD_QUASTU;
      setHours(hours, false, language, matrix);
      SD_hours(hours, matrix);
      break;
    case 4:
      // 20 nach
      SD_E;
      SD_VINTI;
      setHours(hours, false, language, matrix);
      SD_hours(hours, matrix);
      break;
    case 5:
      // 5 vor halb
      SD_E;
      SD_VINTI;
      SD_CHIMBE;
      setHours(hours, false, language, matrix);
      SD_hours(hours, matrix);
      break;
    case 6:
      // halb
      SD_E;
      SD_MESA;
      setHours(hours, false, language, matrix);
      SD_hours(hours, matrix);
      break;
    case 7:
      // 5 nach halb
      SD_MANCU;
      SD_VINTI;
      SD_CHIMBE;
      setHours(hours + 1, false, language, matrix);
      SD_hours(hours + 1, matrix);
      break;
    case 8:
      // 20 vor
      SD_MANCU;
      SD_VINTI;
      setHours(hours + 1, false, language, matrix);
      SD_hours(hours + 1, matrix);
      break;
    case 9:
      // viertel vor
      SD_MANCU;
      SD_UNU;
      SD_QUASTU;
      setHours(hours + 1, false, language, matrix);
      SD_hours(hours + 1, matrix);
      break;
    case 10:
      // 10 vor
      SD_MANCU;
      SD_DEGHE;
      setHours(hours + 1, false, language, matrix);
      SD_hours(hours + 1, matrix);
      break;
    case 11:
      // 5 vor
      SD_MANCU;
      SD_CHIMBE;
      setHours(hours + 1, false, language, matrix);
      SD_hours(hours + 1, matrix);
      break;
    }
    break;
  case FRONTCOVER_NL:
    NL_HETIS;
    switch (minutes / 5)
    {
    case 0:
      // glatte Stunde
      setHours(hours, true, language, matrix);
      break;
    case 1:
      // 5 nach
      NL_VIJF;
      NL_OVER;
      setHours(hours, false, language, matrix);
      break;
    case 2:
      // 10 nach
      NL_TIEN;
      NL_OVER;
      setHours(hours, false, language, matrix);
      break;
    case 3:
      // viertel nach
      NL_KWART;
      NL_OVER2;
      setHours(hours, false, language, matrix);
      break;
    case 4:
      // 10 vor halb
      NL_TIEN;
      NL_VOOR;
      NL_HALF;
      setHours(hours + 1, false, language, matrix);
      break;
    case 5:
      // 5 vor halb
      NL_VIJF;
      NL_VOOR;
      NL_HALF;
      setHours(hours + 1, false, language, matrix);
      break;
    case 6:
      // halb
      NL_HALF;
      setHours(hours + 1, false, language, matrix);
      break;
    case 7:
      // 5 nach halb
      NL_VIJF;
      NL_OVER;
      NL_HALF;
      setHours(hours + 1, false, language, matrix);
      break;
    case 8:
      // 20 vor
      NL_TIEN;
      NL_OVER;
      NL_HALF;
      setHours(hours + 1, false, language, matrix);
      break;
    case 9:
      // viertel vor
      NL_KWART;
      NL_VOOR2;
      setHours(hours + 1, false, language, matrix);
      break;
    case 10:
      // 10 vor
      NL_TIEN;
      NL_VOOR;
      setHours(hours + 1, false, language, matrix);
      break;
    case 11:
      // 5 vor
      NL_VIJF;
      NL_VOOR;
      setHours(hours + 1, false, language, matrix);
      break;
    }
    break;
  case FRONTCOVER_SW:
    SW_KLOCKANAER;
    switch (minutes / 5)
    {
    case 0:
      // glatte Stunde
      setHours(hours, true,language, matrix);
      break;
    case 1:
      // 5 nach
      SW_FEM;
      SW_OEVER;
      setHours(hours, false,language, matrix);
      break;
    case 2:
      // 10 nach
      SW_TIO;
      SW_OEVER;
      setHours(hours, false,language, matrix);
      break;
    case 3:
      // viertel nach
      SW_KVART;
      SW_OEVER;
      setHours(hours, false,language, matrix);
      break;
    case 4:
      // zwanzig nach
      SW_TJUGO;
      SW_OEVER;
      setHours(hours , false,language, matrix);
      break;
    case 5:
      // 5 vor halb
      SW_FEM;
      SW_I1;
      SW_HALV;
      setHours(hours + 1, false,language, matrix);
      break;
    case 6:
      // halb
      SW_HALV;
      setHours(hours + 1, false,language, matrix);
      break;
    case 7:
      // 5 nach halb
      SW_FEM;
      SW_OEVER;
      SW_HALV;
      setHours(hours + 1, false,language, matrix);
      break;
    case 8:
      // 20 vor
      SW_TJUGO;
      SW_I2;
      setHours(hours + 1, false,language, matrix);
      break;
    case 9:
      // viertel vor
      SW_KVART;
      SW_I3;
      setHours(hours + 1, false,language, matrix);
      break;
    case 10:
      // 10 vor
      SW_TIO;
      SW_I4;
      setHours(hours + 1, false,language, matrix);
      break;
    case 11:
      // 5 vor
      SW_FEM;
      SW_I1;
      setHours(hours + 1, false,language, matrix);
      break;
    }
    break;
  default:
    ;
  }
}

// Write hours to screenbuffer.
void Renderer::setHours(uint8_t hours, boolean glatt, uint8_t language, uint16_t matrix[])
{
  switch (language)
  {
  case FRONTCOVER_DE_DE:
  case FRONTCOVER_DE_SW:
  case FRONTCOVER_DE_BA:
  case FRONTCOVER_DE_SA:
    if (glatt)
      DE_UHR;
    switch (hours)
    {
    case 0:
    case 12:
    case 24:
      DE_H_ZWOELF;
      break;
    case 1:
    case 13:
      if (glatt)
        DE_H_EIN;
      else
        DE_H_EINS;
      break;
    case 2:
    case 14:
      DE_H_ZWEI;
      break;
    case 3:
    case 15:
      DE_H_DREI;
      break;
    case 4:
    case 16:
      DE_H_VIER;
      break;
    case 5:
    case 17:
      DE_H_FUENF;
      break;
    case 6:
    case 18:
      DE_H_SECHS;
      break;
    case 7:
    case 19:
      DE_H_SIEBEN;
      break;
    case 8:
    case 20:
      DE_H_ACHT;
      break;
    case 9:
    case 21:
      DE_H_NEUN;
      break;
    case 10:
    case 22:
      DE_H_ZEHN;
      break;
    case 11:
    case 23:
      DE_H_ELF;
      break;
    default:
      ;
    }
    break;
  case FRONTCOVER_D3:
    switch (hours)
    {
    case 0:
    case 12:
    case 24:
      D3_H_ZWOELFE;
      break;
    case 1:
    case 13:
      D3_H_OISE;
      break;
    case 2:
    case 14:
      D3_H_ZWOIE;
      break;
    case 3:
    case 15:
      D3_H_DREIE;
      break;
    case 4:
    case 16:
      D3_H_VIERE;
      break;
    case 5:
    case 17:
      D3_H_FUENFE;
      break;
    case 6:
    case 18:
      D3_H_SECHSE;
      break;
    case 7:
    case 19:
      D3_H_SIEBNE;
      break;
    case 8:
    case 20:
      D3_H_ACHTE;
      break;
    case 9:
    case 21:
      D3_H_NEUNE;
      break;
    case 10:
    case 22:
      D3_H_ZEHNE;
      break;
    case 11:
    case 23:
      D3_H_ELFE;
      break;
    }
    break;
  case FRONTCOVER_CH_BE:
    switch (hours)
    {
    case 0:
    case 12:
    case 24:
      CH_BE_H_ZWOEUFI;
      break;
    case 1:
    case 13:
      CH_BE_H_EIS;
      break;
    case 2:
    case 14:
      CH_BE_H_ZWOEI;
      break;
    case 3:
    case 15:
      CH_BE_H_DRUE;
      break;
    case 4:
    case 16:
      CH_BE_H_VIER;
      break;
    case 5:
    case 17:
      CH_BE_H_FUEFI;
      break;
    case 6:
    case 18:
      CH_BE_H_SAECHSI;
      break;
    case 7:
    case 19:
      CH_BE_H_SIEBNI;
      break;
    case 8:
    case 20:
      CH_BE_H_ACHTI;
      break;
    case 9:
    case 21:
      CH_BE_H_NUENI;
      break;
    case 10:
    case 22:
      CH_BE_H_ZAENI;
      break;
    case 11:
    case 23:
      CH_BE_H_EUFI;
      break;
    default:
      ;
    }
    break;
  case FRONTCOVER_CH_ZH:
    switch (hours)
    {
    case 0:
    case 12:
    case 24:
      CH_ZH_H_ZWOEUFI;
      break;
    case 1:
    case 13:
      CH_ZH_H_EIS;
      break;
    case 2:
    case 14:
      CH_ZH_H_ZWOEI;
      break;
    case 3:
    case 15:
      CH_ZH_H_DRUE;
      break;
    case 4:
    case 16:
      CH_ZH_H_VIER;
      break;
    case 5:
    case 17:
      CH_ZH_H_FUEFI;
      break;
    case 6:
    case 18:
      CH_ZH_H_SAECHSI;
      break;
    case 7:
    case 19:
      CH_ZH_H_SIEBNI;
      break;
    case 8:
    case 20:
      CH_ZH_H_ACHTI;
      break;
    case 9:
    case 21:
      CH_ZH_H_NUENI;
      break;
    case 10:
    case 22:
      CH_ZH_H_ZAENI;
      break;
    case 11:
    case 23:
      CH_ZH_H_EUFI;
      break;
    default:
      ;
    }
    break;
  case FRONTCOVER_CH_ZH_2:
    switch (hours)
    {
    case 0:
    case 12:
    case 24:
      CH_ZH_2_H_ZWOEUFI;
      break;
    case 1:
    case 13:
      CH_ZH_2_H_EIS;
      break;
    case 2:
    case 14:
      CH_ZH_2_H_ZWOEI;
      break;
    case 3:
    case 15:
      CH_ZH_2_H_DRUE;
      break;
    case 4:
    case 16:
      CH_ZH_2_H_VIER;
      break;
    case 5:
    case 17:
      CH_ZH_2_H_FUEFI;
      break;
    case 6:
    case 18:
      CH_ZH_2_H_SAECHSI;
      break;
    case 7:
    case 19:
      CH_ZH_2_H_SIEBNI;
      break;
    case 8:
    case 20:
      CH_ZH_2_H_ACHTI;
      break;
    case 9:
    case 21:
      CH_ZH_2_H_NUENI;
      break;
    case 10:
    case 22:
      CH_ZH_2_H_ZAENI;
      break;
    case 11:
    case 23:
      CH_ZH_2_H_EUFI;
      break;
    default:
      ;
    }
    break;
  case FRONTCOVER_CH_AG:
    switch (hours)
    {
    case 0:
    case 12:
    case 24:
      CH_AG_H_ZWOEUFI;
      break;
    case 1:
    case 13:
      CH_AG_H_EIS;
      break;
    case 2:
    case 14:
      CH_AG_H_ZWOEI;
      break;
    case 3:
    case 15:
      CH_AG_H_DRUE;
      break;
    case 4:
    case 16:
      CH_AG_H_VIER;
      break;
    case 5:
    case 17:
      CH_AG_H_FUEFI;
      break;
    case 6:
    case 18:
      CH_AG_H_SAECHSI;
      break;
    case 7:
    case 19:
      CH_AG_H_SIEBNI;
      break;
    case 8:
    case 20:
      CH_AG_H_ACHTI;
      break;
    case 9:
    case 21:
      CH_AG_H_NUENI;
      break;
    case 10:
    case 22:
      CH_AG_H_ZAENI;
      break;
    case 11:
    case 23:
      CH_AG_H_EUFI;
      break;
    default:
      ;
    }
    break;
  case FRONTCOVER_CH_GR:
    switch (hours)
    {
    case 0:
    case 12:
    case 24:
      CH_GR_H_ZWOEUFI;
      break;
    case 1:
    case 13:
      CH_GR_H_EIS;
      break;
    case 2:
    case 14:
      CH_GR_H_ZWOEI;
      break;
    case 3:
    case 15:
      CH_GR_H_DRUE;
      break;
    case 4:
    case 16:
      CH_GR_H_VIER;
      break;
    case 5:
    case 17:
      CH_GR_H_FUEFI;
      break;
    case 6:
    case 18:
      CH_GR_H_SAECHSI;
      break;
    case 7:
    case 19:
      CH_GR_H_SIEBNI;
      break;
    case 8:
    case 20:
      CH_GR_H_ACHTI;
      break;
    case 9:
    case 21:
      CH_GR_H_NUENI;
      break;
    case 10:
    case 22:
      CH_GR_H_ZAENI;
      break;
    case 11:
    case 23:
      CH_GR_H_EUFI;
      break;
    default:
      ;
    }
    break;
  case FRONTCOVER_CH_GR_2:
    switch (hours)
    {
    case 0:
    case 12:
    case 24:
      CH_GR_2_H_ZWOEUFI;
      break;
    case 1:
    case 13:
      CH_GR_2_H_EIS;
      break;
    case 2:
    case 14:
      CH_GR_2_H_ZWOEI;
      break;
    case 3:
    case 15:
      CH_GR_2_H_DRUE;
      break;
    case 4:
    case 16:
      CH_GR_2_H_VIER;
      break;
    case 5:
    case 17:
      CH_GR_2_H_FUEFI;
      break;
    case 6:
    case 18:
      CH_GR_2_H_SAECHSI;
      break;
    case 7:
    case 19:
      CH_GR_2_H_SIEBNI;
      break;
    case 8:
    case 20:
      CH_GR_2_H_ACHTI;
      break;
    case 9:
    case 21:
      CH_GR_2_H_NUENI;
      break;
    case 10:
    case 22:
      CH_GR_2_H_ZAENI;
      break;
    case 11:
    case 23:
      CH_GR_2_H_EUFI;
      break;
    default:
      ;
    }
    break;
  case FRONTCOVER_CH_VS:
    switch (hours)
    {
    case 0:
    case 12:
    case 24:
      CH_VS_H_ZWOEUFI;
      break;
    case 1:
    case 13:
      CH_VS_H_EIS;
      break;
    case 2:
    case 14:
      CH_VS_H_ZWOEI;
      break;
    case 3:
    case 15:
      CH_VS_H_DRUE;
      break;
    case 4:
    case 16:
      CH_VS_H_VIER;
      break;
    case 5:
    case 17:
      CH_VS_H_FUEFI;
      break;
    case 6:
    case 18:
      CH_VS_H_SAECHSI;
      break;
    case 7:
    case 19:
      CH_VS_H_SIEBNI;
      break;
    case 8:
    case 20:
      CH_VS_H_ACHTI;
      break;
    case 9:
    case 21:
      CH_VS_H_NUENI;
      break;
    case 10:
    case 22:
      CH_VS_H_ZAENI;
      break;
    case 11:
    case 23:
      CH_VS_H_EUFI;
      break;
    default:
      ;
    }
    break;
  case FRONTCOVER_CH_BS:
    switch (hours)
    {
    case 0:
    case 12:
    case 24:
      CH_BS_H_ZWOEUFI;
      break;
    case 1:
    case 13:
      CH_BS_H_EIS;
      break;
    case 2:
    case 14:
      CH_BS_H_ZWOEI;
      break;
    case 3:
    case 15:
      CH_BS_H_DRUE;
      break;
    case 4:
    case 16:
      CH_BS_H_VIER;
      break;
    case 5:
    case 17:
      CH_BS_H_FUEFI;
      break;
    case 6:
    case 18:
      CH_BS_H_SAECHSI;
      break;
    case 7:
    case 19:
      CH_BS_H_SIEBNI;
      break;
    case 8:
    case 20:
      CH_BS_H_ACHTI;
      break;
    case 9:
    case 21:
      CH_BS_H_NUENI;
      break;
    case 10:
    case 22:
      CH_BS_H_ZAENI;
      break;
    case 11:
    case 23:
      CH_BS_H_EUFI;
      break;
    default:
      ;
    }
    break;
  case FRONTCOVER_EN:
    if (glatt)
      EN_OCLOCK;
    switch (hours)
    {
    case 0:
    case 12:
    case 24:
      EN_H_TWELVE;
      break;
    case 1:
    case 13:
      EN_H_ONE;
      break;
    case 2:
    case 14:
      EN_H_TWO;
      break;
    case 3:
    case 15:
      EN_H_THREE;
      break;
    case 4:
    case 16:
      EN_H_FOUR;
      break;
    case 5:
    case 17:
      EN_H_FIVE;
      break;
    case 6:
    case 18:
      EN_H_SIX;
      break;
    case 7:
    case 19:
      EN_H_SEVEN;
      break;
    case 8:
    case 20:
      EN_H_EIGHT;
      break;
    case 9:
    case 21:
      EN_H_NINE;
      break;
    case 10:
    case 22:
      EN_H_TEN;
      break;
    case 11:
    case 23:
      EN_H_ELEVEN;
      break;
    }
    break;
  case FRONTCOVER_ES:
    switch (hours)
    {
    case 0:
    case 12:
    case 24:
      ES_H_DOCE;
      break;
    case 1:
    case 13:
      ES_H_UNA;
      break;
    case 2:
    case 14:
      ES_H_DOS;
      break;
    case 3:
    case 15:
      ES_H_TRES;
      break;
    case 4:
    case 16:
      ES_H_CUATRO;
      break;
    case 5:
    case 17:
      ES_H_CINCO;
      break;
    case 6:
    case 18:
      ES_H_SEIS;
      break;
    case 7:
    case 19:
      ES_H_SIETE;
      break;
    case 8:
    case 20:
      ES_H_OCHO;
      break;
    case 9:
    case 21:
      ES_H_NUEVE;
      break;
    case 10:
    case 22:
      ES_H_DIEZ;
      break;
    case 11:
    case 23:
      ES_H_ONCE;
      break;
    }
    break;
  case FRONTCOVER_FR:
    switch (hours)
    {
    case 0:
    case 24:
      FR_H_MINUIT;
      break;
    case 12:
      FR_H_MIDI;
      break;
    case 1:
    case 13:
      FR_H_UNE;
      break;
    case 2:
    case 14:
      FR_H_DEUX;
      break;
    case 3:
    case 15:
      FR_H_TROIS;
      break;
    case 4:
    case 16:
      FR_H_QUATRE;
      break;
    case 5:
    case 17:
      FR_H_CINQ;
      break;
    case 6:
    case 18:
      FR_H_SIX;
      break;
    case 7:
    case 19:
      FR_H_SEPT;
      break;
    case 8:
    case 20:
      FR_H_HUIT;
      break;
    case 9:
    case 21:
      FR_H_NEUF;
      break;
    case 10:
    case 22:
      FR_H_DIX;
      break;
    case 11:
    case 23:
      FR_H_ONZE;
      break;
    }
    break;
  case FRONTCOVER_IT:
    switch (hours)
    {
    case 0:
    case 12:
    case 24:
      IT_H_DODICI;
      break;
    case 1:
    case 13:
      IT_H_LUNA;
      break;
    case 2:
    case 14:
      IT_H_DUE;
      break;
    case 3:
    case 15:
      IT_H_TRE;
      break;
    case 4:
    case 16:
      IT_H_QUATTRO;
      break;
    case 5:
    case 17:
      IT_H_CINQUE;
      break;
    case 6:
    case 18:
      IT_H_SEI;
      break;
    case 7:
    case 19:
      IT_H_SETTE;
      break;
    case 8:
    case 20:
      IT_H_OTTO;
      break;
    case 9:
    case 21:
      IT_H_NOVE;
      break;
    case 10:
    case 22:
      IT_H_DIECI;
      break;
    case 11:
    case 23:
      IT_H_UNDICI;
      break;
    }
    break;
  case FRONTCOVER_SD:
    switch (hours)
    {
    case 0:
    case 12:
    case 24:
      SD_H_DOIGHI;
      break;
    case 1:
    case 13:
      SD_H_UNA;
      break;
    case 2:
    case 14:
      SD_H_DUAS;
      break;
    case 3:
    case 15:
      SD_H_TRES;
      break;
    case 4:
    case 16:
      SD_H_BATTORO;
      break;
    case 5:
    case 17:
      SD_H_CHIMBE;
      break;
    case 6:
    case 18:
      SD_H_SESE;
      break;
    case 7:
    case 19:
      SD_H_SETTE;
      break;
    case 8:
    case 20:
      SD_H_OTTO;
      break;
    case 9:
    case 21:
      SD_H_NOE;
      break;
    case 10:
    case 22:
      SD_H_DEGHE;
      break;
    case 11:
    case 23:
      SD_H_UNDIGHI;
      break;
    }
    break;
  case FRONTCOVER_NL:
    if (glatt)
      NL_UUR;
    switch (hours)
    {
    case 0:
    case 12:
    case 24:
      NL_H_TWAALF;
      break;
    case 1:
    case 13:
      NL_H_EEN;
      break;
    case 2:
    case 14:
      NL_H_TWEE;
      break;
    case 3:
    case 15:
      NL_H_DRIE;
      break;
    case 4:
    case 16:
      NL_H_VIER;
      break;
    case 5:
    case 17:
      NL_H_VIJF;
      break;
    case 6:
    case 18:
      NL_H_ZES;
      break;
    case 7:
    case 19:
      NL_H_ZEVEN;
      break;
    case 8:
    case 20:
      NL_H_ACHT;
      break;
    case 9:
    case 21:
      NL_H_NEGEN;
      break;
    case 10:
    case 22:
      NL_H_TIEN;
      break;
    case 11:
    case 23:
      NL_H_ELF;
      break;
    }
    break;
  case FRONTCOVER_SW:
    switch (hours) {
    case 0:
    case 12:
    case 24:
      SW_H_TOLV;
      break;
    case 1:
    case 13:
      SW_H_ETT;
      break;
    case 2:
    case 14:
      SW_H_TVA;
      break;
    case 3:
    case 15:
      SW_H_TRE;
      break;
    case 4:
    case 16:
      SW_H_FYRA;
      break;
    case 5:
    case 17:
      SW_H_FEM;
      break;
    case 6:
    case 18:
      SW_H_SEX;
      break;
    case 7:
    case 19:
      SW_H_SJU;
      break;
    case 8:
    case 20:
      SW_H_ATTA;
      break;
    case 9:
    case 21:
      SW_H_NIO;
      break;
    case 10:
    case 22:
      SW_H_TIO;
      break;
    case 11:
    case 23:
      SW_H_ELVA;
      break;
    }
    break;
  default:
    ;
  }
}

// Clear entry words.
void Renderer::clearEntryWords(uint8_t language, uint16_t matrix[])
{
  switch (language)
  {
  case FRONTCOVER_DE_DE:
  case FRONTCOVER_DE_SW:
  case FRONTCOVER_DE_BA:
  case FRONTCOVER_DE_SA:
    DE_ESIST_R; // remove ES IST
    break;
  case FRONTCOVER_D3:
    D3_ESISCH_R; // remove ISCH
    break;
  case FRONTCOVER_CH_BE:
    CH_BE_ESISCH_R; // remove ES ISCH
    break;
  case FRONTCOVER_CH_ZH:
    CH_ZH_ESISCH_R; // remove ES ISCH
    break;
  case FRONTCOVER_CH_ZH_2:
    CH_ZH_2_ESISCH_R; // remove ES ISCH
    break;
  case FRONTCOVER_CH_AG:
    CH_AG_ESISCH_R; // remove ES ISCH
    break;
  case FRONTCOVER_CH_GR:
    CH_GR_ESISCH_R; // remove ES ISCH
    break;
  case FRONTCOVER_CH_GR_2:
    CH_GR_2_ESISCH_R; // remove ES ISCH
    break;
  case FRONTCOVER_CH_VS:
    CH_VS_ESISCH_R; // remove ES ISCH
    break;
  case FRONTCOVER_CH_BS:
    CH_BS_ESISCH_R; // remove ES ISCH
    break;
  case FRONTCOVER_EN:
    EN_ITIS_R; // remove IT IS
    break;
  case FRONTCOVER_ES:
    ES_SONLAS_R; // remove SON LAS
    ES_ESLA_R; // remove ES LA
    break;
  case FRONTCOVER_FR:
    FR_ILEST_R; // remove IL EST
    break;
  case FRONTCOVER_IT:
    IT_SONOLE_R; // remove SONO LE
    IT_E_R; // remove E (L'UNA)
    break;
  case FRONTCOVER_SD:
    SD_SUNUSAS_R; // remove SUNU SAS
    SD_ESA_R; // remove E SA (UNA)
    break;
  case FRONTCOVER_NL:
    NL_HETIS_R; // remove HET IS
    break;
  case FRONTCOVER_SW:
    SW_KLOCKANAER_R; // remove KLOCKAN AER
    break;
  default:
    ;
  }
}

// Turn on AM or PM.
void Renderer::setAMPM(uint8_t hours, uint8_t language, uint16_t matrix[])
{
  switch (language)
  {
  case FRONTCOVER_DE_DE:
  case FRONTCOVER_DE_SW:
  case FRONTCOVER_DE_BA:
  case FRONTCOVER_DE_SA:
    if (hours < 12) DE_AM;
    else DE_PM;
    break;
  case FRONTCOVER_D3:
    if (hours < 12) D3_AM;
    else D3_PM;
    break;
  case FRONTCOVER_CH_BE:
    if (hours < 12) CH_BE_AM;
    else CH_BE_PM;
    break;
  case FRONTCOVER_CH_ZH:
    if (hours < 12) CH_ZH_AM;
    else CH_ZH_PM;
    break;
  case FRONTCOVER_CH_ZH_2:
    if (hours < 12) CH_ZH_2_AM;
    else CH_ZH_2_PM;
    break;
  case FRONTCOVER_CH_AG:
    if (hours < 12) CH_AG_AM;
    else CH_AG_PM;
    break;
  case FRONTCOVER_CH_GR:
  case FRONTCOVER_CH_GR_2:
    if (hours < 12) CH_GR_AM;
    else CH_GR_PM;
    break;
  case FRONTCOVER_CH_VS:
    if (hours < 12) CH_VS_AM;
    else CH_VS_PM;
    break;
  case FRONTCOVER_CH_BS:
    if (hours < 12) CH_BS_AM;
    else CH_BS_PM;
    break;
  case FRONTCOVER_EN:
    if (hours < 12) EN_AM;
    else EN_PM;
    break;
  case FRONTCOVER_ES:
    if (hours < 12) ES_AM;
    else ES_PM;
    break;
  case FRONTCOVER_FR:
    if (hours < 12) FR_AM;
    else FR_PM;
    break;
  case FRONTCOVER_IT:
    if (hours < 12) IT_AM;
    else IT_PM;
    break;
  case FRONTCOVER_SD:
    if (hours < 12) SD_AM;
    else SD_PM;
    break;
  case FRONTCOVER_NL:
    if (hours < 12) NL_AM;
    else NL_PM;
    break;
  case FRONTCOVER_SW:
    if (hours < 12) SW_AM;
    else SW_PM;
    break;
  default:
    ;
  }
}

// Special case ES.
void Renderer::ES_hours(uint8_t hours, uint16_t matrix[])
{
  if ((hours == 1) || (hours == 13))
    ES_ESLA;
  else
    ES_SONLAS;
}

// Special case FR.
void Renderer::FR_hours(uint8_t hours, uint16_t matrix[])
{
  if ((hours == 1) || (hours == 13))
    FR_HEURE;
  else
    if ((hours == 0) || (hours == 12) || (hours == 24))
      ;
    else
      FR_HEURES;
}

// Special case IT.
void Renderer::IT_hours(uint8_t hours, uint16_t matrix[])
{
  if ((hours != 1) && (hours != 13))
    IT_SONOLE;
  else
    IT_E;
}

// Special case SD.
void Renderer::SD_hours(uint8_t hours, uint16_t matrix[])
{
  if ((hours != 1) && (hours != 13))
    SD_SUNUSAS;
  else
    SD_ESA;
}

// Write minutes to screenbuffer.
void Renderer::setCorners(uint8_t minutes, uint16_t matrix[])
{
  uint8_t b_minutes = minutes % 5;
  for (uint8_t i = 0; i < b_minutes; i++)
    bitSet(matrix[i], 4);
}

// Turn alarm-LED on.
void Renderer::setAlarmLed(uint16_t matrix[])
{
  bitSet(matrix[4], 4);
}

// Turn alarm-LED off.
void Renderer::deactivateAlarmLed(uint16_t matrix[])
{
  bitClear(matrix[4], 4);
}

// Write text to screenbuffer.
void Renderer::setSmallText(String menuText, eTextPos textPos, uint16_t matrix[])
{
  if (menuText.length() == 2)
  {
    for (uint8_t i = 0; i <= 4; i++)
    {
      for (uint8_t j = 0; j < menuText.length(); j++)
      {
        if (!isNumber(menuText[j]))
          matrix[textPos + i] |= letters[menuText[j] - 'A'][i] << (5 + ((j + 1) % 2) * 6);
        else
          matrix[textPos + i] |= numbers[menuText[j] - '0'][i] << (5 + ((j + 1) % 2) * 5);
      }
    }
  }
  else if (menuText.length() == 1)
  {
    for (uint8_t i = 0; i <= 4; i++)
    {
      if (!isNumber(menuText[0]))
        matrix[textPos + i] |= letters[menuText[0] - 'A'][i] << 8;
      else
        matrix[textPos + i] |= numbers[menuText[0] - '0'][i] << 8;
    }
  }
}

// Test for number.
boolean Renderer::isNumber(char symbol)
{
  if ((symbol >= '0') && (symbol <= '9'))
    return true;
  else
    return false;
}

// Write pixel to screenbuffer.
void Renderer::setPixelInScreenBuffer(uint8_t x, uint8_t y, uint16_t matrix[])
{
  bitSet(matrix[y], 15-x);
}

// Remove pixel from screenbuffer.
void Renderer::unsetPixelInScreenBuffer(uint8_t x, uint8_t y, uint16_t matrix[])
{
  bitClear(matrix[y], 15-x);
}

// Clear screenbuffer.
void Renderer::clearScreenBuffer(uint16_t matrix[])
{
  for (uint8_t i = 0; i <= 9; i++)
    matrix[i] = 0b0000000000000000;
}

// Turn all LEDs in screenbuffer on.
void Renderer::setAllScreenBuffer(uint16_t matrix[])
{
  for (uint8_t i = 0; i <= 9; i++)
    matrix[i] = 0b1111111111111111;
}
