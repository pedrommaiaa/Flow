#ifndef extern_
#define extern_ extern
#endif

// Global variables

extern_ int                 Line;		        // Current line number
extern_ int                 Putback;		    // Character put back by scanner
extern_ int                 Functionid;		    // Symbol id of the current function
extern_ int                 Globs;		        // Position of next free global symbol slot
extern_ FILE                *Infile;		    // Input and output files
extern_ FILE                *Outfile;
extern_ token_T             Token;	            // Last token scanned
extern_ char                Text[TEXTLEN + 1];	// Last identifier scanned
extern_ SymTable_T          Gsym[NSYMBOLS];	    // Global symbol table

extern_ int                 O_dumpAST;
