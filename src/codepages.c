
/*
 * Current codepages contain no second plane UTF16 ( > 0xFFFF )
 */

#include "codepages.h"
#include "wincp.h"

int load_table(int codepage, ANSIPtr * tbllead, UTF16Ptr * tblpage)
{
	switch(codepage)
	{
	case CODEPAGE_874:
		*tblpage = CP874_UNICODE;
		*tbllead = NULL;
		break;
	case CODEPAGE_932:
		*tblpage = CP932_UNICODE;
		*tbllead = CP932_LEADBYTES;
		break;
	case CODEPAGE_936:
		*tblpage = CP936_UNICODE;
		*tbllead = CP936_LEADBYTES;
		break;
	case CODEPAGE_949:
		*tblpage = CP949_UNICODE;
		*tbllead = CP949_LEADBYTES;
		break;
	case CODEPAGE_950:
		*tblpage = CP950_UNICODE;
		*tbllead = CP950_LEADBYTES;
		break;
	case CODEPAGE_1250:
		*tblpage = CP1250_UNICODE;
		*tbllead = NULL;
		break;
	case CODEPAGE_1251:
		*tblpage = CP1251_UNICODE;
		*tbllead = NULL;
		break;
	case CODEPAGE_1252:
		*tblpage = CP1252_UNICODE;
		*tbllead = NULL;
		break;
	case CODEPAGE_1253:
		*tblpage = CP1253_UNICODE;
		*tbllead = NULL;
		break;
	case CODEPAGE_1254:
		*tblpage = CP1254_UNICODE;
		*tbllead = NULL;
		break;
	case CODEPAGE_1255:
		*tblpage = CP1255_UNICODE;
		*tbllead = NULL;
		break;
	case CODEPAGE_1256:
		*tblpage = CP1256_UNICODE;
		*tbllead = NULL;
		break;
	case CODEPAGE_1257:
		*tblpage = CP1257_UNICODE;
		*tbllead = NULL;
		break;
	case CODEPAGE_1258:
		*tblpage = CP1258_UNICODE;
		*tbllead = NULL;
		break;
/*
	case CODEPAGE_037:
		*tblpage = CP037_UNICODE;
		*tbllead = NULL;
		break;
	case CODEPAGE_500:
		*tblpage = CP500_UNICODE;
		*tbllead = NULL;
		break;
	case CODEPAGE_875:
		*tblpage = CP875_UNICODE;
		*tbllead = NULL;
		break;
	case CODEPAGE_1026:
		*tblpage = CP1026_UNICODE;
		*tbllead = NULL;
		break;
*/
	default:
		return -1;
	}
	return 0;
}

size_t utf16_to_utf8(UTF16Ptr source, size_t source_length, UTF8Ptr target, size_t target_size, size_t * invalid_chars)
{
	UTF16Ptr psource;
	UTF32Char codepoint, second_unit;
	UTF8Ptr ptarget;
	if(source_length == 0) return 0;
	if(source == NULL || target == NULL || target_size == 0) return CODEPAGE_ERROR_INVALID_ARGUMENTS;
	psource = source;
	ptarget = target;
	if(invalid_chars != NULL) *invalid_chars = 0;
	do {
		codepoint = *psource;
   		if (codepoint > 0xD7FF && codepoint < 0xDC00) 
		{
			second_unit = *(psource + 1);
     		if (second_unit > 0xDBFF && second_unit < 0xE000)
			{
				codepoint = (codepoint << 10) + second_unit - 0x35FDC00;
				psource++;
			}
		}
		if (codepoint < 0x80)
		{
			if((ptarget - target) + 1 < target_size)
			{
				*ptarget++ = codepoint;
			} else return CODEPAGE_ERROR_INVALID_BUFFSIZE;
		}
		else if (codepoint < 0x800)
		{
			if((ptarget - target) + 2 < target_size)
			{
				*ptarget++ = (codepoint >> 6) + 0xC0;
				*ptarget++ = (codepoint & 0x3F) + 0x80;
			} else return CODEPAGE_ERROR_INVALID_BUFFSIZE;
		}
		else if (codepoint < 0x10000) 
		{
			if((ptarget - target) + 3 < target_size)
			{
				*ptarget++ = (codepoint >> 12) + 0xE0;
				*ptarget++ = ((codepoint >> 6) & 0x3F) + 0x80;
				*ptarget++ = (codepoint & 0x3F) + 0x80;
			} else return CODEPAGE_ERROR_INVALID_BUFFSIZE;
		}
		else if (codepoint < 0x110000) 
		{
			if((ptarget - target) + 4 < target_size)
			{
				*ptarget++ = (codepoint >> 18) + 0xF0;
				*ptarget++ = ((codepoint >> 12) & 0x3F) + 0x80;
				*ptarget++ = ((codepoint >> 6) & 0x3F) + 0x80;
				*ptarget++ = (codepoint & 0x3F) + 0x80;
			} else return CODEPAGE_ERROR_INVALID_BUFFSIZE;
		}
		else if(invalid_chars != NULL) ++*invalid_chars;
		psource++;
	} while(psource - source < source_length);
	return (size_t) (ptarget - target);
}

// Takes all planes into account,... :)
size_t utf16_to_utf8_size(UTF16Ptr source, size_t source_length, size_t * invalid_chars)
{
	UTF16Ptr psource;
	UTF32Char codepoint, second_unit;
	size_t size;
	if(source_length == 0) return 0;
	if(source == NULL) return CODEPAGE_ERROR_INVALID_ARGUMENTS;
	psource = source;
	size = 0;
	if(invalid_chars != NULL) *invalid_chars = 0;
	do {
		codepoint = *psource;
   		if (codepoint > 0xD7FF && codepoint < 0xDC00) 
		{
			second_unit = *(psource + 1);
     		if (second_unit > 0xDBFF && second_unit < 0xE000)
			{
				codepoint = (codepoint << 10) + second_unit - 0x35FDC00;
				psource++;
			}
		}
		if (codepoint < 0x80) size += 1;
		else if (codepoint < 0x800) size += 2;
		else if (codepoint < 0x10000) size += 3;
		else if (codepoint < 0x110000) size += 4;
		else if(invalid_chars != NULL) ++*invalid_chars;
		psource++;
	} while(psource - source < source_length);
	return size;
}

size_t codepage_to_utf8(int codepage, ANSIPtr source, size_t source_length, UTF8Ptr target, size_t target_size, size_t * invalid_chars)
{
	UTF16Ptr tblpage;
	ANSIPtr tbllead, psource;
	UTF8Ptr ptarget;
	UTF16Char codepoint;
	if(source_length == 0) return 0;
	if(source == NULL) return CODEPAGE_ERROR_INVALID_ARGUMENTS;
	if(load_table(codepage, &tbllead, &tblpage) != 0) return CODEPAGE_ERROR_INVALID_CODEPAGE;
	psource = source;
	ptarget = target;
	if(invalid_chars != NULL) *invalid_chars = 0;
	if(tbllead != NULL)
	{
		do {
			if(tbllead[*psource] != 0)
			{
				codepoint = tblpage[((int)(*psource++)) << 8 | ((int)(*psource++))];
				if(codepoint == 0xFFFF) psource--; // <- try next if lookup with leadbyte is failed?
			}
			else codepoint = tblpage[*psource++];

			if(codepoint < 0x80)
			{
				if((ptarget - target) + 1 < target_size)
				{
					*ptarget++ = codepoint;
				} else return CODEPAGE_ERROR_INVALID_BUFFSIZE;
			}
			else if(codepoint < 0x800)
			{
				if((ptarget - target) + 2 < target_size)
				{
					*ptarget++ = (codepoint >> 6) + 0xC0;
					*ptarget++ = (codepoint & 0x3F) + 0x80;
				} else return CODEPAGE_ERROR_INVALID_BUFFSIZE;
			}
			else if(codepoint < 0x10000) 
			{
				if((ptarget - target) + 3 < target_size)
				{
					*ptarget++ = (codepoint >> 12) + 0xE0;
					*ptarget++ = ((codepoint >> 6) & 0x3F) + 0x80;
					*ptarget++ = (codepoint & 0x3F) + 0x80;
				} else return CODEPAGE_ERROR_INVALID_BUFFSIZE;
			}
			else if(invalid_chars != NULL) ++*invalid_chars; 
		} while(psource - source < source_length);
	}	
	else
	{
		do {
			codepoint = tblpage[*psource++];
			if(codepoint < 0x80)
			{
				if((ptarget - target) + 1 < target_size)
				{
					*ptarget++ = codepoint;
				} else return CODEPAGE_ERROR_INVALID_BUFFSIZE;
			}
			else if(codepoint < 0x800)
			{
				if((ptarget - target) + 2 < target_size)
				{
					*ptarget++ = (codepoint >> 6) + 0xC0;
					*ptarget++ = (codepoint & 0x3F) + 0x80;
				} else return CODEPAGE_ERROR_INVALID_BUFFSIZE;
			}
			else if(codepoint < 0x10000) 
			{
				if((ptarget - target) + 3 < target_size)
				{
					*ptarget++ = (codepoint >> 12) + 0xE0;
					*ptarget++ = ((codepoint >> 6) & 0x3F) + 0x80;
					*ptarget++ = (codepoint & 0x3F) + 0x80;
				} else return CODEPAGE_ERROR_INVALID_BUFFSIZE;
			}
			else if(invalid_chars != NULL) ++*invalid_chars; 
		} while(psource - source < source_length);
	}
	return (size_t)(ptarget - target);
}

size_t codepage_to_utf8_size(int codepage, ANSIPtr source, size_t source_length, size_t * invalid_chars)
{
	UTF16Ptr tblpage;
	ANSIPtr tbllead, psource;
	UTF16Char codepoint;
	size_t length;
	if(source_length == 0) return 0;
	if(source == NULL) return CODEPAGE_ERROR_INVALID_ARGUMENTS;
	if(load_table(codepage, &tbllead, &tblpage) != 0) return CODEPAGE_ERROR_INVALID_CODEPAGE;
	psource = source;
	length = 0;
	if(invalid_chars != NULL) *invalid_chars = 0;
	if(tbllead != NULL)
	{
		do {
			if(tbllead[*psource] != 0)
			{
				codepoint = tblpage[((int)(*psource++)) << 8 | ((int)(*psource++))];
				if(codepoint == 0xFFFF) psource--; // <- try next if lookup with leadbyte is failed?
			}
			else codepoint = tblpage[*psource++];
			if(codepoint == 0xFFFF) { if(invalid_chars != NULL) ++*invalid_chars; }
			else length += (codepoint < 0x80) ? 1 : ((codepoint < 0x800) ? 2 : 3);
		} while(psource - source < source_length);
	}	
	else
	{
		do {
			codepoint = tblpage[*psource++];
			if(codepoint == 0xFFFF) { if(invalid_chars != NULL) ++*invalid_chars; }
			else length += (codepoint < 0x80) ? 1 : ((codepoint < 0x800) ? 2 : 3);
		} while(psource - source < source_length);
	}
	return length;
}

size_t codepage_to_utf16(int codepage, ANSIPtr source, size_t source_length, UTF16Ptr target, size_t target_size, size_t * invalid_chars)
{
	UTF16Ptr tblpage, ptarget;
	ANSIPtr tbllead, psource;
	UTF16Char codepoint;
	if(source_length == 0) return 0;
	if(source == NULL || target == NULL) return CODEPAGE_ERROR_INVALID_ARGUMENTS;
	if(load_table(codepage, &tbllead, &tblpage) != 0) return CODEPAGE_ERROR_INVALID_CODEPAGE;
	psource = source;
	ptarget = target;
	if(invalid_chars != NULL) *invalid_chars = 0;
	if(tbllead != NULL)
	{
		do {
			if(tbllead[*psource] != 0)
			{
				codepoint = tblpage[((int)(*psource++)) << 8 | ((int)(*psource++))];
				if(codepoint == 0xFFFF) psource--; // <- try next if lookup with leadbyte is failed?
			}
			else codepoint = tblpage[*psource++];
			if(codepoint == 0xFFFF) { if(invalid_chars != NULL) ++*invalid_chars; }
			else *ptarget++ = codepoint;
			if(ptarget - target > target_size) return CODEPAGE_ERROR_INVALID_BUFFSIZE;
		} while(psource - source < source_length);
	}	
	else
	{
		do {
			codepoint = tblpage[*psource++];
			if(codepoint == 0xFFFF) { if(invalid_chars != NULL) ++*invalid_chars; }
			else *ptarget++ = codepoint;
			if(ptarget - target > target_size) return CODEPAGE_ERROR_INVALID_BUFFSIZE;
		} while(psource - source < source_length);
	}
	return (size_t)(ptarget - target);
}
