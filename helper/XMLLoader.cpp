#include "lib.h"
#include "libMacros.h"
#include "XMLLoader.h"
#include "DString.h"

////////////////////////
CXml::CXml(void)
{
	m_aPos= 0;
	m_nPosArrayLenght= 0;
	SetDoc(0);
}

CXml::CXml(const char* szDoc)
{
	m_aPos= 0;
	m_nPosArrayLenght= 0;
	SetDoc(szDoc);
}

CXml::CXml(const CXml& markup)
{
	m_aPos= 0;
	m_nPosArrayLenght= 0;
	*this = markup;
}

CXml::~CXml()
{
	SAFE_DELETE(m_aPos);
}

void
CXml::operator=(const CXml& markup)
{
	m_iPosParent= markup.m_iPosParent;
	m_iPos		= markup.m_iPos;
	m_iPosChild	= markup.m_iPosChild;
	m_iPosFree	= markup.m_iPosFree;
	m_nNodeType	= markup.m_nNodeType;

	if(m_nPosArrayLenght){
		delete []m_aPos;
		m_nPosArrayLenght= 0;
	}

	m_aPos= new ElemPos[markup.m_nPosArrayLenght];
	m_nPosArrayLenght= markup.m_nPosArrayLenght;
	memcpy(m_aPos, markup.m_aPos, sizeof(ElemPos) * m_nPosArrayLenght);


	m_csDoc= markup.m_csDoc;
}

bool
CXml::SetDoc(const char* szDoc)
{
	// Reset indexes
	m_iPosFree= 1;
	ResetPos();

	// Set document text
	if(szDoc)
		m_csDoc= szDoc;
	else
		m_csDoc.Empty();

	// Starting size of position array: 1 element per 64 bytes of document
	// Tight fit when parsing small doc, only 0 to 2 reallocs when parsing large doc
	// Start at 8 when creating new document
	int nStartSize= m_csDoc.GetLength() / 64 + 8;

	if(m_nPosArrayLenght < nStartSize){
		m_nPosArrayLenght= nStartSize;
		m_aPos= new ElemPos[nStartSize];
	}

	// Parse document
	bool bWellFormed= false;
	if(m_csDoc.GetLength()){
		memset(&m_aPos[0], 0, sizeof(ElemPos));

		int iPos= x_ParseElem(0);
		if(iPos > 0){
			m_aPos[0].iElemChild = iPos;
			bWellFormed = true;
		}
	}

	// Clear indexes if parse failed or empty document
	if(!bWellFormed){
		memset(&m_aPos[0], 0, sizeof(ElemPos));
		m_iPosFree= 1;
	}

	ResetPos();

	return bWellFormed;
};

bool
CXml::IsWellFormed(void)
{
	if(m_nPosArrayLenght && m_aPos[0].iElemChild)
		return true;

	return false;
}

bool
CXml::FindElem(const char* szName)
{
	// Change current position only if found
	//
	if(m_nPosArrayLenght){
		int iPos= x_FindElem(m_iPosParent, m_iPos, szName);

		if(iPos){
			// Assign new position
			x_SetPos(m_aPos[iPos].iElemParent, iPos, 0);
			return true;
		}
	}

	return false;
}

bool
CXml::FindChildElem(const char* szName)
{
	// Change current child position only if found
	//
	// Shorthand: call this with no current main position
	// means find child under root element
	if(!m_iPos)
		FindElem();

	int iPosChild= x_FindElem(m_iPos, m_iPosChild, szName);
	if(iPosChild){
		// Assign new position
		int iPos= m_aPos[iPosChild].iElemParent;
		x_SetPos(m_aPos[iPos].iElemParent, iPos, iPosChild);

		return true;
	}

	return false;
}


CDString CXml::GetTagName(void)
{
	// Return the tag name at the current main position
	CDString csTagName;


	if ( m_iPos )
		csTagName = x_GetTagName( m_iPos );
	return csTagName;
}

bool CXml::IntoElem()
{
	// If there is no child position and IntoElem is called it will succeed in release 6.3
	// (A subsequent call to FindElem will find the first element)
	// The following short-hand behavior was never part of EDOM and was misleading
	// It would find a child element if there was no current child element position and go into it
	// It is removed in release 6.3, this change is NOT backwards compatible!
	// if ( ! m_iPosChild )
	//	FindChildElem();

	if ( m_iPos && m_nNodeType == MNT_ELEMENT )
	{
		x_SetPos( m_iPos, m_iPosChild, 0 );
		return true;
	}
	return false;
}

bool CXml::OutOfElem()
{
	// Go to parent element
	if ( m_iPosParent )
	{
		x_SetPos( m_aPos[m_iPosParent].iElemParent, m_iPosParent, m_iPos );
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////
// Private Methods
//////////////////////////////////////////////////////////////////////

int CXml::x_GetFreePos()
{
	//
	// This returns the index of the next unused ElemPos in the array
	//
	if ( m_iPosFree == m_nPosArrayLenght ){

		ElemPos *temp= new ElemPos[(m_iPosFree + m_iPosFree / 2)];
		memcpy(temp, m_aPos, sizeof(ElemPos) * m_nPosArrayLenght);

		m_nPosArrayLenght= m_iPosFree + m_iPosFree / 2;

		SAFE_DELETE(m_aPos);
		m_aPos= temp;
	}
	++m_iPosFree;
	return m_iPosFree - 1;
}

int CXml::x_ReleasePos()
{
	//
	// This decrements the index of the next unused ElemPos in the array
	// allowing the element index returned by GetFreePos() to be reused
	//
	--m_iPosFree;
	return 0;
}

int CXml::x_ParseError( const char* szError, const char* szName )
{
	if ( szName )
		m_csError.Format( szError, szName );
	else
		m_csError = szError;
	x_ReleasePos();
	return -1;
}

int CXml::x_ParseElem( int iPosParent )
{
	// This is either called by SetDoc, x_AddSubDoc, or itself recursively
	// m_aPos[iPosParent].nEndL is where to start parsing for the child element
	// This returns the new position if a tag is found, otherwise zero
	// In all cases we need to get a new ElemPos, but release it if unused
	//
	int iPos = x_GetFreePos();
	m_aPos[iPos].nStartL = m_aPos[iPosParent].nEndL;
	m_aPos[iPos].iElemParent = iPosParent;
	m_aPos[iPos].iElemChild = 0;
	m_aPos[iPos].iElemNext = 0;

	// Start Tag
	// A loop is used to ignore all remarks tags and special tags
	// i.e. <?xml version="1.0"?>, and <!-- comment here -->
	// So any tag beginning with ? or ! is ignored
	// Loop past ignored tags
	TokenPos token( m_csDoc.m_arBuffer );
	token.nNext = m_aPos[iPosParent].nEndL;
	CDString csName;
	while ( csName.IsEmpty() )
	{
		// Look for left angle bracket of start tag
		m_aPos[iPos].nStartL = token.nNext;
		if ( ! x_FindChar( token.szDoc, m_aPos[iPos].nStartL, '<' ) )
			return x_ParseError( "Element tag not found" );

		// Set parent's End tag to start looking from here (or later)
		m_aPos[iPosParent].nEndL = m_aPos[iPos].nStartL;

		// Determine whether this is an element, or bypass other type of node
		token.nNext = m_aPos[iPos].nStartL + 1;
		if ( x_FindToken( token ) )
		{
			if ( token.bIsString )
				return x_ParseError( "Tag starts with quote" );
			char cFirstChar = m_csDoc[token.nL];
			if ( cFirstChar == '?' || cFirstChar == '!' )
			{
				token.nNext = m_aPos[iPos].nStartL;
				if ( ! x_ParseNode(token) )
					return x_ParseError( "Invalid node" );
			}
			else if ( cFirstChar != '/' )
			{
				csName = x_GetToken( token );
				// Look for end of tag
				if ( ! x_FindChar(token.szDoc, token.nNext, '>') )
					return x_ParseError( "End of tag not found" );
			}
			else
				return x_ReleasePos(); // probably end tag of parent
		}
		else
			return x_ParseError( "Abrupt end within tag" );
	}
	m_aPos[iPos].nStartR = token.nNext;

	// Is ending mark within start tag, i.e. empty element?
	if ( m_csDoc[m_aPos[iPos].nStartR-1] == '/' )
	{
		// Empty element
		// Close tag left is set to ending mark, and right to open tag right
		m_aPos[iPos].nEndL = m_aPos[iPos].nStartR-1;
		m_aPos[iPos].nEndR = m_aPos[iPos].nStartR;
	}
	else // look for end tag
	{
		// Element probably has contents
		// Determine where to start looking for left angle bracket of end tag
		// This is done by recursively parsing the contents of this element
		int iInner, iInnerPrev = 0;
		m_aPos[iPos].nEndL = m_aPos[iPos].nStartR + 1;
		while ( (iInner = x_ParseElem( iPos )) > 0 )
		{
			// Set links to iInner
			if ( iInnerPrev )
				m_aPos[iInnerPrev].iElemNext = iInner;
			else
				m_aPos[iPos].iElemChild = iInner;
			iInnerPrev = iInner;

			// Set offset to reflect child
			m_aPos[iPos].nEndL = m_aPos[iInner].nEndR + 1;
		}
		if ( iInner == -1 )
			return -1;

		// Look for left angle bracket of end tag
		if ( ! x_FindChar( token.szDoc, m_aPos[iPos].nEndL, '<' ) )
			return x_ParseError( "End tag of %s element not found", csName.m_arBuffer );

		// Look through tokens of end tag
		token.nNext = m_aPos[iPos].nEndL + 1;
		int nTokenCount = 0;
		while ( x_FindToken( token ) )
		{
			++nTokenCount;
			if ( ! token.bIsString )
			{
				// Is first token not an end slash mark?
				if ( nTokenCount == 1 && m_csDoc[token.nL] != '/' )
					return x_ParseError( "Expecting end tag of element %s", csName.m_arBuffer );

				else if ( nTokenCount == 2 && ! token.Match(csName.m_arBuffer) )
					return x_ParseError( "End tag does not correspond to %s", csName.m_arBuffer );

				// Else is it a right angle bracket?
				else if ( m_csDoc[token.nL] == '>' )
					break;
			}
		}

		// Was a right angle bracket not found?
		if ( ! token.szDoc[token.nL] || nTokenCount < 2 )
			return x_ParseError( "End tag not completed for element %s", csName.m_arBuffer );
		m_aPos[iPos].nEndR = token.nL;
	}

	// Successfully parsed element (and contained elements)
	return iPos;
}

bool CXml::x_FindChar( const char* szDoc, int& nChar, char c )
{
	// static function
	const char* pChar = &szDoc[nChar];
	while ( *pChar && *pChar != c )
		pChar ++;
	nChar = pChar - szDoc;
	if ( ! *pChar )
		return false;
	/*
	while ( szDoc[nChar] && szDoc[nChar] != c )
		nChar ++;
	if ( ! szDoc[nChar] )
		return false;
	*/
	return true;
}

bool CXml::x_FindAny( const char* szDoc, int& nChar )
{
	// Starting at nChar, find a non-whitespace char
	// return false if no non-whitespace before end of document, nChar points to end
	// otherwise return true and nChar points to non-whitespace char
	while ( szDoc[nChar] && strchr(" \t\n\r",szDoc[nChar]) )
		++nChar;
	return szDoc[nChar] != '\0';
}

bool CXml::x_FindToken( CXml::TokenPos& token )
{
	// Starting at token.nNext, bypass whitespace and find the next token
	// returns true on success, members of token point to token
	// returns false on end of document, members point to end of document
	const char* szDoc = token.szDoc;
	int nChar = token.nNext;
	token.bIsString = false;

	// By-pass leading whitespace
	if ( ! x_FindAny(szDoc,nChar) )
	{
		// No token was found before end of document
		token.nL = nChar;
		token.nR = nChar;
		token.nNext = nChar;
		return false;
	}

	// Is it an opening quote?
	char cFirstChar = szDoc[nChar];
	if ( cFirstChar == '\"' || cFirstChar == '\'' )
	{
		token.bIsString = true;

		// Move past opening quote
		++nChar;
		token.nL = nChar;

		// Look for closing quote
		x_FindChar( token.szDoc, nChar, cFirstChar );

		// Set right to before closing quote
		token.nR = nChar - 1;

		// Set nChar past closing quote unless at end of document
		if ( szDoc[nChar] )
			++nChar;
	}
	else
	{
		// Go until special char or whitespace
		token.nL = nChar;
		while ( szDoc[nChar] && ! strchr(" \t\n\r<>=\\/?!",szDoc[nChar]) )
			nChar ++;

		// Adjust end position if it is one special char
		if ( nChar == token.nL )
			++nChar; // it is a special char
		token.nR = nChar - 1;
	}

	// nNext points to one past last char of token
	token.nNext = nChar;
	return true;
}

CDString CXml::x_GetToken( const CXml::TokenPos& token )
{
	// The token contains indexes into the document identifying a small substring
	// Build the substring from those indexes and return it
	if ( token.nL > token.nR )
		return "";
	return m_csDoc.Mid( token.nL, token.nR - token.nL + ((token.nR<m_csDoc.GetLength())? 1:0) );
}

int CXml::x_FindElem( int iPosParent, int iPos, const char* szPath )
{
	// If szPath is 0 or empty, go to next sibling element
	// Otherwise go to next sibling element with matching path
	//
	if ( iPos )
		iPos = m_aPos[iPos].iElemNext;
	else
		iPos = m_aPos[iPosParent].iElemChild;

	// Finished here if szPath not specified
	if ( szPath == 0 || !szPath[0] )
		return iPos;

	// Search
	TokenPos token( m_csDoc.m_arBuffer );
	while ( iPos )
	{
		// Compare tag name
		token.nNext = m_aPos[iPos].nStartL + 1;
		x_FindToken( token ); // Locate tag name
		if ( token.Match(szPath) )
			return iPos;
		iPos = m_aPos[iPos].iElemNext;
	}
	return 0;
}

int CXml::x_ParseNode( CXml::TokenPos& token )
{
	// Call this with token.nNext set to the start of the node
	// This returns the node type and token.nNext set to the char after the node
	// If the node is not found or an element, token.nR is not determined
	int nTypeFound = 0;
	const char* szDoc = token.szDoc;
	token.nL = token.nNext;
	if ( szDoc[token.nL] == '<' )
	{
		// Started with <, could be:
		// <!--...--> comment
		// <!DOCTYPE ...> dtd
		// <?target ...?> processing instruction
		// <![CDATA[...]]> cdata section
		// <NAME ...> element
		//
		if ( ! szDoc[token.nL+1] || ! szDoc[token.nL+2] )
			return 0;
		char cFirstChar = szDoc[token.nL+1];
		const char* szEndOfNode = 0;
		if ( cFirstChar == '?' )
		{
			nTypeFound = MNT_PROCESSING_INSTRUCTION;
			szEndOfNode = "?>";
		}
		else if ( cFirstChar == '!' )
		{
			char cSecondChar = szDoc[token.nL+2];
			if ( cSecondChar == '[' )
			{
				nTypeFound = MNT_CDATA_SECTION;
				szEndOfNode = "]]>";
			}
			else if ( cSecondChar == '-' )
			{
				nTypeFound = MNT_COMMENT;
				szEndOfNode = "-->";
			}
			else
			{
				// Document type requires tokenizing because of strings and brackets
				nTypeFound = 0;
				int nBrackets = 0;
				while ( x_FindToken(token) )
				{
					if ( ! token.bIsString )
					{
						char cChar = szDoc[token.nL];
						if ( cChar == '[' )
							++nBrackets;
						else if ( cChar == ']' )
							--nBrackets;
						else if ( nBrackets == 0 && cChar == '>' )
						{
							nTypeFound = MNT_DOCUMENT_TYPE;
							break;
						}
					}
				}
				if ( ! nTypeFound )
					return 0;
			}
		}
		else if ( cFirstChar == '/' )
		{
			// End tag means no node found within parent element
			return 0;
		}
		else
		{
			nTypeFound = MNT_ELEMENT;
		}

		// Search for end of node if not found yet
		if ( szEndOfNode )
		{
			const char* pEnd = strstr( &szDoc[token.nNext], szEndOfNode );
			if ( ! pEnd )
				return 0; // not well-formed
			token.nNext = (pEnd - szDoc) + strlen(szEndOfNode);
		}
	}
	else if ( szDoc[token.nL] )
	{
		// It is text or whitespace because it did not start with <
		nTypeFound = MNT_WHITESPACE;
		token.nNext = token.nL;
		if ( x_FindAny(szDoc,token.nNext) )
		{
			if ( szDoc[token.nNext] != '<' )
			{
				nTypeFound = MNT_TEXT;
				x_FindChar( szDoc, token.nNext, '<' );
			}
		}
	}
	return nTypeFound;
}

CDString CXml::x_GetTagName( int iPos )
{
	// Return the tag name at specified element
	TokenPos token( m_csDoc.m_arBuffer );
	token.nNext = m_aPos[iPos].nStartL + 1;
	if ( ! iPos || ! x_FindToken( token ) )
		return "";

	// Return substring of document
	return x_GetToken( token );
}

bool CXml::x_FindAttrib( CXml::TokenPos& token, const char* szAttrib )
{
	// If szAttrib is 0 find next attrib, otherwise find named attrib
	// Return true if found
	int nAttrib = 0;
	for ( int nCount = 0; x_FindToken(token); ++nCount )
	{
		if ( ! token.bIsString )
		{
			// Is it the right angle bracket?
			char cChar = m_csDoc[token.nL];
			if ( cChar == '>' || cChar == '/' || cChar == '?' )
				break; // attrib not found

			// Equal sign
			if ( cChar == '=' )
				continue;

			// Potential attribute
			if ( ! nAttrib && nCount )
			{
				// Attribute name search?
				if ( ! szAttrib || ! szAttrib[0] )
					return true; // return with token at attrib name

				// Compare szAttrib
				if ( token.Match(szAttrib) )
					nAttrib = nCount;
			}
		}
		else if ( nAttrib && nCount == nAttrib + 2 )
		{
			return true;
		}
	}

	// Not found
	return false;
}

CDString CXml::x_GetAttrib( int iPos, const char* szAttrib )
{
	// Return the value of the attrib
	TokenPos token( m_csDoc.m_arBuffer );
	if ( iPos && m_nNodeType == MNT_ELEMENT )
		token.nNext = m_aPos[iPos].nStartL + 1;
	else
		return "";

	if ( szAttrib && x_FindAttrib( token, szAttrib ) )
		return x_TextFromDoc( token.nL, token.nR - ((token.nR<m_csDoc.GetLength())?0:1) );
	return "";
}

bool CXml::x_SetAttrib( int iPos, const char* szAttrib, const char* szValue )
{
	// Set attribute in iPos element
	TokenPos token( m_csDoc.m_arBuffer );
	int nInsertAt;
	if ( iPos && m_nNodeType == MNT_ELEMENT )
	{
		token.nNext = m_aPos[iPos].nStartL + 1;
		nInsertAt = m_aPos[iPos].nStartR - (m_aPos[iPos].IsEmptyElement()?1:0);
	}
	else
		return false;

	// Create insertion text depending on whether attribute already exists
	int nReplace = 0;
	CDString csInsert;
	if ( x_FindAttrib( token, szAttrib ) )
	{
		// Replace value only
		// Decision: for empty value leaving attrib="" instead of removing attrib
		csInsert = x_TextToDoc( szValue, true );
		nInsertAt = token.nL;
		nReplace = token.nR-token.nL+1;
	}
	else
	{
		// Insert string name value pair
		CDString csFormat;
		csFormat = " ";
		csFormat += szAttrib;
		csFormat += "=\"";
		csFormat += x_TextToDoc( szValue, true );
		csFormat += "\"";
		csInsert = csFormat;
	}

	x_DocChange( nInsertAt, nReplace, csInsert );
	int nAdjust = csInsert.GetLength() - nReplace;
	m_aPos[iPos].nStartR += nAdjust;
	m_aPos[iPos].AdjustEnd( nAdjust );
	x_Adjust( iPos, nAdjust );
	return true;
}

CDString CXml::x_GetData( int iPos )
{

	// Return a string representing data between start and end tag
	// Return empty string if there are any children elements
	if ( !m_aPos[iPos].iElemChild && !m_aPos[iPos].IsEmptyElement() )
	{
		// See if it is a CDATA section
		const char* szDoc = m_csDoc.m_arBuffer;
		int nChar = m_aPos[iPos].nStartR + 1;
		if ( x_FindAny( szDoc, nChar ) && szDoc[nChar] == '<'
				&& nChar + 11 < m_aPos[iPos].nEndL
				&& strncmp( &szDoc[nChar], "<![CDATA[", 9 ) == 0 )
		{
			nChar += 9;
			int nEndCDATA = m_csDoc.Find( "]]>", nChar );
			if ( nEndCDATA != -1 && nEndCDATA < m_aPos[iPos].nEndL )
			{
				return m_csDoc.Mid( nChar, nEndCDATA - nChar );
			}
		}
		return x_TextFromDoc( m_aPos[iPos].nStartR+1, m_aPos[iPos].nEndL-1 );
	}
	return "";
}

CDString CXml::x_TextToDoc( const char* szText, bool bAttrib )
{
	// Convert text as seen outside XML document to XML friendly
	// replacing special characters with ampersand escape codes
	// E.g. convert "6>7" to "6&gt;7"
	//
	// &lt;   less than
	// &amp;  ampersand
	// &gt;   greater than
	//
	// and for attributes:
	//
	// &apos; apostrophe or single quote
	// &quot; double quote
	//
	static const char* szaReplace[] = { "&lt;","&amp;","&gt;","&apos;","&quot;" };
	const char* pFind = bAttrib?"<&>\'\"":"<&>";
	CDString csText;
	const char* pSource = szText;
	int nDestSize = strlen(pSource);
	nDestSize += nDestSize / 10 + 7;
	char* pDest = csText.GetBuffer(nDestSize);
	int nLen = 0;
	char cSource = *pSource;
	char* pFound;
	while ( cSource )
	{
		if ( nLen > nDestSize - 6 )
		{
			csText.ReleaseBuffer(nLen);
			nDestSize *= 2;
			pDest = csText.GetBuffer(nDestSize);
		}
		if ( (pFound= (char*)strchr(pFind,cSource)) != 0 )
		{
			pFound = (char*)szaReplace[pFound-pFind];
			strcpy(&pDest[nLen],pFound);
			nLen += strlen(pFound);
		}
		else
		{
			pDest[nLen]= *pSource;
			nLen ++;
		}
		pSource ++;
		cSource = *pSource;
	}
	csText.ReleaseBuffer(nLen);
	return csText;
}

CDString CXml::x_TextFromDoc( int nLeft, int nRight )
{
	// Convert XML friendly text to text as seen outside XML document
	// ampersand escape codes replaced with special characters e.g. convert "6&gt;7" to "6>7"
	// Conveniently the result is always the same or shorter in byte length
	//
	static const char* szaCode[] = { "lt;","amp;","gt;","apos;","quot;" };
	static int anCodeLen[] = { 3,4,3,5,5 };
	static const char* szSymbol = "<&>\'\"";
	CDString csText;
	const char* pSource = m_csDoc.m_arBuffer;
	int nDestSize = nRight - nLeft + 1;
	char* pDest = csText.GetBuffer(nDestSize);
	int nLen = 0;
	int nChar = nLeft;
	while ( nChar <= nRight )
	{
		if ( pSource[nChar] == '&' )
		{
			// Look for matching &code;
			bool bCodeConverted = false;
			for ( int nMatch = 0; nMatch < 5; ++nMatch )
			{
				if ( nChar <= nRight - anCodeLen[nMatch]
					&& strncmp(szaCode[nMatch],&pSource[nChar+1],anCodeLen[nMatch]) == 0 )
				{
					// Insert symbol and increment index past ampersand semi-colon
					pDest[nLen++] = szSymbol[nMatch];
					nChar += anCodeLen[nMatch] + 1;
					bCodeConverted = true;
					break;
				}
			}

			// If the code is not converted, leave it as is
			if ( ! bCodeConverted )
			{
				pDest[nLen++] = '&';
				++nChar;
			}
		}
		else // not &
		{
			pDest[nLen]= pSource[nChar];
			nLen++;
			nChar++;
		}
	}
	csText.ReleaseBuffer(nLen);
	return csText;
}

void CXml::x_DocChange( int nLeft, int nReplace, const CDString& csInsert )
{
	// Insert csInsert int m_csDoc at nLeft replacing nReplace chars
	// Do this with only one buffer reallocation if it grows
	//
	int nDocLength = m_csDoc.GetLength();
	int nInsLength = csInsert.GetLength();

	// Make sure nLeft and nReplace are within bounds
	nLeft = max( 0, min( nLeft, nDocLength ) );
	nReplace = max( 0, min( nReplace, nDocLength-nLeft ) );

	// Get pointer to buffer with enough room
	int nNewLength = nInsLength + nDocLength - nReplace;
	int nBufferLen = nNewLength;
	char* pDoc = m_csDoc.GetBuffer( nBufferLen );

	// Move part of old doc that goes after insert
	if ( nLeft+nReplace < nDocLength )
		memmove( &pDoc[nLeft+nInsLength], &pDoc[nLeft+nReplace], (nDocLength-nLeft-nReplace));

	// Copy insert
	memcpy( &pDoc[nLeft], csInsert.m_arBuffer, nInsLength);

	// Release
	m_csDoc.ReleaseBuffer( nNewLength );
}

void CXml::x_Adjust( int iPos, int nShift, bool bAfterPos )
{
	// Loop through affected elements and adjust indexes
	// Algorithm:
	// 1. update children unless bAfterPos
	//    (if no children or bAfterPos is true, end tag of iPos not affected)
	// 2. update next siblings and their children
	// 3. go up until there is a next sibling of a parent and update end tags
	// 4. step 2
	int iPosTop = m_aPos[iPos].iElemParent;
	bool bPosFirst = bAfterPos; // mark as first to skip its children
	while ( iPos )
	{
		// Were we at containing parent of affected position?
		bool bPosTop = false;
		if ( iPos == iPosTop )
		{
			// Move iPosTop up one towards root
			iPosTop = m_aPos[iPos].iElemParent;
			bPosTop = true;
		}

		// Traverse to the next update position
		if ( ! bPosTop && ! bPosFirst && m_aPos[iPos].iElemChild )
		{
			// Depth first
			iPos = m_aPos[iPos].iElemChild;
		}
		else if ( m_aPos[iPos].iElemNext )
		{
			iPos = m_aPos[iPos].iElemNext;
		}
		else
		{
			// Look for next sibling of a parent of iPos
			// When going back up, parents have already been done except iPosTop
			while ( (iPos=m_aPos[iPos].iElemParent) != 0 && iPos != iPosTop )
				if ( m_aPos[iPos].iElemNext )
				{
					iPos = m_aPos[iPos].iElemNext;
					break;
				}
		}
		bPosFirst = false;

		// Shift indexes at iPos
		if ( iPos != iPosTop )
			m_aPos[iPos].AdjustStart( nShift );
		m_aPos[iPos].AdjustEnd( nShift );
	}
}

void CXml::x_LocateNew( int iPosParent, int& iPosRel, int& nOffset, int nLength, int nFlags )
{
	// Determine where to insert new element or node
	//
	bool bInsert = (nFlags&1)?true:false;
	bool bHonorWhitespace = (nFlags&2)?true:false;

	int nStartL;
	if ( nLength )
	{
		// Located at a non-element node
		if ( bInsert )
			nStartL = nOffset;
		else
			nStartL = nOffset + nLength;
	}
	else if ( iPosRel )
	{
		// Located at an element
		if ( bInsert ) // precede iPosRel
			nStartL = m_aPos[iPosRel].nStartL;
		else // follow iPosRel
			nStartL = m_aPos[iPosRel].nEndR + 1;
	}
	else if ( ! iPosParent )
	{
		// Outside of all elements
		if ( bInsert )
			nStartL = 0;
		else
			nStartL = m_csDoc.GetLength();
	}
	else if ( m_aPos[iPosParent].IsEmptyElement() )
	{
		// Parent has no separate end tag, so split empty element
		nStartL = m_aPos[iPosParent].nStartR;
	}
	else
	{
		if ( bInsert ) // after start tag
			nStartL = m_aPos[iPosParent].nStartR + 1;
		else // before end tag
			nStartL = m_aPos[iPosParent].nEndL;
	}

	// Go up to start of next node, unless its splitting an empty element
	if ( ! bHonorWhitespace && ! m_aPos[iPosParent].IsEmptyElement() )
	{
		const char* szDoc = m_csDoc.m_arBuffer;
		int nChar = nStartL;
		if ( ! x_FindAny(szDoc,nChar) || szDoc[nChar] == '<' )
			nStartL = nChar;
	}

	// Determine iPosBefore
	int iPosBefore = 0;
	if ( iPosRel )
	{
		if ( bInsert )
		{
			// Is iPosRel past first sibling?
			int iPosPrev = m_aPos[iPosParent].iElemChild;
			if ( iPosPrev != iPosRel )
			{
				// Find previous sibling of iPosRel
				while ( m_aPos[iPosPrev].iElemNext != iPosRel )
					iPosPrev = m_aPos[iPosPrev].iElemNext;
				iPosBefore = iPosPrev;
			}
		}
		else
		{
			iPosBefore = iPosRel;
		}
	}
	else if ( m_aPos[iPosParent].iElemChild )
	{
		if ( ! bInsert )
		{
			// Find last element under iPosParent
			int iPosLast = m_aPos[iPosParent].iElemChild;
			int iPosNext = iPosLast;
			while ( iPosNext )
			{
				iPosLast = iPosNext;
				iPosNext = m_aPos[iPosNext].iElemNext;
			}
			iPosBefore = iPosLast;
		}
	}

	nOffset = nStartL;
	iPosRel = iPosBefore;
}

bool CXml::x_AddElem( const char* szName, const char* szValue, bool bInsert, bool bAddChild )
{
	if ( bAddChild )
	{
		// Adding a child element under main position
		if ( ! m_iPos )
			return false;
	}
	else if ( m_iPosParent == 0 )
	{
		// Adding root element
		if ( IsWellFormed() )
			return false;


		// Locate after any version and DTD
		m_aPos[0].nEndL = m_csDoc.GetLength();
	}

	// Locate where to add element relative to current node
	int iPosParent, iPosBefore, nOffset = 0, nLength = 0;
	if ( bAddChild )
	{
		iPosParent = m_iPos;
		iPosBefore = m_iPosChild;
	}
	else
	{
		iPosParent = m_iPosParent;
		iPosBefore = m_iPos;
	}
	int nFlags = bInsert?1:0;
	x_LocateNew( iPosParent, iPosBefore, nOffset, nLength, nFlags );
	bool bEmptyParent = m_aPos[iPosParent].IsEmptyElement();
	if ( bEmptyParent || m_aPos[iPosParent].nStartR + 1 == m_aPos[iPosParent].nEndL )
		nOffset += 2;

	// Create element and modify positions of affected elements
	// If no szValue is specified, an empty element is created
	// i.e. either <NAME>value</NAME> or <NAME/>
	//
	int iPos = x_GetFreePos();
	m_aPos[iPos].nStartL = nOffset;

	// Set links
	m_aPos[iPos].iElemParent = iPosParent;
	m_aPos[iPos].iElemChild = 0;
	m_aPos[iPos].iElemNext = 0;
	if ( iPosBefore )
	{
		// Link in after iPosBefore
		m_aPos[iPos].iElemNext = m_aPos[iPosBefore].iElemNext;
		m_aPos[iPosBefore].iElemNext = iPos;
	}
	else
	{
		// First child
		m_aPos[iPos].iElemNext = m_aPos[iPosParent].iElemChild;
		m_aPos[iPosParent].iElemChild = iPos;
	}

	// Create string for insert
	CDString csInsert;
	int nLenName = strlen(szName);
	int nLenValue = szValue? strlen(szValue) : 0;
	if ( ! nLenValue )
	{
		// <NAME/> empty element
		csInsert = "<";
		csInsert += szName;
		csInsert += "/>\r\n";
		m_aPos[iPos].nStartR = m_aPos[iPos].nStartL + nLenName + 2;
		m_aPos[iPos].nEndL = m_aPos[iPos].nStartR - 1;
		m_aPos[iPos].nEndR = m_aPos[iPos].nEndL + 1;
	}
	else
	{
		// <NAME>value</NAME>
		CDString csValue = x_TextToDoc( szValue );
		nLenValue = csValue.GetLength();
		csInsert = "<";
		csInsert += szName;
		csInsert += ">";
		csInsert += csValue;
		csInsert += "</";
		csInsert += szName;
		csInsert += ">\r\n";
		m_aPos[iPos].nStartR = m_aPos[iPos].nStartL + nLenName + 1;
		m_aPos[iPos].nEndL = m_aPos[iPos].nStartR + nLenValue + 1;
		m_aPos[iPos].nEndR = m_aPos[iPos].nEndL + nLenName + 2;
	}

	// Insert
	int nReplace = 0, nLeft = m_aPos[iPos].nStartL;
	if ( bEmptyParent )
	{
		CDString csParentTagName = x_GetTagName(iPosParent);
		CDString csFormat;
		csFormat = ">\r\n";
		csFormat += csInsert;
		csFormat += "</";
		csFormat += csParentTagName;
		csInsert = csFormat;
		nLeft = m_aPos[iPosParent].nStartR - 1;
		nReplace = 1;
		// x_Adjust is going to update all affected indexes by one amount
		// This will satisfy all except the empty parent
		// Here we pre-adjust for the empty parent
		// The empty tag slash is removed
		m_aPos[iPosParent].nStartR -= 1;
		// For the newly created end tag, see the following example:
		// <A/> (len 4) becomes <A><B/></A> (len 11)
		// In x_Adjust everything will be adjusted 11 - 4 = 7
		// But the nEndL of element A should only be adjusted 5
		m_aPos[iPosParent].nEndL -= (csParentTagName.GetLength() + 1);
	}
	else if ( m_aPos[iPosParent].nStartR + 1 == m_aPos[iPosParent].nEndL )
	{
		CDString temp= "\r\n";
		temp+=  csInsert;
		csInsert = temp;
		nLeft = m_aPos[iPosParent].nStartR + 1;
	}
	x_DocChange( nLeft, nReplace, csInsert );
	x_Adjust( iPos, csInsert.GetLength() - nReplace );

	if ( bAddChild )
		x_SetPos( m_iPosParent, iPosParent, iPos );
	else
		x_SetPos( iPosParent, iPos, 0 );
	return true;
}
