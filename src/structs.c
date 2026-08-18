struct addrinfo {
    int             ai_flags;
    int             ai_family;
    int             ai_socktype;
    int             ai_protocol;
    size_t          ai_addrlen;
    struct sockaddr *ai_addr;
    char            *ai_canonname;

    struct addrinfo *ai_next;
};

struct sockaddr {
    unsigned short  sa_family;
    char            sa_addr[14];
};

struct sockaddr_in {
    short int           sin_family;
    unsigned short int  sin_port;
    struct in_addr      sin_addr;
    unsigned char       sin_zero[8];
};

struct in_addr {
    uint32_t s_addr;
};

struct sockaddr_in6 {
    u_int16_t       sin6_family;
    u_int16_t       sin6_port;
    u_int32_t       sin6_flowinfo;
    struct in6_addr sin6_addr;
    u_int32_t       sin6_scope_id;
};

struct in6_addr {
    unsigned char   sin6_addr[16];
};

struct sockaddr_storage {
    sa_family_t     ss_family; // address family

    // padding, implementation specific, ignore it
    char            __ss_pad1[_SS_PADSIZE];
    int64_t         __ss_align;
    char            __ss_pad2[_SS_PADSIZE;]
}

