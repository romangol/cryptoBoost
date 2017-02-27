#ifndef _ROMANGOL_H_
#define _ROMANGOL_H_

void outputChar( const unsigned char * const text, size_t len )
{
	for ( size_t i = 0; i < len; ++i )
	{
		printf("0x%02x, ", text[i] );
		if ( i % 16 == 15 )
			puts("");
	}
	puts("");
}	

void outputDword( unsigned int * text, size_t len )
{
	for ( size_t i = 0; i < len; ++i )
	{
		printf("0x%08x ", text[i] );
		if ( i % 4 == 3 )
			puts("");
	}
	
	puts("");
}

unsigned int ROR(unsigned int s, size_t times)
{
    unsigned int t = s;
    for ( size_t i = 0; i < times; ++i)
        t = (t >> 1) + ((t & 1) << 31);
    return t;
}

unsigned int ROL(unsigned int s, size_t times)
{
    unsigned int t = s;
    for ( size_t i = 0; i < times; ++i)
        t = (t << 1) + ((t & 0x80000000) >> 31);
    return t;
}

class NonCopyable
{
protected:
	NonCopyable () {}
	~NonCopyable () {} /// Protected non-virtual destructor
private: 
	NonCopyable (NonCopyable const &);
	NonCopyable & operator = (NonCopyable const &);
};

#define ROTATELEFT(X,n)  (((X)<<(n)) | ((X)>>(32-(n))))

#define forloop(i, start, end) for ( size_t (i) = (start); (i) < (end); ++(i) )
#define forstep(i, start, end, step) for ( size_t (i) = (start); (i) < (end); (i) += (step) )

#endif // end of romangol.h