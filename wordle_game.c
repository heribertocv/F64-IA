#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define END_GAME   0x1010
#define UNKNOW_STATUS 0x1111

const int SIZE_WORD = 5 + 1;
const int MAX_TRIES = 6;

typedef struct{
    int numWords;
    char *words;
}DictionaryWords;

typedef struct{
    int num_tries;
    char target_word[SIZE_WORD];
    char status[SIZE_WORD*(MAX_TRIES+1)];
}WordleGame;


void release_dictionary(DictionaryWords Dictionary)
{
    free(Dictionary.words);
}

void load_words( const char* filename, DictionaryWords *Dictionary )
{
    char *words = NULL;
    FILE* file2read;
    int bufferLength = 255;
    char buffer[bufferLength]; 
    int offset = 0;

    file2read = fopen("words.txt", "r");

    // allocate memory for words
    fgets(buffer, bufferLength, file2read);
    Dictionary->numWords = atoi(buffer);
    Dictionary->words = (char*)malloc( Dictionary->numWords*(SIZE_WORD)*sizeof(char) );
    
    // load words to memory
    while( fgets(buffer, bufferLength, file2read) ) {
        buffer[ SIZE_WORD-1 ] = '\0';
        strcpy(Dictionary->words+offset, buffer);
        offset += SIZE_WORD;
    }

    fclose(file2read);
}

void print_words( DictionaryWords Dictionary )
{
    char *iterWords = Dictionary.words;
    int offset = 0;

    printf("TOTAL WORDS: %d\n", Dictionary.numWords );

    for(int ii=0; ii< Dictionary.numWords; ++ii, iterWords+=SIZE_WORD )
        printf("%d : %s\n", ii, iterWords); 
}

WordleGame create_game()
{
    DictionaryWords Dictionary;
    load_words("words.txt", &Dictionary);
    //print_words( Dictionary );
    
    WordleGame theGame;
    
    srand(121328); //srand(time(0));
    int idx_target = rand() % Dictionary.numWords;
    strcpy(theGame.target_word, Dictionary.words+idx_target*SIZE_WORD);
    theGame.status[0] = '\0';
    theGame.num_tries = 0;

    return theGame;
}

void get_tryword( WordleGame theGame, int num_try,  char word[SIZE_WORD] )
{
    strcpy(word, theGame.status + num_try*SIZE_WORD);
}

void get_current_word( WordleGame theGame,  char word[SIZE_WORD] )
{
    if( theGame.num_tries )
        strcpy(word, theGame.status + (theGame.num_tries-1)*SIZE_WORD);
    else
        word[0] = '\0';
}

void terminate_game( void )
{
    // release_dictionary(Dictionary);
    ;
}

char is_letter_in_word( char letter, char word[SIZE_WORD] )
{
    char *iter = word;
    while( *iter && *iter != letter )
        ++iter;

    return *iter ? '?' : ' ';
}

void generate_hint_code( WordleGame theGame, char word[SIZE_WORD], char hint[SIZE_WORD] )
{
    char *it1 = theGame.target_word; 
    char *it2 = word;
    char *itHint = hint;

    for( ;*it1; ++it1, ++it2, ++itHint)
        *itHint = *it1 == *it2 ? 
                        '*' : is_letter_in_word(*it2, theGame.target_word);

    hint[SIZE_WORD-1] = '\0';
}

void generate_match_code( WordleGame theGame, char bet[SIZE_WORD], char match[SIZE_WORD] )
{
    char *it1 = theGame.target_word; 
    char *it2 = bet;
    char *itMatch = match;

    for( ;*it1; ++it1, ++it2, ++itMatch)
        *itMatch =  *it1 == *it2 ? toupper(*it2) : *it2;

    match[SIZE_WORD-1] = '\0';
}

void display_code( char code[SIZE_WORD])
{
    printf("\t|");
    for(char *it = code; *it; ++it)
        printf(" %c |", *it );
    printf("\n");
}

void display_status( WordleGame theGame )
{
    char word[SIZE_WORD];
    char hint_code[SIZE_WORD];
    char match_code[SIZE_WORD];

    // WARNING: clear terminal is OS depend
    system("clear");  // case *NIX

    printf(" -------------------------------------------------\n");
    printf("\t\t  W O R D L E\n");
    printf(" -------------------------------------------------\n");
    printf("\tAttemps: %d/%d\n", theGame.num_tries, MAX_TRIES);
    //printf("\tTARGET: %s\n", theGame.target_word);
    get_current_word(theGame,word);
    printf("\tstatus: %s\n\n", word );

    for(int ii=0; ii< theGame.num_tries; ++ii){
        get_tryword( theGame, ii, word);

        generate_hint_code( theGame, word, hint_code);
        display_code(hint_code);

        generate_match_code(theGame, word, match_code );
        display_code(match_code);

        printf("\n");
    }

    printf(" -------------------------------------------------\n");
}

void input_word( char *best )
{
    printf("What's your best word: ");
    scanf( "%s",best );
    best[ SIZE_WORD-1 ] = '\0';

    // wordbet to lowercase
    char *iter = best; 
    for( ;*iter; ++iter)
        *iter = tolower(*iter);
}

int check_status( WordleGame theGame )
{
    const char* current_word = theGame.status + (theGame.num_tries-1)*SIZE_WORD;
    if( strcmp(theGame.target_word, current_word ) == 0 ||
        theGame.num_tries >= MAX_TRIES )
        return END_GAME;
    
    return UNKNOW_STATUS;
} 

void update_status( WordleGame *theGame, char* word_attempt )
{
    strcpy( theGame->status + theGame->num_tries*SIZE_WORD,
                word_attempt);
    theGame->num_tries++;
}

void display_score( WordleGame theGame )
{
    const char* last_word = theGame.status + (theGame.num_tries-1)*SIZE_WORD;
    
    printf("\n\n -------------------------------------------------\n");
    if( strcmp(theGame.target_word, last_word ) == 0 ){
        printf("\t C O N G R A T U L A T I O N S\n");
        printf("\t\t YOU WIN!!!\n");
        printf("\t\t score %d/%d \n\t\t %s\n", theGame.num_tries, MAX_TRIES,
                    theGame.num_tries == 1? "PERFECT!!!" : "" );
    }else{
        printf("\t\t YOU LOST!!!\n"); 
    }
    printf(" -------------------------------------------------\n\n\n");

}

int main(void)
{
    char word_attempt[16]; 

    WordleGame Wordle = create_game();
    display_status( Wordle );

    while( 1 )
    {
        input_word( word_attempt );
        update_status( &Wordle, word_attempt );
        display_status( Wordle );

        if( check_status( Wordle ) == END_GAME ) 
            break;
    }

    display_score( Wordle );

    return 0;
}