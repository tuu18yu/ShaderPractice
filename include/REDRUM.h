#ifdef WIN32
// Bold Red, etc.
#define NORUM(X)     X
#define BOLD(X)     X
#define REDRUM(X)     X
#define GREENRUM(X)   X
#define YELLOWRUM(X)  X
#define BLUERUM(X)    X
#define MAGENTARUM(X) X
#define CYANRUM(X)    X

// Regular Red, etc.
#define REDGIN(X)     X
#define GREENGIN(X)   X
#define YELLOWGIN(X)  X
#define BLUEGIN(X)    X
#define MAGENTAGIN(X) X
#define CYANGIN(X)    X
#else

// Bold Red, etc.
#define NORUM(X)       ""<<X<<""
#define REDRUM(X)      "\e[1m\e[31m"<<X<<"\e[m"
#define GREENRUM(X)    "\e[1m\e[32m"<<X<<"\e[m"
#define YELLOWRUM(X)   "\e[1m\e[33m"<<X<<"\e[m"
#define BLUERUM(X)     "\e[1m\e[34m"<<X<<"\e[m"
#define MAGENTARUM(X)  "\e[1m\e[35m"<<X<<"\e[m"
#define CYANRUM(X)     "\e[1m\e[36m"<<X<<"\e[m"
// Regular Red, etc.
#define REDGIN(X)      "\e[31m"<<X<<"\e[m"
#define GREENGIN(X)    "\e[32m"<<X<<"\e[m"
#define YELLOWGIN(X)   "\e[33m"<<X<<"\e[m"
#define BLUEGIN(X)     "\e[34m"<<X<<"\e[m"
#define MAGENTAGIN(X)  "\e[35m"<<X<<"\e[m"
#define CYANGIN(X)     "\e[36m"<<X<<"\e[m"
#define BOLD(X)        "\e[1m"<<X<<"\e[m"
#endif