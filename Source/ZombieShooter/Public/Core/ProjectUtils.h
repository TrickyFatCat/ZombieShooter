// Created by Artyom Volkov during the UE4 course

#pragma once


class FProjectUtils
{
public:
	static bool  GetPlayerViewPoint(AActor* CharacterActor, FVector& ViewLocation, FRotator& ViewRotation);
};