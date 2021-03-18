// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    HiddenWord = "typing";

    PrintLine(TEXT("Welcome to Bull Cows.\nPress tab to enable typing."));
    PrintLine(TEXT("Guess the 6 letter word!")); // Magic Number
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    ClearScreen();

    if (Input == HiddenWord)
    {
        PrintLine(TEXT("You Win!"));
    }
    else
    {
        PrintLine(TEXT("You Lose!"));
    }
}