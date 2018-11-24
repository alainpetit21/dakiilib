#include "lib.h"
#include "libMacros.h"
#include "libMngGraphic.h"
#include "dfcFont.h"
#include "libMngResIO.h"


const char* arrayMap[256]=
{
	"UNUSED",			/*000 -  */
	"UNUSED",			/*001 -  */
	"UNUSED",			/*002 -  */
	"UNUSED",			/*003 -  */
	"UNUSED",			/*004 -  */
	"UNUSED",			/*005 -  */
	"UNUSED",			/*006 -  */
	"UNUSED",			/*007 -  */
	"UNUSED",			/*008 -  */
	"UNUSED",			/*009 -  */
	"UNUSED",			/*010 -  */
	"UNUSED",			/*011 -  */
	"UNUSED",			/*012 -  */
	"UNUSED",			/*013 -  */
	"UNUSED",			/*014 -  */
	"UNUSED",			/*015 -  */
	"UNUSED",			/*016 -  */
	"UNUSED",			/*017 -  */
	"UNUSED",			/*018 -  */
	"UNUSED",			/*019 -  */
	"UNUSED",			/*020 -  */
	"UNUSED",			/*021 -  */
	"UNUSED",			/*022 -  */
	"UNUSED",			/*023 -  */
	"UNUSED",			/*024 -  */
	"UNUSED",			/*025 -  */
	"UNUSED",			/*026 -  */
	"UNUSED",			/*027 -  */
	"UNUSED",			/*028 -  */
	"UNUSED",			/*029 -  */
	"UNUSED",			/*030 -  */
	"UNUSED",			/*031 -  */
	"SPACE",			/*032 -   */
	"EXCLAM_MARK",		/*033 - ! */
	"ENG_QUOTE",		/*034 - " */
	"SHARP",			/*035 - # */
	"DOLLAR",			/*036 - $ */
	"PERCENT",			/*037 - % */
	"AND",				/*038 - & */
	"SPA_QUOTE",		/*039 - ' */
	"PAR_OPEN",			/*040 - ( */
	"PAR_CLOSE",		/*041 - ) */
	"STAR",				/*042 - * */
	"PLUS",				/*043 - + */
	"COMMA",			/*044 - , */
	"MINUS",			/*045 - - */
	"DOT",				/*046 - . */
	"SLASH",			/*047 - / */
	"0",				/*048 - 0 */
	"1",				/*049 - 1 */
	"2",				/*050 - 2 */
	"3",				/*051 - 3 */
	"4",				/*052 - 4 */
	"5",				/*053 - 5 */
	"6",				/*054 - 6 */
	"7",				/*055 - 7 */
	"8",				/*056 - 8 */
	"9",				/*057 - 9 */
	"TWO_DOT",			/*058 - : */
	"DOT_COMMA",		/*059 - ; */
	"SMALLER",			/*060 - < */
	"EQUAL",			/*061 - = */
	"GREATER",			/*062 - > */
	"QUESTION_MARK",	/*063 - ? */
	"AT",				/*064 - @ */
	"A_CAPS",			/*065 - A */
	"B_CAPS",			/*066 - B */
	"C_CAPS",			/*067 - C */
	"D_CAPS",			/*068 - D */
	"E_CAPS",			/*069 - E */
	"F_CAPS",			/*070 - F */
	"G_CAPS",			/*071 - G */
	"H_CAPS",			/*072 - H */
	"I_CAPS",			/*073 - I */
	"J_CAPS",			/*074 - J */
	"K_CAPS",			/*075 - K */
	"L_CAPS",			/*076 - L */
	"M_CAPS",			/*077 - M */
	"N_CAPS",			/*078 - N */
	"O_CAPS",			/*079 - O */
	"P_CAPS",			/*080 - P */
	"Q_CAPS",			/*081 - Q */
	"R_CAPS",			/*082 - R */
	"S_CAPS",			/*083 - S */
	"T_CAPS",			/*084 - T */
	"U_CAPS",			/*085 - U */
	"V_CAPS",			/*086 - V */
	"W_CAPS",			/*087 - W */
	"X_CAPS",			/*088 - X */
	"Y_CAPS",			/*089 - Y */
	"Z_CAPS",			/*090 - Z */
	"BRACKET_OPEN",		/*091 - [ */
	"BACKSLASH",		/*092 - \ */
	"BRACKET_CLOSE",	/*093 - ] */
	"CIRCONFLEX",		/*094 - ^ */
	"UNDERSCORE",		/*095 - _ */
	"GRAVE",			/*096 - ` */
	"A",				/*097 - a */
	"B",				/*098 - b */
	"C",				/*099 - c */
	"D",				/*100 - d */
	"E",				/*101 - e */
	"F",				/*102 - f */
	"G",				/*103 - g */
	"H",				/*104 - h */
	"I",				/*105 - i */
	"J",				/*106 - j */
	"K",				/*107 - k */
	"L",				/*108 - l */
	"M",				/*109 - m */
	"N",				/*110 - n */
	"O",				/*111 - o */
	"P",				/*112 - p */
	"Q",				/*113 - q */
	"R",				/*114 - r */
	"S",				/*115 - s */
	"T",				/*116 - t */
	"U",				/*117 - u */
	"V",				/*118 - v */
	"W",				/*119 - w */
	"X",				/*120 - x */
	"Y",				/*121 - y */
	"Z",				/*122 - z */
	"BRACKET2_OPEN",	/*123 - { */
	"VERT_LINE",		/*124 - | */
	"BRACKET2_CLOSE",	/*125 - } */
	"TILDE",			/*126 - ~ */
	"UNUSED",			/*127 - Š */
	"C_CEDILLE_CAPS",	/*128 - Ç */
	"U_TREMA",			/*129 - ü */
	"E_AIGUE",			/*130 - é */
	"A_CIRCONFLEX",		/*131 - â */
	"A_TREMA",			/*132 - ä */
	"A_GRAVE",			/*133 - à */
	"A_ROND",			/*134 - å */
	"C_CEDILLE",		/*135 - ç */
	"E_CIRCONFLEX",		/*136 - ê */
	"E_TREMA",			/*137 - ë */
	"E_GRAVE",			/*138 - è */
	"I_TREMA",			/*139 - ï */
	"I_CIRCONFLEX",		/*140 - î */
	"I_GRAVE",			/*141 - ì */
	"A_TREMA_CAPS",		/*142 - Ä */
	"A_ROND_CAPS",		/*143 - Å */
	"E_AIGUE_CAPS",		/*144 - É */
	"AE",				/*145 - æ */
	"AE_CAPS",			/*146 - Æ */
	"O_CIRCONFLEX",		/*147 - ô */
	"O_TREMA",			/*148 - ö */
	"O_GRAVE",			/*149 - ò */
	"U_CIRCONFLEX",		/*150 - û */
	"U_GRAVE",			/*151 - ù */
	"Y_TREMA",			/*152 - ÿ */
	"O_TREMA_CAPS",		/*153 - Ö */
	"U_TREMA_CAPS",		/*154 - Ü */
	"CENTS",			/*155 - ¢ */
	"POUNDS",			/*156 - £ */
	"YENS",				/*157 - ¥ */
	"UNUSED",			/*158 - P */
	"UNUSED",			/*159 -  */
	"A_AIGU",			/*160 - á */
	"I_AIGU",			/*161 - í */
	"O_AIGU",			/*162 - ó */
	"U_AIGU"			/*163 - ú */
	"N_TILDE",			/*164 - ñ */
	"N_TILDE_CAPS",		/*165 - Ñ */
	"UNUSED",			/*166 - ª */
	"DEGREE",			/*167 - º */
	"SPA_QUESTION_MARK",/*168 - ¿ */
	"UNUSED",			/*169 - ¬ */
	"UNUSED",			/*170 - ¬ */
	"HALF",				/*171 - œ */
	"QUARTER",			/*172 - Œ */
	"UNUSED",			/*173 - ¡ */
	"FRE_QUOTE_OPEN",	/*174 - « */
	"FRE_QUOTE_CLOSE",	/*175 - » */
	"UNUSED",			/*176 - Š */
	"UNUSED",			/*177 - Š */
	"UNUSED",			/*178 - Š */
	"UNUSED",			/*179 - Š */
	"UNUSED",			/*180 - Š */
	"UNUSED",			/*181 - Š */
	"UNUSED",			/*182 - Š */
	"UNUSED",			/*183 - + */
	"UNUSED",			/*184 - + */
	"UNUSED",			/*185 - Š */
	"UNUSED",			/*186 - Š */
	"UNUSED",			/*187 - + */
	"UNUSED",			/*188 - + */
	"UNUSED",			/*189 - + */
	"UNUSED",			/*190 - + */
	"UNUSED",			/*191 - + */
	"UNUSED",			/*192 - + */
	"UNUSED",			/*193 - - */
	"UNUSED",			/*194 - - */
	"UNUSED",			/*195 - + */
	"UNUSED",			/*196 - - */
	"UNUSED",			/*197 - + */
	"UNUSED",			/*198 - Š */
	"UNUSED",			/*199 - Š */
	"UNUSED",			/*200 - + */
	"UNUSED",			/*201 - + */
	"UNUSED",			/*202 - - */
	"UNUSED",			/*203 - - */
	"UNUSED",			/*204 - Š */
	"UNUSED",			/*205 - - */
	"UNUSED",			/*206 - + */
	"UNUSED",			/*207 - - */
	"UNUSED",			/*208 - - */
	"UNUSED",			/*209 - - */
	"UNUSED",			/*210 - - */
	"UNUSED",			/*211 - + */
	"UNUSED",			/*212 - + */
	"UNUSED",			/*213 - + */
	"UNUSED",			/*214 - + */
	"UNUSED",			/*215 - + */
	"UNUSED",			/*216 - + */
	"UNUSED",			/*217 - + */
	"UNUSED",			/*218 - + */
	"UNUSED",			/*219 - Š */
	"UNUSED",			/*220 - _ */
	"UNUSED",			/*221 - Š */
	"UNUSED",			/*222 - Š */
	"UNUSED",			/*223 - ¯ */
	"UNUSED",			/*224 - a */
	"UNUSED",			/*225 - ß */
	"UNUSED",			/*226 - G */
	"UNUSED",			/*227 - p */
	"UNUSED",			/*228 - S */
	"UNUSED",			/*229 - s */
	"UNUSED",			/*230 - µ */
	"UNUSED",			/*231 - t */
	"UNUSED",			/*232 - F */
	"UNUSED",			/*233 - T */
	"UNUSED",			/*234 - O */
	"UNUSED",			/*235 - d */
	"UNUSED",			/*236 - 8 */
	"UNUSED",			/*237 - f */
	"UNUSED",			/*238 - e */
	"UNUSED",			/*239 - n */
	"UNUSED",			/*240 - = */
	"UNUSED",			/*241 - ± */
	"UNUSED",			/*242 - = */
	"UNUSED",			/*243 - = */
	"UNUSED",			/*244 - ( */
	"UNUSED",			/*245 - ) */
	"UNUSED",			/*246 - ÷ */
	"UNUSED",			/*247 -  */
	"UNUSED",			/*248 - ° */
	"UNUSED",			/*249 - · */
	"UNUSED",			/*250 - · */
	"UNUSED",			/*251 - v */
	"UNUSED",			/*252 - n */
	"UNUSED",			/*253 - ² */
	"UNUSED",			/*254 - Š */
	"UNUSED"			/*255 -   */
};

CDString*	CFont::m_pAllFontSource[255];
CFont*		CFont::m_pAllFontInstance[255];
u32			CFont::m_nbFontInstalled= 0;


CFont::CFont()
{
}

CFont::~CFont()
{
CMngImageIO	*mngImageIO	= GetMngImageIO();
s32			i;

	for(i= 0; i < 255; ++i)
		mngImageIO->Close(&m_arLetters[i].pData);
}

void
CFont::LoadTGA(const char* p_dataSource, int p_idxDest)
{
CMngImageIO	*mngImageIO	= GetMngImageIO();

    mngImageIO->Open(p_dataSource, &m_arLetters[p_idxDest].pData, &m_arLetters[p_idxDest].nWidth, &m_arLetters[p_idxDest].nHeight, CMngImageIO::SystemMem);
}

void
CFont::LoadInstance(const char* p_dataSource)
{
int i;

	CObj::LoadInstance(p_dataSource);

	m_pXml->FindElem("NB_LETTER");
	m_nbLetters= 255;

	for(i= 0; i < 255; ++i){
		if(strcmp(arrayMap[i], "UNUSED") == 0){
			m_arLetters[i].pData= 0;
			continue;
		}

		CDString chaineTemp= arrayMap[i];
		if(m_pXml->FindElem(chaineTemp.m_arBuffer)){
			m_pXml->IntoElem();{
				m_arLetters[i].nHotSpotX	= 0;
				m_arLetters[i].nHotSpotY	= 0;

				m_pXml->FindElem("FILENAME");
				LoadTGA((m_pXml->GetData()).m_arBuffer, i);

				if(m_pXml->FindElem("HOTSPOT_X"))
					m_arLetters[i].nHotSpotX= atoi((m_pXml->GetData()).m_arBuffer);
				if(m_pXml->FindElem("HOTSPOT_Y"))
					m_arLetters[i].nHotSpotY= atoi((m_pXml->GetData()).m_arBuffer);
			}m_pXml->OutOfElem();
		}else{
			m_arLetters[i].pData= 0;
		}
	}

	CMngImageIO	*mngImageIO	= GetMngImageIO();
	for(i= 0; i < 255; ++i){
		if(m_arLetters[i].pData == 0){
			mngImageIO->Copy(m_arLetters[int(' ')].pData, &m_arLetters[i].pData,  &m_arLetters[i].nWidth, &m_arLetters[i].nHeight);
			m_arLetters[i].nHotSpotX= m_arLetters[int(' ')].nHotSpotX;
			m_arLetters[i].nHotSpotY= m_arLetters[int(' ')].nHotSpotY;
		}
	}

	m_pAllFontSource[m_nbFontInstalled]		= new CDString(p_dataSource);
	m_pAllFontInstance[m_nbFontInstalled]	= this;
	++m_nbFontInstalled;

	SAFE_DELETE(m_pXml);
}

void
CFont::Manage(u32 p_nTimeTick)
{
	CObj::Manage(p_nTimeTick);
}

void
CFont::Update(void)
{
	CObj::Update();
}

void
CFont::PutS(int p_nPosX, int p_nPosY, char* p_str)
{
CMngGraphic	*pRenderer	= GetMngGraphic();
int			x			= p_nPosX;
int			y			= p_nPosY;
char		*ptrText	= p_str;
u32			filterColor	= 0;

	while(*ptrText){
		if(*ptrText == '|'){	//Or the next carac wit color specify
			if(filterColor){
				filterColor= 0;
				++ptrText;
				continue;
			}
			sscanf(ptrText, "|%x|", (unsigned int*)&filterColor);
			++ptrText;
			while(*ptrText++ != '|');
			continue;
		}

		if(*ptrText == '\n'){
			x	= p_nPosX;
			y	+= m_arLetters[int(' ')].nHeight;
			++ptrText;
			continue;
		}

		if(*ptrText == ' '){
			x+= m_arLetters[int(' ')].nWidth;
			++ptrText;
			continue;
		}

		if(*ptrText == '\r'){
			y	+= m_arLetters[int(' ')].nHeight;
			++ptrText;
			continue;
		}

		if(filterColor){
			void	*pBitmap= m_arLetters[int(*ptrText)].pData;
			u32		*pData	= (u32 *)pRenderer->LockSurface(pBitmap);
			int		nW		= m_arLetters[int(*ptrText)].nWidth;
			int		nH		= m_arLetters[int(*ptrText)].nHeight;

			if(pRenderer->GetPixelMode() == 32){
				u32	nPitch= pRenderer->GetPitch(pBitmap)>>2;

				for(int i= 0; i < nW; ++i){
					for(int j= 0; j < nH; ++j){
						if(pData[j*nPitch+i] == 0xFFFFFFFF)
							pData[j*nPitch+i]= filterColor;
					}
				}
				pRenderer->UnLockSurface(pBitmap, pData);
			}
		}


		pRenderer->DrawFrame(0, s32(x), s32(y), 0, 1.0f, 0, &m_arLetters[int(*ptrText)]);

		if(filterColor){
			void	*pBitmap= m_arLetters[int(*ptrText)].pData;
			u32		*pData	= (u32 *)pRenderer->LockSurface(pBitmap);
			int		nW		= m_arLetters[int(*ptrText)].nWidth;
			int		nH		= m_arLetters[int(*ptrText)].nHeight;

			if(pRenderer->GetPixelMode() == 32){
				u32	nPitch= pRenderer->GetPitch(pBitmap)>>2;

				for(int i= 0; i < nW; ++i){
					for(int j= 0; j < nH; ++j){
						if(pData[j*nPitch+i] == filterColor)
							pData[j*nPitch+i]= 0xFFFFFFFF;
					}
				}
				pRenderer->UnLockSurface(pBitmap, pData);
			}
		}

			x+= m_arLetters[int(*ptrText)].nWidth;
		++ptrText;
	}
}


CFont*
CFont::Find(const char* p_dataSource)
{


	//Search for an existing instance
	for(u32 i= 0; i < CFont::m_nbFontInstalled; ++i)
		if(*CFont::m_pAllFontSource[i] == p_dataSource)
			return CFont::m_pAllFontInstance[i];

	//Else install the new font
	CFont *m_pNewFont= new CFont;
	m_pNewFont->LoadInstance(p_dataSource);

	return m_pNewFont;
}

u32
CFont::GetTextWidth(const char* p_szText)
{
	u32 widthCarac= strlen(p_szText);
	u32 widthPixel= 0;

	for(u32 i= 0; i < widthCarac; ++i)
		widthPixel+= m_arLetters[int(p_szText[i])].nWidth;

	return widthPixel;
}
