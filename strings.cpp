/// Answers for Chapter 1
///
/// 1.1 Implement an algorithm to determine if a string has all unique characters. What if you can not use additional data structures?
///     Discussion: Very simple if you just have ascii characters: allocate a constant array of booleans (or if space is a concern, 
///     a bitmap) that indexes off of ( char - '0' ) and sets the bit flag to 1 when a match is found. That's O(n) on string length,
///     and very space-efficient. For non-ascii characters, you'd want a hashmap (i.e. an unordered_map in C++), which is also O(n) on
///     string length, although potentially considerably larger. (I don't know the back-end implementation of a C++ unordered map; if
///     it re-reserves on any duplicate hash then you'd want to reserve the size of the string immediately, but if it takes reserve calls
///     as gospel and has a list of values instead of a value on the far end, then we'd be more space-efficient by half.) Finally, if you
///     can't use a separate data structure, I don't see a way to make this better than O(n^2), although you'd be able to halve that
///     performance because you can use i as your starting place.
///
/// I'll implement the bitmap, which seems like the strongest combination of size and speed, although again has a large assumption
/// in its ascii character list.

#include <iostream>
#include <string>
#include <cstring>
#include <vector>

using std::string;
using std::cout;
using std::endl;
using std::memset;
using std::vector;
using std::strlen;

bool stringContainsDuplicates( const string& targetString )
{
    // For the next two variables, c.f. http://www.asciitable.com/index/asciifull.gif
    const char NUM_ASCII_CHARS = 96;    //< This value is approximate and rounded up to the nearest multiple of 8. 
    const char FIRST_ASCII_CHAR = '!';
    const char BITS_IN_A_BYTE = 8;
    const size_t MATCHED_CHARACTERS_ARRAY_SIZE = (size_t)NUM_ASCII_CHARS / (size_t)BITS_IN_A_BYTE;

    char matchedCharacters[ MATCHED_CHARACTERS_ARRAY_SIZE ];
    memset( matchedCharacters, 0, MATCHED_CHARACTERS_ARRAY_SIZE * sizeof( char ) );

    for( int i = 0; i < targetString.length(); ++i )
    {
        char thisLetter = targetString[ i ];
        char offset = thisLetter - FIRST_ASCII_CHAR;

        size_t arrayIndex = (size_t)( offset / BITS_IN_A_BYTE );
        size_t shiftVal = 1 << ( (size_t)( ( offset % BITS_IN_A_BYTE ) ) );

        if( ( matchedCharacters[ arrayIndex ] & ( shiftVal ) )  != 0 )
        {
            return true;
        }

        matchedCharacters[ arrayIndex ] |= shiftVal;
    }

    return false;
}

/// 1.2: I am assuming the question wants the operation performed in-place.
void reverseCStr( char* str )
{
    size_t len = strlen( str );
    for( int i = 0, j = len - 1; i < j; ++i, --j )
    {
        char tmp = str[ i ];
        str[ i ] = str[ j ];
        str[ j ] = tmp;
    }
}

void removeDuplicatesOfCharInPlace( char target, int offset, string& buffer )
{
    if( offset >= buffer.size() )
    {
        return;
    }

    string::iterator iter = buffer.begin() + offset;
    while( iter != buffer.end() )
    {
        if( *iter == target )
        {
            iter = buffer.erase( iter );
        }
        else
        {
            ++iter;
        }
    }
}

/// 1.3: I can't go any faster than O(n^2) with the given space restriction, that I can think of.
void removeDuplicatesInPlace( string& target )
{
    if( target.size() == 0 || target.size() == 1 )
    {
        return;
    }

    // The outer loop can be a simple increment, because you're always guaranteed that after the inner loop,
    //  both str[i] and str[i + 1] (if the latter exists) will be unique. However, the length of the string
    //  is dynamic, so a for loop won't do it because of the dynamic terminator.
    size_t i = 0;
    while( i < ( target.size() - 1 ) )  //< You don't need to do anything for the last character, hence -1.
    {
        removeDuplicatesOfCharInPlace( target[ i ], i + 1, target );
        ++i;
    }
}

int main()
{
    vector< string > stringList = {
        "",
        "a",
        "aa",
        "Hello",
        "world",
        "abcdefghijklm",
        "abcdefghijklmA",
        "abcdefghijklma",
        "abcdefghijklmnopqrstuvwxzy",
        "abcdefghijklmnopqrstuvwxzyA",
        "abcdefghijklmnopqrstuvwxzya",
        "aaabbbcccaaabbbcccaaabcccbbabababbccccabababccbcbbbabbcbabcbabcbac",
        "aza",
    };

    for( vector< string >::const_iterator iter = stringList.cbegin(); iter != stringList.cend(); ++iter )
    {
        cout << "\"" << *iter << "\" contains duplicates: " << stringContainsDuplicates( *iter ) << endl;
    }

    for( vector< string >::const_iterator iter = stringList.cbegin(); iter != stringList.cend(); ++iter )
    {
        char revString[ iter->size() + 1 ];
        strcpy( revString, iter->c_str() );
        reverseCStr( revString );
        cout << "RevString: " << revString << endl;
    }

    for( vector< string >::const_iterator iter = stringList.cbegin(); iter != stringList.cend(); ++iter )
    {
        string noDupes = *iter;
        removeDuplicatesInPlace( noDupes );
        cout << "No dupes of " << *iter << ": " << noDupes << endl;

    }

    return 0;
}