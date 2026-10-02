#include <asm-generic/mman-common.h>
#include <cstring>
#include <linux/mman.h>
#include <sys/mman.h>
const char* monsterToString(int m_id) {

    switch (m_id) {
        case 2002:
            return "Lord";
            break;
        case 2003:
            return "Turtle";
            break;
        case 2004:
            return "Fiend";
            break;
        case 2005:
            return "Serpent";
            break;
	    case 2006:
            return "Crammer";
            break;
    	case 2009:
            return "Rockursa";
            break;
    	case 2011:
    		return "Crab";
    		break;
        case 2012:
            return "Serpent kids";
            break;
        case 2013:
            return "Crab";
            break;
        case 2056:
            return "Lithowanderer";
            break;
    	case 2059:
	    	return  "Crammer";
		    break;
    	case 2072:
	    	return "Lithowanderer";
		    break;
        default:
            return "NO";
    }

}
