#include <asm-generic/mman-common.h>
#include <cstring>
#include <linux/mman.h>
#include <sys/mman.h>
char* MonsterToString(int m_id) {
    size_t size = sizeof(char) * 64;
    char* strMonster = (char*)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    switch (m_id) {
        case 2002:
            strcpy(strMonster, "Lord");
            break;
        case 2003:
            strcpy(strMonster, "Turtle");
            break;
        case 2004:
            strcpy(strMonster, "Fiend");
            break;
        case 2005:
            strcpy(strMonster, "Serpent");
            break;
	    case 2006:
            strcpy(strMonster, "Scaled Lizard");
            break;
	    case 2008:
            strcpy(strMonster, "Crammer");
            break;
    	case 2009:
            strcpy(strMonster, "Rockursa");
            break;
    	case 2011:
    		strcpy(strMonster, "Crab");
    		break;
        case 2012:
            strcpy(strMonster, "Serpent kids");
            break;
        case 2013:
            strcpy(strMonster, "Crab");
            break;
        case 2056:
            strcpy(strMonster, "Lithowanderer");
            break;
    	case 2059:
	    	strcpy(strMonster, "Crammer");
		    break;
    	case 2072:
	    	strcpy(strMonster, "Lithowanderer");
		    break;
        default:
            strcpy(strMonster, "NO");
    }
    return strMonster;
}
