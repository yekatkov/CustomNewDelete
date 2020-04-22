#include <cstdlib>
#include <cstdio>

static int allocationID = 0;

static void* doAlloc( size_t size )
{
    // Append place for allocation ID
    size += sizeof( allocationID );
    char* ret = static_cast<char*>( malloc( size ) );

    printf("malloc : %p : %d\n", ret, allocationID );
    *( reinterpret_cast<typeof( allocationID )*>( ret ) ) = allocationID++;

    return ret + sizeof( allocationID );
}

static void doFree( void* ptr )
{
    if( ptr == 0 ) {
        return;
    }
    // Get real pointer
    char* realPtr = static_cast<char*>( ptr ) - sizeof( allocationID );
    int allocationID = *( reinterpret_cast<typeof( allocationID )*>( realPtr ) );

    printf( "free : %p : %d\n", realPtr, allocationID );
    free( realPtr );
}

void* operator new( size_t size )
{
    return doAlloc( size );
}

void operator delete( void* ptr )  noexcept
{
    doFree( ptr );
}
