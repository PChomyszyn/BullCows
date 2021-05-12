//Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
//#include "Math/UnrealMathUtility.h"

void UBullCowCartridge::BeginPlay() //When the game starts
{
    Super::BeginPlay();

    FBullCowCount Count;

    const FString WordListPath = FPaths::ProjectContentDir() / TEXT("WordLists/HiddenWordList.txt");
    FFileHelper::LoadFileToStringArrayWithPredicate(Isograms, *WordListPath, [](const FString& Word)
        {
            return Word.Len() >= 4 && Word.Len() <= 10 && IsIsogram(Word);
        }); //Load and validate hidden word list

    InitGame();
}

void UBullCowCartridge::OnInput(const FString& Input) //When the player hits enter
{
    ClearScreen();

    if (bGameOver)
    {
        ClearScreen();
        InitGame();
    }
    else
    {
        ProcessGuess(Input);
    }
}

void UBullCowCartridge::InitGame()
{
    HiddenWord = Isograms[FMath::RandRange(0, Isograms.Num() - 1)];
    Lives = HiddenWord.Len() * 2;
    bGameOver = false;

    //PrintLine(TEXT("%s"), *HiddenWord); //Debug code to see the word
    PrintLine(TEXT("Welcome to Bull Cows.\nPress tab to enable typing."));
    PrintLine(TEXT("Guess the %i letter word!"), HiddenWord.Len());
    PrintLine(TEXT("Your lives start at %i lives!"), Lives);
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("Press enter to play again."));
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{
    if (Guess == HiddenWord)
    {
        PrintLine(TEXT("You Win!"));
        EndGame();
        return;
    }

    Lives--;

    if (Lives == 0)
    {
        PrintLine(TEXT("You Lose!"));
        PrintLine(TEXT("The hidden word was: %s"), *HiddenWord);
        EndGame();
        return;
    }

    if (Guess.Len() != HiddenWord.Len())
    {
        PrintLine(TEXT("The hidden word is %i characters long!\nTry guessing the correct length of word!"), HiddenWord.Len());
        PrintLine(TEXT("You Lose a Life!\nYou have %i lives remaining!"), Lives);
        return;
    }

    if (!IsIsogram(Guess))
    {
        PrintLine(TEXT("Your guess was not an isogram!\nPlease guess again!"));
        PrintLine(TEXT("You Lose a Life!\nYou have %i lives remaining!"), Lives);
        return;
    }

    //Show Bulls and Cows
    FBullCowCount Score = GetBullCows(Guess);

    PrintLine(TEXT("You have %i Bulls, and %i Cows."), Score.Bulls, Score.Cows);

    PrintLine(TEXT("You guessed wrong, please guess again!"));
    PrintLine(TEXT("You Lose a Life!\nYou have %i lives remaining!"), Lives);
}

bool UBullCowCartridge::IsIsogram(const FString& Word)
{
    for (int32 i = 0; i < Word.Len(); i++)
    {
        for (int32 j = i + 1; j < Word.Len(); j++)
        {
            if (Word[i] == Word[j])
            {
                return false;
            }
        }
    }

    return true;
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const
{
    FBullCowCount Count;

    for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
    {
        if (Guess[GuessIndex] == HiddenWord[GuessIndex])
        {
            Count.Bulls++;
            continue;
        }

        for (int32 CowIndex = 0; CowIndex < HiddenWord.Len(); CowIndex++)
        {
            if (Guess[GuessIndex] == HiddenWord[CowIndex])
            {
                Count.Cows++;
                break;
            }
        }
    }
    return Count;
}