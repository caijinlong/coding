#ifndef INC_CharStreamIOException_hpp__
#define INC_CharStreamIOException_hpp__

/* ANTLR Translator Generator
 * Project led by Terence Parr at http://www.jGuru.com
 * Software rights: http://www.antlr.org/license.html
 *
 * $Id: //depot/code/org.release/antlr-2.7.4/lib/cpp/CharStreamIOException.hpp#1 $
 */

#include "config.hpp"
#include "CharStreamException.hpp"

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
namespace antlr {
#endif

class ANTLR_API CharStreamIOException : public CharStreamException {
public:
	ANTLR_USE_NAMESPACE(std)exception io;

	CharStreamIOException(ANTLR_USE_NAMESPACE(std)exception& e)
		: CharStreamException(e.what()), io(e) {}
	~CharStreamIOException() throw() {}
};

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
}
#endif

#endif //INC_CharStreamIOException_hpp__