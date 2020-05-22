// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    Isograms = GetValidWords(Words);

    SetupGame(); 
}

void UBullCowCartridge::OnInput(const FString& PlayerInput) // When the player hits enter
{

    if (bGameOver)
    {
       ClearScreen();
       SetupGame();
    }

    else // check if PlayerGuess is correct or not 
    {    
       ProcessGuess(PlayerInput);
    }
}

void UBullCowCartridge::SetupGame()
{

    //Welcomes the player
    PrintLine(TEXT("Welcome to Bulls and Cows!"));

    HiddenWord = Isograms[FMath::RandRange(0, Isograms.Num() - 1)];//Set the HiddenWord
    EasterEgg = TEXT("3006");//My EasterEgg
    GuriEasterEgg = TEXT("1307");//Guri's EasterEgg
    GrossEasterEgg = TEXT("1206");//Gross's EasterEgg
    Lives = 10;//Set Lives
    bGameOver = false;

    PrintLine(TEXT("You have got %i tries left"), Lives);

    PrintLine(TEXT("Guess the %i letter word"), HiddenWord.Len());
    PrintLine(TEXT("Press TAB and submit your Guess Here: ")); //Asks player for Guess
    // PrintLine(TEXT("The Hidden Word is: %s."), *HiddenWord);// Debug Line
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("\nPress ENTER to play again"));
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{
    if (Guess == HiddenWord)
    {
        ClearScreen();
        PrintLine(TEXT("You have Won!"));
        EndGame();
        return;
    } 
   
    //checks EasterEgg
    if (Guess == EasterEgg) 
    {
        ClearScreen();
        PrintLine(TEXT("NICE You know when i was born :)"));
        PrintLine(TEXT("BTW there are 3 easter eggs in this game\nSo if you want you can go find the other \none")); // Magic number of easter eggs change if add more
        PrintLine(TEXT("\nNow Keep guessing!"));
        return;
    }

    //checks Guri's EasterEgg
    if (Guess == GuriEasterEgg)
    {
        ClearScreen();
        PrintLine(TEXT("If you are watching this you are probably Peleg or Yuval."));
        PrintLine(TEXT("BTW there are 3 easter eggs in this game\nSo if you want you can go find the other \none")); // Magic number of easter eggs change if add more
        PrintLine(TEXT("\nNow Keep guessing!"));
        return;
    }

    //checks Gross's EasterEgg
    if (Guess == GrossEasterEgg) 
    {
        ClearScreen();
        PrintLine(TEXT("Gross is apathetic!"));
        PrintLine(TEXT("BTW there are 3 easter eggs in this game\nSo if you want you can go find the other \none")); // Magic number of easter eggs change if add more
        PrintLine(TEXT("\nNow Keep guessing!"));
        return;
    }

    //Check number of characters
    if (Guess.Len() != HiddenWord.Len())
    {  
        ClearScreen();
        PrintLine(TEXT("The Hidden Word is %i Characters long. \nTry Again!"), HiddenWord.Len());
        return;
    }

    //Check if its an isogram
    if (!IsIsogram(Guess))
    {
        ClearScreen();
        PrintLine(TEXT("There are no repeating letters. \nTry Again!"));
        return;
    } 

    //Remove life if wrong
    PrintLine(TEXT("You have got %i tries remaining. \nTry Again!"), --Lives);

    //Checks if the lives are less or zero, and if do ends the game 
    if (Lives <= 0)  
    {
        ClearScreen();
        PrintLine(TEXT("You have no tries left."));
        PrintLine(TEXT("The hidden word was: %s"), * HiddenWord);
        EndGame(); 
        return;
    }

    //Show the player Bulls and Cows
    FBullCowCount Score =  GetBullCows(Guess);

    PrintLine(TEXT("You have %i Bulls and %i Cows"), Score.Bulls, Score.Cows);

}

bool UBullCowCartridge::IsIsogram(const FString& Word) const
{

 /*    
    For each letter.
    Start at element [0].
    Compare againts the next letter.
    Until we reach [Word.Len() -1].
    If any letters are the same return false.
 */

    for (int32 Index = 0; Index < Word.Len(); Index++)
    {
        for (int32 Comparison = Index + 1; Comparison < Word.Len(); Comparison++)
        {
            if (Word[Index] == Word[Comparison])
            {
                return false;
            }
        }
    }
    
    return true;
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordList) const
{
    TArray<FString> ValidWords;

    for (FString Word : WordList)
    {
        if (Word.Len() >= 4 && Word.Len() <= 6 && IsIsogram(Word))
        {
            ValidWords.Emplace(Word);
        }
    }

    return ValidWords;
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const
{

    FBullCowCount Count; 

 // For every index Guess is same as index HiddenWord then the BullCount is going to be ++
 // If it is not a bull is it a cow? if yes CowCount++

    for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
    {
        if (Guess[GuessIndex] == HiddenWord[GuessIndex])
        {
            Count.Bulls++;
            continue;
        }
        
        for (int32 HiddenIndex = 0; HiddenIndex < HiddenWord.Len(); HiddenIndex++)
        {
            if (Guess[GuessIndex] == HiddenWord[HiddenIndex])
            {
                Count.Cows++;
                break;
            }
        }
    }

    return Count;
}